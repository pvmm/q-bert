// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄▄  ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▀█▄  ██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▄▄█▀ ▀█▄█
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// PSG handler module
//
// References:
//  - AY-3-8910 Programmable Sound Generator Data Manual
//     http://map.grauw.nl/resources/sound/generalinstrument_ay-3-8910.pdf
//  - YM2149 Programmable Software-Controlled Sound Generator Manual
//     http://map.grauw.nl/resources/sound/yamaha_ym2149.pdf
//  - The Revised MSX Red Book - PSG
//     https://www.angelfire.com/art2/unicorndreams/msx/RR-PSG.html
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include <stdint.h>
#include "core.h"
#include "system_port.h"
#include "psg_reg.h"

//=============================================================================
// DEFINES
//=============================================================================

#define PSG_REG(a)				((a) & 0x0F)
#define PSG_CHAN(a)				((a) & 0x03)

// Ports
#if (PSG_CHIP == PSG_INTERNAL)
	#define PSG_PORT_REG		P_PSG_REGS     //< Used to select a specific register by writing its number (0 to 15)
	#define PSG_PORT_WRITE		P_PSG_DATA     //< Used to write to any register once it has been selected by the Address Port.	
	#define PSG_PORT_READ		P_PSG_STAT     //< Used to read any register once it has been selected by the Address Port.
	#define PSG_VAR_REG			g_PSG_RegPort
	#define PSG_VAR_WRITE		g_PSG_DataPort
	#define PSG_VAR_READ		g_PSG_StatPort
	
#elif (PSG_CHIP == PSG_EXTERNAL)
	#define PSG_PORT_REG		P_PSG_EXT_REGS //< Used to select a specific register by writing its number (0 to 15)
	#define PSG_PORT_WRITE		P_PSG_EXT_DATA //< Used to write to any register once it has been selected by the Address Port.	
	#define PSG_PORT_READ		P_PSG_EXT_STAT //< Used to read any register once it has been selected by the Address Port.
	#define PSG_VAR_REG			g_PSG_Ext_RegPort
	#define PSG_VAR_WRITE		g_PSG_Ext_DataPort
	#define PSG_VAR_READ		g_PSG_Ext_StatPort

#elif (PSG_CHIP == PSG_BOTH)
	// PRimary PSG
	#define PSG_PORT_REG		P_PSG_REGS
	#define PSG_PORT_WRITE		P_PSG_DATA
	#define PSG_PORT_READ		P_PSG_STAT
	// Secondary PSG
	#define PSG2_PORT_REG		P_PSG_EXT_REGS
	#define PSG2_PORT_WRITE		P_PSG_EXT_DATA
	#define PSG2_PORT_READ		P_PSG_EXT_STAT
	#define PSG2_REG(a)			(PSG_REG(a) | 0x80)
	#define PSG2_CHAN(a)		(PSG_CHAN(a) | 0x80)
#endif

// Channels
#define PSG_CHANNEL_A			0
#define PSG_CHANNEL_B			1
#define PSG_CHANNEL_C			2

// Mixer
#define PSG_TONE_A_ON			0b00000001
#define PSG_TONE_B_ON			0b00000010
#define PSG_TONE_C_ON			0b00000100
#define PSG_NOISE_A_ON			0b00001000
#define PSG_NOISE_B_ON			0b00010000
#define PSG_NOISE_C_ON			0b00100000
#define PSG_TONE_A_OFF			0
#define PSG_TONE_B_OFF			0
#define PSG_TONE_C_OFF			0
#define PSG_NOISE_A_OFF			0
#define PSG_NOISE_B_OFF			0
#define PSG_NOISE_C_OFF			0

#define PSG_USE_ENVELOPE		PSG_F_ENV

#if (PSG_USE_NOTES)

enum NOTE
{
	// Latin notation
	NOTE_DO = 0, // Do
	NOTE_DOd,    // Do#
	NOTE_RE,     // Re
	NOTE_REd,    // Re#
	NOTE_MI,     // Mi
	NOTE_FA,     // Fa
	NOTE_FAd,    // Fa#
	NOTE_SOL,    // Sol
	NOTE_SOLd,   // Sol#
	NOTE_LA,     // La
	NOTE_LAd,    // La#
	NOTE_SI,     // Si
	// Anglo-Germanic notation
	NOTE_C = 0,  // C 
	NOTE_Cd,     // C#
	NOTE_D,      // D
	NOTE_Dd,     // D#
	NOTE_E,      // E
	NOTE_F,      // F
	NOTE_Fd,     // F#
	NOTE_G,      // G
	NOTE_Gd,     // G#
	NOTE_A,      // A
	NOTE_Ad,     // A#
	NOTE_B       // B
};

#endif // (PSG_USE_NOTES)

//-----------------------------------------------------------------------------
struct PSG_Data
{
	uint16_t		Tone[3];	// 6
	uint8_t		    Noise;		// 1
	uint8_t		    Mixer;		// 1
	uint8_t		    Volume[3];	// 3
	uint16_t		Envelope;	// 2
	uint8_t		    Shape;		// 1
	uint8_t		    IOPortA;	// 1
	uint8_t		    IOPortB;	// 1
};

#if (PSG_ACCESS == PSG_INDIRECT)
extern struct PSG_Data g_PSG_Regs;
#if (PSG_CHIP == PSG_BOTH)
extern struct PSG_Data g_PSG2_Regs;
#endif
#endif

//=============================================================================
// PROTOTYPE
//=============================================================================

// Group: Register

// Function: PSG_SetRegister
// Set the value of a given register
void PSG_SetRegister(uint8_t reg, uint8_t value);

// Function: PSG_GetRegister
// Get the value of a given register
uint8_t PSG_GetRegister(uint8_t reg);

// Group: Helper
#if (PSG_USE_EXTRA)
// Function: PSG_SetTone
// Set the tone period of a given channel (tone generator control register)
void PSG_SetTone(uint8_t chan, uint16_t period);

// Function: PSG_SetNoise
// Set the noise period (noise generator control register)
void PSG_SetNoise(uint8_t period);

// Function: PSG_SetMixer
// Setup mixer by enabling tune and noise generators for each channel (mixer control enable register)
void PSG_SetMixer(uint8_t mix);

// Function: PSG_SetVolume
// Set the volume of a given channel (Amplitude control register)
void PSG_SetVolume(uint8_t chan, uint8_t vol);

// Function: PSG_SetEnvelope
// Set the envelope period (Envelope priod control register)
void PSG_SetEnvelope(uint16_t period);

// Function: PSG_SetShape
// Set the envelope shape (Envelope shape control register)
void PSG_SetShape(uint8_t shape);

// Function: PSG_EnableTone
// Enable/disable tone on the given channel
void PSG_EnableTone(uint8_t chan, uint8_t val);

// Function: PSG_EnableNoise
// Enable/disable noise on the given channel
void PSG_EnableNoise(uint8_t chan, uint8_t val);

// Function: PSG_EnableEnvelope
// Enable/disable envelope on the given channel
void PSG_EnableEnvelope(uint8_t chan, uint8_t val);

#endif //(PSG_USE_EXTRA)

// Function: PSG_Mute
// Silent the PSG (set according registers)
void PSG_Mute();

#if (PSG_USE_RESUME)
// Function: PSG_Resume
// Resume PSG sound
void PSG_Resume();
#endif

#if (PSG_ACCESS == PSG_INDIRECT)

// Group: Indirect

// Function: PSG_Apply
// Send data to PSG registers #0 to #13
void PSG_Apply();

#endif


