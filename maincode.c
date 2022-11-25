#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include <sys/wait.h>
#include <pthread.h>

char cwd[1000];
char prevWorkingDirectory[1000];

int stringCompare(char one[], char two[]) {
    return strcmp(one, two);
}
void callChdir(char* add){
    int x=chdir(add);
    if(x==-1){
        printf("Can't change directory to %s\n",add);
    }
    else{
        strcpy(prevWorkingDirectory,cwd);
        strcpy(cwd,add);
    }
}
void cd(char* argument){
    if(argument==NULL || stringCompare(argument,"-")==0){
        callChdir(prevWorkingDirectory);
    }
    else if(stringCompare("..",argument)==0){
        callChdir("../");
    }
    else if(stringCompare("~",argument)==0){
        callChdir(getenv("HOME"));
    }
    else{
        if(!stringCompare("-L",argument) || !stringCompare("-P",argument)){
            argument=strtok(NULL," ");
            if(argument==NULL){
                callChdir(getenv("HOME"));
            }
            else{
                callChdir(argument);
            }
        }
        else {
            callChdir(argument);
        }
    }
}
void pwd(char* argument){
    if(argument!=NULL){
        if(!stringCompare(argument,"-L")){
            printf("%s\n",cwd);
        }
        else{
            printf("Command not found \n");
        }
    }
    else{
        printf("%s\n",cwd);
    }
}
void echo(char* argument){
    if(argument==NULL){
        printf("\n");
    }
    else if(stringCompare(argument,"--help")==0){
        printf("echo [argument]\n");
    }
    else{
        while(argument!=NULL){
            printf("%s ",argument);
            argument=strtok(NULL," ");
        }
        printf("\n");
    }

}
void exxit(char* token){
    if(token==NULL){
        exit(0);
    }
    else{
        printf("Invalid command \n");
    }
}
int checkThread(char *input) {
    char *token = strtok(input, " ");
    while (token != NULL) {
        if (!stringCompare(token, "&t")) {
            return 1;
        }
        token = strtok(NULL, " ");
    }
    return 0;
}
void * ls_thread(void * token) {
    char command[50] = "";
    strcat(command, "./ls ");
    strcat(command, token);
    system(command);
}
void * cat_thread(void * token) {
    char command[50] = "";
    strcat(command, "./cat ");
    strcat(command, token);
    system(command);
}
void * date_thread(void * token) {
    char command[50] = "";
    strcat(command, "./date ");
    strcat(command, token);
    system(command);
}
void * mkdir_thread(void * token) {
    char command[50] = "";
    strcat(command, "./mkdir ");
    strcat(command, token);
    system(command);
}
void * rm_thread(void * token) {
    char command[50] = "";
    strcat(command, "./rm ");
    strcat(command, token);
    system(command);
}

int main() {
    printf("Welcome to Igarashi's shell\n");
    while(1){
        char input[100];
        char splits[105]="";
        char split2[105]="";
        scanf("%[^\n]%*c",input);
        strcat(splits,input);
        strcat(splits," ");
        strcat(split2,input);
        strcat(split2," ");
        if (checkThread(split2)) {
            char *token1;
            token1 = strtok(input, " ");
            if (!stringCompare(token1, "ls")) {
                token1 = strtok(NULL, " ");
                pthread_t ptid;
                pthread_create(&ptid, NULL, &ls_thread, token1);
                pthread_join(ptid, NULL);
                pthread_exit(NULL);
            } else if (!stringCompare(token1, "cat")) {
                token1 = strtok(NULL, " ");
                pthread_t ptid;
                pthread_create(&ptid, NULL, &cat_thread, token1);
                pthread_join(ptid, NULL);
                pthread_exit(NULL);
            } else if (!stringCompare(token1, "date")) {
                token1 = strtok(NULL, " ");
                pthread_t ptid;
                pthread_create(&ptid, NULL, &date_thread, token1);
                pthread_join(ptid, NULL);
                pthread_exit(NULL);
            } else if (!stringCompare(token1, "mkdir")) {
                token1 = strtok(NULL, " ");
                pthread_t ptid;
                pthread_create(&ptid, NULL, &mkdir_thread, token1);
                pthread_join(ptid, NULL);
                pthread_exit(NULL);
            } else if (!stringCompare(token1, "rm")) {
                token1 = strtok(NULL, " ");
                pthread_t ptid;
                pthread_create(&ptid, NULL, &rm_thread, token1);
                pthread_join(ptid, NULL);
                pthread_exit(NULL);
            }
        }
        else {
            char *token;
            token = strtok(input, " ");
            if (!stringCompare(token, "cd")) {
                token = strtok(NULL, " ");
                cd(token);
            }
            else if (!stringCompare(token, "echo")) {
                token = strtok(NULL, " ");
                echo(token);
            }
            else if (!stringCompare(token, "pwd")) {
                token = strtok(NULL, " ");
                pwd(token);
            }
            else if (!stringCompare(token, "exit")) {
                token = strtok(NULL, " ");
                exxit(token);
            }
            else if (!stringCompare(token, "ls")) {
                printf("doing ls\n");
                pid_t pid;
                pid = fork();
                if (pid == 0) {
                    token = strtok(NULL, " ");
                    execl("./ls", "./ls", token, splits, NULL);
                    exit(0);
                } else if (pid > 0) {
                    waitpid(0, NULL, 0);
                } else {
                    printf("Error in forking\n");
                }
            }
            else if (!stringCompare(token, "cat")) {
                pid_t pid;
                pid = fork();
                if (pid == 0) {
                    token = strtok(NULL, " ");
                    execl("./cat", "./cat", token, splits, NULL);
                    exit(0);
                } else if (pid > 0) {
                    waitpid(0, NULL, 0);
                } else {
                    printf("Error in forking\n");
                }
            }
            else if (!stringCompare(token, "date")) {
                pid_t pid;
                pid = fork();
                if (pid == 0) {
                    token = strtok(NULL, " ");
                    execl("./date", "./date", token, splits, NULL);
                    exit(0);
                } else if (pid > 0) {
                    waitpid(0, NULL, 0);
                } else {
                    printf("Error in forking\n");
                }
            }
            else if (!stringCompare(token, "mkdir")) {
                pid_t pid;
                pid = fork();
                if (pid == 0) {
                    token = strtok(NULL, " ");
                    execl("./mkdir", "./mkdir", token, splits, NULL);
                    exit(0);
                } else if (pid > 0) {
                    waitpid(0, NULL, 0);
                } else {
                    printf("Error in forking\n");
                }
            }
            else if (!stringCompare(token, "rm")) {
                pid_t pid;
                pid = fork();
                if (pid == 0) {
                    token = strtok(NULL, " ");
                    execl("./rm", "./rm", token, splits, NULL);
                    exit(0);
                } else if (pid > 0) {
                    waitpid(0, NULL, 0);
                } else {
                    printf("Error in forking\n");
                }
            }
            else {
                printf("Command not found\n");
            }
        }
    }
}
