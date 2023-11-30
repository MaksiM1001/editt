#include "Main.h"

Adapter_ncurses ads;

int Screen::getSymbol(void) const ///получаем 1 символ из ввода
{
	return ads.getchA();
}

void Screen::printStatusBar(MyString& status) const ///строка состояния 
{ 
    start_color();
    init_pair(1, COLOR_RED, COLOR_GREEN);
    init_color(11, 500, 0, 500);
    ads.attronA(A_REVERSE);
    ads.mvprintwA(LINES-1, 0, status.c_str());
    ads.clrtoeolA();
    ads.attroffA(A_REVERSE);

}

void Screen::printConsole(MyString& cmd) const //печать 
{
    ads.mvprintwA(0, 0, cmd.c_str());
    ads.clrtoeolA();
}

void Screen::printBuf(std::vector<MyString> strings, int lowerbound, int x, int y)///печать по строчечкам
{
    int lineCounter = 0;
    for (size_t i = lowerbound; lineCounter < LINES - 1; i++)
    {
        if (i < strings.size())
        {
            ads.mvprintwA(lineCounter, 0, strings[i].c_str());
        }

        ads.clrtoeolA();
        lineCounter++;
    }
    ads.moveA(y, x);
}