#include "t1x.h"

//// Cvars
cvar_t *fs_game;
// VM
// Custom
cvar_t *sv_cracked;
cvar_t *sv_connectMessage;
cvar_t *sv_connectMessageChallenges;
////

//// Game lib
// Objects
// Functions
//Com_SkipRestOfLine_t Com_SkipRestOfLine;
Com_ParseRestOfLine_t Com_ParseRestOfLine;
//Com_ParseInt_t Com_ParseInt;
va_t va;
////

//// Callbacks

//int codecallback_playercrashland = 0;

////

// See https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/sv_client.c#L98

customPlayerState_t customPlayerState[MAX_CLIENTS];
customChallenge_t customChallenge[MAX_CHALLENGES];

cHook *hook_Com_Init;
cHook *hook_SV_SpawnServer;
cHook *hook_Sys_LoadDll;

time_t sys_timeBase = 0; // Base time in seconds
uint64_t Sys_Milliseconds64(void) // Current time in ms, using sys_timeBase as origin
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    if (!sys_timeBase)
    {
        sys_timeBase = tp.tv_sec;
        return tp.tv_usec / 1000;
    }
    return (tp.tv_sec - sys_timeBase) * 1000 + tp.tv_usec / 1000;
}

void custom_Com_Init(char *commandLine)
{
    hook_Com_Init->unhook();
    void (*Com_Init)(char *commandLine);
    *(int*)&Com_Init = hook_Com_Init->from;
    Com_Init(commandLine);
    hook_Com_Init->hook();

    //// Cvars
    // Create references
    fs_game = Cvar_FindVar("fs_game");

    // Register
    Cvar_Get("t1x", "1", CVAR_SERVERINFO);
    // Register and create references
    sv_cracked = Cvar_Get("sv_cracked", "0", CVAR_ARCHIVE);
    sv_connectMessage = Cvar_Get("sv_connectMessage", "Server is running t1x server extension.", CVAR_ARCHIVE);
    sv_connectMessageChallenges = Cvar_Get("sv_connectMessageChallenges", "1", CVAR_ARCHIVE);
    ////
}

void custom_SV_SpawnServer(char *server)
{
    hook_SV_SpawnServer->unhook();
    void (*SV_SpawnServer)(char *server);
    *(int*)&SV_SpawnServer = hook_SV_SpawnServer->from;
    SV_SpawnServer(server);
    hook_SV_SpawnServer->hook();
}

const char* hook_AuthorizeState(int arg)
{
    const char* s = Cmd_Argv(arg);
    if(sv_cracked->integer && !strcmp(s, "deny"))
        return "accept";
    return s;
}

