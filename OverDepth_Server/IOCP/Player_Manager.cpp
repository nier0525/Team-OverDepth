#include "Player_Manager.h"
IMPLEMENT_SINGLETON(PlayerManager)

PlayerManager::PlayerManager()
{

}

void PlayerManager::Begin()
{
    protocol = CProtocol::GetInstance();
}

void PlayerManager::End()
{

}

bool PlayerManager::PlayerMovement(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state)
{
    CLock lock;
    unsigned __int64 full_code = 0;
    char buf[BUFSIZE];
    int size = 0;

    const char* data = ptr->UnPackData();

    int num = 0;
    float anim = .0f;
    float forward = .0f;
    float right = .0f;

    Vector3 position = Vector3();
    Quaternion rotation = Quaternion();
    CClientSection* user = nullptr;

    UnPacking(data, num);
    user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(num) - 1);
    if (user == nullptr)
    {
        printf("[Player Movement] Player Exit\n");
        return true;
    }

    // 플레이어 이동
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::MOVE))
    {
        UnPacking(data, position, rotation, anim);
        user->Getplayer()->Set_Position(position);
        user->Getplayer()->Set_Quaternion(rotation);

        //cout << user->GetUser()->name << " : " << user->Getplayer()->Get_Position().x << " , "
        //    << user->Getplayer()->Get_Position().y << " , " << user->Getplayer()->Get_Position().z << endl;

        protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
        protocol->ProtocolMaker(full_code, state);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::MOVE);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Movement] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, user->Getplayer()->Get_Position(), user->Getplayer()->Get_Quaternion(), anim);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::FOCUS))
    {
        UnPacking(data, position, rotation, forward, right);
        user->Getplayer()->Set_Position(position);
        user->Getplayer()->Set_Quaternion(rotation);

        protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
        protocol->ProtocolMaker(full_code, state);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::FOCUS);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Focus] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, user->Getplayer()->Get_Position(), user->Getplayer()->Get_Quaternion(), forward, right);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    return true;
}

bool PlayerManager::PlayerFocus(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state)
{
    CLock lock;
    unsigned __int64 full_code = 0;
    char buf[BUFSIZE];
    int size = 0;

    int num = 0;
    float forward = .0f;
    float right = .0f;

    Vector3 position = Vector3();
    Quaternion rotation = Quaternion();
    CClientSection* user = nullptr;

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::FOCUS))
    {
        const char* data = ptr->UnPackData();

        UnPacking(data, num);
        user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(num) - 1);
        if (user == nullptr)
        {
            printf("[Player Update] Player Exit\n");
            return true;
        }

        UnPacking(data, position, rotation, forward, right);
        user->Getplayer()->Set_Position(position);
        user->Getplayer()->Set_Quaternion(rotation);

        protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
        protocol->ProtocolMaker(full_code, state);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::FOCUS);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Focus] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, user->Getplayer()->Get_Position(), user->Getplayer()->Get_Quaternion(), forward, right);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

}

bool PlayerManager::PlayerUpdate(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state)
{
    CLock lock;
    unsigned __int64 full_code = 0;
    char buf[BUFSIZE];

    int size = 0;

    int num = 0;
    char anim[MSGSIZE];
    ZeroMemory(anim, sizeof(anim));

    Vector3 position = Vector3();
    Quaternion rotation = Quaternion();

    CClientSection* user = nullptr;

    const char* data = ptr->UnPackData();

    UnPacking(data, num);
    user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(num) - 1);
    if (user == nullptr)
    {
        printf("[Player Update] Player Exit\n");
        return true;
    }

    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
    protocol->ProtocolMaker(full_code, state);

    // 초기값 세팅
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::BEGININFO))
    {
        int hp;
        UnPacking(data, hp, anim);
        printf("Jab : %s\n", anim);
        user->Getplayer()->Set_HP(hp);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::BEGININFO);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Begin Info Update] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, user->GetUser()->name, anim);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    // 회전 값 갱신
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::ROTATION))
    {
        UnPacking(data, rotation);
        user->Getplayer()->Set_Quaternion(rotation);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::BEGININFO);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Rotation Update] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, user->Getplayer()->Get_Quaternion());
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    // 위치, 회전 값 갱신
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::TRANSFROM))
    {
        UnPacking(data, position, rotation);
        user->Getplayer()->Set_Position(position);
        user->Getplayer()->Set_Quaternion(rotation);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::TRANSFROM);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Transfrom Update] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, user->Getplayer()->Get_Position(), user->Getplayer()->Get_Quaternion());
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    // HP 값 갱신
    if(protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::HIT))
    {
        int hp;
        UnPacking(data + sizeof(int), hp);
        user->Getplayer()->OnHitDamage(hp);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::HP);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[HP Update] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, user->Getplayer()->Get_HP());
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    if(protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::SERUM))
    {
        int hp;
        UnPacking(data + sizeof(int), hp);
        user->Getplayer()->Set_HP(hp);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::HP);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[HP Update] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, user->Getplayer()->Get_HP());
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }
}

