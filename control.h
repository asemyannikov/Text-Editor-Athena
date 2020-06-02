#include "./values.h"
#include "./list.h"
#include "./printing.h"

void menu (char *array_of_menu_options[], int size_of_menu_options);

void set_command (int signal);

void redactor (bool create_new_file);

bool is_command_empty (char *entered_command);

bool get_exit_signal (char *entered_command);

int translate_command_to_signal (char *entered_command);

void inner_operations_with_menu (int position_height, char *type, char **inner_line, size_t *lenght_inner_line);

void handler_CHAR ();

void handler_BACKSPACE ();

void handler_DELETE ();

void handler_NEWLINE ();

void handler_LEFT ();

void handler_UP ();

void handler_RIGHT ();

void handler_DOWN ();

void handler_PAGEUP ();

void handler_PAGEDOWN ();

void handler_END ();

void handler_HOME ();

void handler_SEARCH (struct winsize ws);

void handler_REPLACE (struct winsize ws);

void handler_TAB ();
