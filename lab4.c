#include "head.h"
//made this a global variable as it is used in almost every function, note it is only actually set once and should cause no overlap issues


int* openFile(int fileChoice);
void writeToFile(double* arrayPtr,char* string,int fileChoice);
double* offsetFile(double* arrayPtr,double offsetValue);
double* scaleFile(double* arrayPtr,double offsetVlaue);
double meanFunction(double* arrayPtr);
double maximumFunction(double* arrayPtr);
void centerFunction(double* arrayPtr,double mean,int fileChoice);
void normalizeFunction(double* arrayPtr,int fileChoice);
void lab5(int argc,char* argv[]);

int main (int argc,char* argv[])
{
    lab5(argc,argv);
    return 1;
    double mean;	//data type double for the information we will be gathering
    double maximum;
    double offsetValue;
    double scaleValue;
    int functionInput;
    int fileChoice;
    int* arrayPtr;//integer pointer to initially get the array started
    char* string;
    int i = 1;
    char statFileString[50];

    printf("Hello there user!We meet again!!\n");
        printf("Please enter a number for the file you want to work with(enter zero to terminate the program)\n>");
        scanf("%d",&fileChoice);
        while(fileChoice < 0 || fileChoice >11)//error checking to ensure we get a valid file
        {
            printf("sorry, that is not a valid option, you may enter 0 to quit or pick a file numbered 1 thru 11\n");
            scanf("%d",&fileChoice);
        }
        if(fileChoice > 0 && fileChoice <12)
        {
            printf("opening the file\n");
            arrayPtr = openFile(fileChoice);//calls the function to open the initial file, this array will be malloced within openFile()
	    int* startInt = arrayPtr;//used this pointer to keep track of the start of the integer array
            if(arrayPtr == NULL)//checks to make sure the files opened correctly
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
	   	double* doublePtr = malloc(sizeof(double*) * (numElements + 2));//mallocing space for the NEW array that is type double
		i = 1;
		double* startDouble = doublePtr;//using another start pointer to keep track of the double array
		while(i <= (numElements+2))
		{
			*doublePtr = (double)*arrayPtr;//here is where i typecast each element of the array pointer into the corresponding locations in the double pointer...
			i++;				//the reason for seperate arrays is because i need the raw data to perform statistics and such
			doublePtr++;
			arrayPtr++;
		}
		doublePtr = startDouble;//setting the pointers back
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
            	switch(functionInput)//using a switch for the main functionality of main
            	{
                case 1:
		    string = "Offset";//strings are set in this manor in order to write to the correct file once the data has been manipulated
                    printf("what do you want to offset the data by?\n>");
                    scanf("%lf",&offsetValue);
                    doublePtr = offsetFile(doublePtr,offsetValue);
		    writeToFile(doublePtr,string,fileChoice);//writing the new data to the file
                    break;
                case 2://all three cases essentially do the same thing
		    string = "Scaled";
                    printf("what do you want to scale the data by?\n>");
                    scanf("%lf",&scaleValue);
                    scaleFile(doublePtr,scaleValue);
		    writeToFile(doublePtr,string,fileChoice);
                    break;
                case 3:
		    string = "Unmodified";//there is an option to leave the data unmodified 
                    writeToFile(doublePtr,string,fileChoice);
                    break;
                default:
                    printf("program shouldnt go here, check your code\n");
                    break;
            }      
		doublePtr = startDouble;//setting these back as they had been bumped in order to write the file
		arrayPtr = startInt;
		i =1;
		while(i <= (numElements+2))//getting back to the raw data (the integer array contains all of the original data)
		{
			*doublePtr = (double)*arrayPtr;
			i++;
			doublePtr++;
			arrayPtr++;
		}
		doublePtr = startDouble;//again setting the pointers back
		arrayPtr = startInt;   
		i = 1;  
		/*while(i <=(numElements + 2))
		{
			printf("test4: %lf\n",*doublePtr);
			i++;
			doublePtr++;
		}
		*/
		doublePtr = startDouble; //this block of code executes everytime, regardless of the user input, and only works with the RAW data initially in the file
		printf("Okay now were going to get some statistics on our signal...\nFinding the mean of file %d...\n",fileChoice);
		mean = meanFunction(doublePtr);
		printf("found the mean to be %lf\n",mean);
		doublePtr = startDouble;
		printf("Finding the max value of file %d...\n",fileChoice);
		maximum = maximumFunction(doublePtr);
		printf("found the max to be %lf\n",maximum);
		doublePtr = startDouble;
		if(fileChoice < 10)	//this little chunk is for the correct formatting of the file to be written. 
		{	
			sprintf(statFileString,"Statistics_0%d.txt",fileChoice);
		}
		else
		{
			sprintf(statFileString,"Statistics_%d.txt",fileChoice);
		}
		FILE* statFile = fopen(statFileString,"w");	//opening and writing the statistics file
		fprintf(statFile,"%lf	%lf",mean,maximum);
		fclose(statFile);
		printf("Statistics for file %d can be found in %s\nCentering the signal %s file %d..........",fileChoice,statFileString,string,fileChoice);
		centerFunction(doublePtr,mean,fileChoice);
		doublePtr = startDouble;
		//centered file will be written in center
		printf("Normalizing the signal %s file %d..........",string,fileChoice);
		normalizeFunction(doublePtr,fileChoice);
		doublePtr = startDouble;
		//normalized file will be written in normalize
		free(arrayPtr);//freeing both of the arrays that had been malloced throughout the program, note that each array that was malloced has been freed, and every file opened has been closed
	    	free(doublePtr);
	}
	
        else
        {
            printf("terminating the program\n");//pretty needless error check
	    
            return 0;
        }
}
void normalizeFunction(double* arrayPtr,int fileChoice)
{	
	int i =1;
	double maximum;
	char normalizedString[50];
	if(fileChoice < 10)
	{
		sprintf(normalizedString,"Normalized_data_0%d.txt",fileChoice);//again for the correct formatting 
	}
	else
	{
		sprintf(normalizedString,"Normalized_data_%d.txt",fileChoice);
	}
	FILE* fp = fopen(normalizedString,"w");//opens the normalize file 
	fprintf(fp,"%lf",*arrayPtr);
	arrayPtr++;
	maximum = *arrayPtr;//using the maximum to normalize the data between 0 and 1, NOTE that the scaled values will between -1 and 1 if any negative values are present in the data files which i did not see
	fprintf(fp,"%lf\n",(*arrayPtr / maximum));
	arrayPtr++;
	while(i <= numElements)
	{
		fprintf(fp,"%lf\n",(*arrayPtr / maximum));//normalizes each element in the array and prints it out to the file 
		arrayPtr++;
		i++;
	}
	fclose(fp);//file closed
	printf("normalized signal can be found in %s\n",normalizedString);
	return;
}

