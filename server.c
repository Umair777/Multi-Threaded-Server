#include"headers.h"
#include"declarations.h"
#include"request.h"
void *threadfunc(void*); //initiate
void *threadfunc2(void*); //clients
void *rptr;
sem_t sem;
int fd1,fd2,fd3,fd4,i,count=0;
request rq;
int main()
{
	pthread_t thread_id;
	sem_init( &sem , 0, 1);
	unlink("f1");
	unlink("f4");
	unlink("f2");
	unlink("f3");
	creatfifo();
	printf("FIFOS_CREATED\n");
	if((pthread_create(&thread_id,NULL,threadfunc,(void*)rptr))==0)
		printf("creating 1st thread\n");
	while(1);
}
void *threadfunc(void* rptr)
{
	int i=0;
	pthread_t thread_id2;
while(1)
{
	printf("thread 1 \n");
	//	sem_wait(&sem);
		fd1 = open("f1",O_RDONLY);
		i=read(fd1,&rq,sizeof(request));
	while(i>0&&rq.flag==1)
	{
			printf("creating thread for requesting clients\n");
			pthread_create(&thread_id2,NULL,threadfunc2,(void*)rptr);
			i=0;
	}

}	
			printf("coming out from thread 1\n");
	printf("need to wait \n");
}
void *threadfunc2(void * rptr)
{

		//	count++				
		//	printf("******client no : %d*******  \n",count);
//	sem_post(&sem);
	sem_wait(&sem);
	printf("In IPC\n");
	char ch;
	result res;
	pid_t pc1,pc2,fret,key;
	msg block;
	key = msgget((key_t)222,0666|IPC_CREAT);
	printf("messege queue created\n");
	printf("this is thread with pid %d\n",getpid());
	printf("new process_pid %d\n opt :%c\n",rq.pid,rq.opt);
	ch=rq.opt;
	switch(ch)
	{
		case '+':pc1=fork();    
			switch(pc1)
			{
				case -1:
					perror("fork");
					exit(EXIT_FAILURE);
					break;
				case 0:
					printf("this is processing client child with pid %d\n",getpid());
					printf("operation of the request is %c\n",rq.opt);
					execl("./adder","adder",(char *)0);
					break;
				default:

					printf("parent in + opt\n");
					fd2 = open("f2",O_WRONLY);
					write(fd2,&rq,sizeof(request));
					break;
			}
			break;
		case '-':pc2=fork();    
			switch(pc2)
			{
				case -1:

					perror("fork");
					exit(EXIT_FAILURE);
					break;
				case 0:
					printf("this is processing client child with pid %d\n",getpid());
					printf("operation of the request is %c\n",rq.opt);
					execl("./sub","sub",(char *)0);
					break;
				default:

					fd2 = open("f2",O_WRONLY);
					write(fd2,&rq,sizeof(request));
				//	sem_post(&sem);
					break;
			}

		}
			printf("thread2 now handling process client of pid %d\n",getpid());
			fd3 = open("f3",O_RDONLY);
			read(fd3,&res,sizeof(result));
			printf("result in server is %d\n",res.resl);
			printf("......server reading the result.........\n");
			block.msg_rs=res.resl;
			block.type=res.pid;
			msgsnd(key,&block,sizeof(block),0);
			printf("......message is sent in queue.........\n");
			if((sem_post(&sem))==0)
				printf("thread function for clients is releasing the semaphore\n");
}
