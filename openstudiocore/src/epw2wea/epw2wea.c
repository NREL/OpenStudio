/*  Copyright (c) 2003
 *  National Research Council Canada
 *  written by Christoph Reinhart
 */

// epw2wea: daylight analysis subprogram of DAYSIM
// Program converts EnergyPlus weather format (*.ppw) into DAYSIM format (*.wea)

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>

int main( int argc, char  *argv[])
{
	FILE *EPW_FILE;
	FILE* WEA_FILE;
	int year, month,day, hour_in,minute,i;
	float dir_norm_rad, dif_or_rad,dummy_float;
    char keyword[2000]="";
	char epw_file[200]="";
	char wea_file[200] ="";
	char city[200] ="";
	char country[200] ="";
	char latitude[200] ="",longitude[200] ="",time_zone[200] ="",elevation[200] ="";

	if (argc < 3)
	{
		fprintf(stderr,"epw2wea: FATAL ERROR - wrong number of arguments\n");
		fprintf(stderr,"start program with: epw2wea <file-name.epw> <file-name.wea>\n");
		//fprintf(stderr,"\t-h: write out header file\n");
		exit(1);
	}
	if (argc >= 3)
	{
		strcpy(epw_file, argv[1]);
		strcpy(wea_file, argv[2]);
		for (i = 3; i < argc; i++)
		if (argv[i][0] == '-' )
			switch (argv[i][1])
			{
				case 'h'://scaling factor
					break;
			}//end switch (argv[i][1])
		else
		{
			fprintf(stdout,"epw2wea: fatal error - %s bad option for input arguments\n", argv[i]);
			exit(0);
		}//end else
	}

	EPW_FILE=fopen(epw_file, "r");;
	WEA_FILE=fopen(wea_file, "w");;
	fscanf(EPW_FILE,"%[^,]s",keyword);
	if( !strcmp(keyword,"LOCATION") ){
	fscanf(EPW_FILE,",%[^,]s",city);
	fscanf(EPW_FILE,",%[^,]s",country);
	fscanf(EPW_FILE,",%[^,]s",country);
	sprintf(keyword,"place %s_%s\n",city,country);
	printf("%s",keyword);
	fprintf(WEA_FILE,"%s",keyword);

	fscanf(EPW_FILE,",%[^,]s",country);
	fscanf(EPW_FILE,",%[^,]s",country);//printf("%s\n",country);
	fscanf(EPW_FILE,",%[^,]s",latitude);
	printf("latitude %s\n",latitude);
	fprintf(WEA_FILE,"latitude %s\n",latitude);
	fscanf(EPW_FILE,",%[^,]s",longitude);

	printf("longitude %.2f\n",-1.0*atof(longitude));
	fprintf(WEA_FILE,"longitude %.2f\n",-1.0*atof(longitude));
	fscanf(EPW_FILE,",%[^,]s",time_zone);
	printf("time_zone %.0f\n",-15.0*atoi(time_zone));
	fprintf(WEA_FILE,"time_zone %.0f\n",-15.0*atoi(time_zone));
	fscanf(EPW_FILE,",%s[^\n]",elevation);
	printf("site_elevation %s\nweather_data_file_units 1\n",elevation);
	fprintf(WEA_FILE,"site_elevation %s\nweather_data_file_units 1\n",elevation);

	fscanf(EPW_FILE,"%*[^\n]");fscanf(EPW_FILE,"%*[\n\r]");
	fscanf(EPW_FILE,"%*[^\n]");fscanf(EPW_FILE,"%*[\n\r]");
	fscanf(EPW_FILE,"%*[^\n]");fscanf(EPW_FILE,"%*[\n\r]");
	fscanf(EPW_FILE,"%*[^\n]");fscanf(EPW_FILE,"%*[\n\r]");
	fscanf(EPW_FILE,"%*[^\n]");fscanf(EPW_FILE,"%*[\n\r]");
	fscanf(EPW_FILE,"%*[^\n]");fscanf(EPW_FILE,"%*[\n\r]");
	fscanf(EPW_FILE,"%*[^\n]");fscanf(EPW_FILE,"%*[\n\r]");
	fscanf(EPW_FILE,"%*[^\n]");fscanf(EPW_FILE,"%*[\n\r]");


	while( EOF != fscanf(EPW_FILE,"%d,%d,%d,%d,%d",&year,&month,&day, &hour_in,&minute)){
		fprintf(WEA_FILE,"%d %d %.3f ",month,day,hour_in*1.0-minute*(0.5/60));

		fscanf(EPW_FILE,",%[^,]s",city);
		fscanf(EPW_FILE,",%f",&dummy_float);
		fscanf(EPW_FILE,",%f",&dummy_float);
		fscanf(EPW_FILE,",%f",&dummy_float);
		fscanf(EPW_FILE,",%f",&dummy_float);
		fscanf(EPW_FILE,",%f",&dummy_float);
		fscanf(EPW_FILE,",%f",&dummy_float);
		fscanf(EPW_FILE,",%f",&dummy_float);
		fscanf(EPW_FILE,",%f",&dummy_float);

		fscanf(EPW_FILE,",%f,%f",&dir_norm_rad, &dif_or_rad);
		fprintf(WEA_FILE,"%.0f %.0f",dir_norm_rad, dif_or_rad);

		fscanf(EPW_FILE,"%*[^\n]");fscanf(EPW_FILE,"%*[\n\r]");
		fprintf(WEA_FILE,"\n");
	}


}else{
		fprintf(stderr,"epw2wea: FATAL ERROR - this does not seem to be an epw file \n");exit(1);
}



	fclose(EPW_FILE);
	fclose(WEA_FILE);


return 0;
}


