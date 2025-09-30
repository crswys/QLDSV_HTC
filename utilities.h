#define red FOREGROUND_RED
#define green FOREGROUND_GREEN
#define blue FOREGROUND_BLUE
#define white (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define mode FOREGROUND_INTENSITY
#define bg_red BACKGROUND_RED
#define bg_green BACKGROUND_GREEN
#define bg_blue BACKGROUND_BLUE
#define bg_white (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define bg_mode BACKGROUND_INTENSITY


#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

void gotoxy (int x, int y)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(h, coord);
}

/*
    COORD      dwSize;              // Kích thước vùng buffer (số cột, số hàng)
    COORD      dwCursorPosition;    // Vị trí hiện tại của con trỏ
    WORD       wAttributes;         // Thuộc tính màu (foreground, background)
    SMALL_RECT srWindow;            // Vị trí và kích thước "cửa sổ console" đang hiển thị
    COORD      dwMaximumWindowSize; // Kích thước tối đa có thể phóng to của cửa sổ
*/
CONSOLE_SCREEN_BUFFER_INFO getAccConsole()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(h, &csbi);
    return csbi;
}

// typedef struct _CHAR_INFO {
//   union {
//     WCHAR UnicodeChar;    // ký tự Unicode (nếu dùng Unicode)
//     CHAR  AsciiChar;      // ký tự ASCII
//   } Char;
//   WORD Attributes;        // thuộc tính màu sắc (foreground + background)
// } CHAR_INFO;

void clear ()
{
    system("cls");
} 

struct box
{
    int x, y, w, h;
    WORD bg;
    CHAR_INFO* buffer;
    box (int _x, int _y, int _w, int _h, WORD _bg = 0)
    {
        x = _x, y = _y, w = _w, h = _h;
        bg = _bg;
        buffer = new CHAR_INFO [w * h];
    }
    ~box(){ delete[] buffer; }


    void render(HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE))
    {
        SMALL_RECT rect = {(SHORT)x, (SHORT)y, (SHORT)(x + w - 1), (SHORT)(y + h - 1)};
        COORD buffSize = {(SHORT)w, (SHORT)h};
        COORD buffCoord = {(SHORT)0, (SHORT)0};
        WriteConsoleOutput(hand, buffer, buffSize, buffCoord, &rect);
    }
    
    void create ()
    {
        for (int i = 0; i < h*w; i++)
        {
            buffer[i].Char.AsciiChar = ' ';
            buffer[i].Attributes = bg;
        }
    }

    void draw (int dx, int dy, const string content, WORD color_content = white, WORD background = 0)
    {
        int index = dy*w + dx;
        int len = content.size();
        for (int i = 0; i < len; i++)
        {
            buffer[index + i].Attributes = color_content | ( (background > 0) ? background : bg );
            buffer[index + i].Char.AsciiChar = content[i];
        }
    }
};

string itext (size_t s = 0)
{
    string result = "";
    while( true )
    {
        char _char = _getch();
        int num = (int)_char;
        if (_char == '\r')
        {
            cout << endl;
            break;
        }
        else if (result.size() > s){ continue; }
        else if (_char == '\b')
        {
            if ( !result.empty() )
            {    
                cout << "\b \b";
                result.pop_back();
            }
        }
        else if ( num == 32 || ( 65 <= num && num <= 90 ) || ( 97 <= num && num <= 122 ) )
        {
            cout << _char;
            result += _char;
        }
    }
    return result;
}

unsigned int iui ()
{
    string s = "";
    int result = 0;
    while( true )
    {
        char _char = _getch();
        int num = (int)_char;
        if (_char == '\r')
        {
            cout << endl;
            break;
        }
        else if (_char == '\b')
        {
            if ( !s.empty() )
            {    
                cout << "\b \b";
                s.pop_back();
            }
        }
        else if ( 48 <= num && num <= 57 )
        {
            cout << _char;
            s += _char;
        }
    }
    for (char c : s ) result = result * 10 + ((int)c - 48);
    return result;
}

string icode ( size_t s )
{
    string result = "";
    while( true )
    {
        char _char = _getch();
        int num = (int)_char;
        if (_char == '\r')
        {
            cout << endl;
            break;
        }
        else if (_char == '\b')
        {
            if ( !result.empty() )
            {    
                cout << "\b \b";
                result.pop_back();
            }
        }
        else if ( ( result.size() < s ) && ( ( 48 <= num && num <= 57 ) || ( 65 <= num && num <= 90 ) || ( 97 <= num && num <= 122 ) ) )
        {
            cout << _char;
            result += _char;
        }
        else if (num == 45 || num == 95)
        {
            cout << _char;
            result += _char;
        }
    }
    return result;
}

string icodeONumber ( size_t s ) // Code only Number and char '-'
{
    string result = "";
    while( true )
    {
        char _char = _getch();
        int num = (int)_char;
        if (_char == '\r')
        {
            cout << endl;
            break;
        }
        else if (_char == '\b')
        {
            if ( !result.empty() )
            {    
                cout << "\b \b";
                result.pop_back();
            }
        }
        else if ( ( result.size() < s ) && ( ( 48 <= num && num <= 57 )) )
        {
            cout << _char;
            result += _char;
        }
        else if (num == 45 || num == 95)
        {
            cout << _char;
            result += _char;
        }
    }
    return result;
}

void upCharFirst (string &str)
{
    int len = str.size();
    if (len == 0) return;
    str[0] = towupper(str[0]);
    for (int i = 1; i<len; i++)
    {
        str[i] = (str[i-1] == ' ') ? towupper(str[i]) : tolower(str[i]);
    }
}