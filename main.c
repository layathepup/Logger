// Prompt the user to enter a personal log
// save to a file with a date

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define CUST_PATH 01
#define MAXPATHLEN 512
#define MAXUSRNAME 128
#define DEFAULT_BASE_PATH "/home/"
#define DEFAULT_SUB_PATH "/Documents/Logs/"
#define SEPERATOR "\n----\n\n"
#define MAXFNAME 128

int main(int argc, char **argv)
{
    int maxpathname = MAXPATHLEN + MAXFNAME;
    char *path, *path_iter, *name, c;
    int flags = 0;
    FILE *fp;
    char user_name[MAXUSRNAME];

    // get username
    int usrnamelen;
    if (getlogin_r(user_name, MAXUSRNAME) >= MAXUSRNAME) {
        fprintf(stderr, "Logger: User name exceeds maximum allowed length "
                        "(%d)\n", MAXUSRNAME);
        exit(1);
    }

    path = (char *) calloc(MAXPATHLEN+MAXFNAME, sizeof(char));
    name = (char *) calloc(MAXFNAME, sizeof(char));

    // parse arguments
    while (argc-- > 1 && '-' == (*++argv)[0]) {
        while (c = *++argv[0])
            switch (c) {
                case 'd':
                    flags |= CUST_PATH;
                    break;
                default:
                    fprintf(stderr, "Logger: unexpected argument %c", c);
                    exit(1);
            }
    }

    // apply arguments based on argument flags
    if (flags &= CUST_PATH) {
        path_iter = path;
        while ((c = *++argv[0])) {
            if (!(maxpathname-- - 1)) {       // reserve 1 byte for null terminator
                fprintf(stderr,
                        "Logger: path exceeds maximum character length: %d", MAXPATHLEN);
                exit(1);
            }
            *++path_iter = c;
        }
        *++path_iter = '\0';
    }
    else {
        strcat(path, DEFAULT_BASE_PATH);
        strcat(path, user_name);
        strcat(path, DEFAULT_SUB_PATH);
    }

    // get date to name file
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(name, "Log-%02d.%02d.%04d.txt", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);

    //open file in append mode
    fp = fopen(strcat(path, name), "a");

    if (!fp) {
        fprintf(stderr, "Could not open file: %s: error number: %d (%s)",
                path, errno, strerror(errno));
        return -1;
    }

    fprintf(fp, "%02d.%02d.%04d %02d:%02d:%02d\n\n",
            tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    // insert log
    while ((c = getc(stdin)) != EOF)
        putc(c, fp);

    fputs("\n", fp);
    fputs(SEPERATOR, fp);

   exit(0);
}