#pragma once
#include "ClientSection.h"
#include<vector>

#define MAX_USERCOUNT 2

class Lobby
{
private:
    vector< CClientSection*>* user_list;
    bool Valve[MAX_USERCOUNT];
    bool full;
public:
    Lobby();
    ~Lobby();

    bool Add(CClientSection* ptr);
    bool Remove(CClientSection* ptr);
    bool Search(CClientSection* ptr);

    void ValveComplete(int index);
    bool ValveAllComplete();

    void RoomEmpty() { full = false; }
    int Get_User_Count();
    bool Check_Full();
    vector< CClientSection*>* Get_User_List();
};

