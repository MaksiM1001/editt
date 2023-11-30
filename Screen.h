#ifndef SCREEN_H
#define SCREEN_H
//представление 
#include "Main.h"


class Screen
{
	public:
		int getSymbol() const;
		void printStatusBar(MyString &status) const;
		void printConsole(MyString& cmd) const;
		void printBuf(std::vector <MyString> strings, int lowerbound, int x, int y);
		
};


#endif