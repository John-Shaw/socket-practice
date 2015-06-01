//
//  main.c
//  socket-server
//
//  Created by 邵建勇 on 15/3/23.
//  Copyright (c) 2015年 John Shaw. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "Dictionary.h"
#include <assert.h>

#define MAXPENDING 5    /* Max connection requests */
#define BUFFSIZE 1024

#define PORT 10086

//struct nlist *dictionary;
Dict dic;

void Die(char *mess){
    perror(mess);
    exit(1);
}

void HandleClient(int sock) {
    char buffer[BUFFSIZE];
    memset(buffer, 0, sizeof(buffer));
    int received = -1;
    /* Receive message */
    if ((received = (int)recv(sock, buffer, BUFFSIZE, 0)) < 0) {
        Die("Failed to receive initial bytes from client");
    }
    
    while (received > 0) {
        char *recString = buffer;
        
        printf("recString:%s\n",recString);
        if (strcmp(recString, "exit") == 0){
            exit(0);
        }
        const char *value = DictSearch(dic, recString);
        if (value) {
            const char *sendSrting= value;
            int sendLen = (int)strlen(sendSrting);
            if (send(sock, sendSrting, sendLen, 0) != sendLen) {
                Die("Failed to send bytes to client");
            }
            printf("sendString:%s\n",sendSrting);
            memset(buffer, 0, sizeof(buffer));
            memset(recString, 0, sizeof(&recString));
        } else {
            char sendSrting[BUFFSIZE]= "unkown id";
            int sendLen = (int)strlen(sendSrting);
            if (send(sock, sendSrting, sendLen, 0) != sendLen) {
                Die("Failed to send bytes to client");
            }
            printf("sendString:%s\n",sendSrting);
            memset(buffer, 0, sizeof(buffer));
            memset(recString, 0, sizeof(&recString));
        }
        /* Check for more data */
        if ((received = (int)recv(sock, buffer, BUFFSIZE, 0)) < 0) {
            Die("Failed to receive additional bytes from client");
        }
    }
    close(sock);
}

//void split(char **arr, char *str, const char *del) {
//   char *s = strtok(str, del);
//
//   while(s != NULL) {
//     *arr++ = s;
//     s = strtok(NULL, del);
//   }
//}
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    
    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    
    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);
    
    /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
    count++;
    
    result = malloc(sizeof(char*) * count);
    
    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);
        
        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    
    return result;
}

int readParttenFormFile(char *fileName){
    const char del = ',';
    char **partten;
    FILE *fin;
    char *line;
    line = (char*)malloc( 80 * sizeof(char) );
    if((fin = fopen(fileName, "r"))==NULL)
    {
        return 0;
    }
    
    while( fgets(line, 80, fin) != NULL )
    {
        partten = str_split(line, del);
        int length = (int)strlen(partten[1]);
        if (partten[1][length-1] == '\n') {
            partten[1][length-1] = '\0';
        }
        DictInsert(dic, partten[0], partten[1]);
        
    }

    return 1;
}

int main(int argc, char *argv[]) {
    int serversock, clientsock;
    struct sockaddr_in echoserver, echoclient;
    
//    if (argc != 2) {
//        fprintf(stderr, "USAGE: server <port>\n");
//        exit(1);
//    }
    /* Create the TCP socket */
    if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        Die("Failed to create socket");
    }
    /* Construct the server sockaddr_in structure */
    memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
    echoserver.sin_family = AF_INET;                  /* Internet/IP */
    echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
//    echoserver.sin_port = htons(atoi(argv[1]));       /* server port */
    echoserver.sin_port = htons(PORT);
    
    /* Bind the server socket */
    if (bind(serversock, (struct sockaddr *) &echoserver,
             sizeof(echoserver)) < 0) {
        Die("Failed to bind the server socket");
    }
    /* Listen on the server socket */
    if (listen(serversock, MAXPENDING) < 0) {
        Die("Failed to listen on server socket");
    }
    
    
    dic= DictCreate();
    int flag = readParttenFormFile("./strings");
    if (flag) {
        
    } else {
        printf("file not found");
        exit(1);
    }
    /* Run until cancelled */
    while (1) {
        unsigned int clientlen = sizeof(echoclient);
        /* Wait for client connection */
        if ((clientsock =
             accept(serversock, (struct sockaddr *) &echoclient,
                    &clientlen)) < 0) {
                 Die("Failed to accept client connection");
             }
        fprintf(stdout, "Client connected: %s\n",
                inet_ntoa(echoclient.sin_addr));
        HandleClient(clientsock);
        
    }
    
}

















