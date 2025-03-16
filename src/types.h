//#include <stdint.h>
#define qboolean int
#define qtrue   1
#define qfalse  0

// 3D vectors
#define VectorCopy(a, b)        ((b)[0] = (a)[0], (b)[1] = (a)[1], (b)[2] = (a)[2])
#define VectorScale(v, s, o)    ((o)[0] = (v)[0] * (s),(o)[1] = (v)[1] * (s),(o)[2] = (v)[2] * (s))

#define BIG_INFO_STRING 0x2000
#define GENTITYNUM_BITS 10
#define PACKET_BACKUP 32
#define PACKET_MASK (PACKET_BACKUP - 1)
#define SV_OUTPUTBUF_LENGTH (2048 * MAX_CLIENTS - 16) // Stock = (256 * MAX_CLIENTS - 16)
#define HEADER_RATE_BYTES 48
#define SNAPFLAG_SERVERCOUNT 4
#define GAME_INIT_FRAMES 3
#define FRAMETIME 100
#define PORT_MASTER 20510
#define PORT_AUTHORIZE 20500
#define	HEARTBEAT_MSEC 180000
#define HMAX 256
#define AUTHORIZE_TIMEOUT 5000
#define AUTHORIZE_SERVER_NAME "codauthorize.activision.com"

#define MAX_BPS_WINDOW              20
#define MAX_CHALLENGES              1024
#define MAX_CLIENTS                 64
#define MAX_CONFIGSTRINGS           2048
#define MAX_DOWNLOAD_BLKSIZE        2048
#define MAX_DOWNLOAD_BLKSIZE_FAST   0x2000 // See https://github.com/ibuddieat/zk_libcod/blob/dff123fad25d7b46d65685e9bca2111c8946a36e/code/declarations.hpp#L60
#define MAX_DOWNLOAD_WINDOW         8
#define MAX_ENT_CLUSTERS            16
#define MAX_GENTITIES               (1 << GENTITYNUM_BITS)
#define MAX_INFO_STRING             0x400
#define MAX_MSGLEN                  0x4000
#define MAX_NETNAME                 36
#define MAX_OSPATH                  256
#define MAX_QPATH                   64
#define	MAX_NAME_LENGTH             32
#define MAX_RELIABLE_COMMANDS       64
#define MAX_STRINGLENGTH            1024
#define MAX_MASTER_SERVERS          5
#define MAX_WEAPONS                 64
#define MAX_OBJECTIVES              16
#define MAX_HUDELEMENTS             31
#define MAX_HUDELEMS_ARCHIVAL       MAX_HUDELEMENTS
#define MAX_HUDELEMS_CURRENT        MAX_HUDELEMENTS
#define MAX_CVAR_VALUE_STRING       256

#define CVAR_NOFLAG             0               // 0
#define CVAR_ARCHIVE            (1 << 0)        // 1
#define CVAR_SERVERINFO         (1 << 2)        // 4
#define CVAR_SYSTEMINFO         (1 << 3)        // 8
#define CVAR_INIT               (1 << 4)        // 16
#define CVAR_LATCH              (1 << 5)        // 32
#define CVAR_CHEAT              (1 << 9)        // 512

#define SVF_SINGLECLIENT 0x800

#define KEY_MASK_NONE           0
#define KEY_MASK_FORWARD        127
#define KEY_MASK_BACK           -127
#define KEY_MASK_MOVERIGHT      127
#define KEY_MASK_MOVELEFT       -127
#define KEY_MASK_JUMP           127 // upmove. prone and jump = -KEY_MASK_JUMP
#define KEY_MASK_FIRE           0x1
#define KEY_MASK_RELOAD         0x8
#define KEY_MASK_LEANLEFT       0x10
#define KEY_MASK_ADS_MODE       0x10
#define KEY_MASK_LEANRIGHT      0x20
#define KEY_MASK_MELEE          0x20
#define KEY_MASK_USE            0x40
#define KEY_MASK_PRONE          0x40 // wbuttons
#define KEY_MASK_CROUCH         0x80 // wbuttons

#define EF_CROUCHING    0x20
#define EF_PRONE        0x40

#define PMF_PRONE       0x1
#define PMF_CROUCH      0x2
#define PMF_LADDER      0x10
#define PMF_SLIDING     0x100

//typedef void (*xcommand_t)(void);

typedef unsigned char byte;
typedef struct gclient_s gclient_t;
typedef struct gentity_s gentity_t;

