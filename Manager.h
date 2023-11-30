#ifndef MANAGER_H
#define MANAGER_H
///контроллер
#include "Main.h"

class Manager
{
	private:
		TextEditor textEditor_;
	public:
		Manager(TextEditor &editor);
		void handleInput(int input, TextEditor* edit);
		void processConsole(TextEditor* edit, int logic = 0);
		void cursesInit(void);
		void toManage(void);
};





#endif


