#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main()
{
	FILE* fp;
	char str1[21];
	char str2[21];
	int32_t pos;
	int result = 1;
	fp = fopen("status.txt", "r");
	while (result != EOF){
		result = fscanf(fp, "%s", str1);
		printf("%s\n",str1);


		if (strcmp(str1, "Current")==0)
		{
			fscanf(fp, "%s", str2);
			if (strcmp(str2, "position:") == 0)
			{
				fscanf(fp, "%d", &pos);
				printf("current position is %d\n", pos);
				pclose(fp);
				result = EOF;
			}
		}
		
	}
	return 0;
	
}