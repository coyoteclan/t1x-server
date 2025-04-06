#include "gsc.h"

const char * getParamTypeAsString(int type)
{
    switch (type)
    {
        case 0: return "undefined";
        case 1: return "string";
        case 2: return "localized string";
        case 3: return "vector";
        case 4: return "float";
        case 5: return "int";
        case 6: return "codepos";
        case 7: return "object";
        case 8: return "key/value";
        case 9: return "function";
        case 10: return "stack";
        case 11: return "animation";
        case 12: return "thread";
        case 13: return "entity";
        case 14: return "struct";
        case 15: return "array";
        case 16: return "dead thread";
        case 17: return "dead entity";
        case 18: return "dead object";
        default: return "unknown type";
    }
}

const char * stackGetParamTypeAsString(int param)
{
    if(param >= Scr_GetNumParam())
        return "undefined";

    VariableValue *var;
    var = &scrVmPub.top[-param];

    if(var->type == 7) // Pointer to object
        return getParamTypeAsString(Scr_GetPointerType(param));
    else
        return getParamTypeAsString(var->type);
}

scr_function_t scriptFunctions[] =
{
#if ENABLE_UNSAFE == 1
    {"file_exists", gsc_utils_file_exists, 0},
    {"fopen", gsc_utils_fopen, 0},
    {"fwrite", gsc_utils_fwrite, 0},
    {"fread", gsc_utils_fread, 0},
    {"fclose", gsc_utils_fclose, 0},//*/
#endif

    //{"execute", gsc_exec, 0},





    {"sendCommandToClient", gsc_utils_sendcommandtoclient, 0},
    {"logPrintConsole", gsc_utils_logprintconsole, 0},
    {"getSubStr", gsc_utils_getsubstr, 0, },
    {"getAscii", gsc_utils_getascii, 0},
    {"toUpper", gsc_utils_toupper, 0},
    {"toLower", gsc_utils_tolower, 0},//*/
    {"strtok", gsc_utils_strtok, 0},
    {"replace", gsc_utils_replace, 0},
    {"getServerStartTime", gsc_utils_getserverstarttime, 0},
    {"getSystemTime", gsc_utils_getsystemtime, 0},
    {"strftime", gsc_utils_strftime, 0},
    {"getConfigString", gsc_utils_getconfigstring, 0},
    {"makeLocalizedString", gsc_utils_makelocalizedstring, 0},
    {"getLocalizedStringIndex", gsc_utils_getlocalizedstringindex, 0},
    {"makeUpdatedLocalizedString", gsc_utils_makeupdatedlocalizedstring, 0},
    {"ban", gsc_utils_ban, 0},
    {"unban", gsc_utils_unban, 0},
    {"strip", gsc_utils_strip, 0},
    {"strstr", gsc_utils_strstr, 0},
    {"monotone", gsc_utils_monotone, 0},
    {"getType", gsc_utils_gettype, 0},

    // Weapons
    {"getWeaponStrProperty", gsc_weapons_getweaponstrproperty, 0},
    {"setWeaponStrProperty", gsc_weapons_setweaponstrproperty, 0},
    {"getWeaponIntProperty", gsc_weapons_getweaponintproperty, 0},
    {"setWeaponIntProperty", gsc_weapons_setweaponintproperty, 0},
    //

#if COMPILE_SQLITE == 1

#if 0

#endif
#endif

#if COMPILE_CURL == 1
    {"webhookMessage", gsc_curl_webhookmessage, 0},
#endif

#if COMPILE_SSL == 1
    {"hash", gsc_utils_hash, 0},
#endif
    {"testFunction", gsc_testfunction, 0},
    {NULL, NULL, 0} // Terminator
};

xfunction_t Scr_GetCustomFunction(const char **fname, int *fdev)
{
    xfunction_t m = Scr_GetFunction(fname, fdev);
    if(m)
        return m;

    for (int i = 0; scriptFunctions[i].name; i++)
    {
        if(strcasecmp(*fname, scriptFunctions[i].name))
            continue;

        scr_function_t func = scriptFunctions[i];
        *fname = func.name;
        *fdev = func.developer;
        return func.call;
    }
    return NULL;
}

