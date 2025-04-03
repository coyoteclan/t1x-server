typedef int (*VM_Call_t)(vm_t *vm, int callnum, ...);
static const VM_Call_t VM_Call = (VM_Call_t)0x08092158;

typedef char* (*va_t)(const char *format, ...);
extern va_t va;

typedef void (*G_Error_t)(const char *fmt, ...);

typedef void (*Cbuf_ExecuteText_t)(cbufExec_t exec_when, const char* text);
static const Cbuf_ExecuteText_t Cbuf_ExecuteText = (Cbuf_ExecuteText_t)0x0805faf4;

typedef void* (*Z_MallocInternal_t)(int size);
static const Z_MallocInternal_t Z_MallocInternal = (Z_MallocInternal_t)0x080681e8;

typedef void (*Hunk_ClearTempMemoryInternal_t)(void);
static const Hunk_ClearTempMemoryInternal_t Hunk_ClearTempMemoryInternal = (Hunk_ClearTempMemoryInternal_t)0x080686a0;

typedef qboolean (*Sys_IsLANAddress_t)(netadr_t adr);
static const Sys_IsLANAddress_t Sys_IsLANAddress = (Sys_IsLANAddress_t)0x080c72f8;

typedef char* (*SL_ConvertToString_t)(unsigned int index);
static const SL_ConvertToString_t SL_ConvertToString = (SL_ConvertToString_t)0x080a4458;

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
static const Cmd_Argc_t Cmd_Argc = (Cmd_Argc_t)0x0805ff1e;

typedef void (*Cmd_AddCommand_t)(const char *cmd_name, xcommand_t function);
static const Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)0x08060272;

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

typedef void (*Com_PrintMessage_t)(int channel, const char *message);
static const Com_PrintMessage_t Com_PrintMessage = (Com_PrintMessage_t)0x0806fe5f;

typedef void (*Com_Error_t)(errorParm_t code, const char *format, ...);
static const Com_Error_t Com_Error = (Com_Error_t)0x080704ac;

typedef char* (*Com_Parse_t)(const char **data_p);
static const Com_Parse_t Com_Parse = (Com_Parse_t)0x08085e0b;

typedef void (*Com_SkipRestOfLine_t)(const char **data);
static const Com_SkipRestOfLine_t Com_SkipRestOfLine = (Com_SkipRestOfLine_t)0x08085fba;

typedef char* (*Com_ParseRestOfLine_t)(const char **data);

typedef int (*Com_ParseInt_t)(const char **data);
static const Com_ParseInt_t Com_ParseInt = (Com_ParseInt_t)0x080860ad;

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
static const FS_FreeFile_t FS_FreeFile = (FS_FreeFile_t)0x08063207;//writefile: 0x080629f4;//FUN_08063207

typedef int (*FS_FOpenFileByMode_t)(const char *qpath, fileHandle_t *f, fsMode_t mode);
static const FS_FOpenFileByMode_t FS_FOpenFileByMode = (FS_FOpenFileByMode_t)0x08065d85;

//typedef void (*FS_Write_t)(fileHandle_t h, const char *fmt, ...);
typedef void (*FS_Write_t)(void *buffer, size_t len, fileHandle_t f_handle);
static const FS_Write_t FS_Write = (FS_Write_t)0x08062983;

typedef void (*FS_FCloseFile_t)(fileHandle_t f);
static const FS_FCloseFile_t FS_FCloseFile = (FS_FCloseFile_t)0x080610d6;

typedef void (*FS_WriteFile_t)(const char* filename, const void* buffer, int size);
static const FS_WriteFile_t FS_WriteFile = (FS_WriteFile_t)0x0806323f;

////

//// G
//typedef void (*G_Say_t)(gentity_s *ent, gentity_s *target, int mode, const char *chatText);
//extern G_Say_t G_Say;

typedef int (*G_LocalizedStringIndex_t)(const char *string);
extern G_LocalizedStringIndex_t G_LocalizedStringIndex;

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

typedef bool (*NET_CompareAdr_maybe_t)(
    uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5,  // first netadr_t
    uint32_t b1, uint32_t b2, uint32_t b3, uint32_t b4, uint32_t b5   // second netadr_t
);

//// NET

typedef const char* (*NET_AdrToString_t)(netadr_t a);
static const NET_AdrToString_t NET_AdrToString = (NET_AdrToString_t)0x080848f8;

typedef void (*NET_OutOfBandPrint_t)(netsrc_t net_socket, netadr_t adr, const char *format, ...);
static const NET_OutOfBandPrint_t NET_OutOfBandPrint = (NET_OutOfBandPrint_t)0x08084d76;

typedef int (*NET_CompareAdrSigned_t)(netadr_t *a, netadr_t *b);
static const NET_CompareAdrSigned_t NET_CompareAdrSigned = (NET_CompareAdrSigned_t)0x080849fe;

//typedef qboolean (*NET_CompareAdr_maybe_t)(netadr_t a, netadr_t b);
//static const NET_CompareAdr_maybe_t NET_CompareAdr_maybe = (NET_CompareAdr_maybe_t)0x08084af7;//08084af7

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
static const Q_strncpyz_t Q_strncpyz = (Q_strncpyz_t)0x0808691a;

