#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

// takes in a relative path.
// returns 1 if the directory exists.
int isDirectoryExists(const char *path)
{
    struct stat stats;
    stat(path, &stats);
    // Check for file existence
    if (S_ISDIR(stats.st_mode)){
      printf("exists at %s.\n", path);
      return 1;
    }
    printf("created at %s.\n", path);
    return 0;
}

// create apt directory structure.
int createDirectory(char* path, int info){
  int result = mkdir(path, 0777);
  if(result==0 && info==1){
    FILE* info = NULL;
    char info_path[50];
    sprintf(info_path, "%s/info.txt", path);
    info = fopen(info_path,"r");
    if(!info){
      info = fopen(info_path,"w");
      fprintf(info,"%d hours completed.\n", 0);
    }
    fclose(info);
  }
  return result;
}

int getIncrement(int hour){
  if(hour<=5){
    return 1;
  } else if(hour<=10){
    return 2;
  } else if(hour <=14){
    return 50;
  } else{
    return 100;
  }
}

// take the local-time and extract the year, month, day.
void createYearMonthDay(char* year, char* month, char* day, char* dt_tm){
  int i;
  for(i = 20; i < 24; i++){
    year[i-20] = dt_tm[i];
  }
  year[4] = '\0';
  for(i = 4; i < 7; i++){
    month[i-4] = dt_tm[i];
  }
  month[3] = '\0';
  for(i = 8; i < 10; i++){
    day[i-8] = dt_tm[i];
  }
  day[2] = '\0';
}

int main(int argc, char* argv[]){
  /* Get the current time from the system.
     The current time is saved in dt_tm string. */
  time_t t = time(NULL);
  struct tm* tm = localtime(&t);
  char* dt_tm = asctime(tm); //date and time.
  char year[5];
  char month[4];
  char day[3];
  createYearMonthDay(year, month, day, dt_tm);
  
  /* Create a new log in the Logs directory at the correct location.*/
  char* root = "../Logs";
  char year_path[13] = {0};
  char month_path[17] = {0};
  char day_path[20] = {0};
  snprintf(year_path, sizeof(year_path), "%s/%s", root,year);
  snprintf(month_path, sizeof(month_path), "%s/%s/%s", root,year,month);
  snprintf(day_path, sizeof(day_path), "%s/%s/%s/%s", root,year,month,day);
  int yr_flag = createDirectory(year_path, 0);
  int mn_flag = createDirectory(month_path, 0);
  int dy_flag = createDirectory(day_path, 1);
  
  // We have the proper directory structure now.
  char file_name_info[50] = {0};
  char file_name[50] = {0};
  sprintf(file_name_info,"%s/%s",day_path,"/info.txt");
  sprintf(file_name,"%s/%s",day_path,"rd.txt");
  FILE *fptr = NULL;
  FILE *infoptr = NULL; 
  FILE *totalPtr = NULL;
  infoptr = fopen(file_name_info,"a+");
  fptr = fopen(file_name,"a+");
  char* score_file = "../Logs/total_score";
  totalPtr = fopen(score_file, "a+");
  
  // Files are open now.
  // Read and write in the files appropriately.
  char info_buff[100];
  char file_buff[100];
  char total_buff[20];

  int hour = 0;
  long long total_score = 0;
  long long score_increment = 0;
  
  // File-processing
  // info update
  
  while(fgets(info_buff, 100, infoptr));
     /* {
      char storage[100];
    printf("%s",storage);
    strcpy(storage, info_buff);
    hour = atoi(storage);
  }
  */
  hour = atoi(info_buff);
  hour++;
  fprintf(infoptr,"%d hours completed.\n", hour);
  fprintf(fptr,"%d hours completed at %s\n", hour, dt_tm);
  

  //score updation
  score_increment = getIncrement(hour);
  fgets(total_buff,20,totalPtr);
  total_score = atoi(total_buff);
  fclose(totalPtr);
  remove(score_file);
  totalPtr = fopen(score_file,"w+");
  
  fprintf(totalPtr,"%lli\n", (total_score+score_increment));
  printf("%lli\n",total_score+score_increment);
  // Close the files and exit.
  fclose(fptr);
  fclose(infoptr);
  fclose(totalPtr);
  return 0;
}

/*
  Ideas to create in this system:
  1. Add a reinforcement/ scoring system in the app.
  2. For all days, there should be number of hours worked.
  3. Bash Scripts for faster control.
  4. Comment addition system.
  5. Text file should have the following format:-
     Key=Value
*/