//// ioquake3 rate limit connectionless requests
// See https://github.com/ioquake/ioq3/blob/master/code/server/sv_main.c
// This is deliberately quite large to make it more of an effort to DoS
#define MAX_BUCKETS	16384
#define MAX_HASHES 1024
static leakyBucket_t buckets[MAX_BUCKETS];
static leakyBucket_t* bucketHashes[MAX_HASHES];
leakyBucket_t outboundLeakyBucket;
static long SVC_HashForAddress(netadr_t address)
{
    unsigned char *ip = address.ip;
    int	i;
    long hash = 0;

    for(i = 0; i < 4; i++)
        hash += (long)(ip[i]) * (i + 119);

    hash = (hash ^ (hash >> 10) ^ (hash >> 20));
    hash &= (MAX_HASHES - 1);

    return hash;
}
static leakyBucket_t * SVC_BucketForAddress(netadr_t address, int burst, int period)
{
    leakyBucket_t *bucket = NULL;
    int i;
    long hash = SVC_HashForAddress(address);
    uint64_t now = Sys_Milliseconds64();

    for(bucket = bucketHashes[hash]; bucket; bucket = bucket->next)
        if(memcmp(bucket->adr, address.ip, 4) == 0)
            return bucket;

    for (i = 0; i < MAX_BUCKETS; i++)
    {
        int interval;

        bucket = &buckets[i];
        interval = now - bucket->lastTime;

        // Reclaim expired buckets
        if (bucket->lastTime > 0 && (interval > (burst * period) ||
                                       interval < 0))
        {
            if(bucket->prev != NULL)
                bucket->prev->next = bucket->next;
            else
                bucketHashes[bucket->hash] = bucket->next;

            if(bucket->next != NULL)
                bucket->next->prev = bucket->prev;

            memset(bucket, 0, sizeof(leakyBucket_t));
        }

        if (bucket->type == 0)
        {
            bucket->type = address.type;
            memcpy(bucket->adr, address.ip, 4);

            bucket->lastTime = now;
            bucket->burst = 0;
            bucket->hash = hash;

            // Add to the head of the relevant hash chain
            bucket->next = bucketHashes[hash];
            if(bucketHashes[hash] != NULL)
                bucketHashes[hash]->prev = bucket;

            bucket->prev = NULL;
            bucketHashes[hash] = bucket;

            return bucket;
        }
    }

    // Couldn't allocate a bucket for this address
    return NULL;
}
bool SVC_RateLimit(leakyBucket_t *bucket, int burst, int period)
{
    if (bucket != NULL)
    {
        uint64_t now = Sys_Milliseconds64();
        int interval = now - bucket->lastTime;
        int expired = interval / period;
        int expiredRemainder = interval % period;

        if (expired > bucket->burst || interval < 0)
        {
            bucket->burst = 0;
            bucket->lastTime = now;
        }
        else
        {
            bucket->burst -= expired;
            bucket->lastTime = now - expiredRemainder;
        }

        if (bucket->burst < burst)
        {
            bucket->burst++;
            return false;
        }
    }
    return true;
}
bool SVC_RateLimitAddress(netadr_t from, int burst, int period)
{
    leakyBucket_t *bucket = SVC_BucketForAddress(from, burst, period);
    return SVC_RateLimit(bucket, burst, period);
}

void dumpServerStatic() {
    printf("svs at: %p\n", &svs);
    //printf("initialized: %d\n", svs.initialized);
    for (int i = 0; i < MAX_CHALLENGES; i++) {
        challenge_t *challenge = &svs.challenges[i];
        printf("Challenge %d:\n", i);
        printf("  adr: %s\n", NET_AdrToString(challenge->adr));
        printf("  challenge: %d\n", challenge->challenge);
    }
}

// Our hooking function
/*bool hook_NET_CompareAdr_maybe(
    uint32_t fa1, uint32_t fa2, uint32_t fa3, uint32_t fa4, uint32_t fa5,
    uint32_t ta1, uint32_t ta2, uint32_t ta3, uint32_t ta4, uint32_t ta5
)
{
    // Interpret the first 5 params as the "from" address
    netadr_u from;
    from.raw[0] = fa1; 
    from.raw[1] = fa2; 
    from.raw[2] = fa3; 
    from.raw[3] = fa4; 
    from.raw[4] = fa5;

    // Interpret the second 5 params as the "to" / challenge->adr
    netadr_u to;
    to.raw[0] = ta1; 
    to.raw[1] = ta2; 
    to.raw[2] = ta3; 
    to.raw[3] = ta4; 
    to.raw[4] = ta5;

    // Print them out (raw hex)
    printf("NET_CompareAdr_maybe called!\n");
    printf("  from: %08X %08X %08X %08X %08X\n", fa1, fa2, fa3, fa4, fa5);
    printf("  to:   %08X %08X %08X %08X %08X\n", ta1, ta2, ta3, ta4, ta5);

    // Optionally parse them as netadr_t if you want to interpret IP, port, etc.
    // e.g., if from.field1 = type, from.field2 = part of IP, etc.

    // Call the original function so the game logic still runs

    return true;
}

bool hook_NET_OutOfBandPrint(netsrc_t sock, netadr_t adr, const char *format, ...)
{
    printf("sock: %d\n", sock);
    printf("adr: %s\n", NET_AdrToString(adr));
    printf("format: %s\n", format);
    return true; 
}//*/