bool PlayerManager::PlayerAnimation(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state)
{
    CLock lock;
    unsigned __int64 full_code = 0;
    char buf[BUFSIZE];
    int size = 0;

    int num = 0;
    char anim[MSGSIZE];
    ZeroMemory(anim, sizeof(anim));

    Vector3 position = Vector3();
    Quaternion rotation = Quaternion();

    CClientSection* user = nullptr;

    const char* data = ptr->UnPackData();

    UnPacking(data, num);
    user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(num) - 1);
    if (user == nullptr)
    {
        printf("[Player Animation] Player Exit\n");
        return true;
    }

    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
    protocol->ProtocolMaker(full_code, state);

    // 서브 무기 애니메이션
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::GUN))
    {
        UnPacking(data, position, rotation, anim);
        user->Getplayer()->Set_Position(position);
        user->Getplayer()->Set_Quaternion(rotation);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::GUN);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Gun Active] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, position, rotation, anim);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    // 혈청 애니메이션
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::SERUM))
    {
        UnPacking(data, position, rotation, anim);
        user->Getplayer()->Set_Position(position);
        user->Getplayer()->Set_Quaternion(rotation);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::SERUM);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Serum Active] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, position, rotation, anim);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    // 회피기 애니메이션
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::EVADE))
    {
        UnPacking(data, anim);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::EVADE);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Evade Active] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, anim);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    // 기본 공격 애니메이션
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::ATTACK))
    {
        UnPacking(data, anim);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::ATTACK);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Base Attack Active] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, anim);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    // 피격 애니메이션
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::HIT))
    {
        UnPacking(data, anim);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::HIT);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Hit Active] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, anim);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    // 차징 공격 애니메이션
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::CHARGEDOWN))
    {
        UnPacking(data, anim);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::CHARGEDOWN);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Charge Down Active] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, anim);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::CHARGEUP))
    {
        UnPacking(data, anim);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::CHARGEUP);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Charge Up Active] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, anim);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    // 죽음 애니메이션
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::DIE))
    {
        UnPacking(data, anim);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::DIE);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Die Active] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, anim);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

    // 부활 애니메이션
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::RESURRECTION))
    {
        UnPacking(data, position, rotation, anim);
        user->Getplayer()->Set_Position(position);
        user->Getplayer()->Set_Quaternion(rotation);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::RESURRECTION);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            // 클라이언트에서 이미 연산 후 이동처리가 끝났기 때문에 자기 자신에게 이동 정보를 안 보내도 됨
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Resurrection Active] other : %s\n", ohter->GetUser()->name);

                size = Packing(buf, i, position, rotation, anim);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send())
                    return false;
            }
        }
    }

}

// Packing

