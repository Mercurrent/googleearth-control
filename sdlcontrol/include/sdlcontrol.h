#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h> 


#include <SDL/SDL.h>

#include <vector>

const char *SERVER_ADDRESS = "dn3";
const int SERVER_PORT = 23457;

void update(int k_add, int k_sub, int j_add, int j_sub, float *mod);
void update(int k_add, int k_sub, float *mod);

int connect_to_server(const char *address, int port);
