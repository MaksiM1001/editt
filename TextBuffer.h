#ifndef TEXTBUFFER_H
#define TEXTBUFFER_H

#include "Main.h"




class TextBuffer
{
	private:
		std::vector<MyString> strings_;
	public:
		std::vector<MyString> getStrings(void) const;
        void insertString(MyString &str, int pos);
        void insertSymbol(int posx, int posy, const char c);
        void replaceSymbol(int posy, int posx, const char c);
        void deleteSymbol(int posx, int posy);
        void erase(int numstr, int pos, int count);
        void appendToString(int numstr, MyString str);
        void insertToString(int posx, int posy, MyString& str);
        void appendString(MyString str);
        void removeString(int);
        void nullString(int numstr);
        MyString convertTabs(MyString &str);
        void clear(void);
        int getBufferSize() const;
        MyString operator[](int i) const;
        char* slise(int i, int j, char* str);
};






#endif
