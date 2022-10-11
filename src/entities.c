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


inline void update_player()
{
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
            qbert.update = move_right;
            break;

        case MOV_DOWN_LEFT:
            //debug_msg("down-left");
            qbert.pattern = QBERT_DOWN_LEFT;
            jump_table = &jump_down;
            qbert.update = move_left;
            break;

        case MOV_UP_RIGHT:
            //debug_msg("up-right");
            qbert.pattern = QBERT_UP_RIGHT;
            jump_table = &jump_up;
            qbert.update = move_right;
            break;

        case MOV_UP_LEFT:
            //debug_msg("up-left");
            qbert.pattern = QBERT_UP_LEFT;
            jump_table = &jump_up;
            qbert.update = move_left;
            break;
    }
}


void init_player()
{
    // fill sprite allocation table up
    spman_alloc_pat(QBERT_DOWN_RIGHT, *player_sprite, 3, 0);
    spman_alloc_pat(QBERT_DOWN_LEFT , *player_sprite, 3, true);
    spman_alloc_pat(QBERT_UP_RIGHT  , *(player_sprite + (sizeof(player_sprite)/32/2)), 3, 0);
    spman_alloc_pat(QBERT_UP_LEFT   , *(player_sprite + (sizeof(player_sprite)/32/2)), 3, true);

    row = 1;
    qbert.type = QBERT;
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
    qbert.update = update_entity;
}


void init_entities()
{
    for (uint8_t i = 0; i < MAX_ENTITIES; ++i)
        entities[i].active = 0;

    // normal ball
    entities[BALL1].type = BALL1;
    entities[BALL1].x = random() > 128 ? 138 : 106;
    entities[BALL1].y = 208;
    entities[BALL1].pattern = 0;
    entities[BALL1].active = false;
    entities[BALL1].update = update_entity;

    // green ball
    entities[GREENBALL].type = GREENBALL;
    entities[GREENBALL].x = random() > 128 ? 138 : 106;
    entities[GREENBALL].y = 208;
    entities[GREENBALL].pattern = 0;
    entities[GREENBALL].pattern = GREENBALL;
    entities[GREENBALL].active = false;
}


void update_entity(struct entity* entity)
{
    switch (entity->type)
    {
    case QBERT:
        update_player();
        break;

    case BALL1:
    case BALL2:
    case SAM:
    case SLICK:
    case GREENBALL:
        entity->update = random() > 128 ? move_left : move_right;
        break;

    case COILY:
        // TODO: follow the player
        break;

    case UGG1:
    case UGG2:
    case WRONG_WAY1:
    case WRONG_WAY2:
        entity->update = random() > 128 ? move_up : move_down;
        break;
    }
}


void move_up(struct entity* entity)
{
    entity;
}


void move_down(struct entity* entity)
{
    entity;
}


void move_left(struct entity* entity)
{
    if (entity->frame < 16)
    {
        entity->y = entity->y0 + (*jump_table)[entity->frame];
        entity->frame++;
        entity->x = entity->x0 - qbert.frame;
    }
    else
    {
        //qbert.x--;
        entity->tile_x -= 2;

        if (entity->y0 < entity->y)
        {
            // going down
            entity->tile_y += 3;
            entity->pos += row++;
        }
        else
        {
            // going up
            entity->tile_y -= 3;
            entity->pos -= row--;
        }

        // reset animation status
        entity->x0 = entity->x;
        entity->y0 = entity->y;
        entity->frame = 0;

        if (entity->type == QBERT) {
            game_status = press_plate(entity->pos, entity->tile_x, entity->tile_y);
        }

        entity->update = update_entity;
    }
}


void move_right(struct entity* entity)
{
    if (entity->frame < 16)
    {
        entity->y = entity->y0 + (*jump_table)[entity->frame];
        entity->frame++;
        entity->x = entity->x0 + entity->frame;
    }
    else
    {
        entity->tile_x += 2;

        if (entity->y0 < entity->y)
        {
            // going down
            entity->tile_y += 3;
            entity->pos += ++row;
        }
        else
        {
            // going up
            entity->tile_y -= 3;
            entity->pos -= --row;
        }

        game_status = press_plate(qbert.pos, qbert.tile_x, qbert.tile_y);

        // reset animation status
        entity->x0 = entity->x;
        entity->y0 = entity->y;
        entity->frame = 0;
        entity->update = update_entity;
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
