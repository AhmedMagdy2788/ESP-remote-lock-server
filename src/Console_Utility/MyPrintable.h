#ifndef MYPRINTABLE_H_
#define MYPRINTABLE_H_

class MyPrintable{
public:
    virtual void print(int data) = 0;
    virtual void println(int data) = 0;
    virtual void print(const char* data) = 0;
    virtual void println(const char* data) = 0;
};

#endif
