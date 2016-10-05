#include "head.h"

void lab5(int argc,char* argv[])
{
	int offsetYes = 0;//these are the truth values that are set while reading the command line arguments
	int scaleYes = 0;
	int statsYes = 0;
	int normalYes = 0;
	int centerYes = 0;
	int renameYes = 0;
	int i = 1;
	int j;
	int fileNo = 0;
	double offsetVal;
	double scaleVal; 
	int* arrayPtr = malloc(sizeof(int*));//for when we open the file
	char* newNameString;
	double mean;
	double maximum;
 	char statFileString[50];//lots of strings declared here...
	char offsetFileString[50];
	char scaleFileString[50];
	char newFileName[50];
	char* string;
	char normalFileString[50];
	char centerFileString[50];
	char renameFileString[50];

	while(i < argc)//this loops steps through each command line argument and checks its syntax
			//please note the truth value integers being set once a certain tag has been found
	{
		if((argv[i][0] == '-') && (argv[i][1] == 'h'))//checks to see if the help menu is needed, terminates program immediately
		{
			helpFunction();
			return;
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'n'))//file number tag
		{
			if(i >= (argc-1))//ensures we dont run off the end of the argv[] array
			{
				printf("WARNING: Please be sure to append a file number\n");
			}
			else
			{
				i++;
				if(argv[i][0] == '-')//checks to see if they forgot to append a number before the next tag
				{
					printf("WARNING: Need a value appended after tag -n\n");
				}
				if(atoi(argv[i]) < 1 || atoi(argv[i]) > 11)//range for valid files
				{
					printf("WARNING: You did not enter a valid file number with your -n tag, program must terminate\n");
					return;//will seg fault if we try to open a data file that dont exist
				}
				else
				{
				fileNo = atoi(argv[i]);
				printf("You want to work with file %d\n",fileNo);
				}
			}
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'o'))
		{
			if(i >= (argc-1))//ensures we dont run off the end of the argv[] array
			{
				printf("WARNING: Did not provide an offset value\n");
			}
			else
			{
				i++;
				if(argv[i][0] == '-')
				{
					printf("WARNING: Need a value appended after tag -o\n");
					i--;
				}
				else
				{
				offsetVal = (double)atof(argv[i]);//atof() because the offset value can be an decimal number
				printf("You want to offset the data by %lf\n",offsetVal);
				offsetYes=1;
				}
			}
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 's'))
		{
			if(i >= (argc-1))//ensures we dont run off the end of the argv[] array
			{
				printf("WARNING: did not provide an scale value\n");
			}
			else
			{
				i++;
				if(argv[i][0] == '-')
				{
					printf("WARNING: Need a value appended after tag -s\n");
					i--;
				}
				else
				{
				scaleVal = (double)atof(argv[i]);
				printf("You want to scale the data by %lf\n",scaleVal);
				scaleYes=1;
				}
			}
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'C'))//these tags do not require a value or input, and requires less error checking for the argument
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
			if(i >= (argc-1))
			{
				printf("WARNING: Did not provide a file name after -r tag\n");
				i--;
			}
			else
			{
				i++;//gets the new name of the file that the user wants to write
				newNameString = argv[i];
				printf("You want to change the name of a file to %s\n",newNameString);
				renameYes=1;
			}
		}
		else	//this error check basically says that the program did not find a tag, simply bumps past the argument and gives the user a warning
		{
		printf("WARNING: Your argument:\n %s\n has no valid context within the scope of the program, this error will be ignored and the program will continue\n",argv[i]);
		}
		i++;
	}
	if(fileNo == 0)//NO SEG FAULTS
	{
		printf("WARNING: You did not provide any file number to work with, we cannot do anything until you provide a file no.\n");
		return;
	}	
	else
	{
	//	printf("values: offset:%d\tscale:%d\tnormalize:%d\tcenter:%d\trename:%d\tstats:%d\nnumber:%d\t\n",offsetYes,scaleYes,normalYes,centerYes,renameYes,statsYes,fileNo);
		arrayPtr = openFile(fileNo);
		double* doublePtr = malloc(sizeof(double*) * (numElements + 2));//needed to make a new array for lab5.c
		i = 1;
		double* startDouble = doublePtr;//start pointers to keep track once i start typecasting each member and manipulating data
		int* startInt = arrayPtr;
		while(i <= (numElements+2))//casting each element into the new array
		{
			*doublePtr = (double)*arrayPtr;
			i++;				
			doublePtr++;
			arrayPtr++;
		}
		doublePtr = startDouble;
		arrayPtr = startInt;
		i=1;
		if(renameYes ==1)//added this chunk during the lab, latest update
		{
			sprintf(renameFileString,"%s.txt",newNameString);
			FILE* fp2 = fopen(renameFileString,"w");
			fprintf(fp2,"%lf\t",*doublePtr);
			doublePtr++;
			fprintf(fp2,"%lf\n",*doublePtr);
			doublePtr++;
			while(i<numElements)
			{
				fprintf(fp2,"%lf\n",*doublePtr);
				doublePtr++;
				i++;
			}
			printf("copy of file was made in %s\n",renameFileString);
			fclose(fp2);
		}
		doublePtr = startDouble;
		arrayPtr = startInt;           
		i=1;	
		if(statsYes==1){//series of if statements that checks each truth value, then proceeds to write files accordingly
			mean = meanFunction(doublePtr);//found in lab4.c 
			printf("found the mean to be %lf\n",mean);
			doublePtr = startDouble;
			maximum = maximumFunction(doublePtr);//found in lab4.c
			doublePtr = startDouble;
			printf("found the max to be %lf\n",maximum);
			if(fileNo < 10)	//this little chunk is for the correct formatting of the file to be written. 
			{	
				if(renameYes==1)//this loop gets used a lot.. basically just gets the correct format for the file names to be outputted
				{
					sprintf(statFileString,"%s_Statistics.txt",newNameString);
				}
				else
				{
					sprintf(statFileString,"Statistics_0%d.txt",fileNo);
				}
			}
			else
			{
				if(renameYes==1)
				{
					sprintf(statFileString,"%s_Statistics.txt",newNameString);
				}
				else
				{
					sprintf(statFileString,"Statistics_%d.txt",fileNo);
				}
			}
			FILE* statFile = fopen(statFileString,"w");//writes the actual statistics file	
			fprintf(statFile,"%lf	%lf",mean,maximum);
			fclose(statFile);
			printf("Statistics for file %d can be found in %s\n",fileNo,statFileString);
		}
		if(centerYes==1)
		{
			if(fileNo < 10)
			{
				if(renameYes==1)
				{
					sprintf(centerFileString,"%s_Centered.txt",newNameString);
				}
				else
				{
					sprintf(centerFileString,"Centered_data_0%d.txt",fileNo);
				}
			}
			else
			{
				if(renameYes==1)
				{
					sprintf(centerFileString,"%s_Centered.txt",newNameString);
				}
				else
				{
					sprintf(centerFileString,"Centered_data_%d.txt",fileNo);
				}
			}
			centerFunction(doublePtr,mean,fileNo,centerFileString);//calling center function from lab4.c note the file is written in this function and i passed the file name as an argument
			printf("Centered signal can be found in %s\n",centerFileString);
		}
		if(normalYes==1)//normal uses the same logic as center
		{
			if(fileNo < 10)
			{
				if(renameYes==1)
				{
					sprintf(normalFileString,"%s_Normalized.txt",newNameString);
				}
				else
				{
					sprintf(normalFileString,"Normalized_data_0%d.txt",fileNo);//again for the correct formatting
				} 
			}	
			else
			{
				if(renameYes==1)
				{
					sprintf(normalFileString,"%s_Normalized.txt",newNameString);
				}
				else
				{
					sprintf(normalFileString,"Normalized_data_%d.txt",fileNo);
				}
			}
			normalizeFunction(doublePtr,fileNo,normalFileString);
			printf("Normalized signal can be found in %s\n",normalFileString);
		}
		if(offsetYes==1)
		{
			i=0;
			doublePtr = startDouble;
			if(fileNo<10)
			{
				if(renameYes==1)
				{
					sprintf(offsetFileString,"%s_Offset.txt",newNameString);
				}
				else
				{
					sprintf(offsetFileString,"Offset_data_0%d.txt",fileNo);
				}
			}
			else
			{
				if(renameYes==1)
				{
					sprintf(offsetFileString,"%s_Offset.txt",newNameString);
				}
				else
				{
					sprintf(offsetFileString,"Offset_data_%d.txt",fileNo);
				}
			}
			FILE* offsetFile = fopen(offsetFileString,"w");//writes the offset file, does not change the values like my old offset function did, so i just write the whole file here
			fprintf(offsetFile,"%lf\t",*doublePtr);
			numElements = (int)*doublePtr;
			doublePtr++;
			fprintf(offsetFile,"%lf\n",*doublePtr);
			doublePtr++;
			while(i<numElements)
			{
				fprintf(offsetFile,"%lf\n",(*doublePtr) + offsetVal);
				doublePtr++;
				i++;
			}
			printf("Offset data can be found in %s\n",offsetFileString);
			fclose(offsetFile);	
		}
		if(scaleYes==1)//scale uses the same logic as offset 
		{
			i=0;
			doublePtr = startDouble;
			if(fileNo<10)
			{
				if(renameYes==1)
				{
					sprintf(scaleFileString,"%s_Scaled.txt",newNameString);
				}
				else
				{
					sprintf(scaleFileString,"Scaled_data_0%d.txt",fileNo);
				}
			}
			else
			{
				if(renameYes==1)
				{
					sprintf(scaleFileString,"%s_Scaled.txt",newNameString);
				}
				else
				{
					sprintf(scaleFileString,"Scaled_data_%d.txt",fileNo);
				}
			}
			FILE* scaleFile = fopen(scaleFileString,"w");
			fprintf(scaleFile,"%lf\t",*doublePtr);
			numElements = (int)*doublePtr;
			doublePtr++;
			fprintf(scaleFile,"%lf\n",*doublePtr);
			doublePtr++;
			while(i<numElements)
			{
				fprintf(scaleFile,"%lf\n",(*doublePtr) * scaleVal);
				doublePtr++;
				i++;
			}
			printf("Scaled data can be found in %s\n",scaleFileString);
			fclose(scaleFile);	
		}
	}
			
	return;
}
void helpFunction(void)//void function to print out a help menu
{
	printf("WELCOME!\nHere in lab5.c, we want to make it easy to access and manipulate the information located within these select files!\nTo execute this program correctly, run ./lab4 along with any arguments appended\nEXAMPLE:./lab4 -n 3 -o 5.3\nThe tags must be proceeded by a valid value in which to perform the operation.\n\n\n-n <file_choice>\tSelects the file in which you want to work with, note valid files range from 1 to 11\n-o <offset_value>\tOffsets each value in the file that you have selected, by the offset value proceeding the tag\n-s <scale_value>\tScales each value in the file that you have selected, by the scaling value proceeding the tag\n-S\t\t\tProvides a new file in which you will find the mean value of the data file you have selected, as well as the maximum value present in the file\n-C\t\t\tCenters the signal in the file you have chosen and writes an output file\n-N\t\t\tNormalizes the signal in the file you have chosen and writes an output file\n-r <new_file_name>\tRenames the current file you have chosen, be sure to proceed the tag with the NEW desired name of the file\n-h\t\t\tDisplays this help menu\n\n\n");
	return;
}
