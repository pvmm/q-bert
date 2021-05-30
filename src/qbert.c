#include <msxhal.h>
#include <tms99X8.h>
#include <tile.h>

#include "sprites.h"
#include "qbert.h"


#define NUM_SPRITES 4

struct Qbert qbert;

T_SA SA0, SA1;


void init_qbert ()
{
    qbert.dirty = true;
    qbert.pos = 1;
    qbert.frame = 0;
    qbert.x0 = 122;
    qbert.y0 = 8;
    qbert.x = 122;
    qbert.y = 8;
    qbert.tile_x = 14;
    qbert.tile_y = 2;
    qbert.direction = DIR_DOWN_RIGHT;
    qbert.update = nullptr;

    // 0: facing down, 1: facing up
    for (int j = 0; j < 2; ++j)
    {
        for (int i = 0; i < NUM_SPRITES; i++)
        {
            // j = 0: down right
            // 0,1,2,3
            // 4,5,6,7
            // 8,9,10,11
            // 12,13,14,15
            // j = 1: up right
            // 32,33,34,35
            // 36,37,38,39
            // 40,41,42,42
            // 44,45,46,47
            TMS99X8_writeSprite8 (j * 32 + i * 4 + 0, &sprites[j][i][0]);
            TMS99X8_writeSprite8 (j * 32 + i * 4 + 1, &sprites[j][i][8]);
            TMS99X8_writeSprite8 (j * 32 + i * 4 + 2, &sprites[j][i][16]);
            TMS99X8_writeSprite8 (j * 32 + i * 4 + 3, &sprites[j][i][24]);

	    // flipped (down left)
	    // 16,17,18,19
            // 20,21,22,23
            // 24,25,26,27
            // 28,29,30,31
            // j = 1 (up left)
            // 48,49,50,51
            // 52,53,54,55
            // 56,57,58,59
            // 60,61,62,63
            TMS99X8_writeSprite8_flip (j * 32 + i * 4 + 16, &sprites[j][i][16]);
            TMS99X8_writeSprite8_flip (j * 32 + i * 4 + 17, &sprites[j][i][24]);
            TMS99X8_writeSprite8_flip (j * 32 + i * 4 + 18, &sprites[j][i][0]);
            TMS99X8_writeSprite8_flip (j * 32 + i * 4 + 19, &sprites[j][i][8]);
        }
    }

    // Put Q*Bert on screen.
    put_qbert_sprite (MODE2_BUFFER_0, true);
    put_qbert_sprite (MODE2_BUFFER_1, true);
}


/* update sprite allocation table (SAT) */
INLINE void update_sat(T_SA *SA, uint8_t pat, uint8_t id)
{
    static uint8_t colors[] = {BWhite, BBlack, BLightRed, BDarkRed,};

    if (qbert.direction == DIR_DOWN_RIGHT)
    {
            (*SA)[pat + 0].pattern = pat + 0;
            (*SA)[pat + 1].pattern = pat + 1;
            (*SA)[pat + 2].pattern = pat + 2;
            (*SA)[pat + 3].pattern = pat + 3;
    }
    else if (qbert.direction == DIR_DOWN_LEFT)
    {
            (*SA)[pat + 0].pattern = pat + 16;
            (*SA)[pat + 1].pattern = pat + 17;
            (*SA)[pat + 2].pattern = pat + 18;
            (*SA)[pat + 3].pattern = pat + 19;
    }
    else if (qbert.direction == DIR_UP_RIGHT)
    {
            (*SA)[pat + 0].pattern = pat + 32;
            (*SA)[pat + 1].pattern = pat + 33;
            (*SA)[pat + 2].pattern = pat + 34;
            (*SA)[pat + 3].pattern = pat + 35;
    }
    else if (qbert.direction == DIR_UP_LEFT)
    {
            (*SA)[pat + 0].pattern = pat + 48;
            (*SA)[pat + 1].pattern = pat + 49;
            (*SA)[pat + 2].pattern = pat + 50;
            (*SA)[pat + 3].pattern = pat + 51;
    }

    (*SA)[pat].color = (*SA)[pat + 1].color = (*SA)[pat + 2].color = (*SA)[pat + 3].color = colors[id];
}


void put_qbert_sprite(EM2_Buffer BUFFER, bool is_dirty) 
{
    static uint8_t fast_x1;
    static uint8_t fast_x2;
    static uint8_t fast_y1;
    static uint8_t fast_y2;

    uint8_t i, j;
    uint8_t offset = (qbert.direction & DIR_LEFT) ? 4 : 0;
    T_SA *SA = (BUFFER == MODE2_BUFFER_0) ? &SA0 : &SA1;

    fast_x1 = qbert.x - offset;
    fast_x2 = qbert.x - offset + 8;
    fast_y1 = qbert.y;
    fast_y2 = qbert.y + 8;

    if (DIR_LEFT_MASK (qbert.direction))
    {
        fast_x1 += 1;
        fast_x2 += 1;
        fast_y1 += 1;
        fast_y2 += 1;
    }

    for (j = 0, i = 0; i < NUM_SPRITES * 4; i += 4, j++)
    {
        // Update sprite positions.
        (*SA)[i + 0].x = fast_x1;
        (*SA)[i + 1].x = fast_x1;
        (*SA)[i + 2].x = fast_x2;
        (*SA)[i + 3].x = fast_x2;
        (*SA)[i + 0].y = fast_y1;
        (*SA)[i + 1].y = fast_y2;
        (*SA)[i + 2].y = fast_y1;
        (*SA)[i + 3].y = fast_y2;

        // Check if updating sprite shapes is need.
        if (is_dirty)
        {
            update_sat(SA, i, j);
        }
    }

    TMS99X8_writeSpriteAttributes(BUFFER, *SA);
}

