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
void centerFunction(double* arrayPtr,double mean,int fileChoice,char* centerFileString);
void normalizeFunction(double* arrayPtr,int fileChoice,char* normalFileString);
void lab5(int argc,char* argv[]);
void helpFunction(void);
