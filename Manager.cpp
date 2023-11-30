#include "Main.h"

Adapter_ncurses adm;
//TextEditor tasks;

Manager::Manager(TextEditor &editor)
{
	textEditor_ = editor;
}


void Manager::cursesInit(void)
{
	adm.noechoA();
	adm.keypadA(stdscr, true);//вкл обработку функц клавиш
	textEditor_.printScreen();//вывод того что на экране в данном моде
	adm.curs_setA(0);
	processConsole(&textEditor_,textEditor_.getSymbol()); //начало


}



void Manager::handleInput(int input, TextEditor* edit)//обработка команд
{
	edit->start_handline();
	if (input == KEY_ESC)
	{
		edit->button_esc();
		return;
	}

	switch (edit->getMode())
	{
	case NAVIGATION_MODE:
	{
		switch (input)
		{
		case (int)'i':
		{
			edit->setMode(INPUT_MODE);
			break;
		}
		case (int)'I':
		{
			edit->moveStartString();
			edit->setMode(INPUT_MODE);
			break;
		}
		case (int)'/':
		{
			edit->navig_if_slesh();
			break;
		}
		case (int)'?':
		{
			edit->navig_if_qwes();
			break;
		}
		case (int)'^':
		{
			edit->moveStartString();
			break;
		}
		case (int)'$':
		{
			edit->moveEndString();
			break;
		}
		case (int)'G':
		{
			edit->command_G();
			break;
		}
		case (int)'y':
		{
			edit->command_y();
			break;
		}
		case (int)'p':
		{
			edit->pasteClipboard();
			break;
		}
		case (int)'d':
		{
			edit->command_d();
			break;
		}
		case (int)'x':
		{
			edit->setMode(INPUT_MODE);
			handleInput(KEY_DEL, &textEditor_);
			edit->setMode(NAVIGATION_MODE);
			break;
		}
		case KEY_PGD:
		{
			edit->button_pgd();
			break;
		}
		case KEY_PGU:
		{
			edit->button_pgu();
			break;
		}
		case (int)'g':
		{
			edit->command_g();
			break;
		}
		case (int)'r':
		{
			edit->command_r();
			break;
		}
		case (int)'S':
		{
			edit->moveEndString();
			edit->setMode(INPUT_MODE);
			break;
		}
		case (int)'A':
		{
			edit->command_A();
			break;
		}
		case (int)'w':
		{
			edit->moveEndWord();
			break;
		}
		case (int)'b':
		{
			edit->moveStartWord();
			break;
		}
		case KEY_CM:
		case 65414:
		{
			edit->setMode(COMMAND_MODE);
			break;
		}
		case KEY_LEFT:
		{
			edit->moveLeft();
			break;
		}
		case KEY_RIGHT:
		{
			edit->moveRight();
			break;
		}
		case KEY_DOWN:
		{
			edit->moveDown();
			break;
		}
		case KEY_UP:
		{
			edit->moveUp();
			break;
		}
		default:
		{
			edit->navig_if_def(input);
			break;
		}
		}

		break;
	}


	case COMMAND_MODE:
	{
		switch (input)
		{
		case KEY_CODE_ENTER:
		{
			edit->command_mode_start_command();
			break;
		}
		default:
		{
			edit->command_mode_enter(input);
			break;
		}
		}
		break;
	}


	case SEARCH_MODE:
	{
		edit->search_down(input);
		break;
	}



	case SEARCH_MODE2:
	{
		edit->search_up(input);
		break;
	}

	case  INPUT_MODE:
	{
		switch (input)
		{
		case KEY_LEFT:
		{
			edit->moveLeft();
			break;
		}
		case KEY_RIGHT:
		{
			edit->moveRight();
			break;
		}
		case KEY_DOWN:
		{
			edit->moveDown();
			break;
		}
		case KEY_UP:
		{
			edit->moveUp();
			break;
		}
		case KEY_DEL:
		{
			edit->button_del();
			break;
		}
		case KEY_CODE_BACKSPACE:
		{
			edit->button_backspace();
			break;
		}
		case KEY_CODE_ENTER:
		{
			edit->button_enter();
			break;
		}
		case KEY_TAB:
		{
			edit->button_tab();
			break;
		}
		default:
		{
			edit->default_input(input);
			break;
		}
		}
		break;
	}
	}
}






void Manager::processConsole(TextEditor* edit, int logic)///обработка команд командного мода
{
	if (logic != 0)
	{
		int wtg = logic;
		while (wtg != KEY_CODE_ENTER)
		{
			wtg = edit->getSymbol();
		}
		edit->Clean_buf();
		adm.clearA();
		adm.curs_setA(1);
		return;
	}

	if (edit->getNum(edit->console_) > 0 && edit->getNum(edit->console_) < edit->getTexbuffer_size() || edit->getNum(edit->console_) == 0)
	{
		edit->move_num_str();
	}
	else if (edit->console_[0] == 'o' && edit->console_[1] == ' ' && (edit->openFile(edit->console_.substr(2))))
	{
		edit->setMode(NAVIGATION_MODE);
	}
	else if (edit->console_[0] == 'w' && !(edit->writeFile()))
	{
		return;
	}
	else if (edit->console_[0] == 'h')
	{
		edit->print_help_info();
		return;
	}
	else if (edit->console_ == "x")
	{
		edit->write_close();
	}
	else if (edit->console_ == "q!")
	{
		edit->setMode(EXIT_MODE);
	}
	else if (edit->console_ == "q")
	{
		edit->writeFile();
		edit->setMode(EXIT_MODE);
	}
	else
	{
		edit->command_error();
		return;
	}
	edit->show_command_result();
}













void Manager::toManage(void)//регулировка работы
{
	cursesInit();
	while (textEditor_.getMode() != EXIT_MODE)
	{
		if (textEditor_.upstatus)
		{
			textEditor_.updateStatus();
		}

		textEditor_.printScreen();
		handleInput(textEditor_.getSymbol(), &textEditor_);
	}
	adm.endwinA();
}