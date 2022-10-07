// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#include <stdint.h>
#include "core.h"
#include "lvgm_player.h"
#include "bios_mainrom.h"
#if (USE_LVGM_SCC)
#include "scc.h"
#endif

//=============================================================================
// DEFINES
//=============================================================================


//=============================================================================
// READ-ONLY DATA
//=============================================================================

// ay regsiter convertor         -  1  2  3  4  5' 6  7' 8  9' A   B'  C
const uint8_t  g_LVGM_RegTable[] = { 0, 1, 3, 5, 6, 6, 8, 8, 9, 9, 10, 10, 13 };

// File ident data
const uint8_t  g_LVGM_Ident[4] = { 'a', 'y', 'M', ' ' };

//=============================================================================
// MEMORY DATA
//=============================================================================

const struct LVGM_Header* g_LVGM_Header;
const uint8_t* g_LVGM_Pointer;
uint8_t        g_LVGM_Wait;
uint8_t        g_LVGM_State;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Play a LVGM data
bool LVGM_Play(const void* addr, bool loop)
{
	g_LVGM_Header = (const struct LVGM_Header*)(addr);
	for(uint8_t i = 0; i < 4; i++)
		if(g_LVGM_Header->Ident[i] != g_LVGM_Ident[i])
			return false;

	g_LVGM_State = 0;
	if(!(g_LVGM_Header->Flag & LVGM_FLAG_60HZ) || ((g_LVGM_Header->Flag & LVGM_FLAG_50HZ) && (g_ROMVersion.VSF)))
		g_LVGM_State |= LVGM_STATE_50HZ;
	if(loop)
		g_LVGM_State |= LVGM_STATE_LOOP;
	
	g_LVGM_Pointer = (const uint8_t*)(g_LVGM_Header) + sizeof(struct LVGM_Header);
	g_LVGM_Wait = 0;
	LVGM_Resume();
	return true;
}

//-----------------------------------------------------------------------------
// Play a LVGM data
void LVGM_Stop()
{
	LVGM_Pause();
	g_LVGM_Pointer = (const uint8_t*)(g_LVGM_Header) + sizeof(struct LVGM_Header);
	g_LVGM_Wait = 0;
}

