#include <unistd.h>
#include "temp_functions.h"
#define PATH_LEN 255
#define COUNT_LINE 12
  static struct data Data[COUNT_LINE]={0,0,0,0,0};

int main(int argc,char	**argv[])
{
    char file_name[PATH_LEN];
    _Bool file_Exist=false;
    short month=-1;

    opterr=0;
	int rez = 0;
	int len_path;

	while ( (rez = getopt(argc, argv, "hf:m:")) != -1)
    {
		switch (rez)
		{
            case 'h': printf("The application displays statistics on the file.\n");
                      printf("options:\n");
                      printf("-f <filename.csv> input file for processing\n");
                      printf("-m <month number> if the key is set, then statistics for the specified month are displayed\n");
                      return 1;
            case 'f': len_path = strlen(optarg);
                      if (len_path>255)
                      {
                        printf("File path length exceeds 255 characters");
                        return 1;
                      }
                      if (optarg[len_path-4]!= '.' || optarg[len_path-3]!= 'c' || optarg[len_path-2]!='s' || optarg[len_path-1]!='v')
                      {
                        printf("The file type is not correct. Must be <*.csv>\nTry -h for help\n");
                        return 1;
                      }
                      strcpy(file_name,optarg);
                      file_Exist=true;
                      break;
            case 'm': month=atoi(optarg);
                      if (!(month>=1 && month<=12))
                      {
                          printf("Value \"%s\" not correct, month from 1 to 12\n ",optarg);
                          return 1;
                      }
                     break;
            case '?': printf("Unknown argument: %s Try -h for help\n",argv[optind-1]);
                    return 1;
		}
	}
    if(!file_Exist)
    {
        printf("Not input file for processing.\nTry -h for help\n");
        return 1;
    }

   int data_line=parserFiles(file_name,Data);
   if (data_line==0)
   {
       printf("No data to display!");
       exit(1);
   }
    printStatistic(Data,month-1);

	return 0;
}





