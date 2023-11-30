#include "Main.h"



std::vector<MyString> TextBuffer::getStrings(void) const//сама строка
{
	return strings_;
}

MyString TextBuffer::convertTabs(MyString &str)//обработка табов 
{
    int pos = str.find("\t");
    if (pos < 0)
    {
        return str;
    }
    else
    {
        str.replace(pos, 1, "    ");
        return convertTabs(str);
    }
}

void TextBuffer::insertString(MyString &str, int pos)//вставка новой строки
{
    str = convertTabs(str);
    strings_.insert(strings_.begin()+pos, str);
}

void TextBuffer::clear(void)//чистим
{
    strings_.clear();
}

void TextBuffer::nullString(int numstr)//чистим определенную
{
    strings_[numstr].clear();
}


void TextBuffer::replaceSymbol(int posy, int posx, const char c)//замена символа
{
    MyString tmp(1, c);
    strings_[posy].replace(posx, 1, tmp.c_str());
}

void TextBuffer::appendString(MyString str)//вставка назад
{
    str = convertTabs(str);
    strings_.push_back(str);
}

void TextBuffer::removeString(int i)//удаление
{
    strings_.erase(strings_.begin() + i);
}

void TextBuffer::insertSymbol(int posx, int posy, const char c) //вставка
{
   if (posy < strings_.size())
   {
            strings_[posy].insert(posx, 1, c);//1 на posx
   }
    else
    {
        MyString tmp(1, c);
        appendString(tmp);
    }
}

void TextBuffer::appendToString(int numstr, MyString str)//добавляем в конец
{
    strings_[numstr].append(str.data());
}

void TextBuffer::insertToString(int posx, int posy, MyString& str)//вставка
{
    strings_[posy].insert(posx, str.c_str());
}

void TextBuffer::erase(int numstr, int pos, int count)//стирание
{
    strings_[numstr].erase(pos, count);
}


void TextBuffer::deleteSymbol(int posx, int posy)//удаление одного
{
    strings_[posy].erase(posx-1, 1);
}

int TextBuffer::getBufferSize(void) const //размер
{
		return strings_.size();
}


MyString TextBuffer::operator[](int i) const //обращение к элементу
{
  return strings_[i];
}

char* TextBuffer::slise(int i, int j, char* str)
{
    MyString s(str, j);
    s.erase(0, i);
    return s.data();
}
