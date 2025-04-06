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

typedef void (*xcommand_t)(void);

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
	ET_GENERAL = 0,
	ET_PLAYER = 1,
	ET_PLAYER_CORPSE = 2,
	ET_ITEM = 3,
	ET_MISSLE = 4,
	ET_MOVER = 5,
	ET_PORTAL = 6,
	ET_INVISIBLE = 7,
	ET_SCRIPTMOVER = 8,
	ET_SOUND_BLEND = 9,
	ET_LOOP_FX = 10,
	ET_TURRET = 11,
	ET_VEHICLE = 12,
	ET_VEHICLE_COLLMAP = 14,
	ET_EVENTS = 16,
} entityType_t;

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

typedef struct callback_s
{
    int *pos;
    const char *name;
    bool custom;
} callback_t;

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

/*typedef struct
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
} netchan_t;//*/

/*typedef struct {
    netsrc_t    sock;

    int            dropped;            // between last packet and previous

    netadr_t    remoteAddress;
    int            qport;                // qport value to write when transmitting

                                    // sequencing variables
    int            incomingSequence;
    int            outgoingSequence;

    // incoming fragment assembly buffer
    int            fragmentSequence;
    int            fragmentLength;
    byte        fragmentBuffer[MAX_MSGLEN];

    // outgoing fragment buffer
    // we need to space out the sending of large fragmented messages
    qboolean    unsentFragments;
    int            unsentFragmentStart;
    int            unsentLength;
    byte        unsentBuffer[MAX_MSGLEN];
} netchan_t;//*/

typedef struct
{
    netsrc_t sock; //0x0000
	int dropped; //0x0004
	netadr_t remoteAddress; //0x0008
	int qport; //0x001C
	int incomingSequence; //0x0020
	int outgoingSequence; //0x0024
	int fragmentSequence; //0x0028
	int fragmentLength; //0x002C
	char fragmentBuffer[32768]; //0x0030
	int unsentFragments; //0x8030
	int unsentLength; //0x8034
	char unsentBuffer[32768]; //0x8038
} netchan_t;

typedef struct
{
    char command[MAX_STRINGLENGTH];
    int cmdTime;
    int cmdType;
} reliableCommands_t;

typedef enum
{
    TR_STATIONARY,
	TR_INTERPOLATE,             // non-parametric, but interpolate between snapshots
	TR_LINEAR,
	TR_LINEAR_STOP,
	TR_LINEAR_STOP_BACK,        //----(SA)	added.  so reverse movement can be different than forward
	TR_SINE,                    // value = base + sin( time / duration ) * delta
	TR_GRAVITY,
	TR_GRAVITY_LOW,
	TR_GRAVITY_FLOAT,           // super low grav with no gravity acceleration (floating feathers/fabric/leaves/...)
	TR_GRAVITY_PAUSED,          //----(SA)	has stopped, but will still do a short trace to see if it should be switched back to TR_GRAVITY
	TR_ACCELERATE,
	TR_DECCELERATE
} trType_t;

typedef struct
{
    trType_t trType;
	int trTime;
	int trDuration;             // if non 0, trTime + trDuration = stop time
	vec3_t trBase;
	vec3_t trDelta;
} trajectory_t;

typedef struct objective_s
{
    int state;
    vec3_t origin;
    int entNum;
    int teamNum;
    int icon;
} objective_t;

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

/*typedef struct playerState_s
{
    int commandTime;
    pmtype_t pm_type;
    int bobCycle;
    int pm_flags;
    //char gap1[16];
    int pm_time;
    //vec3_t origin;
    //vec3_t velocity;
    //int weaponTime;         // 0x2c
    //int weaponDelay;        // 0x30
    //int grenadeTimeLeft;    // 0x34
    //int iFoliageSoundTime;  // 0x38
    //int gravity;            // 0x3C // 20
    //float leanf;            // 0x40
    //int speed;              // 0x44 // 28
    //vec3_t delta_angles;    // [0] = 0x48, [1] = 0x4C, [2] = 0x50 // 40
    //int groundEntityNum;    // 0x54 // 44
    //vec3_t vLadderVec;      // [0] = 0x58, [1] = 0x5C, [2] = 0x60 // 56
    //char gap2[56];
    //int jumpTime;
    //float jumpOriginZ;
    //char gap2[188];
    char gap[192];
    unsigned int weapon; // 0xd8
    //...
} playerState_t; //*/

