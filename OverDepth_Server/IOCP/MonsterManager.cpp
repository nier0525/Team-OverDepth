#include "MonsterManager.h"
IMPLEMENT_SINGLETON(MonsterManager)

MonsterManager::MonsterManager()
{

}

void MonsterManager::Begin()
{
	protocol = CProtocol::GetInstance();
}

void MonsterManager::End()
{

}

bool MonsterManager::MemenUpdateState(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 _state)
{
    CLock lock;
    unsigned __int64 full_code = 0;
    char buf[BUFSIZE];
    int size = 0;

    int num = 0;
    int index = 0;
    int anim = 0;

    Vector3 position = Vector3();
    Quaternion rotation = Quaternion();

    const char* data = ptr->UnPackData();
    UnPacking(data, num);
    UnPacking(data + sizeof(int), index);

    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::MONSTER_STATE);
    protocol->ProtocolMaker(full_code, _state);

    printf("Mermen Die\n");

    // 죽음
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_DEATH))
    {
        UnPacking(data + sizeof(int), position, rotation, anim);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_DEATH);

        auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(num) - 1);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                auto ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                ZeroMemory(buf, sizeof(buf));

                printf("[Mermen State Update] Die\n");

                size = Packing(buf, index, position, rotation, anim);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send()) return false;
            }
        }
    }

    return true;
}

bool MonsterManager::FanaticSwordUpdateState(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 _state)
{
    CLock lock;
    unsigned __int64 full_code = 0;
    char buf[BUFSIZE];
    int size = 0;

    int index = 0;
    int state = 0;
    int anim = 0;

    Vector3 position = Vector3();
    Quaternion rotation = Quaternion();

    const char* data = ptr->UnPackData();
    UnPacking(data, index);

    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::MONSTER_STATE);
    protocol->ProtocolMaker(full_code, _state);

    // 최초 정보 업데이트 ( HP 정보 )
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_BEGIN))
    {
        int hp;
        UnPacking(data + sizeof(int), hp);

        MonsterManager::GetInstance()->FanaticSwrodInfo[index].HP = hp;
        printf("[(%d) Fanatic Sword State Update] Begin\n", index);
        printf("[(%d) Fanatic Sword HP] %d\n", index, hp);
    }

    // 타겟 선정
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_SET_TARGET))
    {
        int nowState, classNum;
        UnPacking(data + sizeof(int), nowState);
        UnPacking(data + (sizeof(int) * 2), classNum);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_SET_TARGET);
        // auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[Fanatic Sword State Update] SetTarget\n");

            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, index, nowState, classNum);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    // 이동
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_MOVE))
    {
        float speed;
        UnPacking(data, position, anim, speed);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_MOVE);
        //auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            //printf("[TitanicHydra State Update] Move\n");

            ZeroMemory(buf, sizeof(buf));

            size = Packing(buf, index, position, anim, speed);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    // 공격
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_ATTACK))
    {
        float v, h;
        UnPacking(data, position, rotation, v, h, anim);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_ATTACK);
        //auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[Fanatic Sword State Update] Attack\n");

            ZeroMemory(buf, sizeof(buf));

            size = Packing(buf, index, position, rotation, v, h, anim);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    // 상태 변경
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_CHANGE_STATE))
    {
        int nowState, classNum;
        UnPacking(data + sizeof(int), nowState);
        UnPacking(data + (sizeof(int) * 2), classNum);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_CHANGE_STATE);
        // auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[Fanatic Sword State Update] Change State\n");

            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, index, nowState, classNum);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    // 피격
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_ONHIT))
    {
        int serial, hp;
        UnPacking(data, serial);
        UnPacking(data + sizeof(int), index);

        int num;
        int damage;
        UnPacking(data + sizeof(int), position, rotation, anim, damage, num);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_ONHIT);

        hp = MonsterManager::GetInstance()->FanaticSwrodInfo[index].HP - damage;
        MonsterManager::GetInstance()->FanaticSwrodInfo[index].HP = hp;

        auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(serial) - 1);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Fanatic Sword State Update] Hit\n");

                size = Packing(buf, index, position, rotation, anim, hp, num);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send()) return false;
            }
        }
    }

    // 죽음
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_DEATH))
    {
        int serial;
        UnPacking(data, serial);
        UnPacking(data + sizeof(int), index);


        UnPacking(data + sizeof(int), position, rotation);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_DEATH);

        auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(serial) - 1);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            //if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            //{
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Fanatic Sword State Update] Die\n");

                size = Packing(buf, index, position, rotation);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send()) return false;
            //}
        }
    }

    return true;
}

