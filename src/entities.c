#include <string.h>

#define LOCAL
#include "enemy_ball.h"
#define LOCAL
#include "entities.h"

#include "game.h"
#include "palette.h"
#include "lookup_tables.h"
#include "level.h"
#include "spman.h"

// generated
#include "player.h"


const int8_t (*jump_table)[16] = &jump_down;
uint8_t old_pattern;


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

    // fill sprite allocation table up
    spman_alloc_pat(QBERT_DOWN_RIGHT, *player_sprite, 3, 0);
    spman_alloc_pat(QBERT_DOWN_LEFT , *player_sprite, 3, true);
    spman_alloc_pat(QBERT_UP_RIGHT  , *(player_sprite + (sizeof(player_sprite)/32/2)), 3, 0);
    spman_alloc_pat(QBERT_UP_LEFT   , *(player_sprite + (sizeof(player_sprite)/32/2)), 3, true);
}


void init_entities()
{
    for (uint8_t i = 0; i < LAST_ENTITY; ++i)
        entities[i].active = 0;

    // normal ball
    entities[BALL1].type = BALL1;
    entities[BALL1].frame = 0;
    entities[BALL1].x = ENTITY_START_X1; // random() > 128 ? 136 : 104;
    entities[BALL1].x0 = ENTITY_START_X1; // entities[BALL1].x;
    entities[BALL1].y = ENTITY_START_Y; // 208;
    entities[BALL1].y0 = ENTITY_START_Y; // entities[BALL1].y;
    entities[BALL1].tile_y = 5;
    entities[BALL1].pattern = BALL_NORMAL;
    entities[BALL1].active = true;
    entities[BALL1].update = update_entity;

    // fill sprite allocation table up
    spman_alloc_pat(BALL_NORMAL, *enemy_ball_sprite, 1, 0);
    spman_alloc_pat(BALL_PRESSED, *(enemy_ball_sprite + 32), 1, 0);

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

    default:
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


inline void qbert_move_left()
{
    if (qbert.frame < 16)
    {
        qbert.y = qbert.y0 + (*jump_table)[qbert.frame];
        qbert.frame++;
        qbert.x = qbert.x0 - qbert.frame;
    }
    else
    {
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
        qbert.update = update_entity;
    }
}


inline void entity_move_left(struct entity* entity)
{
    if (entity->frame < 16)
    {
        entity->y = entity->y0 + jump_down[entity->frame];
        entity->frame++;
        entity->x = entity->x0 - entity->frame;
    }
    else
    {
        if (entity->y0 < entity->y)
        {
            // going down
            entity->tile_y += 3;
        }
        else
        {
            // going up
            entity->tile_y -= 3;
        }

        // fell off the screen?
        if (entity->tile_y > 22) {
            entity->active = false;
        } else {
            // reset animation status
            entity->x0 = entity->x;
            entity->y0 = entity->y;
            entity->frame = 0;
            entity->update = update_entity;
        }
    }
}


void move_left(struct entity* entity)
{
    switch (entity->type) {
    case QBERT:
        qbert_move_left();
        break;

    default:
        entity_move_left(entity);
        break;
    }
}


inline void qbert_move_right()
{
    if (qbert.frame < 16)
    {
        qbert.y = qbert.y0 + (*jump_table)[qbert.frame];
        qbert.frame++;
        qbert.x = qbert.x0 + qbert.frame;
    }
    else
    {
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
        qbert.update = update_entity;
    }
}


inline void entity_move_right(struct entity* entity)
{
    if (entity->frame < 16)
    {
        entity->y = entity->y0 + jump_down[entity->frame];
        entity->frame++;
        entity->x = entity->x0 + entity->frame;
    }
    else
    {
        if (entity->y0 < entity->y)
        {
            // going down
            entity->tile_y += 3;
        }
        else
        {
            // going up
            entity->tile_y -= 3;
        }

        // fell off the screen?
        if (entity->tile_y > 22) {
            entity->active = false;
        } else {
            // reset animation status
            entity->x0 = entity->x;
            entity->y0 = entity->y;
            entity->frame = 0;
            entity->update = update_entity;
        }
    }
}


void move_right(struct entity* entity)
{
    switch (entity->type) {
    case QBERT:
        qbert_move_right();
        break;

    default:
        entity_move_right(entity);
        break;
    }
}


void put_entity_sprite(struct entity* entity)
{
    struct sprite_attr sa;

    switch (entity->type) {
    case QBERT: {
        sa.x = LEFT_PATTERN_MASK(qbert.pattern) ? qbert.x - 3 : qbert.x;
        sa.y = qbert.y - 1;

        //
        // Update sprite atributes
        //

        // eyes and mouth
        sa.attr = BG_BLACK;
        sa.pattern = qbert.pattern;
        spman_alloc_sprite(&sa);

        // the bod (always visible)
        sa.attr = BG_DARKYELLOW;
        sa.pattern = qbert.pattern + 4;
        spman_alloc_fixed_sprite(&sa);

        // sclera (white of the eyes)
        sa.pattern = qbert.pattern + 8;
        sa.attr = BG_WHITE;
        spman_alloc_sprite(&sa);
        break;
    }
    case BALL1:
        sa.y = entity->y;
        sa.x = entity->x;
        sa.attr = BG_DARKRED;
        sa.pattern = entity->pattern;
        spman_alloc_sprite(&sa);
	break;

    default:
        break;
    }
}