typedef struct playerState_s
{
    int commandTime; //0x0000
	pmtype_t pm_type; //0x0004
    int bobCycle;
    int pm_flags;// not sure about these two
    int pm_time;
    //char pad_0008[12]; //0x0008//*/
	vec3_t origin; //0x0014
	vec3_t velocity; //0x0020
	char pad_002C[8]; //0x002C
	int grenadeTimeLeft; //0x0034
	char pad_0038[12-4]; //0x0038
    int gravity;
	float leanf; //0x0044
    int speed;
	char pad_0048[140-4]; //0x0048
	int clientNum; //0x00D4
	int weapon; //0x00D8
} playerState_t;

struct pmove_t
{
	playerState_t *ps;
    //...
};

struct gclient_s
{
    int commandTime; //0x0000
	int pm_type; //0x0004
	char pad_0008[12]; //0x0008
	vec3_t origin; //0x0014
	vec3_t velocity; //0x0020
	char pad_002C[8]; //0x002C
	int grenadeTimeLeft; //0x0034
	char pad_0038[12]; //0x0038
	float leanf; //0x0044
	char pad_0048[140]; //0x0048
	int clientNum; //0x00D4
	int weapon; //0x00D8
	char pad_00DC[4]; //0x00DC
	float aimProgress; //0x00E0
	char pad_00E4[4]; //0x00E4
	vec3_t viewangles; //0x00E8
	int viewheight_i; //0x00F4
	float viewheight_f; //0x00F8
	char pad_00FC[4]; //0x00FC
	int viewheight_i_2; //0x0100
	int stance_change_dir; //0x0104
	char pad_0108[28]; //0x0108
	int maxhealth; //0x0124
	char pad_0128[1076]; //0x0128
	vec3_t mins; //0x055C
	vec3_t maxs; //0x0568
	int viewheight_prone; //0x0574
	int viewheight_crouched; //0x0578
	int viewheight_standing; //0x057C
	char pad_0580[48]; //0x0580
	float stamina; //0x05B0
	char pad_05B4[16448]; //0x05B4
	char name[32]; //0x45F4
	char pad_4614[8]; //0x4614
	int noclip; //0x461C
	char pad_4620[8]; //0x4620
	int lastCmdTime; //0x4628
	int buttons; //0x462C
	int oldbuttons; //0x4630
	char pad_4634[28]; //0x4634
	vec3_t viewangles2; //0x4650
	char pad_465C[28]; //0x465C
	int inactivityTime; //0x4678
	char pad_467C[4]; //0x467C
	int inactivityWarning; //0x4680
	char pad_4684[496]; //0x4684
};

struct gentity_s
{
    int number; //0x0000
	entityType_t eType; //0x0004
	char pad_0008[4]; //0x0008
	trajectory_t pos; //0x000C
	char pad_0030[12]; //0x0030
	vec3_t viewangles; //0x003C
	char pad_0048[132]; //0x0048
	int weapon; //0x00CC
	char pad_00D0[40]; //0x00D0
	int svFlags; //0x00F8
	char pad_00FC[64]; //0x00FC
	vec3_t currentOrigin; //0x013C
	char pad_0148[12]; //0x0148
	int ownerNum; //0x0154
	int r_eventTime; //0x0158
	char pad_015C[4]; //0x015C
	struct gclient_s *client; //0x0160
	char pad_0164[8]; //0x0164
	int inuse; //0x016C
	char pad_0170[20]; //0x0170
	int classname; //0x0184
	int spawnflags; //0x0188
	int flags; //0x018C
	int eventTime; //0x0190
	int freeAfterEvent; //0x0194
	char pad_0198[8]; //0x0198
	int clipmask; //0x01A0
	uint32_t frames; //0x01A4
	struct gentity_s *parent; //0x01A8
	char pad_01AC[58]; //0x01AC
	int16_t targetname; //0x01E6
	char pad_01E8[36]; //0x01E8
	int nextthink; //0x020C
	void (*think)(gentity_t* self);
	void (*reached)(gentity_t* self);
	void (*blocked)(gentity_t* self, gentity_t* other);
	void (*touch)(gentity_t* self, gentity_t* other, struct trace_t* trace);
	void (*use)(gentity_t* self, gentity_t* other, gentity_t* activator);
	void (*pain)(gentity_t* self, gentity_t* attacker, int damage, vec3_t point);
	void (*die)(gentity_t* self, gentity_t* inflictor, gentity_t* attacker, int damage, int mod);
	char pad_022C[20]; //0x022C
	int health; //0x0240
	char pad_0244[4]; //0x0244
	int damage; //0x0248
	int explosionInnerDamage; //0x024C
	int explosionOuterDamage; //0x0250
	int explosionRadius; //0x0254
	int methodOfDeath; //0x0258
	int splashMethodOfDeath; //0x025C
	char pad_0260[236]; //0x0260
};

