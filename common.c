#include "./common.h"
#include "./screen.h"

__attribute__((__noreturn__)) void interrupt (const char *explanation) {
	perror(explanation);
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &store_terminal_settings) == -1) {
		perror("tcgetattr");
	}
	exit(EXIT_FAILURE);
}

__attribute__((__pure__)) size_t get_count_of_digits (int number) {
	size_t count;
	if ((count = snprintf(NULL, 0, "%d", number)) == -1) {
		interrupt("snprintf");
	}
	return count;
}

void safe_write (int file_descriptor, char *text, size_t lenght) {
	if (write(file_descriptor, text, lenght) == -1) {
		interrupt("write");
	}
}

void print_number (unsigned int number) {
	const int count = get_count_of_digits(number);
	char number_in_string[count + 1];
	if ((sprintf(number_in_string, "%d", number)) == -1) {
		interrupt("sprintf");
	}
	safe_write(STDOUT_FILENO, number_in_string, count);
}

size_t get_size_of_file (int file_descriptor) {
	size_t lenght;
	size_t position;
	if ((position = (size_t) lseek(file_descriptor, (off_t) 0, SEEK_END)) != (size_t) -1) {
		lenght = position;
		if ((position = lseek(file_descriptor, (off_t) 0, SEEK_SET)) == (size_t) -1) {
			interrupt("lseek");
		}
	} else {
		interrupt("lseek");
	}
	return lenght;
}

void insert_string_to_string (char **str, size_t lenght_str, char *insert_str, size_t lenght_insert_str, size_t position) {
	const size_t lenght_new_str = lenght_str + lenght_insert_str + 1;
	char combination[lenght_new_str + 1];
	strncpy(combination, *str, position);
	combination[position] = '\0';
	strncat(combination, insert_str, lenght_insert_str);
	strncat(combination, *str + position, lenght_str - position);
	*str = realloc(*str, (lenght_new_str + 1) * sizeof(char));
	strncpy(*str, combination, lenght_new_str);
}

int get_count_of_substr_occurrences_in_str (char *str, int lenght_str, char *substr, int lenght_substr) {
	int count = 0;
	char *strstr_str = strstr(str, substr);
	while (strstr_str) {
		int position = lenght_str - strlen(strstr_str);
		strstr_str = strstr(str + position + lenght_substr, substr);
		count = count + 1;
	}
	return count;
}

void get_str_with_replace_substr_in_str (char *str, int lenght_str, char *search_str, int lenght_search_str, char *replace_str, int lenght_replace_str, char *container) {
	char *strstr_str = strstr(str, search_str);
	int position = 0;
	int previous_position = 0;
	container[position] = '\0';
	while (strstr_str != NULL) {
		position = lenght_str - strlen(strstr_str);
		strncat(container, str + previous_position, position - previous_position);
		strncat(container, replace_str, lenght_replace_str);
		strstr_str = strstr(str + position + lenght_search_str, search_str);
		previous_position = position + lenght_search_str;
	}
	strncat(container, str + position + lenght_search_str, lenght_str - position + 1);
}

void *posmemcpy (void *destination, const void *source, size_t size_of_destination, size_t size_of_source, size_t position, size_t size_of_element) {
	if (position + size_of_destination > size_of_source) {
		perror("unknown size");
		return NULL;
	}
	size_t index = 0;
	while (index < size_of_destination) {
		memcpy(((char *) destination) + index * size_of_element, ((char *) source) + (position + index) * size_of_element, size_of_element);
		index = index + 1;
	}
	return destination;
}

void reset_array (void *array, size_t size_array) {
	size_t index = 0;
	while (index < size_array) {
		memcpy(((char *) array) + index, "\0", 1);
		index = index + 1;
	}
}
