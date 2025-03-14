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
static const Cmd_TokenizeString_t Cmd_TokenizeString = (Cmd_TokenizeString_t)0x08060257;
////

//// Com

typedef void (*Com_Printf_t)(const char *format, ...);
static const Com_Printf_t Com_Printf = (Com_Printf_t)0x08070248;

typedef void (*Com_DPrintf_t)(const char *format, ...);
static const Com_DPrintf_t Com_DPrintf = (Com_DPrintf_t)0x08070297;

typedef void (*Com_Error_t)(errorParm_t code, const char *format, ...);
static const Com_Error_t Com_Error = (Com_Error_t)0x080704ac;

typedef char* (*Com_Parse_t)(const char **data_p);
static const Com_Parse_t Com_Parse = (Com_Parse_t)0x08085e0b;

typedef void (*Com_SkipRestOfLine_t)(const char **data);

typedef int (*Com_ParseInt_t)(const char **data);

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

typedef int (*FS_ReadFile_t)(const char* qpath, void** buffer);
static const FS_ReadFile_t FS_ReadFile = (FS_ReadFile_t)0x08062eb0; //FUN_08062eb0

typedef void (*FS_FreeFile_t)(void* buffer);
static const FS_FreeFile_t FS_FreeFile = (FS_FreeFile_t)0x080629f4;//FUN_08063207

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

typedef char* (*Info_ValueForKey_t)(const char *s, const char *key);
static const Info_ValueForKey_t Info_ValueForKey = (Info_ValueForKey_t)0x08086e7f;

////

//// MSG

////

//// NET

typedef const char* (*NET_AdrToString_t)(netadr_t a);
static const NET_AdrToString_t NET_AdrToString = (NET_AdrToString_t)0x080848f8;

typedef void (*NET_OutOfBandPrint_t)(netsrc_t net_socket, netadr_t adr, const char *format, ...);
static const NET_OutOfBandPrint_t NET_OutOfBandPrint = (NET_OutOfBandPrint_t)0x08084d76;

typedef int (*NET_CompareAdrSigned_t)(netadr_t *a, netadr_t *b);
static const NET_CompareAdrSigned_t NET_CompareAdrSigned = (NET_CompareAdrSigned_t)0x080849fe;

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

typedef void (*SV_DirectConnect_t)(netadr_t from);
static const SV_DirectConnect_t SV_DirectConnect = (SV_DirectConnect_t)0x0808ac82;
////

//// SVC

////

//// trap

////