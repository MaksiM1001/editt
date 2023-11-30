#include "Main.h"

Adapter_ncurses adt;

int check_input = 0;
int check_input1 = 0;
int ch_in = 0;
TextEditor::TextEditor(void)
{
	mode_ = NAVIGATION_MODE;
	status_ = "";
	clipboard_ = "";
	searchbuf_ = "";
	upstatus = true;
	filename_ = "";
	lowestbound_ = 0;
	x_ = 0;
	y_ = 0;
	openFile(HELLO_FILENAME);
}


bool TextEditor::openFile(MyString filename)
{
	filename_ = filename;
	textbuf_.clear();

	std::ifstream input_file(filename_.c_str());

	if (input_file.is_open())
	{

		while (!input_file.eof())
		{
			MyString temp;
			input_file >> temp;
			if (temp.length() < COLS)
			{
				textbuf_.appendString(temp);
			}
			else
			{
				for (int i = 0; i < temp.length() / COLS; i++)
				{
					MyString ins = temp.substr(i*COLS, COLS);
					textbuf_.appendString(ins);
				}
				textbuf_.appendString(temp.substr(temp.length() - temp.length() % COLS,temp.length() % COLS));
			}
			temp.clear();
		}

		setCursor(0, 0);
		//getScreen().printStatusBar(status_);
		input_file.close();
		return true;
	}
	else
	{
		console_.append("\nFile does not exist. press 'Enter' to continue");
		setMode(COMMAND_MODE);
		input_file.close();
		return false;
	}
	
	
}


void TextEditor::setCursor(int x, int y)
{
	x_ = x;
	y_ = y;
}

void TextEditor::setMode(char m)
{
	mode_ = m;
}

MyString TextEditor::getStatus(void) const
{
	return status_;
}

void TextEditor::updateStatus(void)//статус
{
	status_.clear();
	status_.shrink_to_fit();
	char var = getMode();

	if(var == NAVIGATION_MODE)
	{
		status_ = "Navigation mode\t\t";
		status_.append(CED_TITLE);
		status_.append(CED_VERSION);
		status_.append(CED_AUTHOR);
		status_.append("\t\t");
	}
	else if(var == INPUT_MODE)
	{
		status_ = "Input mode";
	}
	else if (var == SEARCH_MODE|| var == SEARCH_MODE2)
	{
		status_ = "Search mode";
	}
	else if(var == COMMAND_MODE)
	{
		status_ = "Command mode";
	}
	else if (var == EXIT_MODE)
	{
		status_ = "Shutting down...";
	}

	status_.append("\tCOL: ");
	status_.append(std::to_string(getX()+1));
	status_.append("\tROW: ");
	status_.append(std::to_string(getY() + getLowestBound() + 1));
	upstatus = false;
}

char TextEditor::getMode(void) const
{
	return mode_;
}

int TextEditor::getX(void) const
{
	return x_;
}

int TextEditor::getY(void) const
{
	return y_;
}

Screen TextEditor::getScreen(void) const
{
	return screen_;
}

int TextEditor::getSymbol(void) const
{
	
	return getScreen().getSymbol();
}

int TextEditor::getLowestBound(void) const
{
	return lowestbound_;
}


MyString TextEditor::getClipboard(void) const
{
	return clipboard_;
}

TextBuffer TextEditor::getTextBuffer(void) const
{
	return textbuf_;
}

int TextEditor::getTexbuffer_size()
{
	return textbuf_.getBufferSize();
}

void TextEditor::Clean_buf()
{
	textbuf_.clear();
}

void TextEditor::printScreen(void)//экран
{
	MyString st = status_;
	
	if (getMode() == COMMAND_MODE)
	{
		adt.clearA();///очищаем
		getScreen().printStatusBar(status_);//строка состояния 
		getScreen().printConsole(console_);
	}
	else///оставляем что уже есть
	{
		//getScreen().printStatusBar(status_);
		getScreen().printBuf(getTextBuffer().getStrings(), getLowestBound(), getY(), getY());
		getScreen().printStatusBar(status_);
		adt.moveA(getY(), getX());
	}
}



