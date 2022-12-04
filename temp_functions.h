#ifndef TEMP_FUNCTIONS_H_INCLUDED
#define TEMP_FUNCTIONS_H_INCLUDED

#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

struct data
{
    int countLine;
	int sumTemp;
	int min;
	int max;
} data;

int parserFiles(char file[],struct data Dats[]);
bool check_Date(int year, int month, int day);
bool check_Time(int hour,int minutes);
bool check_Temperat(int temperatur);
void printStatistic(struct data Dats[],int manth);


#endif // TEMP_FUNCTIONS_H_INCLUDED
