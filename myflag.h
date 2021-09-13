#ifndef MYFLAG_H
#define MYFLAG_H


class MyFlag
{
public:

    MyFlag() {bStateFlag = false; };
    ~MyFlag() {};

    bool        Is() { return bStateFlag; }
    bool        SetTrue() { bStateFlag = true; return true; }
    bool        SetFalse() { bStateFlag = false; return false; }
    bool        Set(bool bFlag) { bStateFlag = bFlag; return bFlag; }
    bool        Flip() { return (Set( Is() ? false : true )); }

private:
    bool        bStateFlag = false;

};

#endif // MYFLAG_H
