#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
int main()
{
    FILE *fp;	//open file
    char buf[1024];
    fp=fopen("/proc/proc","r");	//read proc
    if(fp==NULL)
    {
        perror("fopen");
        return -1;
    }
    printf("command/process ID\n");	//print title
    while(fgets(buf,sizeof(buf),fp)!=NULL)	//print pid
    {
	printf("%s",buf);
    }
    fclose(fp);	//close file
    return 0;
} 
