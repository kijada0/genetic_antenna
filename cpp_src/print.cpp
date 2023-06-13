#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>
#include "print.h"

static int print_level = 6;

void print(int level, char const *format, ...)
{
	va_list args;
	char buf[256];
	char time_buf [30];
	char usec_buf [10];
	struct stat st{};
	struct timeval tmnow{};
	struct tm *tm;

	FILE *log;

	int ret;

	if(level<=print_level){
		va_start(args, format);
		vsnprintf(buf, sizeof(buf), format, args);
		va_end(args);

		ret = stat(LOG_FILE_PATH, &st);

		gettimeofday(&tmnow, NULL);
		tm = localtime(&tmnow.tv_sec);
		strftime(time_buf,30,"%Y.%m.%d %H:%M:%S.", tm);
		snprintf(usec_buf, 10,"%03d",(int)tmnow.tv_usec/1000);	// ms
		strcat(time_buf, usec_buf);


		if(st.st_size > LOG_SIZE || ret)
		{
			if(ret==0)
			{
				char old_name[256];
				sprintf(old_name, LOG_FILE_PATH "_old" );
				remove(old_name);
				rename(LOG_FILE_PATH, old_name);
			}
			log = fopen(LOG_FILE_PATH, "w");
		}
		else{
			log = fopen(LOG_FILE_PATH, "a+");
		}

		if(log)
			{
				fprintf(log, "[%s]: %s\n" , time_buf, buf);
				fclose(log);
			}
	}
}