void centerFunction(double* arrayPtr,double mean,int fileChoice)//functionality of center function is the same except it subtracts the mean from each element in the array and writes it to a different file
{	
	int i =1;
	char centeredString[50];
	if(fileChoice < 10)
	{
		sprintf(centeredString,"Centered_data_0%d.txt",fileChoice);
	}
	else
	{
		sprintf(centeredString,"Centered_data_%d.txt",fileChoice);
	}
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
	printf("centered signal can be found in %s\n",centeredString);
	return;
}
double meanFunction(double* arrayPtr)//sums each element in the array and divides by the num of elements to return the mean of the data
{
	double* startPtr = arrayPtr;
	double mean;
	int i=1;
	double total = 0;
	arrayPtr++;//bumping past the heading information
	arrayPtr++;
	while(i <= numElements)
	{
		total = total + *arrayPtr;
		i++;
		arrayPtr++;
	}
	mean = total / (double)numElements;//needed to typecast the num of elements as it is an integer and division would not work right otherwise 
	arrayPtr = startPtr;
	return mean;
}
double maximumFunction(double* arrayPtr)//finds the maximum value in the array and returns a double
{
	double* startPtr = arrayPtr;
	int i = 1;
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
	printf("max is %lf\n",maximumVal);
	arrayPtr = startPtr;
	return maximumVal;
}

double* offsetFile(double* arrayPtr,double offsetValue)//offset file will change the values of each element in the double array, meaning that data will be lost (this is why i used the integer array again in main to convert the double array back to what it was before offsetting and scaling
{
	double i;
	double temp;
	double* startPtr = arrayPtr;//keeps track of the start,this is the pointer that will be returned
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
	arrayPtr = startPtr;
	return arrayPtr;
}
double* scaleFile(double* arrayPtr,double scaleValue)//same functionality as offsetfile
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

    
void writeToFile(double* arrayPtr,char* string,int fileChoice)//this function writes the new files for offset or scaled data, note the string used as an argument to write the correct file
{
	char fileString[50];
	//printf("string is %s\n",string);
	if(fileChoice < 10)
	{
		sprintf(fileString,"%s_data_0%d.txt",string,fileChoice);
	}
	else
	{
		sprintf(fileString,"%s_data_%d.txt",string,fileChoice);
	}
	int i;
	printf("test1: %s \n",fileString);
        FILE* fp = fopen(fileString,"w");
	printf("test2\n");
        fprintf(fp,"%lf ",*arrayPtr);
	numElements = (int)*arrayPtr;
	//printf("num elements is %d\n",numElements);
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
    printf("there are %d elements in file %d\nand the max value is %d\n",numElements,fileChoice,maxValue);//just a check to see if we opened the file right
    int* arrayPtr = malloc((numElements+2)*sizeof(int*));//where the integer array is first malloced, this will be freed in main
    int* startPtr = arrayPtr;
    *arrayPtr = numElements;//getting the data from the file and storing it in numelements (global) as well as in the integer array as information
    //printf("#elements: %f\n",*arrayPtr);
    arrayPtr++;
    *arrayPtr = maxValue;
    arrayPtr++;
    while(i <= numElements)//actually reads each value of the file into the dynamic array
    {
        //printf("%d\n",i);//you can basically ignore all of these test prints, they were used for debugging.
        fscanf(fp,"%d",&temp);
       // printf("test1: %d\n",temp);
        //temp = (double)temp;
        *arrayPtr = temp;
        //printf("test: %lf\n",*arrayPtr);
	//fflush(stdout);
        arrayPtr++;
        i++;
    }
    	fclose(fp);//file closed
	fflush(stdout);
   	return startPtr;
}
