
#include "server.h"

#include <stdio.h>
#include <stdlib.h>


//Based on Beejs Guide to Network Programming
//Available here: http://beej.us/guide/bgnet/html/
int main(int argc, char **argv) {
    //C99: No memset required
    if (argc < 1) {
        display_help();
        die("To start the InsecureWebServer three arguments are required");
    }
    server *the_server = server_from_args(argc,argv);
    if(server_start_default(&the_server) != 0)
        return 1;
    printf("Hello, World!\n");
    while (true) {
        server_handle_incoming(&the_server);
        if (server_has_any_error(&the_server))
            server_crash(&the_server);
        if (server_exit_from_cmd(&the_server))
            break;
    }
    server_stats(&the_server);
    server_close(&the_server);
    return 0;
}


void die(const char *msg) {
    if (puts(msg) == EOF)
        return;
    display_help();
    exit(1);
}
void display_help(void) {
    puts("InsecureWebserver:\n"
         "-h -help HELP, prints this\n"
         "-f -logfile ?name? print logs to the file with name\n "
         "-db -database ?name? create database with filename name");
}