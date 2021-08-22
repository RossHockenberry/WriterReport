#ifndef MYFLAG_H
#define MYFLAG_H


class myflag
{
public:

    myflag(bool bInitial = false);
    ~myflag();

    bool        Is() { SetTrue(); return bStateFlag; }
    bool        SetTrue() { bStateFlag = true; return true; }
    bool        SetFalse() { bStateFlag = false; return false; }
    bool        Set(bool bFlag) { bStateFlag = bFlag; return bFlag; }

private:
    bool        bStateFlag = false;

};

#endif // MYFLAG_H
