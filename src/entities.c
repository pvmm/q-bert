#include <string.h>

#define LOCAL
#include "entities.h"

#include "game.h"
#include "palette.h"
#include "lookup_tables.h"
#include "level.h"
#include "spman.h"

// generated
#include "player.h"
#include "enemy.h"


const int8_t (*jump_table)[16];

extern enum game_status game_status;    // completed the plates?
extern uint8_t control;
uint8_t old_pattern;
uint8_t row;                            // player vertical coordinate


void init_entities()
{
    for (uint8_t i = 0; i < MAX_ENTITIES; ++i)
        entities[i].active = 0;

    entities[BALL1].x = random() > 128 ? 138 : 106;
    entities[BALL1].y = 208;
    entities[BALL1].pattern = 0;
    entities[BALL1].active = false;
    entities[BALL1].update = update_entity;

    //entities[GREENBALL].x = 218 > 128 ? 138 : 106;
    //entities[GREENBALL].y = 208;
    //entities[GREENBALL].pattern = GREENBALL;
}


void update_entity(struct entity* entity)
{
    switch (entity->type)
    {
    case BALL1:
    case BALL2:
        entity->update = random() > 128 ? update_entity_left : update_entity_right;
        break;

    case COILY:
        // TODO: follow the player
        break;

    case SAM:
    case SLICK:
        entity->update = random() > 128 ? update_entity_left : update_entity_right;
        break;

    case WRONG_WAY1:
    case WRONG_WAY2:
        entity->update = random() > 128 ? update_entity_up : update_entity_down;
        break;

    case UGG1:
    case UGG2:
        entity->update = random() > 128 ? update_entity_up : update_entity_down;
        break;

    case GREENBALL:
        entity->update = random() > 128 ? update_entity_left : update_entity_right;
        break;
    }
}


void update_entity_left(struct entity* entity)
{
    entity;
}


void update_entity_right(struct entity* entity)
{
    entity;
}


void update_entity_up(struct entity* entity)
{
    entity;
}


void update_entity_down(struct entity* entity)
{
    entity;
}


void init_player()
{
    // fill sprite allocation table up
    spman_alloc_pat(QBERT_DOWN_RIGHT, *player_sprite, 3, 0);
    spman_alloc_pat(QBERT_DOWN_LEFT , *player_sprite, 3, true);
    spman_alloc_pat(QBERT_UP_RIGHT  , *(player_sprite + (sizeof(player_sprite)/32/2)), 3, 0);
    spman_alloc_pat(QBERT_UP_LEFT   , *(player_sprite + (sizeof(player_sprite)/32/2)), 3, true);

    row = 1;
    qbert.pos = 1;
    qbert.frame = 0;
    qbert.x0 = 122;
    qbert.y0 = 8;
    qbert.x = 122;
    qbert.y = 8;
    qbert.tile_x = 14;
    qbert.tile_y = 2;
    qbert.pattern = QBERT_DOWN_RIGHT;
    old_pattern = qbert.pattern;
    qbert.update = update_player;
}


void update_player(struct entity* entity)
{
    entity;
    old_pattern = qbert.pattern;

    switch (control)
    {
        default:
        case 4: // left
        case 1: // up
        case 2: // down
        case 8: // right
            break;

        case MOV_DOWN_RIGHT:
            //debug_msg("down-right");
            qbert.pattern = QBERT_DOWN_RIGHT;
            jump_table = &jump_down;
            qbert.update = update_player_right;
            break;

        case MOV_DOWN_LEFT:
            //debug_msg("down-left");
            qbert.pattern = QBERT_DOWN_LEFT;
            jump_table = &jump_down;
            qbert.update = update_player_left;
            break;

        case MOV_UP_RIGHT:
            //debug_msg("up-right");
            qbert.pattern = QBERT_UP_RIGHT;
            jump_table = &jump_up;
            qbert.update = update_player_right;
            break;

        case MOV_UP_LEFT:
            //debug_msg("up-left");
            qbert.pattern = QBERT_UP_LEFT;
            jump_table = &jump_up;
            qbert.update = update_player_left;
            break;
    }

    put_qbert_sprite();
}


void update_player_left(struct entity* entity)
{
    entity;
    put_qbert_sprite();

    if (qbert.frame < 16)
    {
        qbert.y = qbert.y0 + (*jump_table)[qbert.frame];
        qbert.frame++;
        qbert.x = qbert.x0 - qbert.frame;
    }
    else
    {
        //qbert.x--;
        qbert.tile_x -= 2;

        if (qbert.y0 < qbert.y)
        {
            // going down
            qbert.tile_y += 3;
            qbert.pos += row++;
        }
        else
        {
            // going up
            qbert.tile_y -= 3;
            qbert.pos -= row--;
        }

        game_status = press_plate(qbert.pos, qbert.tile_x, qbert.tile_y);

        // reset animation status
        qbert.x0 = qbert.x;
        qbert.y0 = qbert.y;
        qbert.frame = 0;
        qbert.update = update_player;
    }
}


void update_player_right(struct entity* entity)
{
    entity;
    put_qbert_sprite();

    if (qbert.frame < 16)
    {
        qbert.y = qbert.y0 + (*jump_table)[qbert.frame];
        qbert.frame++;
        qbert.x = qbert.x0 + qbert.frame;
    }
    else
    {
        //qbert.x--;
        qbert.tile_x += 2;

        if (qbert.y0 < qbert.y)
        {
            // going down
            qbert.tile_y += 3;
            qbert.pos += ++row;
        }
        else
        {
            // going up
            qbert.tile_y -= 3;
            qbert.pos -= --row;
        }

        game_status = press_plate(qbert.pos, qbert.tile_x, qbert.tile_y);

        // reset animation status
        qbert.x0 = qbert.x;
        qbert.y0 = qbert.y;
        qbert.frame = 0;
        qbert.update = update_player;
    }
}


void put_qbert_sprite()
{
    struct sprite_attr sa;
    static uint8_t fast_x;
    static uint8_t fast_y;

    fast_x = LEFT_PATTERN_MASK(qbert.pattern) ? qbert.x - 3 : qbert.x;
    fast_y = qbert.y - 1;

    //
    // Update sprite atributes
    //

    sa.y = fast_y;
    sa.x = fast_x;

    // eyes and mouth
    sa.attr = BG_BLACK;
    sa.pattern = qbert.pattern + 0;
    spman_alloc_sprite(&sa);

    // the bod (always visible)
    sa.attr = BG_DARKYELLOW;
    sa.pattern = qbert.pattern + 4;
    spman_alloc_sprite(&sa);

    // face
    //sa.attr = BG_LIGHTYELLOW;
    //sa.pattern = qbert.pattern + 8;
    //spman_alloc_sprite(&sa);

    // sclera (white of the eyes)
    sa.pattern = qbert.pattern + 8;
    sa.attr = BG_WHITE;
    spman_alloc_fixed_sprite(&sa);
}
