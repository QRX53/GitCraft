#define MCPATH_MAX_LEN 100

#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <stdbool.h>
#include <sys/event.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <time.h>

int logo(int, char*);
int daemonize();
int read_config();
int setup();

char MINECRAFT_SAVES_PATH[MCPATH_MAX_LEN];
long int WAIT_TIME = 1040;