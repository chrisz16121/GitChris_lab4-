#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int numElements;


int* openFile(int fileChoice);
void writeToFile(double* arrayPtr,char* string,int fileChoice);
double* offsetFile(double* arrayPtr,double offsetValue);
double* scaleFile(double* arrayPtr,double offsetVlaue);
double meanFunction(double* arrayPtr);
double maximumFunction(double* arrayPtr);
void centerFunction(double* arrayPtr,double mean,int fileChoice);
void normalizeFunction(double* arrayPtr);


int main (void)
{
    double mean;
    double maximum;
    double offsetValue;
    double scaleValue;
    int functionInput;
    int j =1;
    int fileChoice;
    int* arrayPtr;
    char* string;
    int i = 1;
    char statFileString[50];

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
	    int* startInt = arrayPtr;
            if(arrayPtr == NULL)
            {
                printf("your files are corrupted\nterminating\n");
                return 0;
            }
		/*while(i <=(numElements + 2))
		{
			printf("test2: %d\n",*arrayPtr);
			i++;
			arrayPtr++;
		}
		arrayPtr = startInt;
		*/
	   	double* doublePtr = malloc(sizeof(double*) * (numElements + 2));
		i = 1;
		double* startDouble = doublePtr;
		while(i <= (numElements+2))
		{
			*doublePtr = (double)*arrayPtr;
			i++;
			doublePtr++;
			arrayPtr++;
		}
		doublePtr = startDouble;
		arrayPtr = startInt;           
		i=1;
		/*while(i <=(numElements + 2))
		{
			printf("test3: %lf\n",*doublePtr);
			i++;
			doublePtr++;
		}	
		doublePtr = startDouble;
		*/		
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
                    doublePtr = offsetFile(doublePtr,offsetValue);
		    writeToFile(doublePtr,string,fileChoice);
                    break;
                case 2:
		    string = "Scaled";
                    printf("what do you want to scale the data by?\n>");
                    scanf("%lf",&scaleValue);
                    scaleFile(doublePtr,scaleValue);
		    writeToFile(doublePtr,string,fileChoice);
                    break;
                case 3:
		    string = "Unmodified";
                    writeToFile(doublePtr,string,fileChoice);
                    break;
                default:
                    printf("program shouldnt go here, check your code\n");
                    break;
            }      
		doublePtr = startDouble;
		arrayPtr = startInt;
		i =1;
		while(i <= (numElements+2))
		{
			*doublePtr = (double)*arrayPtr;
			i++;
			doublePtr++;
			arrayPtr++;
		}
		doublePtr = startDouble;
		arrayPtr = startInt;   
		i = 1;  
		while(i <=(numElements + 2))
		{
			printf("test4: %lf\n",*doublePtr);
			i++;
			doublePtr++;
		}	
		doublePtr = startDouble; 
		printf("Okay now were going to get some statistics on our signal...\nFinding the mean of %s file %d...\n",string,fileChoice);
		mean = meanFunction(doublePtr);
		printf("found the mean to be %lf\n",mean);
		doublePtr = startDouble;
		printf("Finding the max value of %s file %d...\n",string,fileChoice);
		maximum = maximumFunction(doublePtr);
		printf("found the max to be %lf\n",maximum);
		doublePtr = startDouble;
		sprintf(statFileString,"Statistics_%d.txt",fileChoice);
		FILE* statFile = fopen(statFileString,"w");	
		fprintf(statFile,"%lf	%lf",mean,maximum);
		fclose(statFile);
		printf("Centering the signal %s file %d\n",string,fileChoice);
		centerFunction(doublePtr,mean,fileChoice);
		doublePtr = startDouble;
		//centered file will be written in center
		printf("Normalizing the signal %s file %d\n",string,fileChoice);
		normalizeFunction(doublePtr);
		//normalized file will be written in normalize
		free(arrayPtr);
	    	free(doublePtr);
	}
	
        else
        {
            printf("terminating the program\n");
	    
            return 0;
        }
    }
}
void normalizeFunction(double* arrayPtr)
{	
	printf("normailzing the data\n");
	return;
}

void centerFunction(double* arrayPtr,double mean,int fileChoice)
{	
	int i;
	char centeredString[50];
	sprintf(centeredString,"Centered_data_%d.txt",fileChoice);
	FILE* fp = fopen(centeredString,"w");
	fprintf(fp,"%lf ",*arrayPtr);
	arrayPtr++;
	fprintf(fp,"%lf\n",*arrayPtr - mean);
	arrayPtr++;
	while(i <= numElements)
	{
		fprintf(fp,"%lf\n",*arrayPtr - mean);
		arrayPtr++;
		i++;
	}
	fclose(fp);
	return;
}
double meanFunction(double* arrayPtr)
{
	double mean;
	int i=1;
	double total = 0;
	arrayPtr++;
	arrayPtr++;
	while(i <= numElements)
	{
		total = total + *arrayPtr;
		i++;
		arrayPtr++;
	}
	mean = total / (double)numElements;
	return mean;
}
double maximumFunction(double* arrayPtr)
{
	int i;
	double maximumVal=0;
	arrayPtr++;
	arrayPtr++;
	while(i <= numElements)
	{
		if(maximumVal < *arrayPtr)
		{
			maximumVal = *arrayPtr;
		}
		arrayPtr++;
		i++;
	}
	return maximumVal;
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
		//printf("%lf\n",*arrayPtr);
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
		//printf("%lf\n",*arrayPtr);
		arrayPtr++;
		i++;
	}
	printf("finished scaling\n");
	return startPtr;
}

    
void writeToFile(double* arrayPtr,char* string,int fileChoice)
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
	numElements = (int)*arrayPtr;
	printf("num elements is %d\n",numElements);
        arrayPtr++;
        fprintf(fp,"%lf\n",*arrayPtr);
	arrayPtr++;
	while(i < numElements)
	{
		fprintf(fp,"%lf\n",*arrayPtr);	
		arrayPtr++;
		i++;
	}
        fclose(fp);
	printf("\n\n\n\n\n\n\n\n\n");
        return;
}
int* openFile(int fileChoice)
{
    int maxValue;
    int temp;
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

    fscanf(fp,"%d %d",&numElements,&maxValue);
    printf("there are %d elements in file %d\nand the max value is %d\n",numElements,fileChoice,maxValue);
    int* arrayPtr = malloc((numElements+2)*sizeof(int*));
    int* startPtr = arrayPtr;
    *arrayPtr = numElements;
    //printf("#elements: %f\n",*arrayPtr);
    arrayPtr++;
    *arrayPtr = maxValue;
    arrayPtr++;
    while(i <= numElements)
    {
        //printf("%d\n",i);
        fscanf(fp,"%d",&temp);
        printf("test1: %d\n",temp);
        //temp = (double)temp;
        *arrayPtr = temp;
        //printf("test: %lf\n",*arrayPtr);
	//fflush(stdout);
        arrayPtr++;
        i++;
    }
    	fclose(fp);
	fflush(stdout);
   	return startPtr;
}
