#include "gsc.h"

void gsc_player_processclientcommand(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_processclientcommand() entity %i is not a player", id);
        Scr_AddUndefined();
        return;
    }

    ClientCommand(id);

    Scr_AddBool(qtrue);
}

void gsc_player_connectionlesspackettoclient(scr_entref_t ref)
{
    printf("########## gsc_player_connectionlesspackettoclient: I have been summoned\n");
    int id = ref.entnum;
    char *cmd;

    if (!stackGetParams("s", &cmd))
    {
        stackError("gsc_player_connectionlesspackettoclient() argument is undefined or has a wrong type");
        Scr_AddUndefined();
        return;
    }
    printf("########## gsc_player_connectionlesspackettoclient: after getting params\n");

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_connectionlesspackettoclient() entity %i is not a player", id);
        Scr_AddUndefined();
        return;
    }

    client_t *client = &svs.clients[id];
    NET_OutOfBandPrint(NS_SERVER, client->netchan.remoteAddress, cmd);

    Scr_AddBool(qtrue);
}