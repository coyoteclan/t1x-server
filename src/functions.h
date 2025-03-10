typedef int (*VM_Call_t)(vm_t *vm, int callnum, ...);
static const VM_Call_t VM_Call = (VM_Call_t)0x08092158;

typedef char* (*va_t)(const char *format, ...);
extern va_t va;

typedef void (*G_Error_t)(const char *fmt, ...);

typedef void (*Cbuf_ExecuteText_t)(cbufExec_t exec_when, const char* text);
static const Cbuf_ExecuteText_t Cbuf_ExecuteText = (Cbuf_ExecuteText_t)0x0805a8a0;

typedef void* (*Z_MallocInternal_t)(int size);
static const Z_MallocInternal_t Z_MallocInternal = (Z_MallocInternal_t)0x080681e8;

typedef void (*Hunk_ClearTempMemoryInternal_t)(void);
static const Hunk_ClearTempMemoryInternal_t Hunk_ClearTempMemoryInternal = (Hunk_ClearTempMemoryInternal_t)0x080686a0;

typedef qboolean (*Sys_IsLANAddress_t)(netadr_t adr);
static const Sys_IsLANAddress_t Sys_IsLANAddress = (Sys_IsLANAddress_t)0x080c72f8;

typedef char* (*SL_ConvertToString_t)(unsigned int index);
static const SL_ConvertToString_t SL_ConvertToString = (SL_ConvertToString_t)0x0809cac4;

typedef short (*BigShort_t)(short l);
static const BigShort_t BigShort = (BigShort_t)0x08083460;

typedef char* (*UI_GetMapRotationToken_t)(void);
static const UI_GetMapRotationToken_t UI_GetMapRotationToken = (UI_GetMapRotationToken_t)0x08084014;

typedef void (*ClientCommand_t)(int clientNum);
extern ClientCommand_t ClientCommand;

//// BG
////

//// Cmd
typedef char* (*Cmd_Argv_t)(int arg);
static const Cmd_Argv_t Cmd_Argv = (Cmd_Argv_t)0x0805ff28;

typedef int (*Cmd_Argc_t)();
static const Cmd_Argc_t Cmd_Argc = (Cmd_Argc_t)0x0805b24c;

//typedef void (*Cmd_AddCommand_t)(const char *cmd_name, xcommand_t function);
//static const Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)0x0805aef8;

typedef void (*Cmd_ArgvBuffer_t)(int arg, char *buffer, int bufferLength);
static const Cmd_ArgvBuffer_t Cmd_ArgvBuffer = (Cmd_ArgvBuffer_t)0x0805b27c;

typedef void (*Cmd_TokenizeString_t)(const char *text_in);
static const Cmd_TokenizeString_t Cmd_TokenizeString = (Cmd_TokenizeString_t)0x0805b398;

typedef void (*Cmd_ExecuteString_t)(const char *cmd);
static const Cmd_ExecuteString_t Cmd_ExecuteString = (Cmd_ExecuteString_t)0x0805afe0;
////

//// Com

////

//// Cvar
typedef cvar_t* (*Cvar_Set_t)(const char *var_name, const char *value);
// Cvar_Set
static const Cvar_Set_t Cvar_Set = (Cvar_Set_t)0x8073440;

typedef cvar_t* (*Cvar_Get_t)(const char *var_name, const char *var_value, unsigned short flags);
// If the variable already exists, the value will not be set unless CVAR_ROM. The flags will be or'ed in if the variable exists.
static const Cvar_Get_t Cvar_Get = (Cvar_Get_t)0x8073114;

typedef cvar_t* (*Cvar_FindVar_t)(const char *var_name);
// Cvar_FindVar
static const Cvar_FindVar_t Cvar_FindVar = (Cvar_FindVar_t)0x08072fae;

//// FS

////

//// G
//typedef void (*G_Say_t)(gentity_s *ent, gentity_s *target, int mode, const char *chatText);
//extern G_Say_t G_Say;

////

//// Get

////

//// Huff

////

//// I
typedef int (*I_strncmp_t)(const char *s1, const char *s2, int n);
static const I_strncmp_t I_strncmp = (I_strncmp_t)0x0808315c;
////

//// Info

////

//// MSG

////

//// NET

////

//// PM

////

//// Q
typedef char* (*Q_strlwr_t)(char *s1);
extern Q_strlwr_t Q_strlwr;

typedef char* (*Q_strupr_t)(char *s1);
extern Q_strupr_t Q_strupr;

typedef void (*Q_strcat_t)(char *dest, int size, const char *src);

typedef void (*Q_strncpyz_t)(char *dest, const char *src, int destsize);

typedef void (*Q_CleanStr_t)(char *string);

typedef int (*Q_stricmp_t)(const char *s1, const char *s2);
static const Q_stricmp_t Q_stricmp = (Q_stricmp_t)0x080830e8;
////

//// Scr

////

//// SV

typedef void (*SV_SpawnServer_t)(char *server);
static const SV_SpawnServer_t SV_SpawnServer = (SV_SpawnServer_t)0x0808a220;

typedef void (*SV_AuthorizeIpPacket_t)(netadr_t from);
static const SV_AuthorizeIpPacket_t SV_AuthorizeIpPacket = (SV_AuthorizeIpPacket_t)0x0808a1c5;
////

//// SVC

////

//// trap

////