//-----------------------------------------------------------------------------
//
void LVGM_Decode()
{
	// Check if the music is playing
	if(!(g_LVGM_State & LVGM_STATE_PLAY))
		return;
	
	// Check if there are still waiting cycles
	if(g_LVGM_Wait != 0)
	{
		g_LVGM_Wait--;
		return;
	}
	
	// Parse music data
	while(1)
	{
		uint8_t op = *g_LVGM_Pointer & 0xF0;
		switch(op)
		{
			case 0x00:
			{
				uint8_t reg = *g_LVGM_Pointer;
				PSG_SetRegister(reg, *++g_LVGM_Pointer);
				break;
			}
			case 0xD0: // 50 Hz wait
				if(g_LVGM_State & LVGM_STATE_50HZ)
				{
					g_LVGM_Wait += *g_LVGM_Pointer & 0x0F;
					g_LVGM_Pointer++;
					return;
				}
				break;
			case 0xE0: // 60 Hz wait
				if(!(g_LVGM_State & LVGM_STATE_50HZ))
				{
					g_LVGM_Wait += *g_LVGM_Pointer & 0x0F;
					g_LVGM_Pointer++;
					return;
				}
				break;
			case 0xF0: // Special
			#if (USE_LVGM_SCC)
				switch(*g_LVGM_Pointer)
				{
				// F1 1n    0x81 = n           Frequency channel 1 - MSB
				// F1 3n    0x83 = n           Frequency channel 2 - MSB
				// F1 5n    0x85 = n           Frequency channel 3 - MSB
				// F1 7n    0x87 = n           Frequency channel 4 - MSB
				// F1 9n    0x89 = n           Frequency channel 5 - MSB
				// F1 An    0x8A = n           Volume channel 1
				// F1 Bn    0x8B = n           Volume channel 2
				// F1 Cn    0x8C = n           Volume channel 3
				// F1 Dn    0x8D = n           Volume channel 4
				// F1 En    0x8E = n           Volume channel 5
				// F1 0n    0x8F = n           On/off switch channel 1 to 5
				// F1 Fn    0x8F = n | 0x10    On/off switch channel 1 to 5
				case 0xF1:
				{
					uint8_t val = g_LVGM_Pointer[1];
					if((val & 0xF0) == 0x00)
						SCC_SetRegister(0x8F, val & 0x0F);
					else if((val & 0xF0) == 0xF0)
						SCC_SetRegister(0x8F, (val & 0x0F) | 0x10);
					else
						SCC_SetRegister(0x80 + (val >> 4), val & 0x0F);
					g_LVGM_Pointer++;
					break;
				}
				case 0xF0: // F0 nn    0x80 = nn    Frequency channel 1 - LSB
				case 0xF2: // F2 nn    0x82 = nn    Frequency channel 2 - LSB
				case 0xF4: // F4 nn    0x84 = nn    Frequency channel 3 - LSB
				case 0xF6: // F6 nn    0x86 = nn    Frequency channel 4 - LSB
				case 0xF8: // F8 nn    0x88 = nn    Frequency channel 5 - LSB
				{
					uint8_t val = g_LVGM_Pointer[1];
					SCC_SetRegister(0x80 + (*g_LVGM_Pointer & 0x0F), val);
					g_LVGM_Pointer++;
					break;
				}
				// F9 nn[32]    nn[32] -> 0x00[32]    Waveform channel 1
				// FA nn[32]    nn[32] -> 0x20[32]    Waveform channel 2
				// FB nn[32]    nn[32] -> 0x40[32]    Waveform channel 3
				// FC nn[32]    nn[32] -> 0x60[32]    Waveform channel 4
				// FD nn[32]    nn[32] -> 0xA0[32]    Waveform channel 5 (SCC+ only)
				case 0xF5: // nn[bb] -> (0x00 + aa)[bb] (with bb > 1)
				{
					uint8_t reg = g_LVGM_Pointer[1];
					uint8_t num = g_LVGM_Pointer[2];
					for(uint8_t i = 0; i < num; i++)
						SCC_SetRegister(reg + i, g_LVGM_Pointer[3 + i]);
					g_LVGM_Pointer += 2 + num;
					break;
				}
				case 0xFE: // End of music with loop
					if(g_LVGM_State & LVGM_STATE_LOOP)
					{
						uint16_t loopOffset = *(u16*)++g_LVGM_Pointer;
						g_LVGM_Pointer = (const uint8_t*)(g_LVGM_Header) + sizeof(struct LVGM_Header) + loopOffset;
						return;
					}
				case 0xFF: // End of music
					LVGM_Stop();
					return;
				}
			#else
				// handle loop
				if((*g_LVGM_Pointer == 0xFE) && (g_LVGM_State & LVGM_STATE_LOOP))
				{
					uint16_t loopOffset = *(uint16_t*)++g_LVGM_Pointer;
					g_LVGM_Pointer = (const uint8_t*)(g_LVGM_Header) + sizeof(struct LVGM_Header) + loopOffset;
					return;
				}
				// if(val == 0xF)
				LVGM_Stop();
				return;
			#endif

			// case 0x10:
			// case 0x20:
			// case 0x30:
			// case 0x40:
			// case 0x50:
			// case 0x60:
			// case 0x70:
			// case 0x80:
			// case 0x90:
			// case 0xA0:
			// case 0xB0:
			// case 0xC0:
			default:
				PSG_SetRegister(g_LVGM_RegTable[*g_LVGM_Pointer >> 4], *g_LVGM_Pointer);
				break;
		}
		g_LVGM_Pointer++;
	}
}