void TextEditor::deleteWord(void)
{

l1:
	if (textbuf_[getY() + getLowestBound()][getX()] == ' ' && getY() + getLowestBound() < textbuf_.getBufferSize())
	{
	l3:
		int tmp = textbuf_[getY() + getLowestBound()].find(" ", getX() + 1);
		if (tmp > 0)
		{
			textbuf_.erase(getY() + getLowestBound(), getX(), tmp - getX());
		}
		else
		{
			textbuf_.erase(getY() + getLowestBound(), getX(), textbuf_[getY() + getLowestBound()].length() - getX());
		}
	}
	else if (textbuf_[getY() + getLowestBound()][getX()] == '\0' && getY() + getLowestBound() < textbuf_.getBufferSize() - 1)
	{
		int tmp = textbuf_[getY() + getLowestBound() + 1].find(" ");
		if (tmp > 0)
		{
			textbuf_.appendToString(getY() + getLowestBound(), " ");
			textbuf_.appendToString(getY() + getLowestBound(), textbuf_[getY() + getLowestBound() + 1].substr(0, tmp));
			textbuf_.erase(getY() + getLowestBound() + 1, 0, tmp + 1);
		}
		else
		{
			textbuf_.removeString(getY() + getLowestBound() + 1);
		}
	}
	else
	{
		if (getY() + getLowestBound() >= textbuf_.getBufferSize() || textbuf_.getBufferSize() == 0)
		{
			goto l2;
		}
		else
		{
			if (getX() == 0 && getY() == 0)
			{

				goto l3;
			}
			else
			{
				moveLeft();
				goto l1;
			}
		}
	}

l2:
	return;

}

bool TextEditor::writeFile()//запись в файл
{
	bool f = true;

	std::ofstream tmp;
	if (console_.length() > 2 && console_[1] == ' ')
	{
		tmp.open((console_.substr(2)).c_str());
		f = tmp.is_open();
	}
	else if (console_.length() == 1)
	{
		tmp.open(filename_.c_str());
		f = tmp.is_open();
	}
	else
	{
		f = false;
	}

	if (f)
	{
		for (int i = 0; i < textbuf_.getBufferSize(); i++)
		{
			if (i == textbuf_.getBufferSize() - 1)
			{
				tmp << textbuf_[i];
			}
			else
			{
				tmp << textbuf_[i] << std::endl;
			}
		}
		;
		setMode(NAVIGATION_MODE);
	}
	else
	{
		console_.append("\nIncorrect filename");
		console_.append("\nPress any key to continue.");
		adt.clearA();
		printScreen();
		adt.getchA();
		console_.clear();
	}


	return f;
}

void TextEditor::pasteClipboard(void)//вставка
{
	if (getClipboard().length() <= 0)
	{
		return;
	}

	if (getClipboard()[0] == '\t')
	{
		setMode(INPUT_MODE);
		Manager m(*this);
		m.handleInput(KEY_CODE_ENTER,this);
		setMode(NAVIGATION_MODE);
	}

	if (textbuf_.getBufferSize() > 0)
	{
		if (textbuf_[getY() + getLowestBound()][getX()] == '\0' && getX() + getClipboard().length() < COLS)
		{
			moveDown();
			moveStartString();
			textbuf_.appendToString(getY() + getLowestBound(), getClipboard());
			setCursor(getX() + getClipboard().length(), getY());
		}
		else if (textbuf_[getY() + getLowestBound()].length() + getClipboard().length() <= COLS)
		{
			int b = 0;
			if (getX() > 0)
			{
				b++;
			}
			textbuf_.insertToString(getX() + b, getY() + getLowestBound(), clipboard_);
			setCursor(getX() + getClipboard().length() + b, getY());
		}
	}
	else
	{
		textbuf_.appendString(clipboard_);
		moveEndString();
	}

	adt.clearA();
	adt.refreshA();
}


