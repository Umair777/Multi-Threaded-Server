
typedef struct request
{
    long int pid;
        int opr1;
        int opr2;
        char opt;
	int flag;
}request;
typedef struct result
{
        long int pid;
        int opr1;
        int opr2;
        int resl;
}result;
typedef struct
        {
                long int type;
                int msg_rs;
        }msg;
sem_t sem;
                                                                                                                                                  
                                                                                                                                                     
                                                                                                                                                     

