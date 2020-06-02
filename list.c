#include "./list.h"

void add_line (struct line **first_line, struct line **last_line, char *text) {
	struct line *link = (struct line*) malloc(sizeof(struct line));
	link->lenght = strlen(text);
	link->text = calloc(link->lenght + 1, sizeof(char));
	strcpy(link->text, text);
	
	link->prev = NULL;
	if((*first_line) == NULL) {
		link->index = 1;
		(*last_line) = link;
	} else {
		link->index = (*first_line)->index + 1;
		(*first_line)->prev = link;
	}
	link->next = (*first_line);
	(*first_line) = link;
}

void free_list (struct line *first_line) {
	struct line *current_line = first_line;
	while(current_line != NULL) {
		first_line = current_line->next;
		free(current_line->text);
		free(current_line);
		current_line = first_line;
	}
	lenght_path_to_file = 0;
	free(path_to_file);
}

struct line get_line_from_list_by_index(struct line *first_line, int index) {
	struct line *current_line = first_line;
	struct line result;
	while(current_line != NULL) {
		if (current_line->index == index) {
			result = *current_line;
			break;
		}
		current_line = current_line->next;
	}
	return result;
}

void set_line_to_list(struct line *first_line, struct line setting_line) {
	struct line *current_line = first_line;
	while(current_line != NULL) {
		if (current_line->index == setting_line.index) {
			current_line->lenght = setting_line.lenght;
			current_line->text = setting_line.text;
			break;
		}
		current_line = current_line->next;
	}
}

void free_line_by_index (struct line **first_line, int index) {
	struct line *current_line = *first_line;
	while(current_line != NULL) {
		if (current_line->index == index) {
			if (current_line->prev == NULL ) {
				/* Смещение указателя на новую крайнюю строку */
				*first_line = current_line->next;
				(current_line->next)->prev = NULL;
			} else {
				/* Переназначение ссылок соседних строк исходной строки */
				(current_line->prev)->next = current_line->next;
				/* Если текущая строка первая в списке */
				if (current_line->next == NULL) {
					(current_line->prev)->next = NULL;
				} else {
					(current_line->next)->prev = current_line->prev;
				}
				/* Перенумерация индексов */
				struct line *buffer_line = current_line->prev;
				while(buffer_line != NULL) {
					buffer_line->index = buffer_line->index - 1;
					buffer_line = buffer_line->prev;
				}
			}
			free(current_line->text);
			free(current_line);
			break;
		}
		current_line = current_line->next;
	}
}

void insert_line_to_list(struct line **first_line, int row, char* text) {
	struct line *new_line = calloc(1, sizeof(line));
	new_line->lenght = strlen(text);
	new_line->text = calloc(new_line->lenght + 1, sizeof(char));
	strcpy(new_line->text, text);
	new_line->index = row;
	struct line *current_line = *first_line;
	while(current_line != NULL) {
		if (current_line->index == new_line->index) {
			new_line->prev = current_line->prev;
			new_line->next = current_line;
			(current_line->prev)->next = new_line;
			current_line->prev = new_line;
			/* Перенумерация индексов */
			struct line *buffer_line = current_line->prev;
			while(buffer_line != NULL) {
				buffer_line->index = buffer_line->index + 1;
				buffer_line = buffer_line->prev;
			}
			break;
		}
		current_line = current_line->next;
	}
}

void add_line_to_list(struct line **first_line, struct line **last_line, int row, int list_size, char *text) {
	if (list_size == row) {
		add_line(first_line, last_line, text);
	} else {
		insert_line_to_list(first_line, row, text);
	}
}

void init_list () {
	lenght_search_line = 0;
	free(search_line);
	search_line = calloc(lenght_search_line + 1, sizeof(char));
	lenght_replace_line = 0;
	free(replace_line);
	replace_line = calloc(lenght_replace_line + 1, sizeof(char));
	lenght_replaceable_line = 0;
	free(replaceable_line);
	replaceable_line = calloc(lenght_replaceable_line + 1, sizeof(char));
	lenght_path_to_file = 0;
	path_to_file = calloc(lenght_path_to_file + 1, sizeof(char));
	/* Очистка терминала от всех введенных команд ранее */
	clear_screen();
	/* Установка размеров листа */
	set_windows_size_to_list();
	/* Сохранение параметров терминала управляющего устройства */
	/* Переход терминала в неканонический режим */
	struct termios new_settings;
	get_terminal_settings(&new_settings);
	set_noncanonical_mode(&new_settings);
	/* Установка позиционирования */
	list.list_size = 1;
	list.list_index = 1;
	list.start_row = 1;
	list.start_column = STYLE_SIZE_RIGHT_ARROW + get_count_of_digits(list.list_size) + 1;
	list.position_row = list.start_row;
	list.position_column = list.start_column;
	list.screen_bottom_border = 1;
	list.screen_top_border = list.screen_row - 1;
	/* Установка первой строки */
	list.first_line = NULL;
	list.last_line = NULL;
	add_line_to_list(&(list.first_line), &(list.last_line), list.position_row, list.list_size, " ");
	list.current = get_line_from_list_by_index(list.first_line, list.position_row);
}

void reset_list () {
	free_list(list.first_line);
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &store_terminal_settings) == -1) {
		interrupt("tcgetattr");
	}
	init_list();
}

void close_list () {
	/* Очистка экрана терминала */
	clear_screen();
	/* Очистка листа */
	free_list(list.first_line);
	free(search_line);
	free(replace_line);
	free(replaceable_line);
	/* Возвращение исходных параметров терминала */
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &store_terminal_settings) == -1) {
		interrupt("tcgetattr");
	}
	/* Выход из программы */
	exit(EXIT_SUCCESS);
}
