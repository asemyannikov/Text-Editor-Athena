# athena

athena - консольный текстовый редактор для UNIX и Unix-подобных операционных систем.

# Реализация и запуск

Реализация выполнена на языке C. Запуск драйвера осуществляется make-командой из файла Makefile:

~$ make programm

~$ make programm-valgrind

Проверка на ошибки памяти выполнена с помощью Valgrind-3.14.0 и LibVEX.

# Мультимедия

Внутри корневого каталога репозитория размещен файл "./Video Presentation.flv", в котором продемонстрирована работа редактора.

# Docker

Система собрана в образ actacyberae/athena. Чтобы скачать образ, следует выполнить команду

~$ sudo docker run actacyberae/athena

Затем подсоединить контейнер к терминалу

~$ sudo docker run -it --rm actacyberae/athena

и перейти в исходный каталог ./home/Athena

~# cd ./home/Athena

и запустить модуль athena

~$ make programm

~$ make programm-valgrind

# Технические требования

ОС: Ubuntu 20.04 LTS

Необходимое обеспечение:

Valgrind-3.15.0;

GCC (Ubuntu 9.3.0-10ubuntu2) 9.3.0;

GNU Make 4.2.1
