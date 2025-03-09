#include "iw1x.h"

//// Cvars
cvar_t *fs_game;
// VM
// Custom
////

//// Game lib
// Objects
// Functions
////

//// Callbacks

//int codecallback_playercrashland = 0;

////

// See https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/sv_client.c#L98

cHook *hook_Com_Init;
cHook *hook_SV_SpawnServer;
cHook *hook_Sys_LoadDll;

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
    printf("fs_game: %p\n", fs_game);

    // Register
    //Cvar_Get("iw1x", "1", CVAR_SERVERINFO);
    //Cvar_Get("sv_wwwBaseURL", "", CVAR_ARCHIVE | CVAR_SYSTEMINFO);
    //Cvar_Get("sv_wwwDownload", "0", CVAR_ARCHIVE | CVAR_SYSTEMINFO);
    // Register and create references
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

#if 0
    hook_jmp((int)dlsym(libHandle, "_init") + 0x88C4, (int)custom_PM_CrashLand);
#endif
    printf("custom_Sys_LoadDll successful\n");
    
    return libHandle;
}

class iw1x
{
    public:
    iw1x()
    {
        printf("------------ iw1x ------------\n");
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

        // [exploit patch] q3infoboom
        /* See:
        - https://aluigi.altervista.org/adv/q3infoboom-adv.txt
        - https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/codextended.c#L295
        */
        //*(byte*)0x807f459 = 1;
        
        hook_Sys_LoadDll = new cHook(0x080c5fe4, (int)custom_Sys_LoadDll);
        hook_Sys_LoadDll->hook();
        //hook_Com_Init = new cHook(0x0806c654, (int)custom_Com_Init);
        hook_Com_Init = new cHook(0x0807154e, (int)custom_Com_Init);
        hook_Com_Init->hook();
        //hook_SV_SpawnServer = new cHook(0x0808a220, (int)custom_SV_SpawnServer);
        hook_SV_SpawnServer = new cHook(0x8091b72, (int)custom_SV_SpawnServer);
        hook_SV_SpawnServer->hook();

        printf("Loading complete\n");
        printf("--------------------------------\n");
    }

    ~iw1x()
    {
        printf("iw1x unloaded\n");
        system("stty sane");
    }
};

iw1x *_iw1x;
void __attribute__ ((constructor)) lib_load(void)
{
    _iw1x = new iw1x;
}
void __attribute__ ((destructor)) lib_unload(void)
{
    delete _iw1x;
}