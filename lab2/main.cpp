#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

int main()
{
    cout << "SERVER START" << endl;
    int port = 2008;

    for(;;)
    {


        cout << "-port: " << port << endl;


        int listener;
        int connfd;
        int buffSize;
        struct sockaddr_in serverAddres;
        struct sockaddr_in clientAddres;
        socklen_t clientSize;
        //pid_t     childpid;
        char mesg[1000];




        listener = socket(AF_INET,SOCK_STREAM,0);

        if (listener == -1)
        {
            cout << "-ERROR: creating socket" << endl;
            return 0;
        }

        cout << "-creating socket" << endl;




        serverAddres.sin_family = AF_INET;
        serverAddres.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddres.sin_port = htons(port);

        bind(listener,(struct sockaddr *)&serverAddres,sizeof(serverAddres));

        if(listen(listener,1024) == -1)
        {
            cout << "-ERROR: listening" << endl;
        }

        clientSize = sizeof(clientAddres);

        cout << "-start listening" << endl;
        connfd = accept(listener,(struct sockaddr *)&clientAddres,&clientSize);
        close (listener);

        cout << "-stop listening" << endl;
        cout << "-connect to proc #" << connfd << endl;


        for(;;)
        {
            buffSize = recvfrom(connfd, mesg, 1000, 0, (struct sockaddr*) &clientAddres, &clientSize);
            if( buffSize == -1 || buffSize == 0)
            {
                cout << "-ERROR: process disconnect #" << connfd << endl;
                break;
            }

            int err = 0;
            err = sendto(connfd, mesg, buffSize, 0, (struct sockaddr *)&clientAddres, sizeof(clientAddres));
            if( err == -1 || err == 0)
            {
                cout << "-ERROR: process disconnect #" << connfd << endl;
                break;
            }

            if(mesg != NULL)
            {
                if(strstr(mesg,"-exit"))
                {
                    close(connfd);
                    cout << "-disconnect process #" << connfd << endl;
                    break;
                }
                else
                {
                    mesg[buffSize] = 0;
                    cout << "-process:     #" << connfd << endl;
                    cout << "-new message: " << mesg;
                }
            }

        }

        close(connfd);
        port ++;
    }
}


/*
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char**argv)
{
    int listenfd;
    int connfd;
    int n;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    pid_t     childpid;
    char mesg[1000];


    printf("Server start on port: %d\n",  atoi(argv[1]));


    listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd == -1){
        printf("Error creating socket\n");
        return 0;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(atoi(argv[1]));

    bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    if(listen(listenfd,1024) == -1){
        printf("Error listening");
    }
    for(;;){
        clilen=sizeof(cliaddr);
        connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
        if ((childpid = fork()) == 0){
            close (listenfd);
            for(;;){
                n = recvfrom(connfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&clilen);
                sendto(connfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
                if(mesg != NULL){
                    if(strstr(mesg,"quit")){
                        close(connfd);
                        printf("Disconnect process %d",connfd);
                        exit(childpid);
                    }
                    else{
                        printf("-------------------------------------------------------\n");
                        mesg[n] = 0;
                        printf("Received the following:\n");
                        printf("%s",mesg);
                        printf("-------------------------------------------------------\n");
                    }
                }
            }
        }
	  close(connfd);
    }
}


//*/
/*#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char**argv)
{
   int listenfd;
   int connfd;
   int n;
   struct sockaddr_in servaddr;
   struct sockaddr_in cliaddr;
   socklen_t clilen;
   pid_t     childpid;
   char mesg[1000];

   listenfd=socket(AF_INET,SOCK_STREAM,0);
   if(listenfd == -1){
    printf("Error creating socket\n");
    return 0;
   }

   //bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(32000);
   bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   if(listen(listenfd,1024) == -1){
   printf("Error listening");
   }

   for(;;)
   {
      clilen=sizeof(cliaddr);
      connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);

      if ((childpid = fork()) == 0)
      {
         close (listenfd);

         for(;;)
         {
            n = recvfrom(connfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&clilen);
            sendto(connfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
            printf("-------------------------------------------------------\n");
            mesg[n] = 0;
            printf("Received the following:\n");
            printf("%s",mesg);
            printf("-------------------------------------------------------\n");
         }

      }
      close(connfd);
   }
}*/


/*
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main()
{
    cout << "SERVER START" << endl;

    int listener;
    listener = socket(AF_INET,SOCK_STREAM,0);

    if(listener == -1)
    {
        printf("Error creating socket\n");
        return 0;
    }

    cout << "-creating socket" << endl;

    struct sockaddr_in serverAddres;

    serverAddres.sin_family = AF_INET;
    serverAddres.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddres.sin_port = htons(32000);

    bind(listener,(struct sockaddr *)&serverAddres,sizeof(serverAddres));

    if(listen(listener,1024) == -1)
    {
        printf("Error listening");
    }

    cout << "-start listening" << endl;


    //struct sockaddr_in cliaddr;
    //socklen_t clilen = sizeof(cliaddr);
    //int connfd = accept(listener,(struct sockaddr *)&cliaddr,&clilen);

    close (listener);
    cout << "-close listening" << endl;



   int n;


   socklen_t clilen;
   pid_t     childpid;
   char mesg[1000];

    return 0;
}
*/
