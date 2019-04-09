#ifndef CHARACTER_LISTINGS_H
#define CHARACTER_LISTINGS_H

// Unknown character
#define UNKNOWN_CHARACTER '?'

// Border characters
#define NORTH_SOUTH_WALL_CHARACTER '-'
#define EAST_WEST_WALL_CHARACTER '|'
#define CORNER_WALL_CHARACTER '+'
#define GENERAL_WALL_CHARACTER 'B'

// Floor characters
#define ROCK_CHARACTER ' '
#define CORRIDOR_CHARACTER '#'

// Player characters
#define PLAYER_TELEPORT_CHARACTER '*'
#define PLAYER_CHARACTER '@'

// Room characters
#define ROOM_CHARACTER '.'

// Staircase characters
#define STAIRCASE_UP_CHARACTER '<'
#define STAIRCASE_DOWN_CHARACTER '>'

// Object characters
#define WEAPON_CHARACTER        '|'
#define OFFHAND_CHARACTER       ')'
#define RANGED_CHARACTER        '}'
#define ARMOR_CHARACTER         '['
#define HELMET_CHARACTER        ']'
#define CLOAK_CHARACTER         '('
#define GLOVES_CHARACTER        '{'
#define BOOTS_CHARACTER         '\\'
#define RING_CHARACTER          '='
#define AMULET_CHARACTER        '"'
#define LIGHT_CHARACTER         '_'
#define SCROLL_CHARACTER        '~'
#define BOOK_CHARACTER          '?'
#define FLASK_CHARACTER         '!'
#define GOLD_CHARACTER          '$'
#define AMMUNITION_CHARACTER    '/'
#define FOOD_CHARACTER          ','
#define WAND_CHARACTER          '-'
#define CONTAINER_CHARACTER     '%'

#define ROCK_HARDNESS_MIN 50
#define ROCK_HARDNESS_MAX 225
#define BORDER_HARDNESS 255
#define ROCK_HARDNESS 100
#define ROOM_HARDNESS 0
#define CORRIDOR_HARDNESS 0
#define PLAYER_HARDNESS 0
#define STAIRCASE_HARDNESS 0
#define MONSTER_HARDNESS 0

#endif
