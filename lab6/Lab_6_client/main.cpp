#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

int main()
{
    cout << "CLIENT START" << endl;

    char message[100];
    char buf[100];


    struct sockaddr_in addr;
    char ip[100];
    int port;

    cout << "-input ip:" << endl;
    cin >> ip;

    cout << "-input port:" << endl;
    cin >> port;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // или любой другой порт...
    //addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    inet_pton(AF_INET, ip, & addr.sin_addr);

    //cout << "-p:  " << p << endl;
    //cout << "-ip: " << ip << endl;
    //cout << "-ip: " << addr.sin_addr.s_addr << endl;
    //printf("-ip: %d",addr.sin_addr.s_addr);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

    cout << "-input message:" << endl;
    cin >> message;

    send(sock, message, strlen(message), 0);
    int ff = recv(sock, buf, sizeof(message), 0);

    cout << "\n ff = " << ff << endl;
    cout << buf;
    cout << "-client stop";


    system("pause");
    close(sock);//*/


    return 0;
}
