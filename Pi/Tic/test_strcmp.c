#include <stdio.h>

#include <string.h>
int main()
{
	FILE* fp;
	char str[80];
	int result = 1;
	fp = popen("ls", "r");
	while (result != EOF){
		result = fscanf(fp, "%s", str);
		printf("%s\n",str);
		if (strcmp(str, "test.c")==0)
		{
			printf("test.c found!\n");
			return 0;
		}
		
	}
	pclose(fp);
}