/*
typedef struct client_s
{
    clientConnectState_t state;
    //qboolean sendAsActive;
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
    char name[MAX_NAME_LENGTH]; // needed
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
    int lastPacketTime; // needed
    int lastConnectTime;
    int nextSnapshotTime;
    qboolean rateDelayed;
    int timeoutCount;
    clientSnapshot_t frames[PACKET_BACKUP];
    int ping;
    int rate;
    int snapshotMsec;
    int pureAuthentic;
    netchan_t netchan; // needed
    unsigned short clscriptid;
    int bIsTestClient;
    int serverId;
} client_t;



//*/

///*

typedef struct client_s
{
    clientConnectState_t state; //0x0000
    byte gap[0x10a40];
	char name[MAX_NAME_LENGTH]; //0x10A44
    //char gap2[566748];
    char gap2[440];
    int lastPacketTime; //0x10c1c
    char gap3[566304];
	netchan_t netchan; //0x9B040
	char gap4[60]; //0xAB078*/
    //...
} client_t;//*/

// guid at 0xab080

//

/*typedef struct client_s
{
    clientConnectState_t state; //0x0000
    int sendAsActive; //0x0004
    const char* dropReason; //0x0008
    char userinfo[MAX_INFO_STRING]; //0x000c
    reliableCommands_t reliableCommands[MAX_RELIABLE_COMMANDS];
    int reliableSequence;
    int reliableAcknowledge;
    int reliableSent;
    int messageAcknowledge;
    int gamestateMessageNum; // 4187 * 4
    int challenge; // 4188
    usercmd_t lastUsercmd; // 4189
    int lastClientCommand; // 4189 + 6 = 418f
    char lastClientCommandString[MAX_STRINGLENGTH]; // 4190
    void* gentity; //0x10a40 TODO: find gentity_s
    char name[32]; //0x10a44
    char downloadName[64]; //0x10A64
    int download; //0x10AA4
    int downloadSize; //0x10AA8
    int downloadCount; //0x10AAC ----
    int downloadClientBlock; //0x10AB0
    int downloadCurrentBlock; //0x10AB4
    int downloadXmitBlock; //0x10AB8 ----
    unsigned char* downloadBlocks[MAX_DOWNLOAD_WINDOW]; //0x10ABC
    int downloadBlockSize[MAX_DOWNLOAD_WINDOW]; //0x10ADC
    qboolean downloadEOF; //0x10AFC
    int downloadSendTime; //0x10B00
    int wwwOk; //int unsure_if_fallback_or_time2; //0x10B04
    char downloadURL[MAX_OSPATH]; //0x10B08
    qboolean downloadingWWW; //0x10c08
    qboolean clientDownloadingWWW; //0x10c0C
    qboolean wwwFallback_maybe; //0x10c10
    int deltaMessage; //0x10c14
    int nextReliableTime; //0x10c18
    int lastPacketTime; //0x10c1c
    int lastConnectTime; // 0x10c20
    int nextSnapshotTime; //0x10c24
    qboolean rateDelayed; //0x10c28
    int timeoutCount; // 0x430b * 0x4 = 0x10c2c
    //char gap1[566288]; //0x10AB0
    byte gap1[0x8a400];
    int ping; // 0x26c0c * 0x4 = 0x9b030
    int rate; // 0x26c0d * 0x4 = 0x9b034
    int snapshotMsec; // 0x9b038
    int pureAuthentic; // 0x9b03c
    netchan_t netchan; //0x9B040
    int guid; // 0x2ac20 * 0x4 = 0xab080
    //unsigned short scriptId; // 0x2ac21 * 0x4 = 0xab084
    //unsigned int scriptId; // 0x2ac21 * 0x4 = 0xab084
    //qboolean bIsTestClient; // 0x2ac22 * 0x4 = 0xab088
    //int serverId; // 0x2ac23 * 0x4 = 0xab08c
    char pad_AB078[56]; //0xAB078
} client_t; //*/

// 0x9B040 + 803C = A307C

//#define x sizeof(netchan_t)

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

