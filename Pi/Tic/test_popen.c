#include <stdio.h>


int main()
{
	FILE* fp;
	char str[80];
	int result = 1;
	fp = popen("ls", "r");
	while (result != EOF){
		result = fscanf(fp, "%s", str);
		printf("%s\n",str);
	}
	pclose(fp);
}