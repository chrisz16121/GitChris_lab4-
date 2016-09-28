#include "head.h"

void lab5(int argc,char* argv[])
{
	int offsetYes,scaleYes,statsYes,normalYes,centerYes,renameYes = 0;
	//printf("lab5 is now running\n");
	int i = 1;
	int j;
	int fileNo = 0;
	double offsetVal;
	double scaleVal; 
	int* arrayPtr = malloc(sizeof(int*));
	char* newNameString;
	double mean;
	double maximum;
 	char statFileString[50];

	while(i < argc)
	{
		if((argv[i][0] == '-') && (argv[i][1] == 'h'))
		{
			helpFunction();
			return;
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'n'))
		{
			i++;
			fileNo = atoi(argv[i]);
			if(fileNo < 1 || fileNo > 11)
			{
				printf("You did not enter a valid file number with your -n tag, program must terminate\n");
				return;
			}
			printf("You want to work with file %d\n",fileNo);
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'o'))
		{
			i++;
			offsetVal = (double)atof(argv[i]);
			printf("You want to offset the data by %lf\n",offsetVal);
			offsetYes=1;
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 's'))
		{
			i++;
			scaleVal = (double)atof(argv[i]);
			printf("You want to scale the data by %lf\n",scaleVal);
			scaleYes=1;
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'C'))
		{
			printf("You want to center the data\n");
			centerYes=1;
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'N'))\
		{
			printf("You want to normalize the data\n");
			normalYes=1;
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'S'))
		{
			printf("You want to gather the statistics of our data\n");
			statsYes=1;
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'r'))
		{
			i++;
			newNameString = argv[i];
			printf("You want to change the name of a file to %s\n",newNameString);
			renameYes=1;
		}
		else	
		{
		printf("WARNING: Your argument:\n %s\n has no valid context within the scope of the program, this error will be ignored and the program will continue\n",argv[i]);
		}
		i++;
	}
	if(fileNo == 0)
	{
		printf("You did not provide any file number to work with, we cannot do anything until you provide a file no.\n");
		return;
	}	
	else
	{
		arrayPtr = openFile(fileNo);
		double* doublePtr = malloc(sizeof(double*) * (numElements + 2));
		i = 1;
		double* startDouble = doublePtr;
		int* startInt = arrayPtr;
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
		if(statsYes==1){
			mean = meanFunction(doublePtr);
			printf("found the mean to be %lf\n",mean);
			doublePtr = startDouble;
			printf("Finding the max value of file %d...\n",fileNo);
			maximum = maximumFunction(doublePtr);
			doublePtr = startDouble;
			printf("found the max to be %lf\n",maximum);
			if(fileNo < 10)	//this little chunk is for the correct formatting of the file to be written. 
			{	
				sprintf(statFileString,"Statistics_0%d.txt",fileNo);
			}
			else
			{
				sprintf(statFileString,"Statistics_%d.txt",fileNo);
			}
			printf("test1\n");
			FILE* statFile = fopen(statFileString,"w");
			printf("test2\n");	
			fprintf(statFile,"%lf	%lf",mean,maximum);
			fclose(statFile);
			printf("Statistics for file %d can be found in %s\n",fileNo,statFileString);
		}
		
	}
			
	return;
}
void helpFunction(void)
{
	printf("WELCOME!\nHere in lab5.c, we want to make it easy to access and manipulate the information located within these select files!\nTo execute this program correctly, run ./lab4 along with any arguments appended\nEXAMPLE:./lab4 -n 3 -o 5.3\nThe tags must be proceeded by a valid value in which to perform the operation.\n-n\tSelects the file in which you want to work with, note valid files range from 1 to 11\n-o\tOffsets each value in the file that you have selected, by the offset value proceeding the tag\n-s\tScales each value in the file that you have selected, by the scaling value proceeding the tag\n-S\tProvides a new file in which you will find the mean value of the data file you have selected, as well as the maximum value present in the file\n-C\tCenters the signal in the file you have chosen and writes an output file\n-N\tNormalizes the signal in the file you have chosen and writes an output file\n-r\tRenames the current file you have chosen, be sure to proceed the tag with the NEW desired name of the file\n-h\tDisplays this help menu\n\n\n");
	return;
}
