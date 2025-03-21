#include "t1x.h"

//// Cvars
cvar_t *com_sv_running;
cvar_t *fs_game;
cvar_t *sv_maxclients;
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
//Com_ParseRestOfLine_t Com_ParseRestOfLine;
//Com_ParseInt_t Com_ParseInt;
//Q_strncpyz_t Q_strncpyz;
va_t va;
////

//// Callbacks

//int codecallback_playercrashland = 0;

////

// See https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/sv_client.c#L98

customPlayerState_t customPlayerState[MAX_CLIENTS];
customChallenge_t customChallenge[MAX_CHALLENGES];

cHook *hook_Com_Init;
cHook *hook_SV_AddOperatorCommands;
cHook *hook_SV_SpawnServer;
//cHook *hook_SV_Startup;
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

void sendMessageToClient_orServerConsole(client_t *cl, std::string message)
{
    std::string finalMessage;
    if (cl)
    {
        finalMessage = "e \"";
        finalMessage.append(message);
        finalMessage.append("\"");
        SV_SendServerCommand(cl, SV_CMD_CAN_IGNORE, finalMessage.c_str());
    }
    else
    {
        finalMessage = message;
        finalMessage.append("\n");
        Com_Printf(finalMessage.c_str());
    }
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
    com_sv_running = Cvar_FindVar("sv_running");
    fs_game = Cvar_FindVar("fs_game");
    sv_maxclients = Cvar_FindVar("sv_maxclients");

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

void custom_SV_AddOperatorCommands()
{
    hook_SV_AddOperatorCommands->unhook();
    void (*SV_AddOperatorCommands)();
    *(int*)&SV_AddOperatorCommands = hook_SV_AddOperatorCommands->from;
    SV_AddOperatorCommands();
    hook_SV_AddOperatorCommands->hook();

    Cmd_AddCommand("ipban", ban);
    //Cmd_AddCommand("unban", unban);
    Cmd_AddCommand("meow", meow);
    Cmd_AddCommand("gosha", gosha);
}

/*void custom_SV_Startup(void)
{
    printf("svs.initialized: %d\n", svs.initialized);
    hook_SV_Startup->unhook();
    void (*SV_Startup)(void);
    *(int*)&SV_Startup = hook_SV_Startup->from;
    SV_Startup();
    hook_SV_Startup->hook();
}//*/

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
bool hook_NET_CompareAdr_maybe(
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
    printf("  from: %d.%d.%d.%d:%d\n", from.field2, from.field3, from.field4, from.field5, from.field1);
    printf("  to:   %d.%d.%d.%d:%d\n", to.field2, to.field3, to.field4, to.field5, to.field1);

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
        time_t current_time = time(NULL);
        std::string remainingTime;

        if (std::get<1>(banInfo) != -1) // duration
        {
            int elapsed_seconds = difftime(current_time, std::get<2>(banInfo)); // ban date
            int remaining_seconds = std::get<1>(banInfo) - elapsed_seconds;

            if (remaining_seconds <= 0)
            {
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
            }
        }

        if(!unbanned)
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
            NET_OutOfBandPrint(NS_SERVER, from, banInfoMessage.c_str());
            return;
        }
    }

    if(unbanned)
        Cmd_TokenizeString(argBackup.c_str());

    if(*sv_connectMessage->string && sv_connectMessageChallenges->integer)
    {
        int userinfoChallenge = atoi(Info_ValueForKey(userinfo, "challenge"));
        
        for(int i = 0; i < MAX_CHALLENGES; i++)
        {
            challenge_t *challenge = &svs.challenges[i];
            
            if(NET_CompareAdrSigned(&from, &challenge->adr) == 0)
            {
                if(challenge->challenge == userinfoChallenge)
                {
                    if(customChallenge[i].ignoredCount < sv_connectMessageChallenges->integer)
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

/*bool hook_SkipRestOfLine( const char *( *data ) )
{
    printf("hook_SkipRestOfLine\n");
    printf("data: %s\n", *data);
    return true;
}//*/

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
            break;
        }
        Com_SkipRestOfLine(&text);
    }
    FS_FreeFile(file);
    return banInfo;
}

/*
-i: ip
-n: banned client number
-r: reason
-d: duration
-a: admin client number
*/
const std::array<std::string, 5> banParameters = {"-i", "-n", "-r", "-d", "-a"};
const std::array<std::string, 2> unbanParameters = {"-i", "-a"};

template <std::size_t N>
bool isValidBanParameter(std::string toCheck, std::array<std::string, N> parameters)
{
    for (const std::string&parameter : parameters)
    {
        if(toCheck == parameter)
            return true;
    }
    return false;
}

void hook_Com_Printf(const char *fmt, ...)
{
    printf("hook_Com_Printf input: %s\n", fmt);
}


/*static void ban()
{
    Com_Printf("ban: I HAVE BEEN SUMMONED\n");
    if (!com_sv_running->integer)
    {
        Com_Printf("Server is not running.\n");
        return;
    }
    
    Com_Printf("Size of svs: %d\n", sizeof(svs));
    Com_Printf("Size of svs.clients: %d\n", sizeof(svs.clients[0]));

    int i;
    client_t *clCheck;
    for (i = 0, clCheck = svs.clients; i < sv_maxclients->integer; i++, clCheck++)
    {
        if (clCheck->state > CS_CONNECTED)
        {
            Com_Printf("Client %d: %s, Client State: %d\n", i, clCheck->name, clCheck->state);
            Com_Printf("Client socket: %d\n", clCheck->netchan.sock);
            Com_Printf("Client IP: %s\n", NET_AdrToString(clCheck->netchan.remoteAddress));
            Com_Printf("Client last packet time: %d\n", clCheck->lastPacketTime);
            Com_Printf("Client next snapshot time: %d\n", clCheck->nextSnapshotTime);
        }
    }
    return;
}//*/
static void ban()
{
    if (!com_sv_running->integer)
    {
        Com_Printf("Server is not running.\n");
        return;
    }

    if (Cmd_Argc() < 3)
    {
        Com_Printf("Usage: ban (-i <IP address> | -n <client number>) [-r reason] [-d duration]\n");
        Com_Printf("Notes: Use h for hours or d for days\n");
        return;
    }

    std::vector<std::string> argvList;
    std::map<std::string, std::string> parsedParameters;
    std::string infoMessage;
    bool useIp = false;
    bool useClientnum = false;
    int file;
    bool clAdmin_searched = false;
    client_t *clToBan;
    client_t *clAdmin;
    char ip[16];
    char cleanName[64] = "n/a";
    time_t current_time;
    int duration = -1;
    std::string duration_drop;
    std::string reason_log = "none";
    std::string reason_drop;

    // Directly store all the argv to be able to use Cmd_TokenizeString before the end of the parse
    for (int i = 1; i < Cmd_Argc(); i++)
    {
        std::string argv = Cmd_Argv(i);
        argvList.push_back(argv);
    }
    
    //// Parse and store the parameters
    for (std::size_t i = 0; i < argvList.size(); i++)
    {
        std::string argv = argvList[i];
        if (isValidBanParameter(argv, banParameters)) // Found an option
        {
            if (parsedParameters.find(argv) == parsedParameters.end())
            {
                // Parse the argument
                std::string value;
                for (std::size_t j = i+1; j < argvList.size(); j++)
                {
                    std::string argv_next = argvList[j];
                    if (!isValidBanParameter(argv_next, banParameters))
                    {
                        if(j != i+1)
                            value.append(" ");
                        value.append(argv_next);
                    }
                    else
                        break;
                }
                // Store the pair
                if(!value.empty())
                    parsedParameters[argv] = value;

                /*
                Check if got admin client after first storage and only once
                because it should be passed as first parameter from gsc
                so you can redirect the error messages since the beginning
                //*/
                if (!clAdmin_searched)
                {
                    auto adminParam = parsedParameters.find("-a");
                    if (adminParam != parsedParameters.end())
                    {
                        clAdmin = &svs.clients[std::stoi(adminParam->second)];
                    }
                    clAdmin_searched = true;
                }
            }
            else
            {
                infoMessage = "Duplicated option " + argv;
                sendMessageToClient_orServerConsole(clAdmin, infoMessage);
                return;
            }
        }
        else if (argv[0] == '-' && !isValidBanParameter(argv, banParameters))
        {
            infoMessage = "Unrecognized option " + argv;
            sendMessageToClient_orServerConsole(clAdmin, infoMessage);
            return;
        }
    }
    ////
    
    //// Check the parameters
    // Client number
    if (parsedParameters.find("-n") != parsedParameters.end())
    {
        // Check if specified both an IP and a client number
        if (parsedParameters.find("-i") != parsedParameters.end())
        {
            infoMessage = "Don't use both an IP and a client number";
            sendMessageToClient_orServerConsole(clAdmin, infoMessage);
            return;
        }
        useClientnum = true;
    }

    // IP
    auto ipParam = parsedParameters.find("-i");
    if (ipParam != parsedParameters.end())
    {
        struct sockaddr_in sa;
        if (!inet_pton(AF_INET, ipParam->second.c_str(), &(sa.sin_addr)))
        {
            infoMessage = "Invalid IP address " + ipParam->second;
            sendMessageToClient_orServerConsole(clAdmin, infoMessage);
            return;
        }
        useIp = true;
        std::strcpy(ip, ipParam->second.c_str());
    }

    if (!useClientnum && !useIp)
    {
        infoMessage = "Use an IP or a client number";
        sendMessageToClient_orServerConsole(clAdmin, infoMessage);
        return;
    }

    // Duration
    auto durationParam = parsedParameters.find("-d");
    if (durationParam != parsedParameters.end())
    {
        char durationParam_lastChar = durationParam->second.back();
        if (durationParam_lastChar != 'h' && durationParam_lastChar != 'd')
        {
            infoMessage = "Invalid duration parameter " + durationParam->second;
            sendMessageToClient_orServerConsole(clAdmin, infoMessage);
            return;
        }
        else
        {
            durationParam->second.pop_back(); // Remove unit indicator
            if (durationParam->second.empty())
            {
                infoMessage = "Invalid duration parameter " + durationParam->second;
                sendMessageToClient_orServerConsole(clAdmin, infoMessage);
                return;
            }
            else
            {
                for (int i = 0; durationParam->second[i]; i++)
                {
                    if (durationParam->second[i] < '0' || durationParam->second[i] > '9')
                    {
                        infoMessage = "Invalid duration parameter " + durationParam->second;
                        sendMessageToClient_orServerConsole(clAdmin, infoMessage);
                        return;
                    }
                }
                duration = std::stoi(durationParam->second);
            }
        }
        if(durationParam_lastChar == 'h')
        {
            duration_drop = durationParam->second + " hour";
            if(duration > 1)
                duration_drop.append("s");
            duration *= 3600;
        }
        else if(durationParam_lastChar == 'd')
        {
            duration_drop = durationParam->second + " day";
            if(duration > 1)
                duration_drop.append("s");
            duration *= 86400;
        }
    }

    // Reason
    auto reasonParam = parsedParameters.find("-r");
    if (reasonParam != parsedParameters.end())
    {
        reason_log = reasonParam->second.c_str();
        reason_drop = "Ban reason: " + reasonParam->second;
    }

    // Add duration to drop message after reason
    if (!duration_drop.empty())
    {
        if(reason_drop.empty())
        {
            reason_drop = "Ban duration: " + duration_drop;
        }
        else
        {
            reason_drop.append(" - ");
            reason_drop.append("Duration: ");
            reason_drop.append(duration_drop);
        }
    }
    else if (reason_drop.empty())
    {
        reason_drop = "EXE_PLAYERKICKED";
    }
    ////

    // Find the player
    if (useClientnum)
    {
        clToBan = &svs.clients[std::stoi(parsedParameters.find("-n")->second)];
        if (!clToBan)
        {
            infoMessage = "Couldn't find player by num " + parsedParameters.find("-n")->second;
            sendMessageToClient_orServerConsole(clAdmin, infoMessage);
            return;
        }
        else
        {
            snprintf(ip, sizeof(ip), "%d.%d.%d.%d",
                clToBan->netchan.remoteAddress.ip[0],
                clToBan->netchan.remoteAddress.ip[1],
                clToBan->netchan.remoteAddress.ip[2],
                clToBan->netchan.remoteAddress.ip[3]
            );
        }
    }
    else if (useIp)
    {
        int i;
        client_t *clCheck;
        for (i = 0, clCheck = svs.clients; i < sv_maxclients->integer; i++, clCheck++)
        {
            if (clCheck->state > CS_CONNECTED)
            {
                char ip_check[16];
                snprintf(ip_check, sizeof(ip_check), "%d.%d.%d.%d",
                    clCheck->netchan.remoteAddress.ip[0],
                    clCheck->netchan.remoteAddress.ip[1],
                    clCheck->netchan.remoteAddress.ip[2],
                    clCheck->netchan.remoteAddress.ip[3]
                );
                
                if (!strcmp(ip_check, ip))
                {
                    clToBan = clCheck;
                    break;
                }
            }
        }
    }
    
    auto banInfo = getBanInfoForIp(ip);
    if(std::get<0>(banInfo) == true) // banned
    {
        std::ostringstream oss;
        oss << "This IP (" << ip << ") is already banned";
        infoMessage = oss.str();
        sendMessageToClient_orServerConsole(clAdmin, infoMessage);
        return;
    }
    
    if (clToBan)
        Q_strncpyz(cleanName, clToBan->name, sizeof(cleanName));
    
    // Add IP to ban.txt
    if (FS_FOpenFileByMode("t1x_ban.txt", &file, FS_APPEND) < 0)
    {
        infoMessage = "Couldn't open t1x_ban.txt";
        sendMessageToClient_orServerConsole(clAdmin, infoMessage);
        return;
    }
    else
    {
        current_time = time(NULL);
        printf("\"%s\" \"%s\" \"%i\" \"%li\" \"%s\"\r\n", ip, cleanName, duration, current_time, reason_log.c_str());
        //return;
        char buffer[256];
        int len = snprintf(buffer, sizeof(buffer), "\"%s\" \"%s\" \"%i\" \"%li\" \"%s\"\r\n",
                   ip, cleanName, duration, current_time, reason_log.c_str());
        FS_Write(buffer, len, file);
        //FS_Write(file, "\"%s\" \"%s\" \"%i\" \"%i\" \"%s\"\r\n", ip, cleanName, duration, current_time, reason_log.c_str());
        FS_FCloseFile(file);
    }

    // Disconnect the player
    if (clToBan)
    {
        SV_DropClient(clToBan, reason_drop.c_str());
        clToBan->lastPacketTime = svs.time;
    }
}//*/

////
//////

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
    //Q_strncpyz = (Q_strncpyz_t)dlsym(libHandle, "Q_strncpyz");
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
        hook_call(0x0808af8f, (int)hook_NET_CompareAdr_maybe);
        hook_call(0x0805fcb1, (int)hook_Com_Printf);
        //hook_call(0x0808ad27, (int)hook_NET_OutOfBandPrint);
        //hook_call(0x0806d8d3, (int)hook_SkipRestOfLine);

        hook_Sys_LoadDll = new cHook(0x080c5fe4, (int)custom_Sys_LoadDll);
        hook_Sys_LoadDll->hook();
        hook_Com_Init = new cHook(0x0807154e, (int)custom_Com_Init);
        hook_Com_Init->hook();
        hook_SV_SpawnServer = new cHook(0x8091b72, (int)custom_SV_SpawnServer);
        hook_SV_SpawnServer->hook();
        //hook_SV_Startup = new cHook(0x08091473, (int)custom_SV_Startup);
        //hook_SV_Startup->hook();
        hook_SV_AddOperatorCommands = new cHook(0x08089580, (int)custom_SV_AddOperatorCommands);
        hook_SV_AddOperatorCommands->hook();

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