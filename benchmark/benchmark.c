#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include <keyvalue.h>
#include <fcntl.h>
#include <pthread.h>


    int devfd, number_of_keys = 1024, number_of_transactions = 65536;
    int tid;
    __u64 size;


static void *thread_start1(void *arg) {
    int i;
    char data[number_of_keys][1024];

    for( i = 0; i < number_of_keys; i++)
    {
        memset(data[i], 0, 1024);
        int a = rand();
        sprintf(data[i],"%d",a);
        tid = kv_set(devfd,i,strlen(data[i]),data[i]);
        fprintf(stderr,"S\t%d\t%d\t%d\t%s\n",tid,i,strlen(data[i]),data[i]);
    }
	
}

static void *thread_start2(void *arg) {
  int i;
  void *getdata[number_of_transactions];

  for(i = 0; i < number_of_transactions; i++)
    {
	getdata[i]=malloc(4*1024);
        tid = kv_get(devfd,i,&size,getdata[i]);
        fprintf(stderr,"G\t%d\t%d\t%d\t%s\n",tid,i,size,(char*)getdata[i]);
        
    }

}





int main(int argc, char *argv[])
{
    int number_of_threads = 1;
    pthread_t t1, t2;

    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s number_of_keys number_of_transactions\n",argv[0]);
        exit(1);
    }
    number_of_keys = atoi(argv[1]);
    number_of_transactions = atoi(argv[2]);
    devfd = open("/dev/keyvalue",O_RDWR);

    if(devfd < 0)
    {
        fprintf(stderr, "Device open failed");
        exit(1);
    }
    srand((int)time(NULL)+(int)getpid());
    pthread_create(&t1, NULL, thread_start1, NULL);
    pthread_create(&t2, NULL, thread_start2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
  
    close(devfd);
    return 0;
}