void TextEditor::copyString(void)///копирование строки
{
	clipboard_.clear();
	clipboard_.shrink_to_fit();

	if (textbuf_.getBufferSize() <= 0 || textbuf_[getY() + getLowestBound()] == "")
	{
		clipboard_.append(1, '\t');
	}
	else
	{
		clipboard_ = textbuf_[getY() + getLowestBound()];
	}
}

int TextEditor::getNum(MyString &arr)//string->int
{
	int ret = 0;
	int dec = 1;
	for (int j = arr.length() - 1; j >= 0; j--)
	{
		ret += dec * (int)(arr[j] - '0');
		dec *= 10;
	}

	return ret;
}



void TextEditor::moveLastPage(void)
{
	int cnt = getPageSize();
	int del = textbuf_.getBufferSize() / cnt;
	setCursor(0, 0);
	lowestbound_ = 0;
	while (getY() + getLowestBound() < del * cnt)
	{
		moveDown();
	}
	moveStartString();
}

void TextEditor::moveLeft(void)//влево
{
	if (getX() - 1 >= 0)
	{
		setCursor(getX() - 1, getY());
	}
	else if (getY() > 0)
	{
		moveUp();
		moveEndString();
	}
	
}

void TextEditor::moveRight()//вправо
{
	if (textbuf_.getBufferSize() <= 0)
	{
		return;
	}

	if (getX() + 1 <= textbuf_[getY() + getLowestBound()].length())
	{
		if (getX() + 1 < COLS)
		{
			setCursor(getX() + 1, getY());
		}
		else
		{
			textbuf_.appendString("");
			moveDown();
		}
	}
	else if (getY() + getLowestBound() + 1 < textbuf_.getBufferSize())
	{
		moveDown();
		moveStartString();
	}
}


void TextEditor::moveUp(void)//вверх
{
	if (getY() > 0)
	{
		if (getLowestBound() > 0)
		{
			lowestbound_--;
		}
		else
		{
			setCursor(getX(), getY() - 1);
		}
	}
	else
	{
		return;
	}
	
	if (getX() >= textbuf_[getY() + getLowestBound()].length())
	{
		setCursor(textbuf_[getY() + getLowestBound()].length(), getY());
	}
}


void TextEditor::moveStartWord(void)//в начало слова слева
{
	if (textbuf_.getBufferSize() <= 0)
	{
		return;
	}

	while (textbuf_[getY() + getLowestBound()][getX()] != ' ' && textbuf_[getY() + getLowestBound()][getX()] != '\0')//в начало
	{
		if (getY() == 0 && getX() == 0)
		{
			return;
		}
		moveLeft();
	}
	moveLeft();
	while (textbuf_[getY() + getLowestBound()][getX()] != ' ' && textbuf_[getY() + getLowestBound()][getX()] != '\0')//если был в начале 
	{
		if (getY() == 0 && getX() == 0)
		{
			return;
		}
		moveLeft();
	}
}

void TextEditor::moveEndWord(void)//в конец слова справа
{
	if (getY() + getLowestBound() >= textbuf_.getBufferSize())
	{
		return;
	}

	while (getY() + getLowestBound() < textbuf_.getBufferSize() && textbuf_[getY() + getLowestBound()][getX()] != ' ' && textbuf_[getY() + getLowestBound()][getX()] != '\0')
	{
		moveRight();
	}
	moveRight();
	while (getY() + getLowestBound() < textbuf_.getBufferSize() && textbuf_[getY() + getLowestBound()][getX()] != ' ' && textbuf_[getY() + getLowestBound()][getX()] != '\0')
	{
		moveRight();
	}
}

void TextEditor::moveDown(void)///вниз на одну
{
	getScreen().printStatusBar(status_);
	if (getY() + getLowestBound() + 1 >= textbuf_.getBufferSize())
	{
		return;
	}

	if (getY() + 1 < LINES - 1 && getY() + 1 < textbuf_.getBufferSize())
	{
		setCursor(getX(), getY() + 1);
	}
	else if (getLowestBound() + getY() < textbuf_.getBufferSize())
	{
		lowestbound_++;
	}
	if (getX() >= textbuf_[getY() + getLowestBound()].length())
	{
		setCursor(textbuf_[getY() + getLowestBound()].length(), getY());
	}
	
}

