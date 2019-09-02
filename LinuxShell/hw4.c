#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/procfs.h>
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid */
#include <stdio.h>     /* printf, perror */
#include <stdlib.h>    /* exit */
#include <unistd.h>    /* _exit, fork */
int main()
{
	while(1)
	{	
		char cmd[10];
		char cmd1[10]="ps";
		char cmd2[10]="ls";
		char cmd3[10]="cat";
		char cmd4[10]="pwd";
		char cmd5[10]="echo";
		char cmd6[10]="date";
		char cmd7[10]="time";
		char cmd8[10]="du";
		char cmd9[10]="mkdir";
		char cmd10[10]="cd";
		char cmd11[10]=">>";
		char cmd12[10]="hw4";
		char cmd13[10]="tar";
		char cmd14[10]="df";
		printf("$$");
		scanf("%s",cmd);
		if(strcmp(cmd,cmd1)==0)
		{
			
		}
		else if(strcmp(cmd,cmd2)==0)
		{
			DIR *dir;
			struct dirent *dirent;
			char *where=NULL;
			where=get_current_dir_name();//get current directory name
			dir=opendir(where);
			while((dirent=readdir(dir))!=NULL)
			{
				printf("%s\n",dirent->d_name);
			}
			closedir(dir);
		}
		else if(strcmp(cmd,cmd3)==0)
		{	
			char file[100];
			char data[1024];
			scanf("%s",file);//get file name
			FILE *fp;
			fp=fopen(file,"r");//open file
			if(fp==NULL)perror("file cannot be found");
			else
			{
				while(!feof(fp))
				{			
					fgets(data,1024,fp);
					printf(data);
				}
				fclose(fp);	
			}		
		}
		else if(strcmp(cmd,cmd4)==0)
		{
			char cwd[1024];			
			if(getcwd(cwd,1024)!=NULL)//get current working directory
			fprintf(stdout,"%s\n",cwd);
			else
			perror("pwd cannot be used");
		}
		else if(strcmp(cmd,cmd5)==0)
		{
			char talk[100];
			while(fgets(talk,100,stdin))
			{
				printf("%s",talk);
				break;
			}
		}
		else if(strcmp(cmd,cmd6)==0)
		{
			time_t current_time;//obtain current time as seconds elapsed since the Epoch
    			char* c_time_string;
			current_time = time(NULL); 
    			if (current_time == ((time_t)-1))
    			{
        			fprintf(stderr, "Failure to compute the current time.");
        			return EXIT_FAILURE;
    			}
	    		c_time_string = ctime(&current_time);//convert to local time format
    			if (c_time_string == NULL)
    			{
        			fprintf(stderr, "Failure to convert the current time.");
        			return EXIT_FAILURE;
    			}
    			printf("Current time is %s", c_time_string);
		}
		else if(strcmp(cmd,cmd7)==0)
		{
			clock_t t;
			t=clock();
			scanf("%s",cmd);
			t=clock()-t;
			double time_taken=((double)t)/CLOCKS_PER_SEC;//in seconds
			printf("Output:%f second(s)\n",time_taken);
		}
		else if(strcmp(cmd,cmd8)==0)
		{
			long size;
			char file[100];
			scanf("%s",file);
			FILE *fp;
			fp=fopen(file,"r");
			if(fp==NULL)perror("file cannot be found");
			else
			{
				fseek(fp,0,SEEK_END);//count from first to end
				size=ftell(fp);//get file size
				printf("File size is %ld bytes\n",size);
				fclose(fp);	
			}	
		}
		else if(strcmp(cmd,cmd9)==0)
		{
			char name[100];
			scanf("%s",name);
			int status;
			status=mkdir(name,0777);//make directory with authority 0777
			//S_IRWXU | S_IRWSG | S_IROTH | S_IXOTH
		}
		else if(strcmp(cmd,cmd10)==0)
		{
			char path[100];
			scanf("%s",path);//get path
			int result;
			result=chdir(path);//change directory
		}
		else if(strcmp(cmd,cmd11)==0)
		{
			char text[1024];
			char goal[50];
			gets(text);
			scanf("%s",goal);	
			FILE *fp;
			fp=fopen(goal,"ab");
			if(!fp)perror("file cannot be create");
			else
			{
				fputs(text,fp);
				fclose(fp);	
			}	
		}
		else if(strcmp(cmd,cmd12)==0)
		{
			
		}
		else if(strcmp(cmd,cmd13)==0)
		{
			pid_t pid = fork();
 			if (pid == -1) 
			{
				// When fork() returns -1, an error happened.
      				perror("fork failed");
      				exit(EXIT_FAILURE);
   			}
   			else if (pid == 0) 
			{
      				// When fork() returns 0, we are in the child process.
      				//printf("fork the child process\n");
      				_exit(EXIT_SUCCESS);  
				// exit() is unreliable here, so _exit must be used
   			}
   			else 
			{
      				// When fork() returns a positive number, we are in the parent process
      				// and the return value is the PID of the newly created child process.
      				int status;
      				(void)waitpid(pid, &status, 0);
   			}			
			char a[50];
			char b[50];			
			scanf("%s",a);
			scanf("%s",b);			
			char command[100];
			strcpy(command,cmd);
			strcat(command," ");
			strcat(command,"-czvf");
			strcat(command," ");
			strcat(command,a);
			strcat(command," ");
			strcat(command,b);
			system(command);
		}
		else if(strcmp(cmd,cmd14)==0)
		{
			pid_t pid = fork();
 			if (pid == -1) 
			{
				// When fork() returns -1, an error happened.
      				perror("fork failed");
      				exit(EXIT_FAILURE);
   			}
   			else if (pid == 0) 
			{
      				// When fork() returns 0, we are in the child process.
      				//printf("fork the child process\n");
      				_exit(EXIT_SUCCESS);  
				// exit() is unreliable here, so _exit must be used
   			}
   			else 
			{
      				// When fork() returns a positive number, we are in the parent process
      				// and the return value is the PID of the newly created child process.
      				int status;
      				(void)waitpid(pid, &status, 0);
   			}			
			system(cmd);
		}
		else	printf("Command not found\n");
	}
	return 0;
}