struct vm_t;

typedef struct scr_entref_s
{
    uint16_t entnum;
    uint16_t classnum;
} scr_entref_t;

typedef enum
{
    ERR_FATAL = 0x0,
    ERR_VID_FATAL = 0x1,
    ERR_DROP = 0x2,
    ERR_SERVERDISCONNECT = 0x3,
    ERR_DISCONNECT = 0x4,
    ERR_NEED_CD = 0x5,
    ERR_AUTOUPDATE = 0x6,
} errorParm_t;

enum svc_ops_e
{
    svc_bad,
    svc_nop,
    svc_gamestate,
    svc_configstring,
    svc_baseline,
    svc_serverCommand,
    svc_download,
    svc_snapshot,
    svc_EOF
};

typedef enum
{
    TEAM_FREE = 0x0,
    TEAM_NONE = 0x0,
    TEAM_BAD = 0x0,
    TEAM_AXIS = 0x1,
    TEAM_ALLIES = 0x2,
    TEAM_SPECTATOR = 0x3,
    TEAM_NUM_TEAMS = 0x4,
} team_t;

typedef enum
{
    CS_FREE,
    CS_ZOMBIE,
    CS_CONNECTED,
    CS_PRIMED,
    CS_ACTIVE
} clientConnectState_t;

typedef enum
{
    STATE_PLAYING,
    STATE_DEAD,
    STATE_SPECTATOR,
    STATE_INTERMISSION
} sessionState_t;

typedef enum
{
    CON_DISCONNECTED,
    CON_CONNECTING,
    CON_CONNECTED
} clientConnected_t;

typedef enum
{
    NA_BOT = 0,
    NA_BAD = 1,
    NA_LOOPBACK = 2,
    NA_BROADCAST = 3,
    NA_IP = 4,
    NA_IPX = 5,
    NA_BROADCAST_IPX = 6
} netadrtype_t;

typedef enum {
	NS_CLIENT,
	NS_SERVER
} netsrc_t;

typedef struct
{
    netadrtype_t type;
    byte ip[4];
    byte ipx[10];
    unsigned short port;
} netadr_t;

typedef struct
{
    qboolean overflowed;
    byte *data;
    int maxsize;
    int cursize;
    int readcount;
    int bit;
} msg_t;

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];

typedef struct cvar_s
{
    char *name;
    char *string;
    char *resetString;
    char *latchedString;
    int flags;
    qboolean modified;
    int modificationCount;
    float value;
    int integer;
    struct cvar_s *next;
    struct cvar_s *hashNext;
} cvar_t;

typedef int cvarHandle_t;

typedef struct
{
    cvarHandle_t handle;
    int modificationCount;
    float value;
    int integer;
    char string[MAX_CVAR_VALUE_STRING];
} vmCvar_t;

union VariableUnion
{
    int intValue;
    float floatValue;
    unsigned int stringValue;
    const float *vectorValue;
    const char *codePosValue;
    unsigned int pointerValue;
    //...
};

typedef struct
{
    union VariableUnion u;
    int type;
} VariableValue;

typedef struct
{
    const char *fieldBuffer;
    byte gap[0x4176];
    unsigned int levelId;
    //...
    const char *programBuffer;
    //... 
} scrVarPub_t; // TODO: finish setup

typedef struct
{
    unsigned int *localVars;
    byte gap[356];
    VariableValue *top;
    //...
} scrVmPub_t; // TODO: verify

typedef int fileHandle_t;
typedef void *unzFile;
typedef void (*xfunction_t)();
typedef void (*xmethod_t)(scr_entref_t);

typedef struct scr_function_s
{
    const char *name;
    xfunction_t call;
    qboolean developer;
} scr_function_t;

typedef struct scr_method_s
{
    const char *name;
    xmethod_t call;
    qboolean developer;
} scr_method_t;

struct directory_t
{
    char path[MAX_OSPATH];
    char gamedir[MAX_OSPATH];
};

enum fsMode_t
{
    FS_READ,
    FS_WRITE,
    FS_APPEND,
    FS_APPEND_SYNC
};

struct pack_t
{
    char pakFilename[MAX_OSPATH];
    char pakBasename[MAX_OSPATH];
    char pakGamename[MAX_OSPATH];
    unzFile handle;
    int checksum;
    int pure_checksum;
    int numFiles;
    int referenced;
    int hashSize;
    //...
};

struct searchpath_t
{
    searchpath_t *next;
    pack_t *pak;
    directory_t *dir;
    //...
};

