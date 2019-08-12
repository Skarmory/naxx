#ifndef NAXX_MONTYPE_H
#define NAXX_MONTYPE_H

#include "mon_attr.h"

struct ArmourBase;
struct Weapon;
struct Symbol;

/**
 * Struct containing data about the type of a monster (not an individual monster)
 *
 * e.g. human, ghoul, night elf, etc.
 */
struct MonType
{
    char               name[32]; // Name of this type of mon
    char               desc[32];
    struct Symbol*     symbol;
    struct ArmourBase* base_armour; // All creatures have a base armour for if they don't have equipment
    struct WeaponBase* base_weapon;
    int                strength, agility, intelligence, spirit, stamina;
    int                vision_radius;
    MonAttrMoveFlags   move_flags;
};

struct MonType* mon_type_lookup_by_name(const char* name);

extern struct MonType* g_mon_type;
extern int g_mon_type_count;

#endif
