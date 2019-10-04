#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <ctype.h>
#define MILLION 1000000L
bool found, subSetFound=false;
struct timeval tpend;
struct timeval tpstart;
long timedif;
FILE *inputFile, *outputFile;

void delay(ms){
        clock_t timeDelay = ms + clock();
        while(timeDelay>clock());
}

void displaySubset(int subSet[], int size, int sum) {
        int i;
        bool firstTime = true;
        subSetFound = true;
        fprintf (outputFile,"\n%d --> ", getpid());
        for(i = 0; i < size; i++) {
                if (firstTime){
                        fprintf(outputFile,"%d ",subSet[i]);
                        firstTime = false;
                }
                else{
                        fprintf(outputFile,"+ %d ",subSet[i]);
                }
        }
        fprintf (outputFile,"= %d\n", sum);
}

void subsetSum(int set[], int subSet[], int n, int subSize, int total, int nodeCount, int sum) {
        if (gettimeofday(&tpend, NULL)) {
                fprintf(stderr, "Failed to get end time\n");
                return;
        }
        timedif = MILLION*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;
        if ((timedif/1000000) > 1) {
                fprintf(outputFile,"%d --> time out\n", getpid());
                exit(1);
        }
        if( total == sum) {
                found = true;
                displaySubset(subSet, subSize, sum);
                return;
        } else {
                int i;
                for(i = nodeCount; i < n; i++ ) {
                        subSet[subSize] = set[i];
                        if (!found) subsetSum(set,subSet,n,subSize+1,total+set[i],i+1,sum);
                }
        }
}

void findSubset(int set[], int size, int sum, int size2) {
        int subSet[size];
        int set2 [size2];
        int i, j=0;
        for (i=0;i<size;i++){
                if(set[i]!=0){
                        set2[j] = set[i];
                        j++;
                }
        }
        if (gettimeofday(&tpstart, NULL)) {
                fprintf(stderr, "Failed to get start time\n");
                return;
        }
        subsetSum(set2, subSet, size2, 0, 0, 0, sum);
}

int main (int argc, char *argv[]) {
        char *inputFileName, *outputFileName;
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
        if (hflag){
                printf ("Help Message\n");
                printf ("This program finds the subset which gives us the destintation sum when all added\n");
                printf ("Please see following execute options\nh Print a help message and exit\n");
                printf ("i lets you enter the input file name\n");
                printf ("o lets you enter the output file name\n");
                printf ("t lets you define time to end the program\n");
                exit(1);
        }
        /* open file */
        if (iflag) inputFileName = argOfi;
        else inputFileName = "input.dat";
        if (oflag) outputFileName = argOfo;
        else outputFileName = "output.dat";
        if ((inputFile = fopen(inputFileName, "r")) == NULL){
                perror("Error! opening input file");
                exit(1);
        }
        if ((outputFile = fopen(outputFileName, "w")) == NULL){
                perror("Error! opening output file");
                exit(1);
        }
        /* get the number of processes */
        int numberOfProcesses;
        fscanf(inputFile, "%i", &numberOfProcesses);
        int PIDs [numberOfProcesses];
        /* variables */
        int ch;
        int cnt;
        int LINE_SIZE = 255;
        char line[LINE_SIZE];
        int i, sum, tmp, count, mult, index, j;
        int ARR_SIZE=100;
        int arr[ARR_SIZE];
        int ARR2_SIZE;
        int parentPID = getpid();
        /*consume first end of line*/
        while(1){
                ch = fgetc(inputFile);
                if ( feof(inputFile) || ch == '\n')
                        break;
        }

        /* creating children with fork */
        int g; pid_t childpid = 0;
        for (g = 1; g < numberOfProcesses; g++)
                if (childpid = fork())
                        break;
        /* skipping line that has been processed before with another child */
        int h;
        for (h=1; h<g; h++) {
                /*consume end of line*/
                while(1){
                        ch = fgetc(inputFile);
                        if ( feof(inputFile) || ch == '\n')
                                break;
                }
        }
/* ---------------------------------------------------------- */
        /* fill arr with 0 */
        for (i=0; i<ARR_SIZE; i++){
                arr[i] = 0;
        }
        for (i=0; i<LINE_SIZE; i++){
                line[i] = 0;
        }
        cnt=0;
        /* read a line */
        while(1){
                ch = fgetc(inputFile);
                if (feof(inputFile)){
                        break;
                } else if (ch == '\n'){
                        break ;
                } else {
                        line[cnt] = (char)ch;
                        cnt++;
                }
        }
        /* get line items into variables */
        index=0; tmp=0; count=0; mult=1, ARR2_SIZE=0;
        for(i=strlen(line)-1; i>=0; i--) {
                if(line[i] != ' ' && isdigit(line[i])) {
                        tmp += (line[i]-'0') * mult;
                        mult *= 10;
                }
                else if (line[i] == ' '){
                        arr[index]=tmp;
                        index++;
                        tmp=0;
                        mult=1;
                }
        }
        if (tmp != 0) {
                sum = tmp;
        }

        for (i=0; i<ARR_SIZE; i++){
                if(arr[i]!=0) {
                        ARR2_SIZE++;
                }
        }
        found = false;
        findSubset(arr,ARR_SIZE,sum,ARR2_SIZE);
        while (wait(NULL) > 0);
        if(!subSetFound)
                fprintf(outputFile,"\n%d --> NO SUBSET FOUND for %d\n", getpid(), sum);
/* ---------------------------------------------------------- */
        if(getpid()==parentPID){
                fclose(outputFile);
                fclose(inputFile);
        }
        return 0;
} /* end of main */