scr_method_t scriptMethods[] =
{
    //// Entity
    ////

    //// Player
    {"processClientCommand", gsc_player_processclientcommand, 0},
    {"connectionlessPacketToClient", gsc_player_connectionlesspackettoclient, 0},
    ////

    //// Bot
    ////

#if COMPILE_SQLITE == 1
#endif

    {"testMethod", gsc_testmethod, 0},
    {NULL, NULL, 0} // Terminator
};

xmethod_t Scr_GetCustomMethod(const char **fname, qboolean *fdev)
{
    xmethod_t m = Scr_GetMethod(fname, fdev);
    if(m)
        return m;

    for (int i = 0; scriptMethods[i].name; i++)
    {
        if(strcasecmp(*fname, scriptMethods[i].name))
            continue;
        
        scr_method_t func = scriptMethods[i];
        *fname = func.name;
        *fdev = func.developer;
        return func.call;
    }
    return NULL;
}

void stackError(const char *format, ...)
{
    char s[MAX_STRINGLENGTH];
    int len = 0;
    va_list va;

    va_start(va, format);
    vsnprintf(s, sizeof(s) - 1, format, va);
    va_end(va);

    len = strlen(s);
    s[len] = '\n';
    s[len + 1] = '\0';
    Com_PrintMessage(0, s);
    Scr_CodeCallback_Error(qfalse, qfalse, "stackError", s);
}

int stackGetParams(const char *params, ...)
{
    printf("stackGetParams: %s\n", params);
    va_list args;
    va_start(args, params);

    int errors = 0;

    for (size_t i = 0; i < strlen(params); i++)
    {
        switch (params[i])
        {
        case ' ': // Ignore param
            break;

        case 'i':
        {
            int *tmp = va_arg(args, int *);
            if (!stackGetParamInt(i, tmp))
            {
                Com_DPrintf("\nstackGetParams() Param %i is not an int\n", i);
                errors++;
            }
            break;
        }

        case 'v':
        {
            float *tmp = va_arg(args, float *);
            if (!stackGetParamVector(i, tmp))
            {
                Com_DPrintf("\nstackGetParams() Param %i is not a vector\n", i);
                errors++;
            }
            break;
        }

        case 'f':
        {
            float *tmp = va_arg(args, float *);
            if (!stackGetParamFloat(i, tmp))
            {
                Com_DPrintf("\nstackGetParams() Param %i is not a float\n", i);
                errors++;
            }
            break;
        }

        case 's':
        {

            char **tmp = va_arg(args, char **);
            if (!stackGetParamString(i, tmp))
            {
                Com_DPrintf("\nstackGetParams() Param %i is not a string\n", i);
                errors++;
            }
            break;
        }

        case 'c':
        {
            unsigned int *tmp = va_arg(args, unsigned int *);
            if (!stackGetParamConstString(i, tmp))
            {
                Com_DPrintf("\nstackGetParams() Param %i is not a const string\n", i);
                errors++;
            }
            break;
        }

        case 'l':
        {
            char **tmp = va_arg(args, char **);
            if (!stackGetParamLocalizedString(i, tmp))
            {
                Com_DPrintf("\nstackGetParams() Param %i is not a localized string\n", i);
                errors++;
            }
            break;
        }

        default:
            errors++;
            Com_DPrintf("\nUnknown identifier [%c] passed to stackGetParams()\n", params[i]);
            break;
        }
    }

    va_end(args);
    return errors == 0; // success if no errors
}

int stackGetParamInt(int param, int *value)
{
    if(param >= Scr_GetNumParam())
        return 0;

    VariableValue *var;
    var = &scrVmPub.top[-param];

    if (var->type == STACK_FLOAT)
    {
        *value = var->u.floatValue;
        return 1;
    }

    if(var->type != STACK_INT)
        return 0;

    *value = var->u.intValue;

    return 1;
}

int stackGetParamFunction(int param, int *value)
{
    printf("####### stackGetParamFunction\n");

    if(param >= Scr_GetNumParam())
        return 0;

    VariableValue *var;
    var = &scrVmPub.top[-param];

    if(var->type != STACK_FUNCTION)
        return 0;

    *value = var->u.codePosValue - scrVarPub.programBuffer;

    return 1;
}

