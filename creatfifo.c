#include"headers.h"
int creatfifo()
{
        char** fi;
        int i;
        fi= (char **)malloc(sizeof(char *)*4);
        for(i=0;i<4;i++)
        {
                *(fi+i)= (char *)malloc(sizeof(char )*2);
        }
        *(fi+0)="f1";
        *(fi+1)="f2";
        *(fi+2)="f3";
        *(fi+3)="f4";
        for(i=0;i<4;i++)
        {
                mkfifo(*(fi+i),0666|IPC_CREAT);
        }
        return 0;
}
 