/*void dumpServerStaticRaw() {
    unsigned char *svsPtr = (unsigned char *)&svs;
    size_t size = sizeof(serverStatic_t);
    printf("Raw dump of svs (size = %zu bytes):\n", size);
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", svsPtr[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");
}*/


void hook_SV_DirectConnect(netadr_t from)
{
    // Prevent using connect as an amplifier
    if (SVC_RateLimitAddress(from, 10, 1000))
    {
        Com_DPrintf("SV_DirectConnect: rate limit from %s exceeded, dropping request\n", NET_AdrToString(from));
        return;
    }

    // Allow connect to be DoSed relatively easily, but prevent
    // excess outbound bandwidth usage when being flooded inbound
    if (SVC_RateLimit(&outboundLeakyBucket, 10, 100))
    {
        Com_DPrintf("SV_DirectConnect: rate limit exceeded, dropping request\n");
        return;
    }
    
    bool unbanned;
    char* userinfo;
    char ip[16];
    std::ostringstream oss;
    std::string argBackup;
    
    unbanned = false;
    userinfo = Cmd_Argv(1);
    oss << "connect \"" << userinfo << "\"";
    argBackup = oss.str();
    snprintf(ip, sizeof(ip), "%d.%d.%d.%d", from.ip[0], from.ip[1], from.ip[2], from.ip[3]);

    auto banInfo = getBanInfoForIp(ip);
    if (std::get<0>(banInfo) == true) // banned
    {
        printf("[DEBUG] IP is banned: %s\n", NET_AdrToString(from));
        time_t current_time = time(NULL);
        std::string remainingTime;

        if (std::get<1>(banInfo) != -1) // duration
        {
            printf("[DEBUG] Ban duration is not permanent. Calculating remaining time...\n");
            int elapsed_seconds = difftime(current_time, std::get<2>(banInfo)); // ban date
            int remaining_seconds = std::get<1>(banInfo) - elapsed_seconds;
            printf("[DEBUG] Elapsed seconds: %d, Remaining seconds: %d\n", elapsed_seconds, remaining_seconds);

            if (remaining_seconds <= 0)
            {
                printf("[DEBUG] Ban expired. Unbanning IP: %s\n", ip);
                Cbuf_ExecuteText(EXEC_APPEND, va("unban %s\n", ip));
                unbanned = true;
            }
            else
            {
                int days = remaining_seconds / (60 * 60 * 24);
                int hours = (remaining_seconds % (60 * 60 * 24)) / (60 * 60);
                int minutes = (remaining_seconds % (60 * 60)) / 60;
                int seconds = remaining_seconds % 60;

                oss.str(std::string());
                oss.clear();

                if (days > 0)
                {
                    oss << days << " day" << (days > 1 ? "s" : "");
                    if (hours > 0)
                        oss << ", " << hours << " hour" << (hours > 1 ? "s" : "");
                }
                else if (hours > 0)
                {
                    oss << hours << " hour" << (hours > 1 ? "s" : "");
                    if (minutes > 0)
                        oss << ", " << minutes << " minute" << (minutes > 1 ? "s" : "");
                }
                else if (minutes > 0)
                    oss << minutes << " minute" << (minutes > 1 ? "s" : "");
                else
                    oss << seconds << " second" << (seconds > 1 ? "s" : "");

                remainingTime = oss.str();
                printf("[DEBUG] Remaining ban time: %s\n", remainingTime.c_str());
            }
        }
        else
        {
            printf("[DEBUG] Ban duration is permanent.\n");
        }

        if (!unbanned)
        {
            std::string banInfoMessage = "error\nBanned IP";
            if (std::get<3>(banInfo) != "none")
            {
                banInfoMessage.append(" - Reason: ");
                banInfoMessage.append(std::get<3>(banInfo));
            }
            if (!remainingTime.empty())
            {
                banInfoMessage.append(" - Remaining: ");
                banInfoMessage.append(remainingTime);
            }

            Com_Printf("[DEBUG] Rejected connection from banned IP: %s\n", NET_AdrToString(from));
            printf("[DEBUG] Sending ban info message to client.\n");
            NET_OutOfBandPrint(NS_SERVER, from, banInfoMessage.c_str());
            return;
        }
    }
    else
    {
        printf("[DEBUG] IP is not banned: %s\n", NET_AdrToString(from));
    }

    if(unbanned)
        Cmd_TokenizeString(argBackup.c_str());

    if (*sv_connectMessage->string && sv_connectMessageChallenges->integer)
    {
        int userinfoChallenge = atoi(Info_ValueForKey(userinfo, "challenge"));
        //dumpServerStatic();
        for (int i = 0; i < MAX_CHALLENGES; i++)
        {
            challenge_t *challenge = &svs.challenges[i];
            if (NET_CompareAdrSigned(&from, &challenge->adr) == 0)
            {
                if (challenge->challenge == userinfoChallenge)
                {
                    if (customChallenge[i].ignoredCount < sv_connectMessageChallenges->integer)
                    {
                        NET_OutOfBandPrint(NS_SERVER, from, "print\n%s\n", sv_connectMessage->string);
                        customChallenge[i].ignoredCount++;
                        return;
                    }
                }
            }
        }
    }
    
    SV_DirectConnect(from);
}

