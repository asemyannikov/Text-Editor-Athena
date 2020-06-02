#include "./control.h"

void open_file () {
	int file_descriptor = open(path_to_file, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (file_descriptor == -1) {
		move_to_position(screen_size.ws_row, 1);
		clear_row();
		char warning[] = "Please, enter the name of a file!";
		print_line_in_color(warning, strlen(warning), SS_BOLD, STYLE_SYMBOL_COLOR_RED, STYLE_SIZE_COLOR_RED, NULL, 0);
		menu(open_menu, SIZE_OPEN_MENU);
	} else {
		clear_screen();
		ssize_t number_of_characters_read = 0;
		size_t size_of_file = get_size_of_file(file_descriptor);
		char buf[size_of_file + 1];
		while ((size_of_file > 0) && (number_of_characters_read = read(file_descriptor, buf, size_of_file))) {
			if (number_of_characters_read == -1) {
				interrupt("read");
			}
			size_of_file -= number_of_characters_read;
			*buf += size_of_file;
		}
		int index_symbol_from_buf;
		int lenght = 1;
		
		free_list(list.first_line);
		lenght_path_to_file = 0;
		path_to_file = calloc(lenght_path_to_file + 1, sizeof(char));
		list.start_row = 1;
		list.position_row = list.start_row;
		list.position_column = list.start_column;
		list.screen_bottom_border = 1;
		list.screen_top_border = list.screen_row - 1;
		list.list_size = 0;
		list.list_index = 1;
		list.first_line = NULL;
		list.last_line = NULL;
		for (index_symbol_from_buf = 0; index_symbol_from_buf < number_of_characters_read; index_symbol_from_buf = index_symbol_from_buf + 1) {
			if (buf[index_symbol_from_buf] == '\n') {
				list.list_size = list.list_size + 1;
				buf[index_symbol_from_buf] = ' ';
				char text_in_line[lenght + 1];
				reset_array(text_in_line, lenght + 1);
				posmemcpy(text_in_line, buf, lenght, number_of_characters_read, (index_symbol_from_buf + 1) - lenght, sizeof(char));
				add_line_to_list(&(list.first_line), &(list.last_line), list.list_size, list.list_size, text_in_line);
				lenght = 1;
			} else {
				lenght = lenght + 1;
			}
		}
		close(file_descriptor);
		list.start_column = STYLE_SIZE_RIGHT_ARROW + get_count_of_digits(list.list_size) + 1;
		list.position_column = list.start_column;
		list.current = get_line_from_list_by_index(list.first_line, 1);
	}
}

void save_file () {
	int file_descriptor = open(path_to_file, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (file_descriptor == -1) {
		move_to_position(screen_size.ws_row, 1);
		clear_row();
		char warning[] = "Please, enter the name of a file!";
		print_line_in_color(warning, strlen(warning), SS_BOLD, STYLE_SYMBOL_COLOR_RED, STYLE_SIZE_COLOR_RED, NULL, 0);
		menu(save_menu, SIZE_SAVE_MENU);
	} else {
		ssize_t number_symbols_to_write;
		if (file_descriptor == -1) {
			interrupt("open");
		}
		struct line *current_line = list.last_line;
		while (current_line != NULL) {
			safe_write(file_descriptor, current_line->text, current_line->lenght - 1);
			current_line = current_line->prev;
			safe_write(file_descriptor, "\n", 1);
		}
		close(file_descriptor);
	}
}

void menu (char *array_of_menu_options[], int size_of_menu_options) {
	char menu_command[_SC_LINE_MAX];
	reset_array(menu_command, _SC_LINE_MAX);
	int menu_bottom_border = screen_size.ws_row * 0.5;
	int size_frame = screen_size.ws_col * 0.25;
	int index_of_selected_option = 0;
	bool create_new_file = false;
	int signal;
	print_menu_list(menu_bottom_border, size_frame, index_of_selected_option, array_of_menu_options, size_of_menu_options);
	while (!get_exit_signal(menu_command)) {
		safe_write(STDOUT_FILENO, HIDE_CURSOR, 6);
		signal = translate_command_to_signal(menu_command);
		get_screen_settings();
		switch (signal) {
		case SIG_UP:
			if (index_of_selected_option > 0) {
				index_of_selected_option = index_of_selected_option - 1;
				print_menu_list(menu_bottom_border, size_frame, index_of_selected_option, array_of_menu_options, size_of_menu_options);
			}
			break;
		case SIG_DOWN:
			if (index_of_selected_option < size_of_menu_options - 1) {
				index_of_selected_option = index_of_selected_option + 1;
				print_menu_list(menu_bottom_border, size_frame, index_of_selected_option, array_of_menu_options, size_of_menu_options);
			}
			break;
		case SIG_NEWLINE:
			if (strcmp(array_of_menu_options[index_of_selected_option], STYLE_LABEL_EXIT) == 0) {
				close_list();
			} else if (strcmp(array_of_menu_options[index_of_selected_option], STYLE_LABEL_CONTINUE) == 0) {
				goto back_to_redactor;
			} else if (strcmp(array_of_menu_options[index_of_selected_option], STYLE_LABEL_NEW_FILE) == 0) {
				create_new_file = true;
				goto back_to_redactor;
			} else if (strcmp(array_of_menu_options[index_of_selected_option], STYLE_LABEL_OPEN_FILE) == 0) {
				clear_screen();
				menu(open_menu, SIZE_OPEN_MENU);
			} else if (strcmp(array_of_menu_options[index_of_selected_option], STYLE_LABEL_SAVE_FILE) == 0) {
				clear_screen();
				menu(save_menu, SIZE_SAVE_MENU);
			} else if (strcmp(array_of_menu_options[index_of_selected_option], STYLE_LABEL_CANCEL) == 0) {
				goto back_to_redactor;
			} else if (strcmp(array_of_menu_options[index_of_selected_option], STYLE_LABEL_SAVE) == 0) {
				save_file();
				goto back_to_redactor;
			} else if (strcmp(array_of_menu_options[index_of_selected_option], STYLE_LABEL_OPEN) == 0) {
				open_file();
				goto back_to_redactor;
			} else if (strcmp(array_of_menu_options[index_of_selected_option], STYLE_LABEL_PATH) == 0) {
				safe_write(STDOUT_FILENO, SHOW_CURSOR, 6);
				inner_operations_with_menu(list.screen_row, "Path to file:~", &path_to_file, &lenght_path_to_file);
			}
			break;
		}
		if (screen_size.ws_row != list.screen_row) {
			list.screen_row = screen_size.ws_row;
			index_of_selected_option = 0;
			menu_bottom_border = screen_size.ws_row * 0.5;
			clear_screen();
			print_menu_list(menu_bottom_border, size_frame, index_of_selected_option, array_of_menu_options, size_of_menu_options);
		}
	}
	back_to_redactor: redactor(create_new_file);
}

void set_command (int signal) {
	switch (signal) {
		case SIG_BACKSPACE:
			handler_BACKSPACE();
			break;
		case SIG_NEWLINE:
			handler_NEWLINE();
			print_page();
			break;
		case SIG_PAGEUP:
			handler_PAGEUP();
			break;
		case SIG_PAGEDOWN:
			handler_PAGEDOWN();
			break;
		case SIG_END:
			handler_END();
			break;
		case SIG_HOME:
			handler_HOME();
			break;
		case SIG_LEFT:
			handler_LEFT();
			break;
		case SIG_UP:
			handler_UP();
			break;
		case SIG_RIGHT:
			handler_RIGHT();
			break;
		case SIG_DOWN:
			handler_DOWN();
			break;
		case SIG_SEARCH:
			handler_SEARCH(screen_size);
			break;
		case SIG_REPLACE:
			handler_REPLACE(screen_size);
			break;
		case SIG_DELETE:
			handler_DELETE();
			break;
		case SIG_TAB:
			handler_TAB();
			break;
		case SIG_MENU:
			clear_screen();
			menu(local_menu, SIZE_LOCAL_MENU);
			break;
		default:
			handler_CHAR();
			break;
	}
}

void redactor (bool create_new_file) {
	int signal = 0;
	int delta;
	if (create_new_file == true) {
		reset_list();
	}
	clear_screen();
	print_page();
	print_footer();
	while (!get_exit_signal(command)) {
		signal = translate_command_to_signal(command);
		if (!is_command_empty(command)) {
			set_command(signal);
		}
		get_screen_settings();
		if ((screen_size.ws_row != list.screen_row && screen_size.ws_row > 1) || screen_size.ws_col != list.screen_col && screen_size.ws_col > 1) {
			delta = screen_size.ws_row - list.screen_row;
			list.screen_top_border = list.screen_top_border + delta;
			if (list.position_row == screen_size.ws_row) {
				list.position_row = list.position_row - 1;
				list.position_column = list.start_column;
				list.list_index = list.list_index - 1;
				list.current = *(list.current).next;
			}
			list.screen_row = screen_size.ws_row;
			list.screen_col = screen_size.ws_col;
			print_page();
		}
	}
}

bool is_command_empty (char *entered_command) {
	return (entered_command[0] == '\0');
}

bool get_exit_signal (char *entered_command) {
	return (strcmp(entered_command, CTRL_E) == 0);
}

int translate_command_to_signal (char *entered_command) {
	reset_array(entered_command, _SC_LINE_MAX);
	if (read(STDIN_FILENO, entered_command, sizeof(entered_command)) == -1) {
		interrupt("read");
	}
	if (strcmp(entered_command, BACKSPACE) == 0) {
		return SIG_BACKSPACE;
	} else if (strcmp(entered_command, NEWLINE) == 0) {
		return SIG_NEWLINE;
	} else if (strcmp(entered_command, PAGEUP) == 0) {
		return SIG_PAGEUP;
	} else if (strcmp(entered_command, PAGEDOWN) == 0) {
		return SIG_PAGEDOWN;
	} else if (strcmp(entered_command, END) == 0) {
		return SIG_END;
	} else if (strcmp(entered_command, HOME) == 0) {
		return SIG_HOME;
	} else if (strcmp(entered_command, LEFT) == 0) {
		return SIG_LEFT;
	} else if (strcmp(entered_command, UP) == 0) {
		return SIG_UP;
	} else if (strcmp(entered_command, RIGHT) == 0) {
		return SIG_RIGHT;
	} else if (strcmp(entered_command, DOWN) == 0) {
		return SIG_DOWN;
	} else if (strcmp(entered_command, DELETE) == 0) {
		return SIG_DELETE;
	} else if (strcmp(entered_command, TAB) == 0) {
		return SIG_TAB;
	} else if (strcmp(entered_command, CTRL_F) == 0) {
		return SIG_SEARCH;
	} else if (strcmp(entered_command, CTRL_R) == 0) {
		return SIG_REPLACE;
	} else if (strcmp(entered_command, ESC) == 0) {
		return SIG_MENU;
	} else {
		return SIG_CHAR;
	}
}

void inner_operations_with_menu (int position_height, char *type, char **inner_line, size_t *lenght_inner_line) {
	int start_position = 1 + strlen(type);
	char inner_command[_SC_LINE_MAX];
	reset_array(inner_command, _SC_LINE_MAX);
	move_to_position(position_height, 1);
	clear_row(position_height);
	print_line_in_color(type, strlen(type), SS_BOLD, STYLE_SYMBOL_COLOR_GREEN, STYLE_SIZE_COLOR_GREEN, NULL, 0);
	safe_write(STDOUT_FILENO, (*inner_line), *lenght_inner_line);
	move_to_position(position_height, start_position + *lenght_inner_line);
	int position = *lenght_inner_line;
	int signal = translate_command_to_signal(inner_command);
	while (!get_exit_signal(inner_command)) {
		signal = translate_command_to_signal(inner_command);
		if (!is_command_empty(inner_command)) {
			if (signal == SIG_CHAR) {
				insert_string_to_string(&(*inner_line), *lenght_inner_line, inner_command, 1, position);
				*lenght_inner_line = *lenght_inner_line + 1;
				position = position + 1;
			}
			if (signal == SIG_LEFT && position > 0) {
				position = position - 1;
			}
			if (signal == SIG_RIGHT && position < *lenght_inner_line) {
				position = position + 1;
			}
			if (signal == SIG_BACKSPACE && position > 0) {
				int index_of_deleting_char = position - 1;
				memmove(&(*inner_line)[index_of_deleting_char], &(*inner_line)[index_of_deleting_char + 1], *lenght_inner_line - index_of_deleting_char);
				*lenght_inner_line = *lenght_inner_line - 1;
				position = position - 1;
			}
			if (signal == SIG_NEWLINE) {
				break;
			}
			clear_row(position_height);
			move_to_position(position_height, 1);
			print_line_in_color(type, start_position, SS_BOLD, STYLE_SYMBOL_COLOR_GREEN, STYLE_SIZE_COLOR_GREEN, NULL, 0);
			safe_write(STDOUT_FILENO, (*inner_line), *lenght_inner_line);
			move_to_position(position_height, start_position + position);
		}
	}
}

void handler_CHAR () {
	int position = list.position_column - list.start_column;
	insert_string_to_string(&(list.current).text, (list.current).lenght, command, 1, position);
	(list.current).lenght = (list.current).lenght + 1;
	list.position_column = list.position_column + 1;
	set_line_to_list(list.first_line, list.current);
	print_line_header(list.position_row, (list.current).index, true);
	print_line(&list.current);
	move_to_position(list.position_row, list.position_column);
}

void handler_BACKSPACE () {
	if (list.position_column > list.start_column) {
		int index_of_deleting_char = list.position_column - list.start_column - 1;
		memmove(&(list.current).text[index_of_deleting_char], &(list.current).text[index_of_deleting_char + 1], (list.current).lenght - index_of_deleting_char);
		(list.current).lenght = (list.current).lenght - 1;
		list.position_column = list.position_column - 1;
		set_line_to_list(list.first_line, (list.current));
		print_line_header(list.position_row, (list.current).index, true);
		print_line(&list.current);
		move_to_position(list.position_row, list.position_column);
	} else {
		if ((list.current).next != NULL) {
			line previous_line = *(list.current).next;
			
			list.list_size = list.list_size - 1;
			list.start_column = STYLE_SIZE_RIGHT_ARROW + get_count_of_digits(list.list_size) + 1;
			list.position_column = previous_line.lenght + (list.start_column - 1);
			
			memmove(&previous_line.text[previous_line.lenght - 1], &previous_line.text[previous_line.lenght], 1);
			int size_of_first_part = previous_line.lenght + (list.current).lenght - 1;
			char first_part_of_text[size_of_first_part + 1];
			reset_array(first_part_of_text, size_of_first_part + 1);
			memcpy(first_part_of_text, previous_line.text, previous_line.lenght);
			strncat(first_part_of_text, (list.current).text, (list.current).lenght);
			
			free(previous_line.text);
			previous_line.lenght = size_of_first_part;
			previous_line.text = calloc(previous_line.lenght + 1, sizeof(char));
			memcpy(previous_line.text, first_part_of_text, previous_line.lenght);
			
			set_line_to_list(list.first_line, previous_line);
			free_line_by_index(&(list.first_line), list.list_index);
			list.list_index = list.list_index - 1;
			list.current = get_line_from_list_by_index(list.first_line, list.list_index);
			if (list.position_row > list.start_row) {
				list.position_row = list.position_row - 1;
			}
			if (list.list_index < list.screen_bottom_border) {
				list.screen_bottom_border = list.screen_bottom_border - 1;
				list.screen_top_border = list.screen_top_border - 1;
			}
			print_page();
		}
	}
}

void handler_DELETE () {
	int index_of_deleting_char = list.position_column - list.start_column;
	if (index_of_deleting_char < (list.current).lenght - 1) {
		memmove(&(list.current).text[index_of_deleting_char], &(list.current).text[index_of_deleting_char + 1], (list.current).lenght - index_of_deleting_char);
		(list.current).lenght = (list.current).lenght - 1;
		set_line_to_list(list.first_line, list.current);
		print_line_header(list.position_row, (list.current).index, true);
		print_line(&list.current);
		move_to_position(list.position_row, list.position_column);
	} else {
		if ((list.current).prev != NULL) {
			line next_line = *(list.current).prev;
			
			list.list_size = list.list_size - 1;
			list.start_column = STYLE_SIZE_RIGHT_ARROW + get_count_of_digits(list.list_size) + 1;
			list.position_column = list.current.lenght + list.start_column - 1;
			
			memmove(&(list.current).text[(list.current).lenght - 1], &(list.current).text[(list.current).lenght], 1);
			(list.current).lenght = (list.current).lenght + next_line.lenght - 1;
			(list.current).text = realloc((list.current).text, ((list.current).lenght + 1) * sizeof(char));
			strncat((list.current).text, next_line.text, next_line.lenght);
			set_line_to_list(list.first_line, list.current);
			free_line_by_index(&(list.first_line), list.list_index + 1);
			list.current = get_line_from_list_by_index(list.first_line, list.list_index);
			if (list.list_index < list.screen_bottom_border) {
				list.screen_bottom_border = list.screen_bottom_border - 1;
				list.screen_top_border = list.screen_top_border - 1;
			}
			print_page();
		}
	}
}

void handler_NEWLINE () {
	int delta_position = list.start_column - 1;
	if (list.position_column == (list.current).lenght + delta_position) {
		add_line_to_list(&(list.first_line), &(list.last_line), list.list_index, list.list_size, " ");
	} else {
		int size_of_first_part = list.position_column - list.start_column;
		char first_part_of_text[size_of_first_part + 2];
		reset_array(first_part_of_text, size_of_first_part + 2);
		posmemcpy(first_part_of_text, (list.current).text, size_of_first_part, (list.current).lenght, 0, sizeof(char));
		
		int size_of_second_part = (list.current).lenght - size_of_first_part;
		char second_part_of_text[size_of_second_part + 1];
		reset_array(second_part_of_text, size_of_second_part + 1);
		posmemcpy(second_part_of_text, (list.current).text, size_of_second_part - 1, (list.current).lenght, size_of_first_part, sizeof(char));
		
		reset_array((list.current).text, (list.current).lenght);
		strcpy((list.current).text, first_part_of_text);
		(list.current).lenght = size_of_first_part + 1;
		strcat((list.current).text, " ");
		set_line_to_list(list.first_line, list.current);
		
		strncat(second_part_of_text, " ", 1);
		add_line_to_list(&(list.first_line), &(list.last_line), list.list_index, list.list_size, second_part_of_text);
	}
	list.list_size = list.list_size + 1;
	list.start_column = STYLE_SIZE_RIGHT_ARROW + get_count_of_digits(list.list_size) + 1;
	list.position_column = list.start_column;
	int new_list_index = list.list_index + 1;
	if (new_list_index <= list.list_size) {
		list.list_index = new_list_index;
	}
	list.current = get_line_from_list_by_index(list.first_line, list.list_index);
	if (list.position_row < list.screen_row - 1) {
		list.position_row = list.position_row + 1;
	}
	if (list.list_index > list.screen_top_border) {
		list.screen_bottom_border = list.screen_bottom_border + 1;
		list.screen_top_border = list.screen_top_border + 1;
	}
}

void handler_LEFT () {
	if (list.position_column > list.start_column) {
		list.position_column = list.position_column - 1;
		move_to_position(list.position_row, list.position_column);
	} else {
		if ((list.current).next != NULL) {
			int new_list_index = list.list_index - 1;
			if (new_list_index <= list.list_size) {
				list.list_index = new_list_index;
			}
			list.current = get_line_from_list_by_index(list.first_line, list.list_index);
			if (list.list_index < list.screen_bottom_border) {
				list.screen_bottom_border = list.screen_bottom_border - 1;
				list.screen_top_border = list.screen_top_border - 1;
			}
			list.position_column = list.current.lenght + (list.start_column - 1);
			if (list.position_row > list.start_row) {
				print_line_header(list.position_row, (list.current).index + 1, false);
				list.position_row = list.position_row - 1;
				print_line_header(list.position_row, (list.current).index, true);
				move_to_position(list.position_row, list.position_column);
			} else {
				list.position_row = list.start_row;
				print_page();
			}
		}
	}
}

void handler_UP () {
	if (list.list_index > 1) {
		int new_list_index = list.list_index - 1;
		if (new_list_index <= list.list_size) {
			list.list_index = new_list_index;
		}
		list.current = get_line_from_list_by_index(list.first_line, list.list_index);
		if (list.list_index < list.screen_bottom_border) {
			list.screen_bottom_border = list.screen_bottom_border - 1;
			list.screen_top_border = list.screen_top_border - 1;
		}
		list.position_column = list.start_column;
		if (list.position_row > list.start_row) {
			print_line_header(list.position_row, (list.current).index + 1, false);
			list.position_row = list.position_row - 1;
			print_line_header(list.position_row, (list.current).index, true);
			move_to_position(list.position_row, list.position_column);
		} else {
			list.position_row = list.start_row;
			print_page();
		}
	}
}

void handler_RIGHT () {
	int delta_position = list.start_column - 1;
	if (list.position_column < (list.current).lenght + delta_position) {
		list.position_column = list.position_column + 1;
		move_to_position(list.position_row, list.position_column);
	} else {
		if ((list.current).prev != NULL) {
			int new_list_index = list.list_index + 1;
			if (new_list_index <= list.list_size) {
				list.list_index = new_list_index;
			}
			list.current = get_line_from_list_by_index(list.first_line, list.list_index);
			if (list.list_index > list.screen_top_border) {
				list.screen_bottom_border = list.screen_bottom_border + 1;
				list.screen_top_border = list.screen_top_border + 1;
			}
			list.position_column = list.start_column;
			if (list.position_row < list.screen_row - 1) {
				print_line_header(list.position_row, (list.current).index - 1, false);
				list.position_row = list.position_row + 1;
				print_line_header(list.position_row, (list.current).index, true);
				move_to_position(list.position_row, list.position_column);
			} else {
				list.position_row = list.screen_row - 1;
				print_page();
			}
		}
	}
}

void handler_DOWN () {
	if (list.position_row + list.screen_bottom_border <= list.list_size) {
		int new_list_index = list.list_index + 1;
		if (new_list_index <= list.list_size) {
			list.list_index = new_list_index;
		}
		list.current = get_line_from_list_by_index(list.first_line, list.list_index);
		if (list.list_index > list.screen_top_border) {
			list.screen_bottom_border = list.screen_bottom_border + 1;
			list.screen_top_border = list.screen_top_border + 1;
		}
		list.position_column = list.start_column;
		if (list.position_row < list.screen_row - 1) {
			print_line_header(list.position_row, (list.current).index - 1, false);
			list.position_row = list.position_row + 1;
			print_line_header(list.position_row, (list.current).index, true);
			move_to_position(list.position_row, list.position_column);
		} else {
			list.position_row = list.screen_row - 1;
			print_page();
		}
	}
}

void handler_PAGEUP () {
	print_line_header(list.position_row, (list.current).index, false);
	if (list.position_row > 1) {
		list.position_row = 1;
		list.list_index = list.screen_bottom_border;
		list.current = get_line_from_list_by_index(list.first_line, list.list_index);
		list.position_column = list.start_column;
	}
	print_line_header(list.position_row, list.list_index, true);
	move_to_position(list.position_row, list.position_column);
}

void handler_PAGEDOWN () {
	print_line_header(list.position_row, (list.current).index, false);
	if (list.screen_row - 1 < list.list_size) {
		list.position_row = list.screen_row - 1;
		list.list_index = list.screen_top_border;
	} else {
		list.position_row = list.list_size;
		list.list_index = list.list_size;
	}
	list.current = get_line_from_list_by_index(list.first_line, list.list_index);
	print_line_header(list.position_row, list.list_index, true);
	list.position_column = list.start_column;
}

void handler_END () {
	int lenght_line = (list.current).lenght + (list.start_column - 1);
	if (list.position_column < lenght_line) {
		list.position_column = lenght_line;
	}
	move_to_position(list.position_row, list.position_column);
}

void handler_HOME () {
	if (list.position_column > list.start_column) {
		list.position_column = list.start_column;
	}
	move_to_position(list.position_row, list.position_column);
}

void handler_SEARCH (struct winsize ws) {
	inner_operations_with_menu(list.screen_row, STYLE_LABEL_SEARCH, &search_line, &lenght_search_line);
	redactor(false);
}

void handler_REPLACE (struct winsize ws) {
	inner_operations_with_menu(list.screen_row, STYLE_LABEL_SEARCH, &replaceable_line, &lenght_replaceable_line);
	inner_operations_with_menu(list.screen_row, STYLE_LABEL_REPLACE, &replace_line, &lenght_replace_line);
	if (lenght_replaceable_line != 0 && lenght_replace_line != 0) {
		// clear_screen();
		// print_number(lenght_replaceable_line);
		// print_number(lenght_replace_line);
		// pause();
		struct line *current_line = list.last_line;
		while (current_line != NULL) {
			char text[current_line->lenght + 1];
			reset_array(text, current_line->lenght + 1);
			strncpy(text, current_line->text, current_line->lenght);
			int count = get_count_of_substr_occurrences_in_str(text, current_line->lenght, replaceable_line, lenght_replaceable_line);
			if (count > 0) {
				const size_t lenght_replaced_text = count * (lenght_replace_line - lenght_replaceable_line) + current_line->lenght;
				char replaced_text[lenght_replaced_text + 1];
				reset_array(replaced_text, lenght_replaced_text + 1);
				get_str_with_replace_substr_in_str(text, current_line->lenght, replaceable_line, lenght_replaceable_line, replace_line, lenght_replace_line, replaced_text);
				current_line->lenght = lenght_replaced_text;
				current_line->text = realloc(current_line->text, (current_line->lenght + 1) * sizeof(char));
				strncpy(current_line->text, replaced_text, lenght_replaced_text);
				if (list.current.index == current_line->index) {
					list.current = get_line_from_list_by_index(list.first_line, current_line->index);
					list.start_column = STYLE_SIZE_RIGHT_ARROW + get_count_of_digits(list.list_size) + 1;
					list.position_column = list.start_column;
					move_to_position(list.position_row, list.position_column);
				}
			}
			current_line = current_line->prev;
		}
	}
	redactor(false);
}

void handler_TAB () {
	int position = list.position_column - list.start_column;
	insert_string_to_string(&(list.current).text, (list.current).lenght, STYLE_SYMBOL_TAB, STYLE_SIZE_TAB, position);
	(list.current).lenght = (list.current).lenght + STYLE_SIZE_TAB;
	list.position_column = list.position_column + STYLE_SIZE_TAB;
	set_line_to_list(list.first_line, list.current);
	print_line_header(list.position_row, (list.current).index, true);
	print_line(&list.current);
	move_to_position(list.position_row, list.position_column);
}