void TextEditor::start_handline()
{
	adt.moveA(getY(), getX());
	upstatus = true;
}

void TextEditor::button_del(void)
{
	if (textbuf_.getBufferSize() <= 0)
	{
		return;
	}
	if ((getX() >= textbuf_[getY() + getLowestBound()].length() - 1) && (getY() + getLowestBound() < textbuf_.getBufferSize() - 1))
	{
		MyString tmp(textbuf_[getY() + getLowestBound() + 1]);
		if (tmp.length() > 0)
		{
			textbuf_.appendToString(getY() + getLowestBound(), tmp);
		}
		textbuf_.removeString(getY() + getLowestBound() + 1);
	}
	else if (getX() < textbuf_[getY() + getLowestBound()].length() - 1)
	{
		textbuf_.erase(getY() + getLowestBound(), getX() + 1, 1);
	}
	
}

void TextEditor::button_backspace(void)
{
	if (getX() == 0 && getY() > 0)
	{
		MyString tmp(textbuf_[getY() + getLowestBound()]);
		if (lowestbound_ == 0)
		{
			setCursor(textbuf_[getY() + getLowestBound() - 1].length(), getY() - 1);
		}
		else
		{
			lowestbound_--;
			setCursor(textbuf_[getY() + getLowestBound()].length(), getY());
		}
		textbuf_.appendToString(getY() + getLowestBound(), tmp);
		textbuf_.removeString(getY() + getLowestBound() + 1);
	}
	else if (getX() > 0)
	{
		textbuf_.deleteSymbol(getX(), getY() + getLowestBound());
		moveLeft();
	}
	
}

void TextEditor::button_enter(void)
{
	if (textbuf_.getBufferSize() <= 0)
	{
		textbuf_.appendString("");
		textbuf_.appendString("");
		moveDown();
		return;
	}
	if (getX() < textbuf_[getY() + getLowestBound()].length())
	{
		MyString tmp(textbuf_[getY() + getLowestBound()].substr(getX(), textbuf_[getY() + getLowestBound()].length() - getX()));
		textbuf_.insertString(tmp, getY() + getLowestBound() + 1);
		textbuf_.erase(getY() + getLowestBound(), getX(), textbuf_[getY() + getLowestBound()].length() - getX());
	}
	else
	{
		textbuf_.appendString("");
	}
	moveDown();
	moveStartString();
	
}

void TextEditor::button_tab(void)
{
	for (int i = 0; i < 4; i++)
	{
		textbuf_.insertSymbol(getX(), getY() + getLowestBound(), ' ');
		moveRight();
	}

}

void TextEditor::button_esc()
{
	console_.clear();
	searchbuf_.clear();
	setMode(NAVIGATION_MODE);
}

void TextEditor::default_input(int input)
{
	textbuf_.insertSymbol(getX(), getY() + getLowestBound(), (const char)input);
	moveRight();
}

