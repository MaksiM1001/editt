#ifndef MAIN_H
#define MAIN_H

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <cassert>
#include <vector>
#include <fstream>
#include <Windows.h>
#pragma comment(lib, "pdcurses.lib")
#pragma comment(lib, "MyStringLib.lib")
#include "Adapter.h"
#include "curses.h"
#include "MyString.h"
#include "TextBuffer.h"
#include "Screen.h"
#include "TextEditor.h"
#include "Manager.h"
#define NAVIGATION_MODE 'n'
#define INPUT_MODE 'i'
#define COMMAND_MODE 'c'
#define SEARCH_MODE 's'
#define SEARCH_MODE2 'e'
#define EXIT_MODE 'q'
#define CED_TITLE "Text Editor"
#define CED_VERSION "1.0.0 "
#define CED_AUTHOR "by A.D.Rassvetalova"
#define HELLO_FILENAME "welcome.txt"
#define KEY_ESC 27
#define KEY_CODE_ENTER 10
#define KEY_CODE_BACKSPACE 8
#define KEY_DEL 330
#define KEY_TAB 9
#define KEY_CM 58
#define KEY_PGD 457
#define KEY_PGU 451
#define HELP_INFO1 "I - Go to the beginning of the lineand start entering text.\n\
S - Go to the end of the lineand start typing.\n\
A - Delete the contents of the rowand start typing.\n\
r - Replace one character under the cursor.\n\
/ - Activates search mode until the end of the document.\n\
? - Activates search mode until the start of the document.\n"
#define HELP_INFO2 "PAGE_UP - Raises the cursor one page.\n\
PAGE_DOWN - Moves the cursor down one page.\n\
gg - Moves the cursor to the first page of the text.\n"
#define HELP_INFO3 "G - Moves the cursor to the last page of the text.\n\
^ - Moves the cursor to the start of the line.\n\
$ - Moves the cursor to the end of the line.\n\
w - Move the cursor to the end of a word to the right of the cursor.\n\
b - Move the cursor to the beginning of the word to the left of the cursor.\n\
x - Delete the character after the cursor.\n\
diw - Delete the word under the cursor, including one space on the right.\n"
#define HELP_INFO4 "dd - Cut(copy and delete) the current line.\n\
ys - Copy the current line.\n\
yw - Copy the word under the cursor.\n\
p - Insert after the cursor.\n\
NG - Go to the line with the number N.\n\
o - <filename>	Open the file <filename>.\n\
x - Write to the current fileand close it.\n\
w - Write to the current file.\n\
w <filename> -Write to file <filename>.\n"
#define HELP_INFO5 "q - Exit program.\n\
q!- Exit program without saving current file.\n\
num - Go to the line with the number <num>.\n"


#endif