bool MonsterManager::FanaticMaseUpdateState(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 _state)
{
    CLock lock;
    unsigned __int64 full_code = 0;
    char buf[BUFSIZE];
    int size = 0;

    int serial = 0;
    int index = 0;
    int state = 0;
    int anim = 0;
    int hp = 0;

    Vector3 position = Vector3();
    Quaternion rotation = Quaternion();

    const char* data = ptr->UnPackData();
    UnPacking(data, index);

    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::MONSTER_STATE);
    protocol->ProtocolMaker(full_code, _state);

    // 최초 정보 업데이트 ( HP 정보 )
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_BEGIN))
    {
        UnPacking(data + sizeof(int), hp);

        MonsterManager::GetInstance()->FanaticMaseInfo[index].HP = hp;
        printf("[(%d) Fanatic Mase State Update] Begin\n", index);
        printf("[(%d) Fanatic Mase HP] %d\n", index, hp);
    }

    // 타겟 선정
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_SET_TARGET))
    {
        int nowState, classNum;
        UnPacking(data + sizeof(int), nowState);
        UnPacking(data + (sizeof(int) * 2), classNum);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_SET_TARGET);
        // auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[Fanatic Mase State Update] SetTarget\n");

            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, index, nowState, classNum);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    // 이동
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_MOVE))
    {
        float speed;
        UnPacking(data, position, anim, speed);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_MOVE);
        //auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            //printf("[Fanatic Mase State Update] Move\n");

            ZeroMemory(buf, sizeof(buf));

            size = Packing(buf, index, position, anim, speed);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    // 공격
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_ATTACK))
    {
        float v, h;
        UnPacking(data, position, rotation, v, h, anim);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_ATTACK);
        //auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[Fanatic Mase State Update] Attack\n");

            ZeroMemory(buf, sizeof(buf));

            size = Packing(buf, index, position, rotation, v, h, anim);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    // 상태 변경
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_CHANGE_STATE))
    {
        int nowState, classNum;
        UnPacking(data + sizeof(int), nowState);
        UnPacking(data + (sizeof(int) * 2), classNum);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_CHANGE_STATE);
        // auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[Fanatic Mase State Update] Change State\n");

            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, index, nowState, classNum);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    // 피격
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_ONHIT))
    {
        UnPacking(data, serial);
        UnPacking(data + sizeof(int), index);

        int num;
        int damage;
        UnPacking(data + sizeof(int), position, rotation, anim, damage, num);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_ONHIT);

        hp = MonsterManager::GetInstance()->FanaticMaseInfo[index].HP - damage;
        MonsterManager::GetInstance()->FanaticMaseInfo[index].HP = hp;

        auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(serial) - 1);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Fanatic Mase State Update] Hit\n");

                size = Packing(buf, index, position, rotation, anim, hp, num);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send()) return false;
            }
        }
    }

    // 죽음
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_DEATH))
    {
        UnPacking(data, serial);
        UnPacking(data + sizeof(int), index);


        UnPacking(data + sizeof(int), position, rotation);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_DEATH);

        auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(serial) - 1);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            //if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            //{
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Fanatic Mase State Update] Die\n");

                size = Packing(buf, index, position, rotation);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send()) return false;
            //}
        }
    }

    return true;
}

