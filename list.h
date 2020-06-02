#include "./values.h"
#include "./screen.h"
#include "./terminal.h"

void add_line (struct line **first_line, struct line **last_line, char *text);

void insert_line_to_list (struct line **first_line, int row, char* text);

void add_line_to_list (struct line **first_line, struct line **last_line, int row, int list_size, char *text);

void reset_list ();

void free_line_by_index (struct line **first_line, int index);

void free_list (struct line *first_line);

void set_line_to_list (struct line *first_line, struct line setting_line);

struct line get_line_from_list_by_index (struct line *first_line, int index);

void add_text_to_line (struct line *first_line, int index, char* text);

void set_windows_size_to_list ();

void init_list ();

void reset_list ();

void close_list ();
