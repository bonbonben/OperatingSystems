#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <pthread.h>
#include <netdb.h>

/*�w�q�P�ɯ�B�z���̤j�s���n�D*/
#define backlog 10
/*�w�q��B�z���̤j��Ƥj�p*/
#define maxDataSize 1024

int sock_fd,new_fd;
pthread_t receiveThread;

/*�����T�����*/
void receiveText(void)
{
	while(1)
	{
		int numBytes;
		char buf[maxDataSize];
		if((numBytes=recv(sock_fd,buf,maxDataSize,0))==-1)
		{
			perror("recv");
			exit(1);
		}
		buf[numBytes]='\0';
		/*����file�}�l����*/
		if(strcmp(buf,"file")==0)
		{
			char filename2[1024];
			recv(sock_fd,filename2,maxDataSize,0);/*�����ɦW*/
			FILE *fr=fopen(filename2,"w");/*�}�l�g��*/
			printf("receiveing file from server ......\n");
			if(fr==NULL)
			{
				printf("file %s cannot be opened\n",filename2);
			}
			else
			{
				bzero(buf,maxDataSize);
				int fr_block_sz=0;
	    			while((fr_block_sz=recv(sock_fd,buf,maxDataSize,0))>0)
	    			{
					int write_sz=fwrite(buf,sizeof(char),fr_block_sz,fr);
	        			if(write_sz<fr_block_sz)
					{
	            				error("file write failed\n");
	        			}
					bzero(buf,maxDataSize);
					if (fr_block_sz==0||fr_block_sz!=maxDataSize)
					{
						break;
					}
				}
				if(fr_block_sz<0)
        			{
					if(errno==EAGAIN)
					{
						printf("recv() timed out\n");
					}
					else
					{
						fprintf(stderr,"recv() failed due to errno=%d\n", errno);
					}
				}
	    			printf("received from server\n");
	    			fclose(fr);
			}
		}
		/*����quit���}*/
		else if(strcmp(buf,"quit")==0)
		{
			printf("server is closed\n");
			close(sock_fd);
			exit(1);
		}
		/*����T���L�X*/
		else printf("server:%s\n",buf);
	}
}
int main(int argc,char *argv[])
{
	struct hostent* hostinfo;
	struct sockaddr_in serv_addr;
	/*�קK�S��Jip�Mport*/
	if(argc<2)
	{
		fprintf(stderr,"usage:%s ip_address port_number\n",argv[0]);
		exit(1);
	}
	/*����D��ip��}*/
	if((hostinfo=gethostbyname(argv[1]))==NULL)
	{
		herror("gethostbyname");
		exit(1);
	}
	/*�гysocket�ð���*/
	if((sock_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(1);
	}
	/*��l��sockaddr_in���c�����Ѽ�*/
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(atoi(argv[2]));
	serv_addr.sin_addr=*((struct in_addr *)hostinfo->h_addr);
	bzero(&(serv_addr.sin_zero),8);
	/*�Vserver�o�e�s���ШD*/
	if(connect(sock_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr))==-1)
	{
		perror("connect");
		exit(1);
	}
	/*�Ыؤl�u�{�Ω󱵨��T��*/
	if((pthread_create(&receiveThread,NULL,(void*)receiveText,NULL))!=0)
	{
		printf("create thread error\r\n");
		exit(1);
	}
	while(1)
	{
		char msg[maxDataSize];
		scanf("%s",msg);/*Ū����J�T��*/
		if(send(sock_fd,msg,strlen(msg),0)==-1)
		{
			perror("send");
			exit(1);
		}
		/*��Jfile����*/
		if(strcmp(msg,"file")==0)
		{
			char filename3[1024];
			printf("filename:");/*��J�ɦW*/
			scanf("%s",filename3);/*Ū���ɦW*/
			send(sock_fd,filename3,maxDataSize,0);/*�ǥX�ɦW*/
			char sdbuf[maxDataSize];
			printf("sending %s to server......\n",filename3);
			FILE *fs=fopen(filename3,"r");/*�}�lŪ��*/
			/*�קK�䤣���ɮ�*/
			if(fs==NULL)
			{
				printf("file %s not found\n",filename3);
				exit(1);
			}
			bzero(sdbuf,maxDataSize);
			int fs_block_sz;
			while((fs_block_sz=fread(sdbuf,sizeof(char),maxDataSize,fs))>0)
			{
		    		if(send(sock_fd,sdbuf,fs_block_sz,0)<0)
		    		{
		        		fprintf(stderr,"ERROR:Failed to send file %s.(errno = %d)\n",filename3,errno);
		        		break;
		    		}
		    		else bzero(sdbuf,maxDataSize);
			}
			printf("file %s from client was sent\n",filename3);
			fclose(fs);
		}
		/*��Jquit���}*/
		if(strcmp(msg,"quit")==0)
		{
			printf("goodbye\n");
			close(sock_fd);
			exit(1);
		}
	}
	return 0;
}