void TextEditor::search_down(int input)
{
	searchbuf_.append(1, (const char)input);
	bool found = false;
	int X = getX();
	int Y = getY();
	int lb = getLowestBound();
	if (input != (int)'n') {
		if (searchbuf_[0] == '/')
		{
			searchbuf_.erase(0, 1);

			int pre_y = -1;
			while (getY() + getLowestBound() < textbuf_.getBufferSize() ) {
				//pre_y = getY();
				if (textbuf_[getY() + getLowestBound()].find(searchbuf_.c_str(), getX()) >= 0)
				{
					int found_pos = textbuf_[getY() + getLowestBound()].find(searchbuf_.c_str(), getX());
					found = true;
					while (getX() < found_pos)
					{
						moveRight();
					}
					break;
				}
				moveDown();
				moveStartString();
			}

			searchbuf_.insert(0, "/");
		}

		if (!found)
		{
			setCursor(X, Y);
			lowestbound_ = lb;
			return;

		}
	}
	check_input++;


	if (input == (int)'n') {
		bool found = false;
		int X = getX();
		int Y = getY();
		int lb = getLowestBound();
		moveRight();

		if (searchbuf_[0] == '/')
		{
			searchbuf_.erase(0, 1);
			searchbuf_.erase(searchbuf_.length() - 1, 1);

			int pre_y = -1;
			while (getY() + getLowestBound() < textbuf_.getBufferSize() && pre_y != getY()) {
				pre_y = getY();
				if (textbuf_[getY() + getLowestBound()].find(searchbuf_.c_str(), getX()) >= 0)
				{
					int found_pos = textbuf_[getY() + getLowestBound()].find(searchbuf_.c_str(), getX());
					found = true;
					while (getX() < found_pos)
					{
						moveRight();
					}
					break;
				}
				moveDown();
				moveStartString();
			}

			searchbuf_.insert(0, "/");
		}
		if (!found)
		{
			setCursor(X, Y);
			lowestbound_ = lb;
			return;

		}
	}

}

void TextEditor::search_up(int input)
{
	int count = 0;

	searchbuf_.append(1, (const char)input);

	if (ch_in == 0) {
		int next_symb = getSymbol();
		while (next_symb != KEY_CODE_ENTER) {
			searchbuf_.append(1, (const char)next_symb);
			next_symb = getSymbol();

		}
		ch_in++;
	}

	bool found = false;
	int X = getX();
	int Y = getY();
	int lb = getLowestBound();
	if (input != (int)'N') {
		if (searchbuf_[0] == '?')
		{
			searchbuf_.erase(0, 1);
			setCursor(0, getY());
			lowestbound_ = 0;


			if (count == 0) {
				count++;

				MyString new_buf(textbuf_[getY() + getLowestBound()]);
				MyString new_buf2(textbuf_.slise(0, X, new_buf.data()));

				if (new_buf2.find(searchbuf_.c_str(), getX()) >= 0)
				{
					int found_pos = 0;
					int gen_pos = 0;
					int new_x = 0;
					new_buf2.shrink_to_fit();
					while (found_pos != -1 && new_x != new_buf2.length()) {
						gen_pos = found_pos;
						found_pos = new_buf2.find(searchbuf_.c_str(), new_x);
						new_x++;
					}
					found = true;
					while (getX() < gen_pos)
					{
						moveRight();
					}
					searchbuf_.insert(0, "?");
					return;
				}
				moveUp();
				moveStartString();


			}

			do
			{
				if (textbuf_[getY() + getLowestBound()].find(searchbuf_.c_str(), getX()) >= 0)
				{
					moveEndString();
					int found_pos = 0;
					int gen_pos = 0;
					int new_x = getX();
					while (found_pos != -1 && new_x != textbuf_[getY() + getLowestBound()].length()) {
						gen_pos = found_pos;
						found_pos = textbuf_[getY() + getLowestBound()].find(searchbuf_.c_str(), new_x);
						new_x++;
					}
					found = true;
					while (getX() < gen_pos)
					{
						moveRight();
					}
					break;
				}
				moveUp();
				moveStartString();
			} while (getY() + getLowestBound() <= Y);
			searchbuf_.insert(0, "?");
		}

		if (!found)
		{
			setCursor(X, Y);
			lowestbound_ = lb;
		}
		check_input1++;

	}



	count = 0;
	if (input == (int)'N') {
		bool found = false;
		int X = getX();
		int Y = getY();
		int lb = getLowestBound();
		moveLeft();

		if (searchbuf_[0] == '?')
		{
			searchbuf_.erase(0, 1);
			searchbuf_.erase(searchbuf_.length() - 1, 1);
			setCursor(0, getY());
			lowestbound_ = 0;


			if (count == 0) {
				count++;

				MyString new_buf(textbuf_[getY() + getLowestBound()]);
				MyString new_buf2(textbuf_.slise(0, X, new_buf.data()));

				if (new_buf2.find(searchbuf_.c_str(), getX()) >= 0)
				{
					int found_pos = 0;
					int gen_pos = 0;
					int new_x = getX();
					new_buf2.shrink_to_fit();
					while (found_pos != -1 && new_x != new_buf2.length()) {
						gen_pos = found_pos;
						found_pos = new_buf2.find(searchbuf_.c_str(), new_x);
						new_x++;
					}
					found = true;
					while (getX() < gen_pos)
					{
						moveRight();
					}
					searchbuf_.insert(0, "?");
					return;
				}
				moveUp();
				moveStartString();


			}

			do
			{
				if (textbuf_[getY() + getLowestBound()].find(searchbuf_.c_str(), getX()) >= 0 && getY() > 0)
				{
					moveEndString();
					int found_pos = 0;
					int gen_pos = 0;
					int new_x = getX();
					while (found_pos != -1 && new_x != textbuf_[getY() + getLowestBound()].length()) {
						gen_pos = found_pos;
						found_pos = textbuf_[getY() + getLowestBound()].find(searchbuf_.c_str(), new_x);
						new_x++;
					}
					found = true;
					while (getX() < gen_pos)
					{
						moveRight();
					}
					break;
				}
				moveUp();
				moveStartString();
			} while (getY() + getLowestBound() < Y);
			searchbuf_.insert(0, "?");
		}

		if (!found)
		{
			setCursor(X, Y);
			lowestbound_ = lb;
		}
	}
}

