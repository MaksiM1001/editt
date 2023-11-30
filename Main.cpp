#include "Main.h"

Adapter_ncurses ad;

int main()
{
	ad.initsrcA();
	TextEditor execEditor;
	Manager execManager(execEditor);
	execManager.toManage();
	return 0;
}