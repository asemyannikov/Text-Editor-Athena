#include <locale.h>
#include "./values.h"

#include "./common.h"
#include "./control.h"
#include "./list.h"
#include "./printing.h"
#include "./screen.h"
#include "./terminal.h"

int main (int argc, char *argv[]) {
	// setlocale(LC_ALL, "Russian");
	// write(STDOUT_FILENO, "Привет!", 12);
	init_list();
	get_screen_settings();
	menu(main_menu, SIZE_MAIN_MENU);
	close_list();
	return 0;
}