bool hook_SkipRestOfLine( const char *( *data ) )
{
    printf("hook_SkipRestOfLine\n");
    printf("data: %s\n", *data);
    return true;
}

////// Custom operator commands
//// ban & unban
std::tuple<bool, int, int, std::string> getBanInfoForIp(char* ip)
{
    char *file;
    std::string token;
    const char *text;
    std::tuple<bool, int, int, std::string> banInfo;

    banInfo = std::make_tuple(false, 0, 0, "");

    if(FS_ReadFile("t1x_ban.txt", (void **)&file) < 0)
        return banInfo;

    text = file;
    while (1)
    {
        token = Com_Parse(&text);
        if(token.empty())
            break;

        if (!strcmp(token.c_str(), ip))
        {
            std::get<0>(banInfo) = true;                // banned
            Com_Parse(&text);                           // player name
            std::get<1>(banInfo) = Com_ParseInt(&text); // duration
            std::get<2>(banInfo) = Com_ParseInt(&text); // ban date
            std::get<3>(banInfo) = Com_Parse(&text);    // reason
            printf("successful check\n");
            break;
        }
        printf("before skip\n");
        Com_SkipRestOfLine(&text);
        printf("after skip\n");
    }
    FS_FreeFile(file);
    return banInfo;
}

void ServerCrash(int sig)
{
    int fd;
    FILE *fp;
    void *array[20];
    size_t size = backtrace(array, 20);

    // Write to crash log
    fp = fopen("./crash.log", "a");
    if (fp)
    {
        fd = fileno(fp);
        fseek(fp, 0, SEEK_END);
        fprintf(fp, "Error: Server crashed with signal 0x%x {%d}\n", sig, sig);
        fflush(fp);
        backtrace_symbols_fd(array, size, fd);
    }
    
    // Write to stderr
    fprintf(stderr, "Error: Server crashed with signal 0x%x {%d}\n", sig, sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    
    system("stty sane");
    exit(1);
}

void *custom_Sys_LoadDll(const char *name, char *fqpath, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...))
{
    hook_Sys_LoadDll->unhook();
    void *(*Sys_LoadDll)(const char *name, char *fqpath, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...));
    *(int*)&Sys_LoadDll = hook_Sys_LoadDll->from;
    void* libHandle = Sys_LoadDll(name, fqpath, entryPoint, systemcalls);
    hook_Sys_LoadDll->hook();

    //// Unprotect game.mp.i386.so
    // See https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/librarymodule.c#L55
    char libPath[512];
    char buf[512];
    char flags[4];
    void *low, *high;
    FILE *fp;
    
    if(*fs_game->string)
        sprintf(libPath, "%s/game.mp.uo.i386.so", fs_game->string);
    else
        sprintf(libPath, "uo/game.mp.uo.i386.so");
    
    fp = fopen("/proc/self/maps", "r");
    if(!fp)
        return 0;

    while (fgets(buf, sizeof(buf), fp))
    {
        if(!strstr(buf, libPath))
            continue;
        if(sscanf (buf, "%p-%p %4c", &low, &high, flags) != 3)
            continue;
        mprotect((void *)low, (int)high-(int)low, PROT_READ | PROT_WRITE | PROT_EXEC);
    }
    fclose(fp);

    // Functions
    va = (va_t)dlsym(libHandle, "va");
    //Com_SkipRestOfLine = (Com_SkipRestOfLine_t)dlsym(libHandle, "Com_SkipRestOfLine");
    //Com_ParseRestOfLine = (Com_ParseRestOfLine_t)dlsym(libHandle, "Com_ParseRestOfLine");
    //Com_ParseInt = (Com_ParseInt_t)dlsym(libHandle, "Com_ParseInt");

