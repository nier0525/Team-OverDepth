#include "Crypt.h"

const INT C1 = 52845;
const INT C2 = 22719;
const INT KEY = 78695;

CCrypt::CCrypt(void)
{
}



CCrypt::~CCrypt(void)
{

}



BOOL CCrypt::Encrypt(BYTE* source, BYTE* destination, DWORD length)
{
    DWORD i;
    INT key = KEY;

    if (!source || !destination || length <= 0)
    {

        return FALSE;

    }


    for (i = 0; i < length; i++)
    {

        destination[i] = source[i] ^ key >> 8;

        key = (destination[i] + key) * C1 + C2;

    }
    return TRUE;
}



BOOL CCrypt::Decrypt(BYTE* source, BYTE* destination, DWORD length)
{

    DWORD i;

    BYTE previousBlock;

    INT key = KEY;



    if (!source || !destination || length <= 0)
    {

        return FALSE;

    }



    for (i = 0; i < length; i++)

    {

        previousBlock = source[i];

        destination[i] = source[i] ^ key >> 8;

        key = (previousBlock + key) * C1 + C2;

    }

    return TRUE;



}
