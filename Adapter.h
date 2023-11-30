#ifndef ADAPTER_H
#define ADAPTER_H

#include <iostream>
#include "curses.h"

class Adapter_ncurses
{
	public:
		WINDOW* initsrcA(void);
		int noechoA(void);
		int keypadA(WINDOW* win, _bool b);
		int curs_setA(int i);
		int endwinA(void);
		int getchA(void);
		int attronA(chtype t);
		int attroffA(chtype t);
		int clrtoeolA(void);
		int mvprintwA(int i, int j, const char* str, ...);
		int moveA(int y, int x);
		int clearA(void);
		int refreshA(void);
		int printwA(const char*, ...);
};



#endif