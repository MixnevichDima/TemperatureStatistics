#include "temp_functions.h"
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>



int parserFiles(char file[],struct data Dats[])
{
    FILE *date_File;
    FILE *Log;
    char *file_log_Name="log_error.txt";

    int countLine=0;
    int countError=0;
    bool NotProblem=true;
    int countInsert=0;


    int year;
	int month;
	int day;
	int hour;
	int minutes;
	int temperature;

	struct tm *ptr;
	time_t lt;
	char buffer[26];
	int ch;


	lt = time(NULL);
	ptr = localtime(&lt);

    if ((Log=fopen(file_log_Name,"a+"))==NULL)
    {
        printf("Can not open the log file %s",file_log_Name);
        exit(1);
    }
    if ((date_File=fopen(file,"r"))==NULL)
    {
        printf("Can not open the file %s",file);
        exit(1);
    }
    fprintf(Log,"\n Reading a file %s is %s",file,asctime(ptr));


    while(fscanf(date_File,"%20[^\n]s",buffer)!=-1)
    {
       //printf("Str = %s\n",buffer);
        countLine++;
        do
       {ch=fgetc(date_File);}
       while (ch!='\n' && ch!=EOF);

        if(sscanf(buffer,"%d;%d;%d;%d;%d;%d",&year,&month,&day,&hour,&minutes,&temperature)!=6)
        {
           //printf("year %d,month %d,day %d\n",year,month,day);
            fprintf(Log,"Error line %d: (%s)\n",countLine,buffer);
            countError++;
            NotProblem=false;
            continue;
        }
        if(!check_Date(year,month,day))
        {
            fprintf(Log,"Error line %d: Not correct date - %d.%d.%d \n",countLine,day,month,year);
            countError++;
            NotProblem=false;
        }
        if(!check_Time(hour,minutes))
        {
            fprintf(Log,"Error line %d: Not correct time - %d:%d\n",countLine,hour,minutes);
            countError++;
            NotProblem=false;
        }
        if(!check_Temperat(temperature))
        {
            fprintf(Log,"Error line %d: Not correct temperature - %d\n",countLine,temperature);
            countError++;
            NotProblem=false;
        }
        if (NotProblem)
        {
            month--;
            if (Dats[month].countLine==0)
            {
                Dats[month].min=temperature;
                Dats[month].max=temperature;
            }
            else
            {
                if (Dats[month].min > temperature) Dats[month].min = temperature;
                if (Dats[month].max < temperature) Dats[month].max = temperature;
            }
            Dats[month].countLine+=1;
            Dats[month].sumTemp+=temperature;
            countInsert++;
        }
        else NotProblem=true;
    }
    if (countError>0) printf("*****File error details - %s******\n\n",file_log_Name);
    fclose(date_File);
    fclose(Log);
    return countInsert;
}

bool check_Date(int year, int month, int day)
{
    bool res=false;
    int dayInMonth[12]={31,29,31,30,31,30,31,31,30,31,30,31};
    if  (month>=1 && month<=12)
        if (day>=1 && day<=dayInMonth[month-1])
            if (year>999 && year<=9999)
                res=true;
    return res;
}
bool check_Time(int hour,int minutes)
{
    return ((hour>=0 && hour<=23)&&(minutes>=0 && minutes<=59));
}
bool check_Temperat(int temperatur)
{
    return (temperatur>=-99 && temperatur<=99);
}

void printStatistic(struct data Dats[],int manth)
{
    int i;
    int count=0;
    int countLine=0;
    int sumTemp=0;
    int min;
    int max;
    int isNotManth[12]={0};
    int countIsNotManth=0;
    char * MANTH[12]=
                    {
                    "january","february","march",
                    "april","may","june",
                    "july","august","september",
                    "october","november","december"
                    };
    if(manth>=0)
    {
        if (Dats[manth].countLine>0)
            printf("In %s  ave: %.2f  min: %d   max:  %d\n",MANTH[manth],(float)Dats[manth].sumTemp/Dats[manth].countLine,Dats[manth].min,Dats[manth].max);
    }
    else
    {
        while(count<12)
             printStatistic(Dats,count++);

        min=Dats[0].min;
        max=Dats[0].max;
        for(i=0;i<12;i++)
        {
          if (Dats[i].countLine>0)
          {
              countLine+=Dats[i].countLine;
              sumTemp+=Dats[i].sumTemp;
              if (Dats[i].min < min) min=Dats[i].min;
              if (Dats[i].max > max) max=Dats[i].max;
          }
          else
          {
              isNotManth[countIsNotManth++]=i;
          }
        }
        printf("Statistics for the year ave: %.2f  min: %d  max:  %d .",(float)sumTemp/countLine,min,max);
        if (countIsNotManth>0)
        {
            printf("The following months were not included in the calculation:");
            for(i=0;i<countIsNotManth;i++)
            {
                printf("%s",MANTH[i]);
                (i+1<countIsNotManth)? printf(", "):printf(".");
            }
        }
        printf("\n");
    }
}