// Basic union: 5 x 32 bits = 20 bytes
/*typedef union netadr_u {
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
    int time2;
    int snapFlagServerBit;
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

typedef struct weaponinfo_t
{
    int number; //0x0000
	char *name; //0x0004
	char *displayName; //0x0008
	char *AIOverlayDescription; //0x000C
	char *gunModel; //0x0010
	char *handModel; //0x0014
	char pad_0018[4]; //0x0018
	char *idleAnim; //0x001C
	char *emptyIdleAnim; //0x0020
	char *fireAnim; //0x0024
	char *holdFireAnim; //0x0028
	char *lastShotAnim; //0x002C
	char pad_0030[4]; //0x0030
	char *meleeAnim; //0x0034
	char *reloadAnim; //0x0038
	char *reloadEmptyAnim; //0x003C
	char pad_0040[8]; //0x0040
	char *raiseAnim; //0x0048
	char *dropAnim; //0x004C
	char *altRaiseAnim; //0x0050
	char *altDropAnim; //0x0054
	char *adsFireAnim; //0x0058
	char *adsLastShotAnim; //0x005C
	char pad_0060[16]; //0x0060
	char *adsUpAnim; //0x0070
	char *adsDownAnim; //0x0074
	char *modeName; //0x0078
	char pad_007C[24]; //0x007C
	char *viewFlashEffect; //0x0094
	char *worldFlashEffect; //0x0098
	char *pickupSound; //0x009C
	char pad_00A0[8]; //0x00A0
	char *pullbackSound; //0x00A8
	char *fireSound; //0x00AC
	char pad_00B0[20]; //0x00B0
	char *reloadSound; //0x00C4
	char *reloadEmptySound; //0x00C8
	char pad_00CC[12]; //0x00CC
	char *altSwitchSound; //0x00D8
	char pad_00DC[36]; //0x00DC
	char *shellEjectEffect; //0x0100
	char pad_0104[4]; //0x0104
	char *reticle; //0x0108
	char *reticleSide; //0x010C
	char pad_0110[180]; //0x0110
	char *radiantName; //0x01C4
	char *worldModel; //0x01C8
	char pad_01CC[8]; //0x01CC
	char *hudIcon; //0x01D4
	char *modeIcon; //0x01D8
	char *ammoIcon; //0x01DC
	int startAmmo; //0x01E0
	char pad_01E4[12]; //0x01E4
	int clientIndex; //0x01F0
	int maxAmmo; //0x01F4
	int clipSize; //0x01F8
	char *sharedAmmoCapName; //0x01FC
	char pad_0200[8]; //0x0200
	int damage; //0x0208
	char pad_020C[8]; //0x020C
	int damageInnerRadius; //0x0214
	int damageOuterRadius; //0x0218
	char pad_021C[76]; //0x021C
	int fuseTime; //0x0268
    float moveSpeedScale; // 0x026c
    float adsSensitivity; // 0x0270
    float adsZoomFov; // 0x0274 <-----|
    float adsZoomInFrac; // 0x0278 <--|--- just guessed these and they seem to work
    float adsZoomOutFrac; // 0x027c <-|
    char *adsOverlayShader; // 0x0280
    char gap_0280[4]; // not sure if this is adsOverlayReticle
    float adsOverlayWidth;
    float adsOverlayHeight;
	char pad_026C[192]; //0x0280
	char *killIcon; //0x0350
	char pad_0354[20]; //0x0354
	char *altWeapon; //0x0368
	char pad_036C[12]; //0x036C
	int explosionRadius; //0x0378
	int explosionInnerDamage; //0x037C
	int explosionOuterDamage; //0x0380
	char pad_0384[8]; //0x0384
	char* projectileModel; //0x038C
    //...      
} weaponinfo_t;

typedef struct weaponslot_s
{
    weaponinfo_t *weapondef;
	int clip;
	int reserve;
} weaponslot_t;

struct WeaponProperties // Custom struct for g_legacyStyle
{
    int reloadAddTime;
    int adsTransInTime;
    float adsZoomInFrac;
    float idleCrouchFactor;
    float idleProneFactor;
    int rechamberWhileAds;
    float adsViewErrorMin;
    float adsViewErrorMax;
};

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

#define MAX_ERROR_BUFFER 64
typedef struct src_error_s
{
    char internal_function[64];
    char message[1024];
} scr_error_t;

extern gentity_t *g_entities;

extern stringIndex_t *scr_const;

// #define scrVmPub (*((scrVmPub_t*)(0x082f57e0)))

#define com_errorEntered (*((int*)(0x084897dc))) // 084897dc
#define scrVarPub (*((scrVarPub_t*)(0x082f17d8))) // To test
#define scrVmPub (*((scrVmPub_t*)(0x08433e00))) // To find and test
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

typedef struct customChallenge_s
{
    int ignoredCount;
} customChallenge_t;
////