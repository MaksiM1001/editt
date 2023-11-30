#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
///модель
#include "Main.h"


class TextEditor
{
	private:
		char mode_; //для смены режимов
		int x_, y_; //координаты курсора 
		int lowestbound_;//верхняя граница (для страниц)
		TextBuffer textbuf_;//текст
		Screen screen_;//то что вводится в консоль (работа с консолью)
		MyString searchbuf_;///для поиска
		MyString clipboard_;/// для вставки
		MyString status_;//мод для вывода в строку 
		MyString filename_; //имя файла 
		


	public:
		MyString console_; //ввод в консоль
		bool upstatus;
		TextEditor(void);
		char getMode(void) const;
		int getLowestBound(void) const;
		MyString getStatus(void) const;
		Screen getScreen(void) const;
		TextBuffer getTextBuffer(void) const;
		int getTexbuffer_size();
		void Clean_buf();
		MyString getClipboard(void) const;
		int getNum(MyString &arr);
		int getPageSize(void);
		void pasteClipboard(void);
		bool openFile(MyString filename);
		void printScreen(void);
		bool writeFile();
		int getSymbol(void) const;
		int getX(void) const;
		int getY(void) const;
		void deleteWord(void);
		void isolateWord(void);
		void copyString(void);
		void setMode(char m);
		void setCursor(int x, int y);
		void updateStatus(void);
		void moveEndString(void);
		void moveStartString(void);
		void moveStartWord(void);
		void moveLastPage(void);
		void moveEndWord(void);
		void moveLeft(void);
		void moveUp(void);
		void moveRight(void);
		void moveDown(void);

		void start_handline();

		void button_del(void);
		void button_backspace(void);
		void button_enter(void);
		void button_tab(void);
		void button_esc();

		void default_input(int input);
		void search_down(int input);
		void search_up(int input);
		void command_mode_start_command();
		void command_mode_enter(int input);

		void navig_if_slesh();
		void navig_if_qwes();
		void navig_if_def(int input);
		void command_A();
		void command_r();
		void command_g();
		void command_d();
		void command_G();
		void command_y();

		void button_pgu();
		void button_pgd();

		void print_help_info();
		void write_close();

		void command_error();
		void show_command_result();
		void move_num_str();

		
};



#endif