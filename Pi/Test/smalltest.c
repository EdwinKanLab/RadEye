#include <stdio.h>
#include <stdint.h>

int main(){
	int16_t num = -400;
	
	uint8_t l;
	uint8_t r;
	
	l = (uint8_t)( (num>>8) & 0x00FF);
	r = (uint8_t) (num & 0x00FF);
	
	printf("%d \n", num);
	printf("%d \n", l);
	printf("%d \n", r);
	return 0;


}
