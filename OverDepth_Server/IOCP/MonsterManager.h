#pragma once
#include"Global.h"
#include"Player.h"
#include"CProtocol.h"
#include"ClientSection.h"

struct Monster_Value
{
    int HP;
};

class MonsterManager : public CMultiThreadSyns<MonsterManager>
{
private:
	DECLARE_SINGLETONE(MonsterManager)
	MonsterManager();
	~MonsterManager() = default;

	enum class PROTOCOL : unsigned __int64
	{
        M_BEGIN = 0x0000990000000000,
        M_IDLE = 0x0000010000000000,
        M_MOVE = 0x0000020000000000,
        M_ATTACK = 0x0000030000000000,
        M_ONHIT = 0x0000040000000000,
        M_DEATH = 0x0000050000000000,
        M_SET_TARGET = 0x0000060000000000,
        M_SET_SPEED = 0x0000070000000000,
        M_CHANGE_STATE = 0x0000080000000000,
        M_BATTLEINFO = 0x0000090000000000,
        M_HPINFO = 0x0000100000000000,
        M_STONEMOVE = 0x0000110000000000,
        M_FINDSTONE = 0x0000120000000000,
    };
private:
	CProtocol* protocol;
    
    Monster_Value FanaticSwrodInfo[20];
    Monster_Value FanaticMaseInfo[20];
    Monster_Value FanaticGunInfo[20];
    Monster_Value TitanicHydraInfo;

    // Packing

    int Packing(char* buf, int num, int value);
    int Packing(char* buf, int num, float value);

    int Packing(char* buf, int num, int value, int value2);
    int Packing(char* buf, int num, const char* Anim);

    int Packing(char* buf, int num, Vector3 pos, int state, float speed);

    int Packing(char* buf, int num, Vector3 pos, Quaternion rot);
    int Packing(char* buf, int num, Vector3 pos, Quaternion rot, const char* Anim);
    int Packing(char* buf, int num, Vector3 pos, Quaternion rot, int state);
    int Packing(char* buf, int num, Vector3 pos, Quaternion rot, int anim, int hp);
    int Packing(char* buf, int num, Vector3 pos, Quaternion rot, int anim, int anim2, int hp);

    int Packing(char* buf, int num, Vector3 pos, Quaternion rot, float anim);
    int Packing(char* buf, int num, Vector3 pos, Quaternion rot, float _forward, float _right);
    int Packing(char* buf, int num, Vector3 pos, Quaternion rot, float _forward, float _right, float _speed);
    int Packing(char* buf, int num, Vector3 pos, Quaternion rot, float _forward, float _right, int anim);

    // UnPacking

    void UnPacking(const char* buf, int& num);
    void UnPacking(const char* buf, float& num);

    void UnPacking(const char* buf, char* Anim);
    void UnPacking(const char* buf, Vector3& pos, int& state, float& speed);
    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot);    
    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot, char* Anim);
    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot, int& state);
    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot, int& anim, int& hp);
    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot, int& anim, int& anim2, int& hp);

    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot, float& anim);
    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot, float& _forward, float& _right);
    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot, float& _forward, float& _right, float& _speed);
    void UnPacking(const char* buf, Vector3& pos, Quaternion& rot, float& _forward, float& _right, int& anim);

public:
	void Begin();
	void End();

    bool MemenUpdateState(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state);
    bool FanaticSwordUpdateState(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state);
    bool FanaticMaseUpdateState(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state);
    bool FanaticGunUpdateState(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state);
    bool TitanicHydraUpdateState(CClientSection* ptr, unsigned __int64 fullcode, unsigned __int64 state);

};
