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

struct TXT;
class FileRW;
int MessageConvert (char* mesg,int buffSize,TXT &filename,TXT &fileContent);


struct TXT
{
    int size;
    char text[4096];

    void Write()
    {
        cout << "\n Input text: \n";
        cin >> text;
        size = strlen(text);
        cout << "\n len = " << size;
    }
};

class FileRW
{
    public:
    FileRW()
    {

    }

    // функция записи в файл, FileName - путь к файлу,
    // если файла нет он будет создан
    static void WriteFile(char* FileName,TXT txt)
    {
        FILE *f;
        f = fopen(FileName, "wb");

        for(int i = 0; i < txt.size; i++)
        {
            //if(text[i] == '\0') break;
            fwrite(&txt.text[i], sizeof(char), 1, f);
        }

        fclose(f);
    }

    // функция чтения из файла, FileName - путь к файлу,
    static int ReadFile(char* FileName,TXT &txt)
    {
        FILE *f;
        f = fopen(FileName, "rb");

        if( f == NULL )
        {
            cout << "\n ERROR: can not open the file";
            return -1;
        }

        char a;
        int i = 0;

        while(fread(&a, sizeof(char), 1, f) != 0)
        {
            txt.text[i] = a;
            i++;
        }
        txt.text[i-1] = '\0';
        txt.size = i - 1;

        fclose(f);

        return 0;
    }

    /*  TXT txt; txt.Write();
    /   FileRW::WriteFile(s,txt);
    /   FileRW::ReadFile(s,txt);
    /                            */
};



int main()
{
    cout << "SERVER START" << endl;
    int port = 2001;

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
            buffSize = recv(connfd, mesg, 1000, 0);
            if( buffSize <= 0)
            {
                cout << "-ERROR(RECV): process disconnect #" << connfd << " buffSize = " << buffSize << endl;
                break;
            }
            int err = 0;
            err = send(connfd, mesg, buffSize , 0);
            if( err <= 0)
            {
                cout << "-ERROR(SEND): process disconnect #" << connfd << " err = " << err << endl;
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
                    //mesg[buffSize] = 0;
                    cout << "-process: #" << connfd ;
                    cout << " new message: " << endl;
                    //cout << "-file name: ";

                    //int j;
                    //for( j = 0; j < buffSize; j++)
                    //{
                    //    cout << mesg[j];
                    //    if(mesg[j] == '.')
                    //       break;
                    //}
                    //cout << mesg[++j];
                    //cout << mesg[++j];
                    //cout << mesg[++j];
                    //j+=2;
                    //cout << endl << "-file content: ";

                    //for( ; j < buffSize; j++)
                    //{
                    //    cout << mesg[j];
                    //}
                    TXT a;
                    TXT b;
                    if(MessageConvert(mesg,buffSize,a,b) == 0)
                    {
                        FileRW::WriteFile(a.text,b);
                        cout << "-file: <" << a.text << "> create" << endl;
                        cout << "\n+++";
                    }
                }
            }
        }

        close(connfd);
        port ++;
    }
}//*/


int MessageConvert (char* mesg,int buffSize,TXT &filename,TXT &fileContent)
{
    int flg = 0;
    int j;
    for( j = 0; j < buffSize; j++)
    {
        filename.text[j] = mesg[j];
        if(mesg[j] == '.')
        {
            flg = 1;
            break;
        }
    }

    if(flg == 0)
    {
        cout << "\n -ERROR: filename not found!!! ";
        cout << "\n+++";
        return -1;
    }

    j++; filename.text[j] = mesg[j];
    j++; filename.text[j] = mesg[j];
    j++; filename.text[j] = mesg[j];
    j++; filename.text[j] = '\0';
    filename.size = strlen(filename.text);
    j++;

    int i = 0;
    for( ; j < buffSize; j++)
    {
        fileContent.text[i++] = mesg[j];
    }
    fileContent.text[i] = '\0';

    fileContent.size = strlen(fileContent.text);

    cout << "\n filename    text: <" << filename.text << "> ";
    //cout << "\n filename size:    <" << filename.size << "> ";
    cout << "\n fileContent text: <" << fileContent.text << "> ";
    //cout << "\n fileContent size: <" << fileContent.size << "> ";
    return 0;
}




