typedef enum
{
    EV_STANCE_FORCE_STAND = 0x8c,
    // ...
} entity_event_t;

typedef struct nodetype
{
    struct nodetype *left, *right, *parent;
    struct nodetype *next, *prev;
    struct nodetype **head;
    int weight;
    int symbol;
} node_t;

typedef struct
{
    int blocNode;
    int blocPtrs;
    node_t* tree;
    node_t* lhead;
    node_t* ltail;
    node_t* loc[HMAX + 1];
    node_t** freelist;
    node_t nodeList[768];
    node_t* nodePtrs[768];
} huff_t;

typedef struct
{
    huff_t compressor;
    huff_t decompressor;
} huffman_t;

typedef struct netField_s
{
    char *name;
    int offset;
    int bits;
} netField_t;

typedef struct trace_s
{
    float fraction;     // 0x0
    vec3_t endpos;      // 0x4
    vec3_t normal;      // 0x10
    int surfaceFlags;   // 0x1C
    byte gap0x20[8];
    uint16_t entityNum; // 0x28
    uint16_t partName;  // 0x2A
    byte gap0x2C[2];
    byte allsolid;      // 0x2E
    byte startsolid;    // 0x2F
} trace_t;

typedef struct usercmd_s
{
    int serverTime;
    byte buttons;   // console, chat, ads, attack, use
    byte wbuttons;  // lean left, lean right, reload
    byte weapon;
    byte flags;
    int angles[3];
    signed char forwardmove;
    signed char rightmove;
    signed char upmove;
    byte unknown;
} usercmd_t;

typedef enum
{
    G_PRINT,
    G_ERROR,
    //...
} gameImport_t;

typedef enum
{
    GAME_CLIENT_COMMAND = 6,
    GAME_CLIENT_SCORE_GET = 0x14
} gameExport_t;

typedef void netProfileInfo_t;

typedef struct
{
    netsrc_t sock;
    int dropped;
    netadr_t remoteAddress;
    int qport;
    int incomingSequence;
    int outgoingSequence;
    int fragmentSequence;
    int fragmentLength;
    byte fragmentBuffer[MAX_MSGLEN];
    qboolean unsentFragments;
    int unsentFragmentStart;
    int unsentLength;
    byte unsentBuffer[MAX_MSGLEN];
    netProfileInfo_t *netProfile;
} netchan_t;

typedef struct
{
    char command[MAX_STRINGLENGTH];
    int cmdTime;
    int cmdType;
} reliableCommands_t;

typedef struct objective_s
{
    int state;
    vec3_t origin;
    int entNum;
    int teamNum;
    int icon;
} objective_t;

typedef struct hudelem_s
{
    byte gap[112];
} hudelem_t;

typedef struct hudElemState_s
{
    hudelem_t current[31];
    hudelem_t archival[31];
} hudElemState_t;

typedef enum
{
    PM_NORMAL = 0x0,
    PM_NORMAL_LINKED = 0x1,
    PM_NOCLIP = 0x2,
    PM_UFO = 0x3,
    PM_SPECTATOR = 0x4,
    PM_INTERMISSION = 0x5,
    PM_DEAD = 0x6,
    PM_DEAD_LINKED = 0x7,
} pmtype_t;

typedef enum
{
    WEAPON_READY = 0x0,
    WEAPON_RAISING = 0x1,
    WEAPON_DROPPING = 0x2,
    WEAPON_FIRING = 0x3,
    WEAPON_RECHAMBERING = 0x4,
    WEAPON_RELOADING = 0x5,
    WEAPON_RELOADING_INTERUPT = 0x6,
    WEAPON_RELOAD_START = 0x7,
    WEAPON_RELOAD_START_INTERUPT = 0x8,
    WEAPON_RELOAD_END = 0x9,
    WEAPON_MELEE_INIT = 0xA,
    WEAPON_MELEE_FIRE = 0xB,
    WEAPONSTATES_NUM = 0xC,
} weaponstate_t;

