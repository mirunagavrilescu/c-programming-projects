#include "functii.h"

int main(void)
{
	game gs;
	init_game(&gs);
	char command[100000];
	while (gs.running && fgets(command, sizeof(command), stdin)) {
		str_newline(command);
		if (strncmp(command, "EXIT", 4) == 0) {
			gs.running = 0;
		} else if (strncmp(command, "LOAD ", 5) == 0) {
			do_load(&gs, command);
		} else if (strncmp(command, "SAVE ", 5) == 0) {
			do_save(&gs, command);
		} else if (strncmp(command, "TURTLE ", 7) == 0) {
			do_turtle(&gs, command);
		} else if (strncmp(command, "LSYSTEM ", 8) == 0) {
			do_lsystem(&gs, command);
		} else if (strncmp(command, "FONT ", 5) == 0) {
			do_font(&gs, command);
		} else if (strncmp(command, "TYPE ", 5) == 0) {
			do_type(&gs, command);
		} else if (strcmp(command, "UNDO") == 0) {
			do_undo(&gs);
		} else if (strcmp(command, "REDO") == 0) {
			do_redo(&gs);
		} else if (strncmp(command, "DERIVE ", 7) == 0) {
			do_derive(&gs, command);
		} else if (strncmp(command, "BITCHECK", 8) == 0) {
			bitcheck(&gs.img);
		}
	}
	close_game(&gs);
	return 0;
}
