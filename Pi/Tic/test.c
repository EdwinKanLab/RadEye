#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



// // Runs the given shell command.  Returns 0 on success, -1 on failure.
// int run_command(const char * command)
// {
//   int result = system(command);
//   if (result)
//   {
//     fprintf(stderr, "Command failed with code %d: %s\n", result, command);
//     return -1;
//   }
//   return 0;
// }




// static int setStepMode(const char* sno, uint8_t val)
// {
// 	// self->step_mode = val;
// 	char command[1024];
// 	char mode[7];

// 	switch (val)
// 	{
// 		case 0: //Full-Step
//   			// snprintf(command, sizeof(command), "ticcmd --step-mode full");
// 			snprintf(mode, sizeof(mode), "full");
// 			break;

// 		case 1:
// 			snprintf(mode, sizeof(mode), "half");
// 			break;

// 		case 2:
// 			snprintf(mode, sizeof(mode), "4");
// 			break;

// 		case 3:
// 			snprintf(mode, sizeof(mode), "8");
// 			break;

// 		case 4:
// 			snprintf(mode, sizeof(mode), "16");
// 			break;

// 		case 5:
// 			snprintf(mode, sizeof(mode), "32");
// 			break;

// 		case 6:
// 			snprintf(mode, sizeof(mode), "2_100p");
// 			break;

// 		default: 
// 			printf("Invalid step mode");
// 			return -1;
// 			break;
// 	}
// 	snprintf(command, sizeof(command), "ticcmd -d %s --step-mode %s",sno, mode);
// 	return run_command(command);
// }

// int tic_energize(const char* sno)
// {
// 	char command[1024];
// 	snprintf(command, sizeof(command), "ticcmd -d %s --energize", sno);
// 	return run_command(command);
// }

// int tic_set_target_position(const char* sno, int32_t target)
// {
//   char command[1024];
//   snprintf(command, sizeof(command), "ticcmd --exit-safe-start -d %s --position %d", sno, target);
//   return run_command(command);
// }

// int halt_and_set_position(const char* sno, int32_t pos)
// {
//   char command[1024];
//   snprintf(command, sizeof(command), "ticcmd -d %s --halt-and-set-position %d  ", sno, pos);
//   return run_command(command);
// }

int main()
{
	const char vertical_sno [9] = "00295780";
	const char horizontal_sno [9] = "00286960";
	printf("Setting step mode to 1.\n");
	int result1 = setStepMode(vertical_sno, 0);
	int result2 = setStepMode(horizontal_sno, 0);
	halt_and_set_position(vertical_sno, 0);
	halt_and_set_position(horizontal_sno, 0);
	tic_energize(vertical_sno);
	tic_energize(horizontal_sno);


	tic_set_target_position(vertical_sno, 81);
	tic_set_target_position(horizontal_sno, 10);

	if (result1 && re	char command[1024];
	snprintf(command, sizeof(command), "ticcmd -d %s --starting-speed %d  ", self->serial_no, val);
	return run_command(command);sult2) { return 1; }
	return 0;
}