#include"headers.h"
#include"request.h"
#include"declarations.h"
int main()
{
        printf("rc1 begin\n",__func__);
        request rq1;
        result res1;
        msg block;
        int fd1,key;
        key = msgget((key_t)222,0666|IPC_CREAT);
        rq1.pid=getpid();
        rq1.opr1=5;
        rq1.opr2=6;
        rq1.opt='+';
	rq1.flag=1;
        printf("rc1_pid :%d\n operation : %c\n",rq1.pid,rq1.opt);
        fd1 = open("f1",O_WRONLY);
        write(fd1,&rq1,sizeof(request));
	printf("written into fifo 1\n");
        msgrcv(key,&block,sizeof(block),rq1.pid,0);
        printf("result of the 'Addition' is %d\n",block.msg_rs);
        printf("rc1 End\n");
}

