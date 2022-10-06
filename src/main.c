#include <stdint.h>

#include "ubox.h"
//#include "mplayer.h"

#include "helpers.h"
#include "game.h"

#define LOCAL
#include "main.h"

// generated
#include "tiles.h"


void draw_menu()
{
    uint8_t i;

    ubox_disable_screen();

    ubox_fill_screen(WHITESPACE_TILE);

    // game logo; starts in tile 32 and it is 10 x 3 tiles
    for (i = 0; i < 10; ++i)
    {
        ubox_put_tile(11 + i, 6, 32 + i);
        ubox_put_tile(11 + i, 7, 64 + i);
        ubox_put_tile(11 + i, 8, 96 + i);
    }

    put_text(2, 13, "PRESS SPACE OR FIRE1 BUTTON");

    put_text(9, 2, "MSXDEV'2? ENTRY");
    put_text(9, 16, "CODE & GRAPHICS");
    put_text(7, 17, "-PEDRO DE MEDEIROS-");
    //put_text(9, 19, "SOUND & MUSIC");
    //put_text(8, 20, "-PABLO VASQUES-");
    // 037 is ASCII 31 in octal, our Copyright sign
    //put_text(6, 22, "\037202X ?????????????");

    ubox_enable_screen();
}

void draw_end_game()
{
    ubox_disable_screen();

    ubox_fill_screen(WHITESPACE_TILE);

    put_text(3, 9, "WELL DONE!");
    put_text(3, 10, "YOU HAVE FINISHED THE DEMO.");
    put_text(3, 12, "NOW GO AND MAKE GAMES :)");

    put_text(3, 14, "(PRESS ESC)");

    ubox_enable_screen();

    // wait until ESC is pressed
    while (1)
    {
        if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
            break;

        ubox_wait();
    }
}

void draw_game_over()
{
    ubox_disable_screen();

    ubox_fill_screen(WHITESPACE_TILE);
    put_text(11, 10, "GAME  OVER");

    ubox_enable_screen();

    // play game over music
    //mplayer_init(SONG, SONG_GAME_OVER);

    ubox_wait_for(128);
}

void main()
{
    debug_msg("Q*Bert demo");
    debug_break();

    //  PAL: 50/1 = 50 FPS
    // NTSC: 60/1 = 60 FPS
    ubox_init_isr(1);

    // set screen 2
    ubox_set_mode(2);
    // all black
    ubox_set_colors(1, 1, 1);

    // avoid showing garbage on the screen
    // while setting up the tiles
    ubox_disable_screen();

    // upload our tileset
    ubox_set_tiles(tiles);
    // and the colour information
    ubox_set_tiles_colors(tiles_colors);

    // clear the screen
    ubox_fill_screen(WHITESPACE_TILE);

    ubox_enable_screen();

    // reg 1: activate sprites, v-blank int on, 16x16 sprites
    ubox_wvdp(1, 0xe2);

    // init the player
    //mplayer_init(SONG, SONG_SILENCE);
    //mplayer_init_effects(EFFECTS);

    // we don't need anything in our ISR
    // other than the play function, so we
    // use that!
    //ubox_set_user_isr(mplayer_play);
    ubox_set_user_isr(LVM_Decode);

redraw_menu:
    draw_menu();

    // wait until fire is pressed
    // can be space (control will be cursors), or any fire button on a joystick
    while (1)
    {
        ctl = ubox_select_ctl();
        if (ctl != UBOX_MSX_CTL_NONE)
        {
            //mplayer_play_effect_p(EFX_START, EFX_CHAN_NO, 0);
            ubox_wait_for(16);

            // play the game
            run_game();

            if (!lives)
                draw_game_over();
            else if (!batteries)
                draw_end_game();

            goto redraw_menu;
        }
        ubox_wait();
    }
}