typedef struct playerState_s
{
    int commandTime;        // 0x0
    pmtype_t pm_type;       // 0x4
    int bobCycle;           // 0x8
    int pm_flags;           // 0xC
    int pm_time;            // 0x10
    vec3_t origin;          // [0] = 0x14, [1] = 0x18, [2] = 0x1C
    vec3_t velocity;        // [0] = 0x20, [1] = 0x24, [2] = 0x28
    int weaponTime;         // 0x2c
    int weaponDelay;        // 0x30
    int grenadeTimeLeft;    // 0x34
    int iFoliageSoundTime;  // 0x38
    int gravity;            // 0x3C
    float leanf;            // 0x40
    int speed;              // 0x44
    vec3_t delta_angles;    // [0] = 0x48, [1] = 0x4C, [2] = 0x50
    int groundEntityNum;    // 0x54
    vec3_t vLadderVec;      // [0] = 0x58, [1] = 0x5C, [2] = 0x60
    int jumpTime;           // 0x64
    float fJumpOriginZ;     // 0x68
    int legsTimer;          // 0x6C
    int legsAnim;           // 0x70
    int torsoTimer;         // 0x74
    int torsoAnim;          // 0x78
    int movementDir;        // 0x7C
    int eFlags;             // 0x80
    int eventSequence;      // 0x84
    int events[4];          // 0x88
    unsigned int eventParms[4]; // 0x98
    int oldEventSequence;       // 0xA8
    int clientNum;              // 0xAC
    unsigned int weapon;        // 0xB0
    weaponstate_t weaponstate;  // 0xB4
    float fWeaponPosFrac;       // 0xB8
    int viewmodelIndex;         // 0xBC
    vec3_t viewangles;          // 0xC0
    int viewHeightTarget;       // 0xCC
    float viewHeightCurrent;    // 0xD0
    int viewHeightLerpTime;     // 0xD4
    int viewHeightLerpTarget;   // 0xD8
    int viewHeightLerpDown;     // 0xDC
    int viewHeightLerpPosAdj;   // 0xE0
    int damageEvent;            // 0xe4
    int damageYaw;              // 0xe8
    int damagePitch;            // 0xec
    int damageCount;            // 0xf0
    int stats[6];               // 0xf4
    int ammo[MAX_WEAPONS];      // 0x10c
    int ammoclip[MAX_WEAPONS];  // 0x20c
    unsigned int weapons[2];    // 0x30c
    byte weaponslots[8];        // 0x314
    unsigned int weaponrechamber[2]; // 0x31c
    vec3_t mins;                // 0x324
    vec3_t maxs;                // 0x330
    int proneViewHeight;        // 0x33C
    int crouchViewHeight;       // 0x340
    int standViewHeight;        // 0x344
    int deadViewHeight;         // 0x348
    float walkSpeedScale;       // 0x34C // ADS
    float runSpeedScale;        // 0x350
    float proneSpeedScale;      // 0x354
    float crouchSpeedScale;     // 0x358
    float strafeSpeedScale;     // 0x35C
    float backSpeedScale;       // 0x360
    byte gap_0x364[4];
    float proneDirection;       // 0x368
    float proneDirectionPitch;  // 0x36c
    float proneTorsoPitch;      // 0x370
    int viewlocked;             // 0x374
    int viewlocked_entNum;      // 0x378
    float friction;             // 0x37C
    int gunfx;                  // 0x380
    int serverCursorHint;       // 0x384
    int serverCursorHintVal;    // 0x388
    trace_t serverCursorHintTrace; // 0x38C
    byte gap_0x3BC[4];
    int iCompassFriendInfo;     // 0x3C0
    float fTorsoHeight;         // 0x3c4
    float fTorsoPitch;          // 0x3c8
    float fWaistPitch;          // 0x3cc
    int entityEventSequence;    // 0x3D0
    int weapAnim;               // 0x3d4
    float aimSpreadScale;       // 0x3d8
    int shellshockIndex;        // 0x3dc
    int shellshockTime;         // 0x3e0
    int shellshockDuration;     // 0x3e4
    objective_t objective[MAX_OBJECTIVES]; // 0x3E8
    hudElemState_t hud;         // 0x5A8
    int ping;                   // 0x20C8
    int deltaTime;              // 0x20CC
} playerState_t;

typedef struct
{
    playerState_t ps;
    int num_entities;
    int num_clients;
    int first_entity;
    int first_client;
    unsigned int messageSent;
    unsigned int messageAcked;
    int messageSize;
} clientSnapshot_t;

