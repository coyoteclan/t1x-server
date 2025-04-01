/* gsc functions */
#include "shared.h"

//__attribute__ ((naked)) void hook_PM_WalkMove_Naked();
//__attribute__ ((naked)) void hook_PM_SlideMove_Naked();

double custom_Jump_GetLandFactor();
double custom_PM_GetReducedFriction();

typedef struct playerState_s
{
    //int commandTime;
    //pmtype_t pm_type;
    //int bobCycle;
    char gap1[12];
    int pm_flags;
    int pm_time;
    vec3_t origin;
    vec3_t velocity;
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
    char gap2[56];
    int jumpTime;
    float jumpOriginZ;
    //...
} playerState_t;

struct pmove_t
{
	playerState_t *ps;
    //...
};
