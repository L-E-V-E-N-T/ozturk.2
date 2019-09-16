#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main (int argc, char * const argv[]) {
        int option;
        char *argOfi;
	char *argOfo;
	int argOft;
        int optCount = 0;
        bool hflag = false;
        bool iflag = false;
        bool oflag = false;
        bool tflag = false;
        while ((option = getopt (argc,argv,"hi:o:t:")) != -1) {
                switch (option) {
                        case 'h':
                                hflag = true;
                                optCount++;
                                break;
                        case 'i':
                                iflag = true;
                                optCount++;
				argOfi = optarg;
                                break;
                        case 'o':
                                oflag = true;
                                optCount++;
				argOfo = optarg; 
                                break;
                        case 't':
                                tflag = true;
                                optCount++;
				argOft = atoi(optarg); 
                                break;
                } /* end of switch*/
        } /* end of while */
        printf("\n\n");
        if (hflag){
                printf ("Help Message\n");
                printf ("This program traverse directories\n");
                printf ("Please see following execute options\nh Print a help message and exit\nI n Change indentation tonspaces for each level\n");
                printf ("L Follow symbolic links, if any. Default will be to not follow symbolic links\n");
                printf ("t Print information on file type\np Print permission bits asrwxrwxrwx\ni Print the number of links to file in inode table\n");
                printf ("u Print theuidassociated with the file\ng Print thegidassociated with the file\ns Print the size of file in bytes\nd Show the time of last modification\n");
                printf ("l This option will be used to print information on the file as if the optionstpiugsare all specified\n");
                exit(1);
        }
	

        return 0;

} //end of main

