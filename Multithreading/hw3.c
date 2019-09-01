#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct bst	//create structure include data,left and right
{
    int data;	//value
    struct bst* left;	//left child 
    struct bst* right;	//right child
};
void delete(struct bst* node)	//delete a node recursively
{
	if(node==NULL)return;	//delete complete return
	delete(node->right);	//delete right child
	delete(node->left);	//delete left child 
	free(node);
}
int main(int argc, char *argv[])
{	
	if(argc<2)	//if did not enter input file name and output file name
	{
		perror("please enter input file name and output file name");
		exit(1);
	}
	else
	{
		int *new_test;	//integer test data
		int str_space[1000];	//1000 data
		int num=0,i=0;
		char c[25],mystring[25];
		char *space=" ";	//space between number
		char *test;	//character test data
		FILE* fp;	//open file
		fp=fopen(argv[1],"r");	//read input file
		if(fp==NULL)perror("file cannot be found");	//input file does not exist	
		fgets(c,25,fp);	//get test data from first line
		for(i=0;i<25;i++)mystring[i]=c[i];	//copy test data
		test=strtok(mystring,space);
		while(test!=NULL)
		{
			num++;
			test=strtok(NULL,space);	//remove the space between test data
		}
		new_test=(int*)malloc(sizeof(int)*num);
		test=strtok(c,space);
		i=0;
		while(test!=NULL)
    		{
	    		new_test[i]=atoi(test);		//character to integer
			i++;
        		test=strtok(NULL,space);	//remove the space between test data
    		}
		i=0;
		while(EOF!=fscanf(fp,"%d",&str_space[i]))	//end of file
		i++;
		if(i!=1000)printf("data only have %d\n",i);	//data not enough
		fclose(fp);
		struct bst* root=(struct bst*)malloc(sizeof(struct bst));	//initialize root
		root->data=str_space[0];	//first data
		root->right=NULL;
		root->left=NULL;
		struct bst* now;
		for(i=1;i<1000;i++)
		{
			now=root;
			while(1)
			{
				if(str_space[i]<=now->data)	//data not bigger than root
				{
					if(now->left==NULL)
					{
						struct bst* temp=(struct bst*)malloc(sizeof(struct bst));
						temp->data=str_space[i];
						temp->right=NULL;
						temp->left=NULL;
						now->left=temp;	//data go to left
						break;
					}
					else now=now->left;	//root go to left
				}
				else	//data bigger than root
				{
					if(now->right==NULL)
                			{
	                			struct bst* temp=(struct bst*)malloc(sizeof(struct bst));
                    				temp->data=str_space[i];
                    				temp->right=NULL;
                    				temp->left=NULL;
                    				now->right=temp;	//data go to right
                    				break;
                			}
                			else now=now->right;	//root go to right
				}
			}
		}
		fp=fopen(argv[2],"w");	//write output file
   		if(fp==NULL)perror("file cannot be found");
		#pragma omp parallel num_threads(num)
		{
			int id=omp_get_thread_num();
			int count=0;
			struct bst* find=root;
			while(find!=NULL)
			{
				if(new_test[id]>find->data)find=find->right;	//if test data bigger
				else	//if test data not bigger
				{
					if(new_test[id]==find->data)count++;	//find same number
					find=find->left;
				}
			}
			fprintf(fp,"%d shows %d time(s)\n",new_test[id],count);	//print result
		}
		fclose(fp);	//close file
		delete(root);
	}
	return 0;
}