#if 0
    hook_jmp((int)dlsym(libHandle, "_init") + 0x88C4, (int)custom_PM_CrashLand);
#endif
    printf("custom_Sys_LoadDll successful\n");
    
    return libHandle;
}

class t1x
{
    public:
    t1x()
    {
        printf("------------ t1x ------------\n");
        printf("Compiled on %s %s using g++ %s\n", __DATE__, __TIME__, __VERSION__);

        // Don't inherit lib of parent
        unsetenv("LD_PRELOAD");

        // Crash handlers for debugging
        signal(SIGSEGV, ServerCrash);
        signal(SIGABRT, ServerCrash);
        
        // Otherwise the printf()'s are printed at crash/end on older os/compiler versions
        // See https://github.com/M-itch/libcod/blob/e58d6a01b11c911fbf886659b6ea67795776cf4a/libcod.cpp#L1346
        setbuf(stdout, NULL);

        // Allow to write in executable memory
        mprotect((void *)0x08048000, 0x135000, PROT_READ | PROT_WRITE | PROT_EXEC);
        printf("Allow to write in executable memory\n");
        
        hook_call(0x0808a2c9, (int)hook_AuthorizeState);
        hook_call(0x0809466c, (int)hook_SV_DirectConnect);
        //hook_call(0x0808af8f, (int)hook_NET_CompareAdr_maybe);
        //hook_call(0x0808ad27, (int)hook_NET_OutOfBandPrint);
        //hook_call(0x0806d8d3, (int)hook_SkipRestOfLine);
        //hook_call(0x0806dac7, (int)hook_SkipRestOfLine);
        //hook_call(0x0806e256, (int)hook_SkipRestOfLine);
        //hook_call(0x0806e28e, (int)hook_SkipRestOfLine);
        //hook_call(0x0806e9de, (int)hook_SkipRestOfLine);
        //hook_call(0x0806eaaa, (int)hook_SkipRestOfLine);
        //hook_call(0x0806ee7a, (int)hook_SkipRestOfLine);
        //hook_call(0x0806f162, (int)hook_SkipRestOfLine);
        //hook_call(0x0606f2c4, (int)hook_SkipRestOfLine);
        //hook_call(0x08072d3c, (int)hook_SkipRestOfLine);

        hook_Sys_LoadDll = new cHook(0x080c5fe4, (int)custom_Sys_LoadDll);
        hook_Sys_LoadDll->hook();
        hook_Com_Init = new cHook(0x0807154e, (int)custom_Com_Init);
        hook_Com_Init->hook();
        hook_SV_SpawnServer = new cHook(0x8091b72, (int)custom_SV_SpawnServer);
        hook_SV_SpawnServer->hook();

        printf("Loading complete\n");
        printf("--------------------------------\n");
    }

    ~t1x()
    {
        printf("t1x unloaded\n");
        system("stty sane");
    }
};

t1x *_t1x;
void __attribute__ ((constructor)) lib_load(void)
{
    _t1x = new t1x;
}
void __attribute__ ((destructor)) lib_unload(void)
{
    delete _t1x;
}