void TextEditor::command_mode_start_command()
{
	adt.clearA();
	Manager n(*this);
	n.processConsole(this);
}

void TextEditor::command_mode_enter(int input)
{
	printScreen();
	console_.append(1, (const char)input);
	adt.moveA(getY(), console_.length());
}

void TextEditor::navig_if_slesh()
{
	check_input = 0;
	searchbuf_.clear();
	searchbuf_.append(1, '/');
	setMode(SEARCH_MODE);
}

void TextEditor::navig_if_qwes()
{
	ch_in = 0;
	check_input1 = 0;
	searchbuf_.clear();
	searchbuf_.append(1, '?');
	setMode(SEARCH_MODE2);
}

void TextEditor::navig_if_def(int input)
{
	console_.append(1, (const char)input);
}

void TextEditor::command_A()
{
	moveStartString();
	textbuf_.nullString(getY() + getLowestBound());
	setMode(INPUT_MODE);
}

void TextEditor::command_r()
{
	if (getY() + getLowestBound() >= textbuf_.getBufferSize())
	{
		return;
	}
	if (getX() < textbuf_[getY() + getLowestBound()].length())
	{
		textbuf_.replaceSymbol(getY() + getLowestBound(), getX(), (const char)getSymbol());
	}
	else
	{
		int next = getSymbol();
		setMode(INPUT_MODE);
		Manager n(*this);
		n.handleInput(KEY_DEL,this);
		n.handleInput(next,this);
		setMode(NAVIGATION_MODE);
	}
}

void TextEditor::command_g()
{
	if (getSymbol() == (int)'g')
	{
		setCursor(0, 0);
		lowestbound_ = 0;
	}
}

void TextEditor::command_d()
{
	if (textbuf_.getBufferSize() <= 0)
	{
		return;
	}

	int next = getSymbol();
	if (next == (int)'d')
	{
		copyString();
		if (textbuf_.getBufferSize() > 0)
		{
			textbuf_.removeString(getY() + getLowestBound());
			moveStartString();
		}
	}
	else if (next == (int)'i')
	{
		if (getSymbol() == (int)'w')
		{
			deleteWord();
		}
	}
	adt.clearA();
	adt.refreshA();
}

