#include "./printing.h"

void print_line_header (int position_row, int index, bool put_arrow) {
	move_to_position(list.position_row, 1);
	if (put_arrow == true) {
		safe_write(STDOUT_FILENO, STYLE_SYMBOL_RIGHT_ARROW, STYLE_SIZE_RIGHT_ARROW);
	} else {
		safe_write(STDOUT_FILENO, "  ", STYLE_SIZE_RIGHT_ARROW);
	}
	move_to_position(position_row, STYLE_SIZE_RIGHT_ARROW);
	print_number(index);
	move_to_position(position_row, list.start_column);
}

void print_footer () {
	char hot_key_menu[] = "[ESC]: MENU";
	char hot_key_search[] = " | [CTRL + F]: SEARCH";
	char hot_key_replace[] = " | [CTRL + R]: REPLACE";
	int lenght_hot_key_menu = strlen(hot_key_menu);
	int lenght_hot_key_search = strlen(hot_key_search);
	int lenght_hot_key_replace = strlen(hot_key_replace);
	char footer[screen_size.ws_col + 1];
	reset_array(footer, screen_size.ws_col + 1);
	int footer_size = screen_size.ws_col;
	strncat(footer, STYLE_BCKGRND_COLOR_YELLOW, STYLE_SIZE_COLOR_YELLOW);
	move_to_position(list.screen_row + 1, 1);
	safe_write(STDOUT_FILENO, STYLE_BCKGRND_COLOR_YELLOW, STYLE_SIZE_COLOR_YELLOW);
	safe_write(STDOUT_FILENO, STYLE_SYMBOL_COLOR_BLACK, STYLE_SIZE_COLOR_BLACK);
	if (lenght_hot_key_menu < footer_size) {
		safe_write(STDOUT_FILENO, hot_key_menu, lenght_hot_key_menu);
		footer_size = footer_size - lenght_hot_key_menu;
	}
	if (lenght_hot_key_search < footer_size - lenght_hot_key_menu) {
		safe_write(STDOUT_FILENO, hot_key_search, lenght_hot_key_search);
		footer_size = footer_size - lenght_hot_key_search;
	}
	if (lenght_hot_key_replace < footer_size - lenght_hot_key_menu - lenght_hot_key_search) {
		safe_write(STDOUT_FILENO, hot_key_replace, lenght_hot_key_replace);
		footer_size = footer_size - lenght_hot_key_replace;
	}
	int index;
	for (index = 0; index < footer_size; index = index + 1) {
		safe_write(STDOUT_FILENO, " ", 1);
	}
	safe_write(STDOUT_FILENO, STYLE_SYMBOL_NORMAL, STYLE_SYMBOL_SIZE);
	move_to_position(list.position_row, list.position_column);
}

void print_line (struct line *current_line) {
	if (lenght_search_line > 0) {
		char text[current_line->lenght + 1];
		reset_array(text, current_line->lenght + 1);
		strncpy(text, current_line->text, current_line->lenght);
		int count = get_count_of_substr_occurrences_in_str(text, current_line->lenght, search_line, lenght_search_line);
		if (count > 0) {
			int lenght_insert_line = STYLE_SIZE_COLOR_YELLOW + STYLE_SYMBOL_SIZE + lenght_search_line;
			char insert_line[lenght_insert_line + 1];
			reset_array(insert_line, lenght_insert_line + 1);
			strcat(insert_line, STYLE_SYMBOL_COLOR_YELLOW);
			strcat(insert_line, search_line);
			strcat(insert_line, STYLE_SYMBOL_NORMAL);
			size_t lenght_highlighted_text = count * (lenght_insert_line - lenght_search_line) + current_line->lenght;
			char highlighted_text[lenght_highlighted_text + 1];
			reset_array(highlighted_text, lenght_highlighted_text + 1);
			get_str_with_replace_substr_in_str(text, current_line->lenght, search_line, lenght_search_line, insert_line, lenght_insert_line, highlighted_text);
			safe_write(STDOUT_FILENO, highlighted_text, strlen(highlighted_text));
		} else {
			safe_write(STDOUT_FILENO, current_line->text, current_line->lenght);
		}
	} else {
		safe_write(STDOUT_FILENO, current_line->text, current_line->lenght);
	}
}

void print_line_in_color (char *text, size_t lenght, int oflag, char *style_color_text, int size_of_color_style, char *style_background, int size_of_background_style) {
	int index;
	size_t lenght_format_line = 0;
	for (index = 0; index < 4; index = index + 1) {
		if (((1 << index) & oflag) != 0) {
			lenght_format_line = lenght_format_line + STYLE_SYMBOL_SIZE;
		}
	}
	lenght_format_line = lenght_format_line + size_of_color_style + size_of_background_style + lenght + STYLE_SYMBOL_SIZE;
	char format_line[lenght_format_line + 1];
	reset_array(format_line, lenght_format_line + 1);
	for (index = 0; index < 4; index = index + 1) {
		if (((1 << index) & oflag) != 0) {
			switch (index) {
				case 1:  strcat(format_line, STYLE_SYMBOL_BOLD); break;
				case 2:  strcat(format_line, STYLE_SYMBOL_ITALICS); break;
				case 3:  strcat(format_line, STYLE_SYMBOL_UNDERLINE); break;
				default: strcat(format_line, STYLE_SYMBOL_NORMAL); break;
			}
		}
	}
	strncat(format_line, style_color_text, size_of_color_style);
	strncat(format_line, style_background, size_of_background_style);
	strncat(format_line, text, lenght);
	strcat(format_line, STYLE_SYMBOL_NORMAL);
	safe_write(STDOUT_FILENO, format_line, lenght_format_line);
}

void print_page () {
	clear_screen();
	struct line *current_line = list.last_line;
	int position_row = 1;
	while (current_line != NULL) {
		if (current_line->index >= list.screen_bottom_border && current_line->index <= list.screen_top_border) {
			print_line_header(position_row, current_line->index, true);
			print_line(current_line);
			position_row = position_row + 1;
		}
		current_line = current_line->prev;
	}
	print_footer();
	move_to_position(list.position_row, list.position_column);
}

void print_menu_list (int menu_bottom_border, int size_frame, int index_of_selected_option, char *array_of_menu_options[], int size_of_menu_options) {
	int index_label;
	int index_menu;
	int menu_position = menu_bottom_border;
	int size_of_label_of_selected_option;
	for (index_menu = 0; index_menu < size_of_menu_options; index_menu = index_menu + 1) {
		move_to_position(menu_position, (screen_size.ws_col - size_frame) * 0.5);
		if (index_menu == index_of_selected_option) {
			char label_of_selected_option[size_frame + 1];
			size_of_label_of_selected_option = strlen(array_of_menu_options[index_menu]);
			strncpy(label_of_selected_option, array_of_menu_options[index_menu], size_of_label_of_selected_option);
			for (index_label = size_of_label_of_selected_option; index_label < size_frame; index_label = index_label + 1) {
				label_of_selected_option[index_label] = ' ';
			}
			print_line_in_color(label_of_selected_option, size_frame, SS_BOLD, STYLE_SYMBOL_COLOR_WHITE, STYLE_SIZE_COLOR_WHITE, STYLE_BCKGRND_COLOR_GREEN, STYLE_SIZE_COLOR_GREEN);
		} else {
			clear_row();
			safe_write(STDOUT_FILENO, array_of_menu_options[index_menu], strlen(array_of_menu_options[index_menu]));
		}
		menu_position = menu_position + 1;
	}
}
