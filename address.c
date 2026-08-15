#include "address.h"

/** Addresses */
/**
  NOTES:
  - The static base address never changes on every program startup;
  - Square brackets [] mean data, stored by address
  - Rewritten for 1.6.x

*/

//
// The following addresses and offsets are essential and may differ between
// versions of PW and particular server you're playing.
// Obtained using Cheat Engine
//
// More info on finding base address:
// https://zhyk.org/forum/showthread.php?t=116666
//

DWORD_PTR offset_base = 0xAC2D58;
DWORD_PTR offset_to_character_struct = 0x68; // Offset from base address to character structure

// Offsets in character structure (suppose: Character->hp , Character->mp and etc.)
DWORD_PTR offset_target         = 0x70C;
DWORD_PTR offset_hp             = 0x62C;
DWORD_PTR offset_maxhp          = 0x688;
DWORD_PTR offset_mp             = 0x630;
DWORD_PTR offset_maxmp          = 0x68C;
DWORD_PTR offset_money          = 0x710;
DWORD_PTR offset_exp            = 0x634;
DWORD_PTR offset_maxexp         = 0x634;    // TODO: find maxexp offset
DWORD_PTR offset_lvl            = 0x624;
DWORD_PTR offset_pos_x          = 0x50;
DWORD_PTR offset_pos_y          = 0x54;
DWORD_PTR offset_pos_z          = 0x58;

DWORD_PTR address_base; // = 0x00400000 + 0x00CA30F8; // Base module address + offset
DWORD_PTR address_character_struct;
DWORD_PTR address_target;
DWORD_PTR address_hp;
DWORD_PTR address_maxhp;
DWORD_PTR address_maxmp;
DWORD_PTR address_mp;
DWORD_PTR address_exp;
DWORD_PTR address_lvl;
DWORD_PTR address_maxexp;
DWORD_PTR address_money;
DWORD_PTR address_pos_x;
DWORD_PTR address_pos_y;
DWORD_PTR address_pos_z;


