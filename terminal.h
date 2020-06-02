#include "./values.h"
#include "./common.h"
// #include "./screen.h"
// #include "./list.h"

/*
	void get_terminal_settings
	struct termios *terminal_settings
	
	void get_terminal_settings, с помощью системного вызова tcgetattr, сохраняет текущие настройки терминала в структуру по указателю *terminal_settings и копирует полученные данные в структуру по адресу &store_terminal_settings. Данная операция выполняется для сохранения исходных атрибутов терминала и их корректного восстановления после работы с программой из структуры с адресом &store_terminal_settings.
*/

void get_terminal_settings (struct termios *terminal_settings);

/*
	void set_noncanonical_mode
	struct termios *previous_settings
	
	void set_noncanonical_mode, с помощью системного вызова tcsetattr, устанавливает параметры терминала из структуры по указателю *terminal_settings. Параметры терминала соответствуют стандартному неканоническому режиму, за исключением активного флага ввода ICRNL, который будет включен, чтобы производить чтение команд из пользовательского меню программы.
	
	Отключенные флаги локального режима (c_lflag):
	ICANON - ввод осуществляется в неканоническом режиме;
	ECHO   - вывод вводимых символов будет перехватываться и рассматриваться отдельно;
	ISIG   - возможность ввода символов генерирующих сигналы INTR, QUIT, SUSP. Выход из программы осуществляется специальной управляющей комбинацией [CTRL+E], либо через пользовательское меню;
	IEXTEN - расширенная обработка вводимых символов;
	
	Отключена интекрпретация символов CR, NL, BREAK. Выполнено урезание до 8 бит и проверка на ошибки соответствия. Отключено управление потоком с помощью символов START и STOP.
	
	Отключенные флаги вывода (c_oflag):
	OPOST  - постобработка вывода. Вывод сиволов и управляющих комбинаций обрабатывается отдельно.
	
	Чтение из стандартного ввода осуществляется с временным интервалом. Выбор обусловлен реализацией динамической перерисовки области терминала, при изменении размеров окна.
	
	Выбор значения TCSAFLUSH для параметра optional_actions системного вызова tcsetattr выполнен из соображений необходимости отклонения данных, находящихся в очереди из стандартного ввода, с файловым дескриптором STDIN_FILENO, на момент возникновения ошибки.
	
	На входе осуществляется проверка на пустую структуру по указателю *terminal_settings и, в случае пустой структуры, производится резервное копирование текущих параметров терминала и заполнение ими структур с указателем *terminal_settings и адресом &store_terminal_settings. Тем не менее, проверка на NULL структуры типа struct termios не является на текущий момент актуальной, так как по умолчанию она не является пустой. Однако, чтобы сохранить надежность, мы установим дополнительную проверку.
*/

void set_noncanonical_mode (struct termios *terminal_settings);