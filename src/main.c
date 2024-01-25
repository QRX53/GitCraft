#include <string.h>
#include "main.h"

int main(void) {

    if (setup() != 0) {
        exit(EXIT_FAILURE);
    }

    int d = daemonize();
    if (d == 1) {
        logo(LOG_ERR, "Failed to daemonize");
        exit(EXIT_FAILURE);
    }

    logo(LOG_INFO, "gitcraftd started");
    int gitAddResults;
    int gitCommitResults;

    while (true) {
        char *gitadd;
        asprintf(&gitadd, "cd %s ; git add . ;", MINECRAFT_SAVES_PATH);
        gitAddResults = system(gitadd);
        printf("GitAdd Str: %s\n", gitadd);

        char *gitcommmit;
        asprintf(&gitcommmit, "cd %s ; git commit -m \"gitcraftd commit\" ;", MINECRAFT_SAVES_PATH);
        gitCommitResults = system(gitcommmit);
        printf("GitCommit Str: %s\n", gitcommmit);

        if (gitAddResults) {
            logo(LOG_ERR, "Couldn't add files to Git.");
            printf("Couldn't add files to Git\n");
        }
        if (gitCommitResults) {
            logo(LOG_ERR, "Couldn't commit files to GitHub.");
            printf("Couldn't commit files to GitHub\n");
        }

        sleep(WAIT_TIME);
    }
}

int daemonize() {
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0) {
        printf("Pid less than 0\n");
        return 1;
    }

    /* Success: Let the parent terminate */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    /* On success: The child process becomes session leader */
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    /* Catch, ignore and handle signals */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Success: Let the parent terminate */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }

    printf("End successfully, PID: %i\n", pid);
    return 0;
}

int read_config() {
    char *home = getenv("HOME");
    char *confpath;
    asprintf(&confpath, "%s/.config/gitcraftd/config.txt", home);

    FILE *config = fopen(confpath, "r");
    if (!config) {
        return 1; // couldn't open file
    }

    char currentline[100];

    int cntr = 0;
    while (fgets(currentline, sizeof(currentline), config) != NULL) {
        if (cntr == 0) {
            strcpy(MINECRAFT_SAVES_PATH, currentline);
            for (int i = 0; i < MCPATH_MAX_LEN; i++) {
                if (MINECRAFT_SAVES_PATH[i] == '\n') {
                    MINECRAFT_SAVES_PATH[i] = ' ';
                }
            }
        } else if (cntr == 1) {
            char *output;
            WAIT_TIME = strtol(currentline, &output, 10) + 1;

        }
        cntr++;
    }
    (void) fclose(config);

    return 0;
}

int logo(int level, char *str) {
    FILE *output;

    char *home = getenv("HOME");
    char *logpath;
    asprintf(&logpath, "%s/.config/gitcraftd/log.log", home);

    output = fopen(logpath, "ab+");

    if (!output) {
        perror("Couldn't read/write to logfile.");
        return 1;
    }

    time(NULL);
    time_t rawtime;
    struct tm *info;
    char curTime[80];

    time(&rawtime);
    info = localtime(&rawtime);

    strftime(curTime, 80, "%x - %I:%M%p", info);
    fprintf(output, "[L%i] [%s] %s\n", level, curTime, str);
    fclose(output);

    return 0;
}

int setup() {

    if (read_config() == 1) {
        printf("Failed to read from config file\n");
        return 1;
    }

    char *home = getenv("HOME");

    char *gitpath;
    asprintf(&gitpath, "%s/.git", MINECRAFT_SAVES_PATH);

    char *gitinit;
    asprintf(&gitinit, "cd %s ; sudo git init > /dev/null", MINECRAFT_SAVES_PATH);
    if (access(gitpath, F_OK) != 0) {
        if (system(gitinit)) {
            printf("Failed to initialize git repository at %s\n", MINECRAFT_SAVES_PATH);
            return 1;
        }
    }

    char *logpath;
    asprintf(&logpath, "%s/.config/gitcraftd/log.log", home);
    if (access(logpath, F_OK) != 0) {
        FILE *logfile;
        logfile = fopen(logpath, "ab+");
        if (!logfile) {
            printf("Couldn't create logfile. Try creating it yourself at %s/.config/gitcraftd/log.log\n", home);
            return 1;
        }
    }

    return 0;
}