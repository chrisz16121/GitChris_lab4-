#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double numElements;


double* openFile(int fileChoice);
int writeToFile(double* arrayPtr,char* string,int fileChoice);
double* offsetFile(double* arrayPtr,double offsetValue);
double* scaleFile(double* arrayPtr,double offsetVlaue);

int main (void)
{
    double offsetValue;
    double scaleValue;
    int functionInput;
    int j =1;
    int fileChoice;
    double* arrayPtr;
    char* string;

    printf("Hello there user!We meet again!!\n");
    while(1)
    {
        printf("Please enter a number for the file you want to work with(enter zero to terminate the program)\n>");
        scanf("%d",&fileChoice);
        while(fileChoice < 0 || fileChoice >11)
        {
            printf("sorry, that is not a valid option, you may enter 0 to quit or pick a file numbered 1 thru 11\n");
            scanf("%d",&fileChoice);
        }
        if(fileChoice > 0 && fileChoice <12)
        {
            printf("opening the file\n");
            arrayPtr = openFile(fileChoice);
            if(arrayPtr == NULL)
            {
                printf("your files are corrupted\nterminating\n");
                return 0;
            }
            printf("okay user, we got it opened,do you want to offset, scale, or leave the signal alone??\n1: Offset\n2: Scale\n3: Do Nothing\n>");
            scanf("%d",&functionInput);
            while(functionInput < 1 || functionInput > 3)
            {
                printf("not valid, user, try that again, 1 thru 3\n");
                scanf("%d",&functionInput);
            }
            switch(functionInput)
            {
                case 1:
		    string = "Offset";
                    printf("what do you want to offset the data by?\n>");
                    scanf("%lf",&offsetValue);
                    arrayPtr = offsetFile(arrayPtr,offsetValue);
		    writeToFile(arrayPtr,string,fileChoice);
                    break;
                case 2:
		    string = "Scaled";
                    printf("what do you want to scale the data by?\n>");
                    scanf("%lf",&scaleValue);
                    scaleFile(arrayPtr,scaleValue);
		    writeToFile(arrayPtr,string,fileChoice);
                    break;
                case 3:
		    string = "Unmodified";
                    writeToFile(arrayPtr,string,fileChoice);
                    break;
                default:
                    printf("program shouldnt go here, check your code\n");
                    break;
            }        
        }
        else
        {
            printf("terminating the program\n");
	    free(arrayPtr);
            return 0;
        }
    }
}

double* offsetFile(double* arrayPtr,double offsetValue)
{
	double i;
	double temp;
	double* startPtr = arrayPtr;
	printf("offsetting the data\n");
	numElements = *arrayPtr;
	//printf("%lf\n",numElements);
	arrayPtr += 2;
	//printf("%lf",*arrayPtr);
	while(i < numElements)
	{
		temp = *arrayPtr;
		*arrayPtr = temp + offsetValue;
		printf("%lf\n",*arrayPtr);
		arrayPtr++;
		i++;
	}
	printf("finished offsetting\n");
	return startPtr;
}
double* scaleFile(double* arrayPtr,double scaleValue)
{
	double i;
	double temp;
	double* startPtr = arrayPtr;
	printf("scaling the data\n");
	numElements = *arrayPtr;
	arrayPtr += 2;
	while(i < numElements)
	{
		temp = *arrayPtr;
		*arrayPtr = temp * scaleValue;
		printf("%lf\n",*arrayPtr);
		arrayPtr++;
		i++;
	}
	printf("finished scaling\n");
	return startPtr;
}

    
int writeToFile(double* arrayPtr,char* string,int fileChoice)
{
	char fileString[50];
	printf("string is %s\n",string);
	if(fileChoice < 10)
	{
		sprintf(fileString,"%s_data_0%d.txt",string,fileChoice);
	}
	else
	{
		sprintf(fileString,"%s_data_%d.txt",string,fileChoice);
	}
	int i;
        FILE* fp = fopen(fileString,"w");
        fprintf(fp,"%lf ",*arrayPtr);
	numElements = *arrayPtr;
        arrayPtr++;
        fprintf(fp,"%lf\n",*arrayPtr);
	while(i <= numElements)
	{
		fprintf(fp,"%lf\n",*arrayPtr);	
		arrayPtr++;
	}
        fclose(fp);
        return 0;
}
double* openFile(int fileChoice)
{
    double maxValue;
    double temp;
    char fileString[50];
    int i =1;
    if(fileChoice < 10)
    {
    sprintf(fileString,"Raw_data_0%d.txt",fileChoice);
    }
    else
    {
    	sprintf(fileString,"Raw_data_%d.txt",fileChoice);
    }
    FILE* fp = fopen(fileString,"r");
    if(fp == NULL)
    {
        printf("there is no file to open here, something went wrong\n\nneed to terminate\n");
        return NULL;
    }
	printf("file opened\n");

    fscanf(fp,"%lf %lf",&numElements,&maxValue);
    printf("there are %lf elements in file %d\nand the max value is %lf\n",numElements,fileChoice,maxValue);
    double* arrayPtr = malloc((numElements+2)*sizeof(double*));
    double* startPtr = arrayPtr;
    *arrayPtr = numElements;
    //printf("#elements: %f\n",*arrayPtr);
    arrayPtr++;
    *arrayPtr = maxValue;
    arrayPtr++;
    while(i <= numElements)
    {
        //printf("%d\n",i);
        fscanf(fp,"%lf",&temp);
        //printf("test1: %lf\n",temp);
        //temp = (double)temp;
     //   *arrayPtr = temp;
        //printf("test: %lf\n",*arrayPtr);
	//fflush(stdout);
        arrayPtr++;
        i++;
    }
	
	printf("test2\n");

	
    	fclose(fp);
	printf("test\n\n\n");
	fflush(stdout);
   	return startPtr;
}
