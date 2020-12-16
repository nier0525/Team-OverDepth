#pragma once
#include"Global.h"

class CProtocol
{
private:
    //1$3 = 일치
    //2&6 = 일치
    //2&7 = 일치 
    //& 연산을 하기 때문에 1이 아닌것은 다 0으로 처리 하기 때문.
    //1, 3, 7, f, 3f, 7f, ff
    DECLARE_SINGLETONE(CProtocol);
    CProtocol() = default;
    ~CProtocol() = default;
    enum class FULL_CODE :unsigned __int64
    {
        MAIN = 0xff00000000000000,
        SUB =  0x00ff000000000000,
        PROTOCOL = 0x0000ff0000000000
    };
public:
    void ProtocolMaker(unsigned __int64& Full_Code, unsigned __int64 input);
    bool ProtocolUnpacker(unsigned __int64 _full_code, unsigned __int64 main, unsigned __int64 sub, unsigned __int64 protocol);
};


