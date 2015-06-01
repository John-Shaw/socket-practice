//
//  main.c
//  sockt-test
//
//  Created by 邵建勇 on 15/3/9.
//  Copyright (c) 2015年 John Shaw. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <strhash.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFFSIZE 1024
#define PORT 10086
#define IP_ADDRESS "127.0.0.1"

void Die(char *mess){
    perror(mess);
    exit(1);
}

int main(int argc, const char * argv[]) {
    int sock;
    struct sockaddr_in echoserver;
    
//    if (argc != 3) {
//        fprintf(stderr, "USAGE: client <server_ip> <port>\n");
//        exit(1);
//    }
    //creat the TCP socket
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        Die("Failed to create socket");
    }
    
    // Construct the server sockaddr_in structure
    memset(&echoserver,0,sizeof(echoserver));
    echoserver.sin_family = AF_INET;                    //Internet/IP
//    echoserver.sin_addr.s_addr = inet_addr(argv[1]);    //IP address
    echoserver.sin_addr.s_addr = inet_addr(IP_ADDRESS);
//    echoserver.sin_port = htons(atoi(argv[2]));         //port
    echoserver.sin_port = htons(PORT);
    
    //Establish connection
    if (connect(sock,
                (struct sockaddr *)&echoserver,
                sizeof(echoserver)) < 0 ) {
        Die("Failed to connect with server");
    }
    
    


    
    char sendbuf[BUFFSIZE];
    char recvbuf[BUFFSIZE];
    memset(sendbuf, 0, sizeof(sendbuf));
    memset(recvbuf, 0, sizeof(recvbuf));
    
    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        sendbuf[strlen(sendbuf) -1]='\0';
        
        send(sock, sendbuf, strlen(sendbuf),0); ///发送
        if(strcmp(sendbuf,"exit")==0){
            break;
        }
        recv(sock, recvbuf, sizeof(recvbuf),0); ///接收
        fputs(recvbuf, stdout);
        
        printf("\n");
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }
    
    close(sock);
    return 0;
    
//    unsigned int echolen;
//    
//    //Send the word to the server
//    echolen = (int)strlen(argv[3]);
//    if (send(sock, argv[3], echolen, 0) != echolen) {
//        Die("Mismatch in number of send bytes");
//    }
//    
//    int received = 0;
//    char buffer[BUFFSIZE];
//    Receive the word back from the server
//    fprintf(stdout, "Received: ");
//    while (received < echolen) {
//        int bytes = 0;
//        if ((bytes = (int)recv(sock, buffer, BUFFSIZE -1, 0)) < 1) {
//            Die("Failed to receive bytes from server");
//        }
//        received += bytes;
//        buffer[bytes] = '\0';
//        fprintf(stdout, "%s", buffer);
//    }
//    fprintf(stdout, "\n");
//    close(sock);
//    exit(0);
}

























