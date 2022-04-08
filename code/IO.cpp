#include<bits/stdc++.h>
using namespace std;

class Istream {
    static const int BUFF_SIZE = 10000000;
    FILE *F;
    char bf[BUFF_SIZE], *p1, *p2;

public:
    Istream(FILE *_F = stdin) : F(_F), p1(bf), p2(bf) {}
    Istream(string f) : F(fopen(f.c_str(), "r")), p1(bf), p2(bf) {}
    ~Istream() {fclose(F);}

    int getChar() {
        return p1 == p2 && (p2 = (p1 = bf) + fread(bf, sizeof(char), BUFF_SIZE, F)) == bf ? 
            EOF : 
            static_cast<int>(*p1++);
    }
    template<typename T = int>
    T getNumber() {
        int f = 1, c;
        T x = 0;
        while(!isdigit(c = getChar()))
            if(c == '-') f = -1;
        while(isdigit(c))
            x = x * 10 + c - '0', c = getChar();
        return x * f;
    }
    
    Istream& operator>> (char &c) {
        while(isspace(c = static_cast<char>(getChar())));
        return *this;
    }
    template<typename T>
    Istream& operator>> (T &x) {
        x = getNumber<T>();
        return *this;
    }
};

class Ostream {
    const static int BUFF_SIZE = 5000000;
    FILE *F;
    char bf[BUFF_SIZE], *p1, *const p2;
    
public:
    Ostream(FILE *_F = stdout) : F(_F), p1(bf), p2(bf + BUFF_SIZE) {}
    Ostream(string f) : F(fopen(f.c_str(), "w")), p1(bf), p2(bf + BUFF_SIZE) {}
    ~Ostream() {flush(), fclose(F);}
    
    void putchar(int x) {
        *p1++ = static_cast<char>(x);
        if(p1 == p2) flush();
    }
    Ostream& operator<< (char c) {return putchar(c), *this;}
    template<typename T>
    Ostream& operator<< (T x) {
        static char dig[45], *top = dig;
        if(x < 0) putchar('-'), x = -x;
        do *top++ = static_cast<char>('0' + x % 10);
        while(x /= 10);
        while(top != dig) putchar(*--top);
        return *this;
    }
    Ostream& operator<< (const char *s) {
        while(*s) putchar(*s++);
        return *this;
    }
    void flush() {fwrite(bf, sizeof(char), p1 - bf, F), p1 = bf;}
};