bool MonsterManager::FanaticGunUpdateState(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 _state)
{
    CLock lock;
    unsigned __int64 full_code = 0;
    char buf[BUFSIZE];
    int size = 0;

    int serial = 0;
    int index = 0;
    int state = 0;
    int anim = 0;
    int hp = 0;

    Vector3 position = Vector3();
    Quaternion rotation = Quaternion();

    const char* data = ptr->UnPackData();
    UnPacking(data, index);

    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::MONSTER_STATE);
    protocol->ProtocolMaker(full_code, _state);

    // 최초 정보 업데이트 ( HP 정보 )
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_BEGIN))
    {
        UnPacking(data + sizeof(int), hp);

        MonsterManager::GetInstance()->FanaticGunInfo[index].HP = hp;
        printf("[(%d) Fanatic Gunner State Update] Begin\n", index);
        printf("[(%d) Fanatic Gunner HP] %d\n", index, hp);
    }

    // 타겟 선정
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_SET_TARGET))
    {
        int nowState, classNum;
        UnPacking(data + sizeof(int), nowState);
        UnPacking(data + (sizeof(int) * 2), classNum);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_SET_TARGET);
        // auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[Fanatic Gunner State Update] SetTarget\n");

            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, index, nowState, classNum);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    // 이동
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_MOVE))
    {
        float speed;
        UnPacking(data, position, anim, speed);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_MOVE);
        //auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            //printf("[TitanicHydra State Update] Move\n");

            ZeroMemory(buf, sizeof(buf));

            size = Packing(buf, index, position, anim, speed);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    // 공격
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_ATTACK))
    {
        float v, h;
        UnPacking(data, position, rotation, v, h, anim);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_ATTACK);
        //auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[Fanatic Gunner State Update] Attack\n");

            ZeroMemory(buf, sizeof(buf));

            size = Packing(buf, index, position, rotation, v, h, anim);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    // 상태 변경
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_CHANGE_STATE))
    {
        int nowState, classNum;
        UnPacking(data + sizeof(int), nowState);
        UnPacking(data + (sizeof(int) * 2), classNum);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_CHANGE_STATE);
        // auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[Fanatic Gunner State Update] Change State\n");

            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, index, nowState, classNum);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    // 피격
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_ONHIT))
    {
        UnPacking(data, serial);
        UnPacking(data + sizeof(int), index);

        int num;
        int damage;
        UnPacking(data + sizeof(int), position, rotation, num, damage);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_ONHIT);

        hp = MonsterManager::GetInstance()->FanaticGunInfo[index].HP - damage;
        MonsterManager::GetInstance()->FanaticGunInfo[index].HP = hp;

        auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(serial) - 1);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Fanatic Gunner State Update] Hit\n");

                size = Packing(buf, index, position, rotation, num, hp);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send()) return false;
            }
        }
    }

    // 죽음
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_DEATH))
    {
        UnPacking(data, serial);
        UnPacking(data + sizeof(int), index);


        UnPacking(data + sizeof(int), position, rotation);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_DEATH);

        auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(serial) - 1);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            //if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            //{
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[Fanatic Gunner State Update] Die\n");

                size = Packing(buf, index, position, rotation);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send()) return false;
            //}
        }
    }

    return true;
}

