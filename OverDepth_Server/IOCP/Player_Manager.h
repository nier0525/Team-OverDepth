#pragma once
#include"Global.h"
#include"Player.h"
#include"CProtocol.h"
#include"ClientSection.h"

class PlayerManager : public CMultiThreadSyns<PlayerManager>
{
private:
    DECLARE_SINGLETONE(PlayerManager)
    PlayerManager();
    ~PlayerManager() = default;

    enum class PROTOCOL : unsigned __int64
    {        
        MOVE = 0x0000010000000000,
        FOCUS = 0x0000020000000000,

        GUN = 0x0000030000000000,
        HIT = 0x0000040000000000,
        DIE = 0x0000050000000000,
        SERUM = 0x0000060000000000,
        EVADE = 0x0000070000000000,
        ATTACK = 0x0000080000000000,        
        CHARGEDOWN = 0x0000090000000000,
        CHARGEUP = 0x0000100000000000,
        RESURRECTION = 0x0000110000000000,

        TRANSFROM = 0x0000200000000000,
        HP = 0x0000210000000000,
        BEGININFO = 0x0000220000000000,
        ROTATION = 0x0000230000000000,
    };
private:
    CProtocol* protocol;

    // Packing

    int Packing(char* buf, int num, int value);
    int Packing(char* buf, int num, const char* Anim);
    int Packing(char* buf, int num, const char* Anim, const char* Anim2);

    int Packing(char* buf, int num, Quaternion rot);
    int Packing(char* buf, int num, Vector3 pos, Quaternion rot);
    int Packing(char* buf, int num, Vector3 pos, Quaternion rot, const char* Anim);
    int Packing(char* buf, int num, Vector3 pos, Quaternion rot, float anim);
    int Packing(char* buf, int num, Vector3 pos, Quaternion rot, float _forward, float _right);

    // UnPacking

    void UnPacking(const char* buf, int& num);
    void UnPacking(const char* buf, int& num, char* Anim);

    void UnPacking(const char* buf, char* Anim);
    void UnPacking(const char* buf, Quaternion& rot);

    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot);
    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot, char* Anim);
    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot, float& anim);
    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot, float& _forward, float& _right);
public:
    void Begin();
    void End();

    bool PlayerMovement(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state);
    bool PlayerFocus(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state);
    bool PlayerUpdate(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state);
    bool PlayerAnimation(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state);
};

/*
class Player_Manager : public CMultiThreadSyns<Player_Manager>
{
private:
    DECLARE_SINGLETONE(Player_Manager)
    Player_Manager();
    ~Player_Manager() = default;

	vector< CClientSection*>* player_list;
    CProtocol* protocol;

    Vector3D vect_pos[4] =
    {
        {-8.9,-3.56,-56.56},
         {-11,-3.56,-56.56},
        {5,1,1},
        {7,1,1}
    };//기본 시작 좌표

    enum class PROTOCOL : unsigned __int64
    {
        SPAWN =                 0x0000010000000000,
        POSITION =              0x0000020000000000,
        INITROTATION =          0x0000040000000000,
        ROTATION =              0x0000050000000000,
        CHARACTERFORWARD =      0x0000060000000000
    };

public:
    void Begin();
    void End();

    int Packing(char* buf, int number, Player* player);
    int Packing(char* buf, int serial, Vector3D* position);
    int Packing(char* buf, int serial, Quaternion* rotation);
    int Packing(char* buf, int serial, float x, float y);
    void UnPacking(const char* buf, float& x, float& y);
    int UnPacking(const char* buf, float& rotVertical, Vector3D& m_CamForward, float& rotHorizontal, Vector3D& Cam_right);
    void UnPacking(const char* buf, Player* player);

    void Send_Into_Game(CClientSection* ptr);
    bool Player_Movement(CClientSection* ptr, unsigned __int64 _protocol);
    void Remove_From_Game(CClientSection* ptr);
    void Character_Forward(CClientSection* ptr);
    void Character_Rotation(CClientSection* ptr);
};
*/
