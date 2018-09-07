#include"headers.h"
#include"request.h"
int main(int argc , char* argv[])
{
        printf("processing client as 'sub': begin\n");
        result res;
        request rq;
        int fd3,fd2;

        fd2 = open("f2",O_RDONLY);
        read(fd2,&rq,sizeof(rq));
        res.pid=rq.pid;
        res.opr1=rq.opr1;
        res.opr2=rq.opr2;
        res.resl=res.opr1-res.opr2;
        printf("pc2_as_sub_id :%d\n for : %c ",res.pid,rq.opt);
        fd3 = open("f3",O_WRONLY);
        write(fd3,&res,sizeof(result));
        printf("processing client as 'sub': End\n");
}