void TextEditor::command_G()
{
	if (console_.length() > 0)//NG
	{
		setMode(COMMAND_MODE);
		Manager n(*this);
		n.processConsole(this);
		setMode(NAVIGATION_MODE);
		adt.clearA();
		adt.refreshA();
	}
	else ///G
	{
		moveLastPage();
	}
}

void TextEditor::command_y()
{
	int next = getSymbol();
	if (next == (int)'w')
	{
		copyString();
		isolateWord();
	}
	else
	{
		copyString();
	}
}

void TextEditor::button_pgu()
{
	int cnt = getPageSize();
	if (cnt <= 0)
	{
		return;
	}

	int tmp = (getLowestBound() + getY()) / cnt;
	while (getLowestBound() + getY() > (tmp - 1) * cnt && getY() > 0)
	{
		moveUp();
	}
	moveStartString();
}

void TextEditor::button_pgd()
{
	int cnt = getPageSize();
	if (cnt <= 0)
	{
		return;
	}

	int tmp = (getLowestBound() + getY()) / cnt;
	while (getLowestBound() + getY() < (tmp + 1) * cnt && getLowestBound() + getY() < textbuf_.getBufferSize() - 1)
	{
		moveDown();
		if (getLowestBound() + getY() >= textbuf_.getBufferSize() - 1)
		{
			while (getLowestBound() + getY() > tmp * cnt)
			{
				moveUp();
			}
			break;
		}
	}
	moveStartString();
}

void TextEditor::print_help_info()
{
	adt.clearA();
	adt.printwA(HELP_INFO1);
	adt.printwA(HELP_INFO2);
	adt.printwA(HELP_INFO3);
	adt.printwA(HELP_INFO4);
	adt.printwA(HELP_INFO5);
	adt.printwA("\nPress any key to continue.");
	adt.getchA();
	console_.clear();
}

void TextEditor::write_close()
{
	console_ = "w";
	Manager n(*this);
	n.processConsole(this);

	textbuf_.clear();
	getScreen().printConsole(console_);
	setCursor(0, 0);
}

void TextEditor::command_error()
{


	console_.append("\nUnavailable command!");

	console_.append("\nPress any key to continue.");
	adt.clearA();
	printScreen();
	adt.getchA();
	console_.clear();
}

void TextEditor::show_command_result()
{

	console_.clear();
	if (textbuf_.getBufferSize() > 0)
	{
		adt.moveA(getY() + getLowestBound(), textbuf_[getY() + getLowestBound()].length());
	}
	else
	{
		adt.moveA(0, 0);
	}
}

void TextEditor::move_num_str()
{
	setCursor(0, 0);

	lowestbound_ = 0;
	while (getY() + getLowestBound() < getNum(console_) - 1 && getY() + getLowestBound() + 1 < textbuf_.getBufferSize())
	{
		moveDown();
	}
	moveStartString();
	setMode(NAVIGATION_MODE);
}



void TextEditor::isolateWord(void)/// copy word
{
	if (getClipboard().length() <= 1)
	{
		return;
	}
	else
	{
		int space = getClipboard().find(" ", getX());
		if (space != -1) {
			clipboard_.erase(space, getClipboard().length() - space);
		}
		clipboard_.shrink_to_fit();

		space = 0;
		while (space != -1) {
			space = getClipboard().find(" ");
			clipboard_.erase(0, space+1);
		}
		clipboard_.shrink_to_fit();

	}
}

int TextEditor::getPageSize(void)
{
	int curx = getX();
	int cury = getY();
	int curlb = getLowestBound();
	int cnt = 0;
	setCursor(0, 0);
	lowestbound_ = 0;
	while (getLowestBound() == 0 && getY() + getLowestBound() < textbuf_.getBufferSize() - 1)
	{
		cnt++;
		moveDown();
	}
	setCursor(curx, cury);
	lowestbound_ = curlb;
	return cnt;
}

void TextEditor::moveEndString(void)
{
	setCursor(textbuf_[getY() + getLowestBound()].length(), getY());
}


void TextEditor::moveStartString(void)
{
	setCursor(0, getY());
}