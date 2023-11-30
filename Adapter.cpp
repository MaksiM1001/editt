#include "Adapter.h"


WINDOW* Adapter_ncurses::initsrcA(void)
{
	return initscr();
}

int Adapter_ncurses::noechoA(void)
{
	return noecho();
}

int Adapter_ncurses::keypadA(WINDOW* win, _bool b)
{
	return keypad(win, b);
}

int Adapter_ncurses::curs_setA(int i)
{
	return curs_set(i);
}

int Adapter_ncurses::endwinA(void)
{
	return endwin();
}

int Adapter_ncurses::getchA(void)
{

	return getch();
}

int Adapter_ncurses::attroffA(chtype t)
{
	return attroff(t);
}

int Adapter_ncurses::attronA(chtype t)
{
	return attron(t);
}

int Adapter_ncurses::clrtoeolA(void)
{
	return clrtoeol();
}

int Adapter_ncurses::mvprintwA(int i, int j, const char* str, ...)
{
	return mvprintw(i, j, str);
}

int Adapter_ncurses::moveA(int y, int x)
{
	return move(y, x);
}

int Adapter_ncurses::clearA(void)
{
	return clear();
}

int Adapter_ncurses::refreshA(void)
{
	return refresh();
}

int Adapter_ncurses::printwA(const char* str, ...)
{
	return printw(str);
}