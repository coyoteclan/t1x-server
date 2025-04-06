#include "gsc.h"

qboolean isValidWeaponId(int id)
{
    int weps = BG_GetNumWeapons();

    if (id > weps || id <= 0 || weps == 0)
        return qfalse;

    return qtrue;
}

// Get a weapon's string property
void gsc_weapons_getweaponstrproperty()
{
    int id;
    char *name;
    char *property;
    char *value;

    if(!stackGetParams("ss", &name, &property))
    {
        stackError("gsc_weapons_getweaponstrproperty() one or more arguments is undefined or has a wrong type");
        Scr_AddUndefined();
        return;
    }
    id = BG_GetWeaponIndexForName(name);
    if (!isValidWeaponId(id))
    {
        Scr_AddUndefined();
        return;
    }

    weaponinfo_t *weapon = BG_GetInfoForWeapon(id);
    
    if(strcmp(property, "name") == 0)
    {
        value = weapon->name;
    }
    else if(strcmp(property, "displayName") == 0)
    {
        value = weapon->displayName;
    }
    else if(strcmp(property, "modeName") == 0)
    {
        value = weapon->modeName;
    }
    else if(strcmp(property, "altWeapon") == 0)
    {
        value = weapon->altWeapon;
    }
    else
    {
        stackError("gsc_weapons_getweaponstrproperty() property %s is not valid", property);
        Scr_AddUndefined();
        return;
    }
    if (value == NULL)
    {
        stackError("gsc_weapons_getweaponstrproperty() property %s is not valid", property);
        Scr_AddUndefined();
        return;
    }

    Scr_AddString(value);
    return;
}

// Set a weapon's string property
void gsc_weapons_setweaponstrproperty()
{
    int id;
    char *name;
    char *property;
    char *value;

    if(!stackGetParams("sss", &name, &property, &value))
    {
        stackError("gsc_weapons_setweaponstrproperty() one or more arguments is undefined or has a wrong type");
        Scr_AddUndefined();
        return;
    }
    id = BG_GetWeaponIndexForName(name);
    if (!isValidWeaponId(id))
    {
        Scr_AddUndefined();
        return;
    }

    weaponinfo_t *weapon = BG_GetInfoForWeapon(id);
    
    if(strcmp(property, "name") == 0)
    {
        weapon->name = value;
    }
    else if(strcmp(property, "displayName") == 0)
    {
        weapon->displayName = value;
    }
    else if(strcmp(property, "modeName") == 0)
    {
        weapon->modeName = value;
    }
    else if(strcmp(property, "altWeapon") == 0)
    {
        weapon->altWeapon = value;
    }
    else
    {
        stackError("gsc_weapons_setweaponstrproperty() property %s is not valid", property);
        Scr_AddUndefined();
        return;
    }
    if (value == NULL)
    {
        stackError("gsc_weapons_setweaponstrproperty() property %s is not valid", property);
        Scr_AddUndefined();
        return;
    }

    Scr_AddBool(qtrue);
    return;
}

// Get a weapon's integer property
void gsc_weapons_getweaponintproperty()
{
    int id;
    char *name;
    char *property;
    int value = -1;

    if(!stackGetParams("ss", &name, &property))
    {
        stackError("gsc_weapons_getweaponintproperty() one or more arguments is undefined or has a wrong type");
        Scr_AddUndefined();
        return;
    }
    id = BG_GetWeaponIndexForName(name);
    if (!isValidWeaponId(id))
    {
        Scr_AddUndefined();
        return;
    }

    weaponinfo_t *weapon = BG_GetInfoForWeapon(id);
    
    if(strcmp(property, "startAmmo") == 0)
    {
        value = weapon->startAmmo;
    }
    else if(strcmp(property, "maxAmmo") == 0)
    {
        value = weapon->maxAmmo;
    }
    else if(strcmp(property, "clipSize") == 0)
    {
        value = weapon->clipSize;
    }
    else if(strcmp(property, "damage") == 0)
    {
        value = weapon->damage;
    }
    else if(strcmp(property, "damageInnerRadius") == 0)
    {
        value = weapon->damageInnerRadius;
    }
    else if(strcmp(property, "damageOuterRadius") == 0)
    {
        value = weapon->damageOuterRadius;
    }
    else if(strcmp(property, "fuseTime") == 0)
    {
        value = weapon->fuseTime;
    }
    else if(strcmp(property, "explosionRadius") == 0)
    {
        value = weapon->explosionRadius;
    }
    else if(strcmp(property, "explosionInnerDamage") == 0)
    {
        value = weapon->explosionInnerDamage;
    }
    else if(strcmp(property, "explosionOuterDamage") == 0)
    {
        value = weapon->explosionOuterDamage;
    }
    else
    {
        stackError("gsc_weapons_getweaponintproperty() property %s is not valid", property);
        Scr_AddUndefined();
        return;
    }
    if (value == -1)
    {
        stackError("gsc_weapons_getweaponintproperty() property %s is not valid", property);
        Scr_AddUndefined();
        return;
    }

    Scr_AddInt(value);
    return;
}

// Set a weapon's integer property
void gsc_weapons_setweaponintproperty()
{
    int id;
    char *name;
    char *property;
    int value = -1;

    if(!stackGetParams("ssi", &name, &property, &value))
    {
        stackError("gsc_weapons_setweaponintproperty() one or more arguments is undefined or has a wrong type");
        Scr_AddUndefined();
        return;
    }
    id = BG_GetWeaponIndexForName(name);
    if (!isValidWeaponId(id))
    {
        Scr_AddUndefined();
        return;
    }

    weaponinfo_t *weapon = BG_GetInfoForWeapon(id);
    
    if(strcmp(property, "startAmmo") == 0)
    {
        weapon->startAmmo = value;
    }
    else if(strcmp(property, "maxAmmo") == 0)
    {
        weapon->maxAmmo = value;
    }
    else if(strcmp(property, "clipSize") == 0)
    {
        weapon->clipSize = value;
    }
    else if(strcmp(property, "damage") == 0)
    {
        weapon->damage = value;
        printf("[DEBUG] set weapon->damage = %i\n", value);
    }
    else if(strcmp(property, "damageInnerRadius") == 0)
    {
        weapon->damageInnerRadius = value;
    }
    else if(strcmp(property, "damageOuterRadius") == 0)
    {
        weapon->damageOuterRadius = value;
    }
    else if(strcmp(property, "fuseTime") == 0)
    {
        weapon->fuseTime = value;
    }
    else if(strcmp(property, "explosionRadius") == 0)
    {
        weapon->explosionRadius = value;
    }
    else if(strcmp(property, "explosionInnerDamage") == 0)
    {
        weapon->explosionInnerDamage = value;
    }
    else if(strcmp(property, "explosionOuterDamage") == 0)
    {
        weapon->explosionOuterDamage = value;
    }
    else
    {
        stackError("gsc_weapons_setweaponintproperty() property %s is not valid", property);
        Scr_AddUndefined();
        return;
    }
    if (value == -1)
    {
        stackError("gsc_weapons_setweaponintproperty() property %s is not valid", property);
        Scr_AddUndefined();
        return;
    }

    Scr_AddBool(qtrue);
    return;
}