typedef void (*Q_CleanStr_t)(char *string);

//typedef int (*Q_stricmp_t)(const char *s1, const char *s2);
//static const Q_stricmp_t Q_stricmp = (Q_stricmp_t)0x0;//0x080830e8;
////

//// Scr

typedef xfunction_t (*Scr_GetFunction_t)(const char** v_functionName, qboolean *v_developer);
extern Scr_GetFunction_t Scr_GetFunction;

typedef xmethod_t (*Scr_GetMethod_t)(const char** v_methodName, qboolean *v_developer);
extern Scr_GetMethod_t Scr_GetMethod;

typedef void (*Scr_Error_t)(const char *string);
extern Scr_Error_t Scr_Error;

typedef short (*Scr_ExecThread_t)(int callbackHook, unsigned int numArgs);
extern Scr_ExecThread_t Scr_ExecThread;

typedef short (*Scr_FreeThread_t)(short thread_id);
extern Scr_FreeThread_t Scr_FreeThread;

typedef short (*Scr_ExecEntThread_t)(gentity_t* ent, int callbackHook, unsigned int numArgs);
extern Scr_ExecEntThread_t Scr_ExecEntThread;

typedef void (*Scr_AddBool_t)(qboolean value);
extern Scr_AddBool_t Scr_AddBool;

typedef void (*Scr_AddInt_t)(int value);
extern Scr_AddInt_t Scr_AddInt;

typedef void (*Scr_AddFloat_t)(float value);
extern Scr_AddFloat_t Scr_AddFloat;

typedef void (*Scr_AddString_t)(const char *string);
extern Scr_AddString_t Scr_AddString;

typedef void (*Scr_AddUndefined_t)(void);
extern Scr_AddUndefined_t Scr_AddUndefined;

typedef void (*Scr_AddVector_t)(vec3_t vec);
extern Scr_AddVector_t Scr_AddVector;

typedef void (*Scr_MakeArray_t)(void);
extern Scr_MakeArray_t Scr_MakeArray;

typedef void (*Scr_AddArray_t)(void);
extern Scr_AddArray_t Scr_AddArray;

typedef void (*Scr_AddObject_t)(unsigned int object);
extern Scr_AddObject_t Scr_AddObject;

typedef unsigned int (*Scr_LoadScript_t)(const char *filename);

typedef int (*Scr_GetFunctionHandle_t)(const char* scriptName, const char* labelName);

typedef int (*Scr_GetInt_t)(unsigned int param);
extern Scr_GetInt_t Scr_GetInt;

typedef const char* (*Scr_GetString_t)(unsigned int param);
extern Scr_GetString_t Scr_GetString;

typedef int (*Scr_GetNumParam_t)(void);
extern Scr_GetNumParam_t Scr_GetNumParam;

typedef int (*Scr_IsSystemActive_t)();
extern Scr_IsSystemActive_t Scr_IsSystemActive;

typedef int (*Scr_GetPointerType_t)(unsigned int param);
extern Scr_GetPointerType_t Scr_GetPointerType;

////

//// SV

typedef void (*SV_SpawnServer_t)(char *server);
static const SV_SpawnServer_t SV_SpawnServer = (SV_SpawnServer_t)0x08091b72;

typedef void (*SV_Startup_t)(void);
static const SV_Startup_t SV_Startup = (SV_Startup_t)0x08091473;

typedef void (*SV_AuthorizeIpPacket_t)(netadr_t from);
static const SV_AuthorizeIpPacket_t SV_AuthorizeIpPacket = (SV_AuthorizeIpPacket_t)0x0808a1c5;

typedef void (*SV_DirectConnect_t)(netadr_t from);
static const SV_DirectConnect_t SV_DirectConnect = (SV_DirectConnect_t)0x0808ac82;

typedef void (*SV_ConnectionlessPacket_t)(netadr_t from, msg_t *msg);
static const SV_ConnectionlessPacket_t SV_ConnectionlessPacket = (SV_ConnectionlessPacket_t)0x0;

typedef void (*SV_SendServerCommand_t)(client_t *cl, int type, const char *fmt, ...);
static const SV_SendServerCommand_t SV_SendServerCommand = (SV_SendServerCommand_t)0x08092f86;

typedef void (*SV_DropClient_t)(client_t *drop, const char *reason);
static const SV_DropClient_t SV_DropClient = (SV_DropClient_t)0x0808ba15;

////

//// SVC

////

//// trap

typedef void (*trap_SendServerCommand_t)(int clientnum, svscmd_type type, const char *text);
extern trap_SendServerCommand_t trap_SendServerCommand;

typedef void (*trap_Argv_t)(int arg, char *buffer, int bufferLength);

typedef const char* (*trap_GetConfigstringConst_t)(int index);
extern trap_GetConfigstringConst_t trap_GetConfigstringConst;

typedef void (*trap_GetConfigstring_t)(int index, char *buffer, int bufferSize);
extern trap_GetConfigstring_t trap_GetConfigstring;

typedef void (*trap_SetConfigstring_t)(int index, const char *val);
extern trap_SetConfigstring_t trap_SetConfigstring;

////