int PlayerManager::Packing(char* buf, int num, int value)
{
    char* ptr = buf;
    int size = 0;

    memcpy(ptr, &num, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &value, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    return size;
}

int PlayerManager::Packing(char* buf, int num, const char* anim)
{
    char* ptr = buf;
    int size = 0;

    memcpy(ptr, &num, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    int len = strlen(anim);
    memcpy(ptr, &len, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, anim, len);
    ptr += len;
    size += len;

    return size;
}

int PlayerManager::Packing(char* buf, int num, const char* anim, const char* anim2)
{
    char* ptr = buf;
    int size = 0;

    memcpy(ptr, &num, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    int len = strlen(anim);
    memcpy(ptr, &len, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, anim, len);
    ptr += len;
    size += len;

    int len2 = strlen(anim2);
    memcpy(ptr, &len2, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, anim2, len2);
    ptr += len2;
    size += len2;

    return size;
}

int PlayerManager::Packing(char* buf, int num, Quaternion rot)
{
    char* ptr = buf;
    int size = 0;

    memcpy(ptr, &num, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &rot, sizeof(rot));
    ptr += sizeof(rot);
    size += sizeof(rot);

    return size;
}

int PlayerManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot)
{
    char* ptr = buf;
    int size = 0;

    memcpy(ptr, &num, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &pos, sizeof(pos));
    ptr += sizeof(pos);
    size += sizeof(pos);

    memcpy(ptr, &rot, sizeof(rot));
    ptr += sizeof(rot);
    size += sizeof(rot);

    return size;
}

int PlayerManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot, const char* anim)
{
    char* ptr = buf;
    int size = 0;

    memcpy(ptr, &num, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &pos, sizeof(pos));
    ptr += sizeof(pos);
    size += sizeof(pos);

    memcpy(ptr, &rot, sizeof(rot));
    ptr += sizeof(rot);
    size += sizeof(rot);

    int len = strlen(anim);
    memcpy(ptr, &len, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, anim, len);
    ptr += len;
    size += len;

    return size;
}

int PlayerManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot, float anim)
{
    char* ptr = buf;
    int size = 0;

    memcpy(ptr, &num, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &pos, sizeof(pos));
    ptr += sizeof(pos);
    size += sizeof(pos);

    memcpy(ptr, &rot, sizeof(rot));
    ptr += sizeof(rot);
    size += sizeof(rot);

    memcpy(ptr, &anim, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    return size;
}

int PlayerManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot, float _forward, float _right)
{
    char* ptr = buf;
    int size = 0;

    memcpy(ptr, &num, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &pos, sizeof(pos));
    ptr += sizeof(pos);
    size += sizeof(pos);

    memcpy(ptr, &rot, sizeof(rot));
    ptr += sizeof(rot);
    size += sizeof(rot);

    memcpy(ptr, &_forward, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    memcpy(ptr, &_right, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    return size;
}

// UnPacking

void PlayerManager::UnPacking(const char* buf, int& num)
{
    const char* ptr = buf;

    memcpy(&num, ptr, sizeof(int));    
}

void PlayerManager::UnPacking(const char* buf, int& num, char* anim)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&num, ptr, sizeof(int));
    ptr += sizeof(int);

    int len = 0;
    memcpy(&len, ptr, sizeof(int));
    ptr += sizeof(int);

    memcpy(anim, ptr, len);
}

void PlayerManager::UnPacking(const char* buf, char* anim)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    int len = 0;
    memcpy(&len, ptr, sizeof(int));
    ptr += sizeof(int);

    memcpy(anim, ptr, len);
}

void PlayerManager::UnPacking(const char* buf, Quaternion& rot)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&rot, ptr, (sizeof(float) * 4));
}

void PlayerManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&pos, ptr, (sizeof(float) * 3));
    ptr += (sizeof(float) * 3);

    memcpy(&rot, ptr, (sizeof(float) * 4));
}

void PlayerManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot, char* anim)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&pos, ptr, (sizeof(float) * 3));
    ptr += (sizeof(float) * 3);

    memcpy(&rot, ptr, (sizeof(float) * 4));
    ptr += (sizeof(float) * 4);

    int len = 0;
    memcpy(&len, ptr, sizeof(int));
    ptr += sizeof(int);

    memcpy(anim, ptr, len);
}

void PlayerManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot, float& anim)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&pos, ptr, (sizeof(float) * 3));
    ptr += (sizeof(float) * 3);

    memcpy(&rot, ptr, (sizeof(float) * 4));
    ptr += (sizeof(float) * 4);

    memcpy(&anim, ptr, sizeof(float));
}

void PlayerManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot, float& _forward, float& _right)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&pos, ptr, (sizeof(float) * 3));
    ptr += (sizeof(float) * 3);

    memcpy(&rot, ptr, (sizeof(float) * 4));
    ptr += (sizeof(float) * 4);

    memcpy(&_forward, ptr, sizeof(float));
    ptr += sizeof(float);

    memcpy(&_right, ptr, sizeof(float));
}

/*
IMPLEMENT_SINGLETON(Player_Manager)

Player_Manager::Player_Manager()
{
    player_list = nullptr;
    protocol = nullptr;
}
void Player_Manager::Begin()
{
    player_list = new vector< CClientSection*>();
    protocol = CProtocol::GetInstance();
}
void Player_Manager::End()
{
    player_list->clear();
    if (player_list != nullptr)
    {
        delete player_list;
    }
}

bool Player_Manager::Player_Movement(CClientSection* ptr, unsigned __int64 _protocol)
{
    printf("Player_Movement\n");
    CLock lock;
    int size = 0;
    unsigned __int64 full_code = NULL;
    char buf[BUFSIZE];
    ZeroMemory(buf, sizeof(buf));

    if (CProtocol::GetInstance()->ProtocolUnpacker(_protocol, NULL, NULL,(unsigned __int64)PROTOCOL::INITROTATION))
    {
        printf("INITROTATION\n");
        UnPacking(ptr->UnPackData(), ptr->Getplayer()->verticalRotation, ptr->Getplayer()->horizontalRotation);
    }
    if (CProtocol::GetInstance()->ProtocolUnpacker(_protocol, NULL, NULL, (unsigned __int64)PROTOCOL::ROTATION))
    {
        float mouse_x = 0.0f;
        float mouse_y = 0.0f;

        printf("ROTATION\n");
        UnPacking(ptr->UnPackData(), mouse_y, mouse_x);
        ptr->Getplayer()->Character_Rotation(-mouse_y, mouse_x);

        protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
        protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::ROTATION);

        for (int i = 0; i < player_list->size(); i++)
        {
            if (player_list->at(i) != nullptr)
            {
                ZeroMemory(buf, sizeof(buf));

                size = Packing(buf, ptr->GetUser()->number, ptr->Getplayer()->verticalRotation, ptr->Getplayer()->horizontalRotation);//내 정보를 패킹
                player_list->at(i)->PackingData(full_code, buf, size);
                if (!player_list->at(i)->Send())//전체에게 보낸다.
                {
                    printf("Player_Rotation_Send_Error\n");
                    return false;
                }
            }
        }

    }
    if (CProtocol::GetInstance()->ProtocolUnpacker(_protocol, NULL, NULL, (unsigned __int64)PROTOCOL::POSITION))
    {
        printf("POSITION\n");

        UnPacking(ptr->UnPackData(), ptr->Getplayer());
        for (int i = 0; i < 4; i++)
        {
            printf("player->input[%d] = %d\n", i, ptr->Getplayer()->input[i]);
        }
        printf("W = %f\n", ptr->Getplayer()->Get_Rotation()->w);
        printf("X = %f\n", ptr->Getplayer()->Get_Rotation()->x);
        printf("Y = %f\n", ptr->Getplayer()->Get_Rotation()->y);
        printf("Z = %f\n", ptr->Getplayer()->Get_Rotation()->z);
        ptr->Getplayer()->Update_Character_Vector3D(ptr->Getplayer()->input);

        protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
        protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::POSITION);

        for (int i = 0; i < player_list->size(); i++)
        {
            if (player_list->at(i) != nullptr)
            {
                ZeroMemory(buf, sizeof(buf));

                size = Packing(buf, ptr->GetUser()->number, ptr->Getplayer()->Get_Position());//내 정보를 패킹
                player_list->at(i)->PackingData(full_code, buf, size);
                if (!player_list->at(i)->Send())//전체에게 보낸다.
                {
                    printf("Player_Movement_Send_Error_1\n");
                    return false;
                }
            }
        }
        //full_code = NULL;
        //protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
        //protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());
        //protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::ROTATION);
        //cout << "ROTATION" << hex << full_code << endl;
        //for (int i = 0; i < player_list->size(); i++)
        //{
        //	if (player_list->at(i) != nullptr)
        //	{
        //		if (player_list->at(i)->Getplayer()->Get_Serial_num() != ptr->Getplayer()->Get_Serial_num())//나를 제외한
        //		{
        //			ZeroMemory(buf, sizeof(buf));
        //			size = Packing(buf, ptr->GetUser()->number, ptr->Getplayer()->Get_Rotation());//다른 사람들의 정보를 패킹
        //			player_list->at(i)->PackingData(full_code, buf, size);
        //			if (!player_list->at(i)->Send())//전체에게 보낸다.
        //			{
        //				printf("Player_Movement_Send_Error_2\n");
        //				return false;
        //			}
        //		}
        //	}
        //}
    }
}

int Player_Manager::Packing(char* buf, int number ,Player* player)
{
    char* ptr = buf;
    int size = 0;

    //id
    memcpy(ptr, &number, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    //vector 3 x
    memcpy(ptr, &player->Get_Position()->x, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    //vector 3 y
    memcpy(ptr, &player->Get_Position()->y, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //vector 3 z
    memcpy(ptr, &player->Get_Position()->z, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //Quaternion w
    memcpy(ptr, &player->Get_Rotation()->w, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //Quaternion x
    memcpy(ptr, &player->Get_Rotation()->x, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //vQuaternion y
    memcpy(ptr, &player->Get_Rotation()->y, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //Quaternion z
    memcpy(ptr, &player->Get_Rotation()->z, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    return size;
}

int Player_Manager::Packing(char* buf, int serial, Vector3D* position)
{
    char* ptr = buf;
    int size = 0;

    //serial
    memcpy(ptr, &serial, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);
    //vector 3 x
    memcpy(ptr, &position->x, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    //vector 3 y
    memcpy(ptr, &position->y, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //vector 3 z
    memcpy(ptr, &position->z, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    return size;
}

int Player_Manager::Packing(char* buf, int serial,  Quaternion* rotation)
{
    char* ptr = buf;
    int size = 0;

    //serial
    memcpy(ptr, &serial, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    //Quaternion w
    memcpy(ptr, &rotation->w, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //Quaternion x
    memcpy(ptr, &rotation->x, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //vQuaternion y
    memcpy(ptr, &rotation->y, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //Quaternion z
    memcpy(ptr, &rotation->z, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    return size;
}

int Player_Manager::Packing(char* buf, int serial, float vertical, float horizontal)
{
    char* ptr = buf;
    int size = 0;

    //serial
    memcpy(ptr, &serial, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &vertical, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);


    memcpy(ptr, &horizontal, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    return size;
}

int Player_Manager::UnPacking(const char* buf, float& rotVertical, Vector3D& m_CamForward, float& rotHorizontal, Vector3D& Cam_right)
{
    const char* ptr = buf;

    memcpy(&rotVertical, ptr, sizeof(float));
    ptr += sizeof(float);
    //
    memcpy(&m_CamForward.x, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&m_CamForward.y, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&m_CamForward.z, ptr, sizeof(float));
    ptr += sizeof(float);
    //
    memcpy(&rotHorizontal, ptr, sizeof(float));
    ptr += sizeof(float);
    //
    memcpy(&Cam_right.x, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&Cam_right.y, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&Cam_right.z, ptr, sizeof(float));
    ptr += sizeof(float);


    return 0;
}

void Player_Manager::UnPacking(const char* buf, Player* player)
{
    int bool_size = 0;
    const char* ptr = buf;
    Quaternion rot;
    memcpy(&bool_size, ptr, sizeof(int));
    ptr += sizeof(int);
    player->input = new bool[bool_size];

   // printf("bool_size = %d\n", bool_size);
    for (int i = 0; i < bool_size; i++)
    {
        memcpy(&player->input[i], ptr, sizeof(bool));
        // printf("player->input[%d] = %d\n", i, player->input[i]);
        ptr += sizeof(bool);
    }
    memcpy(&rot.w, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&rot.x, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&rot.y, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&rot.z, ptr, sizeof(float));

    player->Set_Rotation(rot);
}

void Player_Manager::UnPacking(const char* buf, float& x, float& y)
{
    const char* ptr = buf;
   
    memcpy(&x, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&y, ptr, sizeof(float));
}

void Player_Manager::Send_Into_Game(CClientSection* ptr)
{
    CLock lock;
    int size = 0;
    unsigned __int64 full_code = NULL;
    char buf[BUFSIZE];
    ZeroMemory(buf, sizeof(buf));

    printf("Send_Into_Game\n");
    //create_Player obj
    ptr->player_online(new Player());
    player_list->push_back(ptr);

    ptr->Getplayer()->Set_Serial_num(player_list->size());
    ptr->Getplayer()->Set_Position(vect_pos[player_list->size() - 1]);
    ptr->Getplayer()->Set_Rotation(new Quaternion());

    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
    protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());
    protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::SPAWN);

    for (int i = 0; i < player_list->size(); i++)
    {
        if (player_list->at(i) != nullptr)
        {
            if (player_list->at(i)->Getplayer()->Get_Serial_num() != ptr->Getplayer()->Get_Serial_num())//나를 제외한
            {
                printf("1\n");
                ZeroMemory(buf, sizeof(buf));
                size = Packing(buf, player_list->at(i)->GetUser()->number,player_list->at(i)->Getplayer());//다른 사람들의 정보를 패킹
                ptr->PackingData(full_code, buf, size);
                if (!ptr->Send())//나한테 보낸다.
                {
                    printf("Player_Spawn_Send_Error_1\n");
                    return;
                }
            }
        }
    }
    for (int i = 0; i < player_list->size(); i++)
    {
        if (player_list->at(i) != nullptr)
        {
            printf("2\n");
            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, ptr->GetUser()->number, ptr->Getplayer());//나의 대한 정보를 패킹
            player_list->at(i)->PackingData(full_code, buf, size);
            if (!player_list->at(i)->Send())//전체에게 보낸다.
            {
                printf("Player_Spawn_Send_Error_2\n");
                return;
            }
        }
    }
}

void Player_Manager::Remove_From_Game(CClientSection* ptr)
{
    CLock lock;
    for (int i = 0; i < player_list->size(); i++)
    {
        if (player_list->at(i) != nullptr)
        {
            if (player_list->at(i)->Getplayer()->Get_Serial_num() == ptr->Getplayer()->Get_Serial_num())//나를 제외한
            {
                ptr->player_offline(ptr->Getplayer());
                player_list->erase(player_list->begin() + i);
            }
        }
    }
}
void Player_Manager::Character_Forward(CClientSection* ptr)
{
    float rotVertical;
    Vector3D m_CamForward;
    float rotHorizontal;
    Vector3D Cam_right;

    unsigned __int64 full_code = NULL;
    char buf[BUFSIZE];
    ZeroMemory(buf, sizeof(buf));

    ZeroMemory(&m_CamForward, sizeof(m_CamForward));
    ZeroMemory(&Cam_right, sizeof(Cam_right));

    UnPacking(ptr->UnPackData(), rotVertical,m_CamForward,rotHorizontal,Cam_right);

    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
    protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());
    protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::CHARACTERFORWARD);


    Vector3D Rotatedirection = ptr->Getplayer()->Character_Forward(rotVertical, m_CamForward, rotHorizontal, Cam_right);
    int size = Packing(buf, ptr->GetUser()->number, &Rotatedirection);//다른 사람들의 정보를 패킹
    for (int i = 0; i < player_list->size(); i++)
    {
        if (player_list->at(i) != nullptr)
        {
            ZeroMemory(buf, sizeof(buf));
            player_list->at(i)->PackingData(full_code, buf, size);
            if (!player_list->at(i)->Send())//전체에게 보낸다.
            {
                printf("Player_Movement_Send_Error_1\n");
                return;
            }
        }
    }
}

void Player_Manager::Character_Rotation(CClientSection* ptr)
{

}
*/