bool MonsterManager::TitanicHydraUpdateState(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 _state)
{
    CLock lock;
    unsigned __int64 full_code = 0;
    char buf[BUFSIZE];
    int size = 0;

    int index = 0;
    int state = 0;
    int anim = 0;
    int hp;

    Vector3 position = Vector3();
    Quaternion rotation = Quaternion();

    const char* data = ptr->UnPackData();
    UnPacking(data, index);

    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::MONSTER_STATE);
    protocol->ProtocolMaker(full_code, _state);

    // 최초 정보 업데이트 ( HP 정보 )
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_BEGIN))
    {
        UnPacking(data + sizeof(int), hp);

        MonsterManager::GetInstance()->TitanicHydraInfo.HP = 3000;
        printf("[TitanicHydra State Update] Begin\n");
        printf("[TitanicHydra HP] %d\n", hp);
    }

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_SET_TARGET))
    {
        int nowState, classNum;
        UnPacking(data + sizeof(int), nowState);
        UnPacking(data + (sizeof(int) * 2), classNum);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_SET_TARGET);
        // auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);
        
        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[TitanicHydra State Update] SetTarget\n");

            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, index, nowState, classNum);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_IDLE))
    {

    }

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_MOVE))
    {
        float speed;
        UnPacking(data, position, anim, speed);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_MOVE);
        //auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            //printf("[TitanicHydra State Update] Move\n");

            ZeroMemory(buf, sizeof(buf));

            size = Packing(buf, index, position, anim, speed);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_SET_SPEED))
    {
        float speed = 0;
        UnPacking(data, speed);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_SET_SPEED);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[TitanicHydra State Update] Speed\n");

            ZeroMemory(buf, sizeof(buf));

            size = Packing(buf, index, speed);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_ATTACK))
    {
        float v, h;
        UnPacking(data, position, rotation, v, h, anim);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_ATTACK);
        //auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[TitanicHydra State Update] Attack\n");

            ZeroMemory(buf, sizeof(buf));

            size = Packing(buf, index, position, rotation, v, h, anim);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_CHANGE_STATE))
    {
        int nowState, classNum;
        UnPacking(data + sizeof(int), nowState);
        UnPacking(data + (sizeof(int) * 2), classNum);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_CHANGE_STATE);
        // auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(ptr->Get_RoomInfo()->Get_User_Count()) - 1);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[TitanicHydra State Update] Change State\n");

            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, index, nowState, classNum);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_BATTLEINFO))
    {
        UnPacking(data + sizeof(int), state);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_BATTLEINFO);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[TitanicHydra State Update] Battle Info\n");

            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, index, state);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_ONHIT))
    {
        int serial;
        UnPacking(data, serial);
        UnPacking(data + sizeof(int), index);

        int num;
        int damage;
        UnPacking(data + sizeof(int), position, rotation, num, damage);
        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_ONHIT);

        hp = MonsterManager::GetInstance()->TitanicHydraInfo.HP - damage;
        MonsterManager::GetInstance()->TitanicHydraInfo.HP = hp;

        auto user = ptr->Get_RoomInfo()->Get_User_List()->at(static_cast<__int64>(serial) - 1);

        for (int i = 0; i < user->Get_RoomInfo()->Get_User_Count(); i++)
        {
            if (user->Get_RoomInfo()->Get_User_List()->at(i) != user)
            {
                ZeroMemory(buf, sizeof(buf));
                CClientSection* ohter = user->Get_RoomInfo()->Get_User_List()->at(i);
                printf("[TitanicHydra State Update] Hit\n");

                size = Packing(buf, index, position, rotation, num, hp, serial);
                ohter->PackingData(full_code, buf, size);

                if (!ohter->Send()) return false;
            }
        }
    }

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_STONEMOVE))
    {
        int stone = 0;
        UnPacking(data + sizeof(int), stone);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_STONEMOVE);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[TitanicHydra State Update] Stone Move\n");

            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, index, stone);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_FINDSTONE))
    {
        int find = 0;
        UnPacking(data + sizeof(int), find);

        protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::M_FINDSTONE);

        for (int i = 0; i < ptr->Get_RoomInfo()->Get_User_Count(); i++)
        {
            auto user = ptr->Get_RoomInfo()->Get_User_List()->at(i);
            printf("[TitanicHydra State Update] Find Stone\n");

            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, index, find);
            user->PackingData(full_code, buf, size);

            if (!user->Send()) return false;
        }
    }

    /*
    if (protocol->ProtocolUnpacker(fullcode, NULL, NULL, (unsigned __int64)PROTOCOL::M_DEATH))
    {

    }
    */

    return true;
}

// Packet

// Packing

int MonsterManager::Packing(char* buf, int num, int value)
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

int MonsterManager::Packing(char* buf, int num, float value)
{
    char* ptr = buf;
    int size = 0;

    memcpy(ptr, &num, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &value, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    return size;
}

int MonsterManager::Packing(char* buf, int num, int value, int value2)
{
    char* ptr = buf;
    int size = 0;

    memcpy(ptr, &num, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &value, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &value2, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    return size;
}

int MonsterManager::Packing(char* buf, int num, const char* anim)
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

int MonsterManager::Packing(char* buf, int num, Vector3 pos, int anim, float speed)
{
    char* ptr = buf;
    int size = 0;

    memcpy(ptr, &num, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &pos, sizeof(pos));
    ptr += sizeof(pos);
    size += sizeof(pos);

    memcpy(ptr, &anim, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &speed, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    return size;
}

int MonsterManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot)
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

int MonsterManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot, const char* anim)
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

int MonsterManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot, int anim)
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

    memcpy(ptr, &anim, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    return size;
}

int MonsterManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot, int anim, int hp)
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

    memcpy(ptr, &anim, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &hp, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    return size;
}

int MonsterManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot, int anim, int anim2, int hp)
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

    memcpy(ptr, &anim, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &anim2, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    memcpy(ptr, &hp, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    return size;
}

int MonsterManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot, float anim)
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

int MonsterManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot, float _forward, float _right)
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

int MonsterManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot, float _forward, float _right, float _speed)
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

    memcpy(ptr, &_speed, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    return size;
}

int MonsterManager::Packing(char* buf, int num, Vector3 pos, Quaternion rot, float _forward, float _right, int anim)
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

    memcpy(ptr, &anim, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    return size;
}


// UnPacking

void MonsterManager::UnPacking(const char* buf, int& num)
{
    const char* ptr = buf;

    memcpy(&num, ptr, sizeof(int));
}

void MonsterManager::UnPacking(const char* buf, float& value)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&value, ptr, sizeof(float));
}

void MonsterManager::UnPacking(const char* buf, char* anim)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    int len = 0;
    memcpy(&len, ptr, sizeof(int));
    ptr += sizeof(int);

    memcpy(anim, ptr, len);
}

void MonsterManager::UnPacking(const char* buf, Vector3& pos, int& anim, float& speed)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&pos, ptr, (sizeof(float) * 3));
    ptr += (sizeof(float) * 3);

    memcpy(&anim, ptr, sizeof(int));
    ptr += sizeof(int);

    memcpy(&speed, ptr, sizeof(int));
}

void MonsterManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&pos, ptr, (sizeof(float) * 3));
    ptr += (sizeof(float) * 3);

    memcpy(&rot, ptr, (sizeof(float) * 4));
}

void MonsterManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot, char* anim)
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

void MonsterManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot, int& anim)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&pos, ptr, (sizeof(float) * 3));
    ptr += (sizeof(float) * 3);

    memcpy(&rot, ptr, (sizeof(float) * 4));
    ptr += (sizeof(float) * 4);

    memcpy(&anim, ptr, sizeof(int));
}

void MonsterManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot, int& anim, int& hp)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&pos, ptr, (sizeof(float) * 3));
    ptr += (sizeof(float) * 3);

    memcpy(&rot, ptr, (sizeof(float) * 4));
    ptr += (sizeof(float) * 4);

    memcpy(&anim, ptr, sizeof(int));
    ptr += sizeof(int);

    memcpy(&hp, ptr, sizeof(int));
}

void MonsterManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot, int& anim, int& anim2, int& hp)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&pos, ptr, (sizeof(float) * 3));
    ptr += (sizeof(float) * 3);

    memcpy(&rot, ptr, (sizeof(float) * 4));
    ptr += (sizeof(float) * 4);

    memcpy(&anim, ptr, sizeof(int));
    ptr += sizeof(int);

    memcpy(&anim2, ptr, sizeof(int));
    ptr += sizeof(int);

    memcpy(&hp, ptr, sizeof(int));
}

void MonsterManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot, float& anim)
{
    const char* ptr = buf;
    ptr += sizeof(int);

    memcpy(&pos, ptr, (sizeof(float) * 3));
    ptr += (sizeof(float) * 3);

    memcpy(&rot, ptr, (sizeof(float) * 4));
    ptr += (sizeof(float) * 4);

    memcpy(&anim, ptr, sizeof(float));
}

void MonsterManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot, float& _forward, float& _right)
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

void MonsterManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot, float& _forward, float& _right, float& _speed)
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
    ptr += sizeof(float);

    memcpy(&_speed, ptr, sizeof(float));   
}

void MonsterManager::UnPacking(const char* buf, Vector3& pos, Quaternion& rot, float& _forward, float& _right, int& anim)
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
    ptr += sizeof(float);

    memcpy(&anim, ptr, sizeof(int));
}