int stackGetParamString(int param, char **value)
{
    //printf("######### stackGetParamString: %d, %s\n", param, (char *)value);
    if(param >= Scr_GetNumParam())
        return 0;

    VariableValue *var;
    //printf("######## stackGetParamString: before scrVmPub.top\n");
    var = &scrVmPub.top[-param];
    //printf("######## stackGetParamString: after scrVmPub.top\n");
    //printf("######## stackGetParamString: var->type %d\n", var->type);

    if(var->type != STACK_STRING)
        return 0;

    *value = SL_ConvertToString(var->u.stringValue);
    //printf("######## stackGetParamString: after SL_ConvertToString\n");

    return 1;
}

int stackGetParamConstString(int param, unsigned int *value)
{
    if(param >= Scr_GetNumParam())
        return 0;

    VariableValue *var;
    var = &scrVmPub.top[-param];

    if(var->type != STACK_STRING)
        return 0;

    *value = var->u.stringValue;

    return 1;
}

int stackGetParamLocalizedString(int param, char **value)
{
    if(param >= Scr_GetNumParam())
        return 0;

    VariableValue *var;
    var = &scrVmPub.top[-param];

    if(var->type != STACK_LOCALIZED_STRING)
        return 0;

    *value = SL_ConvertToString(var->u.stringValue);

    return 1;
}

int stackGetParamVector(int param, vec3_t value)
{
    if(param >= Scr_GetNumParam())
        return 0;

    VariableValue *var;
    var = &scrVmPub.top[-param];

    if(var->type != STACK_VECTOR)
        return 0;

    VectorCopy(var->u.vectorValue, value);

    return 1;
}

int stackGetParamFloat(int param, float *value)
{
    if(param >= Scr_GetNumParam())
        return 0;

    VariableValue *var;
    var = &scrVmPub.top[-param];

    if (var->type == STACK_INT)
    {
        *value = var->u.intValue;
        return 1;
    }

    if(var->type != STACK_FLOAT)
        return 0;

    *value = var->u.floatValue;

    return 1;
}

int stackGetParamObject(int param, unsigned int *value)
{
    if(param >= Scr_GetNumParam())
        return 0;

    VariableValue *var;
    var = &scrVmPub.top[-param];

    if(var->type != STACK_OBJECT)
        return 0;

    *value = var->u.pointerValue;

    return 1;
}


// For tests
void gsc_testfunction()
{
    printf("##### gsc_testfunction\n");
}
void gsc_testmethod(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_getip() entity %i is not a player", id);
        Scr_AddUndefined();
        return;
    }

    client_t *client = &svs.clients[id];
    char ip[64];
    
    snprintf(ip, sizeof(ip), "%s: %d.%d.%d.%d",
        client->name,
        client->netchan.remoteAddress.ip[0],
        client->netchan.remoteAddress.ip[1],
        client->netchan.remoteAddress.ip[2],
        client->netchan.remoteAddress.ip[3]);

    Scr_AddString(ip);
    
    playerState_t *ps = SV_GameClientNum(id);
    //gentity_t *gentity = &g_entities[id];
    weaponinfo_t *weapon = BG_GetInfoForWeapon(ps->weapon);
    
    printf("###### ps->clientNum = %i\n", ps->clientNum);
    //printf("###### ps->bobCycle = %i\n", ps->bobCycle);
    printf("###### ps->pm_type = %i\n", ps->pm_type);
    //printf("###### ps->pm_flags = %i\n", ps->pm_flags);
    printf("###### ps->gravity = %i\n", ps->gravity);
    printf("###### ps->speed = %i\n", ps->speed);
    printf("###### ps->weapon = %i\n", ps->weapon);
    printf("###### weapon->name = %s\n", weapon->name);
    printf("###### weapon->displayName = %s\n", weapon->displayName);
    printf("###### weapon->damage = %i\n", weapon->damage);
    printf("###### weapon->number = %i\n", weapon->number);

    /*printf("###### ps->weapon = %i\n", ps->weapon);
    printf("###### weapon->maxAmmo = %i\n", weapon->maxAmmo);*/



    


}