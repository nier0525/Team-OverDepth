#include "Lobby.h"

Lobby::Lobby()
{
    full = false;
    user_list = new vector< CClientSection*>();

    for (int i = 0; i < MAX_USERCOUNT; i++)
        Valve[i] = false;
}

Lobby::~Lobby()
{
    if (user_list != nullptr)
        delete user_list;
}

bool Lobby::Add(CClientSection* ptr)
{
    user_list->push_back(ptr);
    if (user_list->size() == LOBBY_MAXSIZE)
    {
        full = true;
        return false;
    }
    else
    {
        full = false;
        return true;
    }  
}

bool Lobby::Remove(CClientSection* ptr)
{
    for (auto it = user_list->begin(); it != user_list->end(); ++it)
    {
        if (*it == ptr)
        {
            user_list->erase(it);
            return true;
        }
    }
    return false;
}

bool Lobby::Search(CClientSection* ptr)
{
    for (auto it = user_list->begin(); it != user_list->end(); ++it)
    {
        if (*it == ptr)
        {
            return true;
        }
    }
    return false;
}

void Lobby::ValveComplete(int index)
{
    Valve[index - 1] = true;
}

bool Lobby::ValveAllComplete()
{
    for (int i = 0; i < MAX_USERCOUNT; i++)
    {
        if (!Valve[i])
            return false;
    }

    return true;
}

int Lobby::Get_User_Count()
{
    return user_list->size();
}

bool Lobby::Check_Full()
{
    return this->full;
}

vector<CClientSection*>* Lobby::Get_User_List()
{
    return this->user_list;
}