typedef struct client_s
{
    clientConnectState_t state;
    qboolean sendAsActive;
    const char *dropReason;
    char userinfo[MAX_INFO_STRING];
    reliableCommands_t reliableCommands[MAX_RELIABLE_COMMANDS];
    int reliableSequence;
    int reliableAcknowledge;
    int reliableSent;
    int messageAcknowledge;
    int gamestateMessageNum;
    int challenge;
    usercmd_t lastUsercmd;
    int lastClientCommand;
    char lastClientCommandString[MAX_STRINGLENGTH];
    gentity_t *gentity;
    char name[MAX_NAME_LENGTH];
    char downloadName[MAX_QPATH];
    fileHandle_t download;
    int downloadSize;
    int downloadCount;
    int downloadClientBlock;
    int downloadCurrentBlock;
    int downloadXmitBlock;
    unsigned char *downloadBlocks[MAX_DOWNLOAD_WINDOW];
    int downloadBlockSize[MAX_DOWNLOAD_WINDOW];
    qboolean downloadEOF;
    int downloadSendTime;
    int deltaMessage;
    int nextReliableTime;
    int lastPacketTime;
    int lastConnectTime;
    int nextSnapshotTime;
    qboolean rateDelayed;
    int timeoutCount;
    clientSnapshot_t frames[PACKET_BACKUP];
    int ping;
    int rate;
    int snapshotMsec;
    int pureAuthentic;
    netchan_t netchan;
    unsigned short clscriptid;
    int bIsTestClient;
    int serverId;
} client_t;
///*
typedef struct
{
    netadr_t adr;
    int challenge;
    /*int time;
    int pingTime;
    int firstTime;
    int firstPing;
    qboolean connected;//*/
} challenge_t;//*/

/*// Basic union: 5 x 32 bits = 20 bytes
typedef union netadr_u {
    struct {
        // 20 bytes in total
        uint32_t field1;
        uint32_t field2;
        uint32_t field3;
        uint32_t field4;
        uint32_t field5;
    };
    uint32_t raw[5];
} netadr_u;//*/


///*

typedef struct
{
    qboolean initialized;
    int time;
    int snapFlagServerBit;
    byte pad1[4]; // grok rocks
    client_t *clients;
    /*int numSnapshotEntities;
    int numSnapshotClients;
    int nextSnapshotEntities;
    int nextSnapshotClients;//*/
    //byte gap[0x4c]; //without clients
    byte gap[0x44];
    int nextHeartbeatTime;
    int nextStatusResponseTime;
    challenge_t challenges[MAX_CHALLENGES]; //at 0x084f7060
    //netadr_t redirectAddress;
    //netadr_t authorizeAddress;
    //int sv_lastTimeMasterServerCommunicated;
} serverStatic_t;//*/
///*
typedef enum
{
    SS_DEAD,
    SS_LOADING,
    SS_GAME
} serverState_t;//*/

enum clc_ops_e
{
    clc_move,
    clc_moveNoDelta,
    clc_clientCommand,
    clc_EOF
};

enum svscmd_type
{
    SV_CMD_CAN_IGNORE = 0x0,
    SV_CMD_RELIABLE = 0x1,
};

typedef enum
{
    EXEC_NOW,
    EXEC_INSERT,
    EXEC_APPEND
} cbufExec_t;

typedef struct
{
    byte gap[4];
    unsigned short allies;
    byte gap2[2];
    unsigned short axis;
    byte gap3[114];
    unsigned short spectator;
    byte gap4[122];
    unsigned short none;
    //...
} stringIndex_t;

extern stringIndex_t *scr_const;

#define svs (*((serverStatic_t*)(0x084f7000)))//DAT_084f7000, 0x084886e0

// Require structure sizes to match
#if __GNUC__ >= 6
#endif


//// Custom

typedef struct leakyBucket_s leakyBucket_t;
struct leakyBucket_s
{
    netadrtype_t type;
    unsigned char adr[4];
    uint64_t lastTime;
    signed char	burst;
    long hash;
    leakyBucket_t *prev, *next;
};

#define MAX_ERROR_BUFFER 64

typedef struct customPlayerState_s
{
    //// Bots    
    int botButtons;
    int botWButtons;
    int botWeapon;
    char botForwardMove;
    char botRightMove;
    char botUpMove;
    ////
    int speed;
    int gravity;
    int fps;
    int frames;
    uint64_t frameTime;
    bool overrideJumpHeight;
    int jumpHeight;
    int airJumpsAvailable;
    bool overrideJumpHeight_air;
    bool sprintActive;
    bool sprintRequestPending;
    int sprintTimer;
    bool noAutoPickup;
    bool hiddenFromScoreboard;
} customPlayerState_t;

typedef struct customChallenge_s
{
    int ignoredCount;
} customChallenge_t;
////