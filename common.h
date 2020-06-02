#include "./values.h"

/*
	__attribute__((__noreturn__)) void interrupt
	const char *explanation
	
	Невозвращаемая функция interrupt, с помощью системного вызова perror, сообщает о возникшей ошибке с пояснением, хранящимся в переменной explanation, а также производит выход из программы, предварительно установив параметры терминала из переменной terminal_settings.
	
	Установка новых параметров терминала производится с помощью системного вызова tcsetattr.
	Выбор значения TCSAFLUSH для параметра optional_actions системного вызова tcsetattr выполнен из соображений необходимости отклонения данных, находящихся в очереди из стандартного ввода, с файловым дескриптором STDIN_FILENO, на момент возникновения ошибки.
	
	Выход из программы производится с указанием макроса наличия ошибки EXIT_FAILURE.
*/

__attribute__((__noreturn__)) void interrupt (const char *explanation);

/*
	__attribute__((__pure__) size_t get_count_of_digits
	int number
	
	Чистая функция get_counts_of_digits определяет количество знаков, из которого состоит целое число number. Количество знаков числа number определяется с помощью системного вызова snprintf.
*/

__attribute__((__pure__)) size_t get_count_of_digits (int number);

/*
	void safe_write
	int file_descriptor
	char *text
	size_t lenght
	
	Функция safe_write записывает текст длины lenght из массива с указателем *text с проверкой на наличие ошибки. При возникновении проблем с записью, функция сообщает о возникшей ошибке и безопасно завершает работу программы.
*/

void safe_write (int file_descriptor, char *text, size_t lenght);

/*
	void print_number
	int number
	
	Функция распечатки произвольного положительного числа number.
*/

void print_number (unsigned int number);

/*
	size_t get_size_of_file
	int file_descriptor
	
	Функция, с помощью системных вызовов lseek, определяет размер файла с файловым дескриптором file_descriptor, последовательно устанавливая границу на конец файла и его начало. При возникновении ошибки, функция безопасно завершает работу и сообщает об обнаруженных проблемах.
*/

size_t get_size_of_file (int file_descriptor);

void insert_string_to_string (char **str, size_t lenght_str, char *insert_str, size_t lenght_insert_str, size_t position);

int get_count_of_substr_occurrences_in_str (char *str, int lenght_str, char *substr, int lenght_substr);

void get_str_with_replace_substr_in_str (char *str, int lenght_str, char *search_str, int lenght_search_str, char *replace_str, int lenght_replace_str, char *container);

void *posmemcpy (void *destination, const void *source, size_t size_of_destination, size_t size_of_source, size_t position, size_t size_of_element);

void reset_array (void *array, size_t size_array);
