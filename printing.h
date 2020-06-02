#include "./values.h"
#include "./common.h"
#include "./screen.h"

void print_line_header (int position_row, int index, bool put_arrow);

void print_line (struct line *current_line);

void print_line_in_color (char *text, size_t lenght, int oflag, char *style_color_text, int size_of_color_style, char *style_background, int size_of_background_style);

void print_footer ();

void print_page ();

void print_menu_list (int menu_bottom_border, int size_frame, int index_of_selected_option, char *array_of_menu_options[], int size_of_menu_options);
