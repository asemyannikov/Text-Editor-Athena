#ifndef ADD_H
#define ADD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdbool.h>
#include <sys/ioctl.h>

#define STYLE_SYMBOL_NORMAL    "\x1b[0m"
#define STYLE_SYMBOL_BOLD      "\x1b[1m"
#define STYLE_SYMBOL_ITALICS   "\x1b[3m"
#define STYLE_SYMBOL_UNDERLINE "\x1b[4m"
#define STYLE_SYMBOL_SIZE      4

#define SS_NORMAL              1
#define SS_BOLD                2
#define SS_ITALICS             4
#define SS_UNDERLINE           8

#define STYLE_SYMBOL_TAB            "    "
#define STYLE_SIZE_TAB              4

#define STYLE_SYMBOL_RIGHT_ARROW    "> "// "\u2192"
#define STYLE_SIZE_RIGHT_ARROW      3

#define STYLE_SYMBOL_COLOR_WHITE   "\x1b[38;2;255;255;255m"
#define STYLE_BCKGRND_COLOR_WHITE  "\x1b[48;2;255;255;255m"
#define STYLE_SIZE_COLOR_WHITE     strlen(STYLE_SYMBOL_COLOR_WHITE) + 1

#define STYLE_SYMBOL_COLOR_GREEN   "\x1b[38;2;70;150;60m"
#define STYLE_BCKGRND_COLOR_GREEN  "\x1b[48;2;70;150;60m"
#define STYLE_SIZE_COLOR_GREEN     strlen(STYLE_SYMBOL_COLOR_GREEN) + 1

#define STYLE_SYMBOL_COLOR_RED     "\x1b[38;2;170;0;0m"
#define STYLE_BCKGRND_COLOR_RED    "\x1b[48;2;170;0;0m"
#define STYLE_SIZE_COLOR_RED       strlen(STYLE_SYMBOL_COLOR_RED) + 1

#define STYLE_SYMBOL_COLOR_YELLOW  "\x1b[38;2;230;200;65m"
#define STYLE_BCKGRND_COLOR_YELLOW "\x1b[48;2;230;200;65m"
#define STYLE_SIZE_COLOR_YELLOW    strlen(STYLE_SYMBOL_COLOR_YELLOW) + 1

#define STYLE_SYMBOL_COLOR_BLACK  "\x1b[38;2;51;51;51m"
#define STYLE_BCKGRND_COLOR_BLACK "\x1b[48;2;51;51;51m"
#define STYLE_SIZE_COLOR_BLACK    strlen(STYLE_SYMBOL_COLOR_BLACK) + 1

#define STYLE_LABEL_EXIT      " Exit "
#define STYLE_LABEL_CONTINUE  " Continue "
#define STYLE_LABEL_NEW_FILE  " New file "
#define STYLE_LABEL_OPEN_FILE " Open file "
#define STYLE_LABEL_SAVE_FILE " Save file "
#define STYLE_LABEL_PATH      " Path to file "
#define STYLE_LABEL_SAVE      " Save "
#define STYLE_LABEL_CANCEL    " Cancel "
#define STYLE_LABEL_OPEN      " Open "
#define STYLE_LABEL_SEARCH    "Search:"
#define STYLE_LABEL_REPLACE   "Replace:"

#define SIZE_LOCAL_MENU 5
#define SIZE_MAIN_MENU  3
#define SIZE_OPEN_MENU  3
#define SIZE_SAVE_MENU  3

//

#define UP          "\x1b[A"
#define DOWN        "\x1b[B"
#define RIGHT       "\x1b[C"
#define LEFT        "\x1b[D"
#define END         "\x1b[F"
#define HOME        "\x1b[H"
#define REMOVE      "\x1b[3~"
#define PAGEUP      "\x1b[5~"
#define PAGEDOWN    "\x1b[6~"
#define DELETE      "\x1b[3~"
#define TAB         "\t"
#define CTRL_A      "\x01"
#define CTRL_B      "\x02"
#define CTRL_C      "\x03"
#define CTRL_D      "\x04"
#define CTRL_E      "\x05"
#define CTRL_F      "\x06"
#define CTRL_P      "\x10"
#define CTRL_Q      "\x11"
#define CTRL_R      "\x12"
#define CTRL_N      "\x0e"
#define CTRL_M      "\x0d"
#define ESC         "\x1b"
#define BACKSPACE   "\x7f"
#define NEWLINE     "\n"
#define HIDE_CURSOR "\x1b[?25l"
#define SHOW_CURSOR "\x1b[?25h"

/* Управляющие сигналы */
#define SIG_CHAR      1
#define SIG_BACKSPACE 8
#define SIG_TAB       9
#define SIG_NEWLINE   13
#define SIG_PAGEUP    33
#define SIG_PAGEDOWN  34
#define SIG_END       35
#define SIG_HOME      36
#define SIG_LEFT      37
#define SIG_UP        38
#define SIG_RIGHT     39
#define SIG_DOWN      40
#define SIG_DELETE    46
#define SIG_MENU      77
#define SIG_SEARCH    70
#define SIG_REPLACE   82

char command[_SC_LINE_MAX];

static char *main_menu[] = {
	STYLE_LABEL_NEW_FILE,
	STYLE_LABEL_OPEN_FILE,
	STYLE_LABEL_EXIT
};

static char *save_menu[] = {
	STYLE_LABEL_PATH,
	STYLE_LABEL_SAVE,
	STYLE_LABEL_CANCEL
};

static char *open_menu[] = {
	STYLE_LABEL_PATH,
	STYLE_LABEL_OPEN,
	STYLE_LABEL_CANCEL
};

static char *local_menu[] = {
	STYLE_LABEL_CONTINUE,
	STYLE_LABEL_NEW_FILE,
	STYLE_LABEL_OPEN_FILE,
	STYLE_LABEL_SAVE_FILE,
	STYLE_LABEL_EXIT
};

typedef struct line {
	char *text;
	size_t lenght;
	size_t index;
	struct line *next;
	struct line *prev;
} line;

typedef struct screen {
	int position_row;
	int position_column;
	int start_row;
	int start_column;
	size_t list_size;
	size_t list_index;
	int screen_row;
	int screen_col;
	int screen_bottom_border;
	int screen_top_border;
	struct line *first_line;
	struct line *last_line;
	struct line current;
} screen;

screen list;

/*
	struct termios store_terminal_settings
	
	Структура хранит в себе резервные параметры исходного терминала до его перевода режима в канонический режим.
*/

struct termios store_terminal_settings;

/*
	struct winsize screen_size
	
	Структура хранит в себе информацию о текущих размерах окна.
*/

struct winsize screen_size;

/*
	size_t lenght_path_to_file
	char *path_to_file
	
	Переменные длины и содержимого под путь к открываемому, либо сохраняемому файлу.
*/

size_t lenght_path_to_file;

char *path_to_file;

/*
	size_t lenght_search_line
	char *search_line
	
	Переменные длины и содержимого под искомую в тексте строку.
*/

size_t lenght_search_line;

char *search_line;

/*
	size_t lenght_replace_line
	char *replace_line
	
	Переменные длины и содержимого под замену найденных совпадений в тексте.
*/

size_t lenght_replace_line;

char *replace_line;

/*
	size_t lenght_replaceable_line
	char *replaceable_line
	
	Переменные длины и содержимого под замену найденных совпадений в тексте.
*/

size_t lenght_replaceable_line;

char *replaceable_line;

#endif
