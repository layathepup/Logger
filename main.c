// Prompt the user to enter a personal log
// save to a file with a date

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CUST_PATH 01
#define MAXPATHLEN 1000
#define DEFAULT_PATH "/home/jack/Documents/Logs/"
#define SEPERATOR "\n----\n\n"
#define MAXFNAME 100

int main(int argc, char **argv)
{
    int maxpathname = MAXPATHLEN + MAXFNAME;
    char *path, *pathcpy, *name, c;
    int flags = 0;
    FILE *fp;

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

    // apply arguments using set argument flags
    if (flags &= CUST_PATH) {
        pathcpy = path;
        while (c = *++argv[0]) {
            if (!(maxpathname-- - 1)) {       // reverse 1 for null terminator
                fprintf(stderr,
                        "Logger: directory exceeds maximum length: %d", MAXPATHLEN);
                exit(1);
            }
            *++pathcpy = c;
        }
        *++pathcpy = '\0';
    }
    else
        strcpy(path, DEFAULT_PATH);

    strcat(path, name);




    // get date to name file
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(name, "Log-%02d.%02d.%04d.txt", tm.tm_mon, tm.tm_mday, tm.tm_year + 1900);

    //open file in append mode
    fp = fopen(strcat(path, name), "a");

    if (!fp) {
        fprintf(stderr, "Could not open file: %s", path);
        return -1;
    }

    fprintf(fp, "%02d.%02d.%04d %02d:%02d:%02d\n\n",
            tm.tm_mon, tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    // insert log
    while ((c = getc(stdin)) != EOF)
        putc(c, fp);

    fputs("\n", fp);
    fputs(SEPERATOR, fp);

   exit(0);
}