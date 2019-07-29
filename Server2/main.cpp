#include "server.h"

int main()
{
    GServer.InitServer();
    GServer.MainLoop();

    printf("Done.\n");
    getchar();
    //abort();
    return 0;
}
