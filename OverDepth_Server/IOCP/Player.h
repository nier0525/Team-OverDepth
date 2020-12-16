#pragma once
//#include"Vector3D.h"
//#include"Quaternion.h"
//#include"ClientSection.h"

#include "Global.h"

class Player
{
private:
    int Serial_Num;
    int HP;

    Vector3 position;
    Quaternion rotation;
public:
    Player();
    ~Player();

    void Set_SerialNum(int num) { Serial_Num = num; }
    int Get_SerialNum() { return Serial_Num; }

    void PlusSerumHP(int _hp) 
    {
        HP += _hp; 
    }
    void OnHitDamage(int _hp) 
    {
        HP -= _hp;
    }

    void Set_HP(int _hp) { HP = _hp; }
    int Get_HP() { return HP; }

    void Set_Position(Vector3 pos) { position = pos; }
    void Set_Quaternion(Quaternion rot) { rotation = rot; }
    Vector3 Get_Position() { return position; }
    Quaternion Get_Quaternion() { return rotation; }
};

/*
class Player
{
public:
    Player();
    ~Player() = default;
    void Update_Character_Vector3D(bool* _input);
    Vector3D* Get_Position();
    void Set_Position(Vector3D _pos);
    Quaternion* Get_Rotation();
    void Set_Rotation(Quaternion* _rot);
    void Set_Rotation(Quaternion _rot);
    int  Get_Serial_num();
    void Set_Serial_num(int _num);
    Vector3D Character_Forward(float rotVertical, Vector3D m_CamForward, float rotHorizontal, Vector3D Cam_right);
    void Character_Rotation(float _mouseVertical, float _mouseHorizontal)
    {
        verticalRotation += _mouseVertical * sensitivity * 0.0333f;
        verticalRotation = Mathf_Clamp(verticalRotation, -clampAngle, clampAngle);
        // https://hvstudy.tistory.com/7
        horizontalRotation += _mouseHorizontal * sensitivity * 0.0333f;

        // ¿ø·¡ ½Ä.
        //verticalRotation += _mouseVertical * sensitivity * Time.deltaTime;
        //horizontalRotation += _mouseHorizontal * sensitivity * Time.deltaTime;

        //verticalRotation = Mathf.Clamp(verticalRotation, -clampAngle, clampAngle);
    }
	float Get_verticalRotation()
	{
        return this->verticalRotation;
	}
	float Get_horizontalRotation()
	{
        return this->horizontalRotation;
	}
public:
    bool* input;
	float moveSpeed;
	float sensitivity = 100.0f;
	float clampAngle = 85.0f;
	float verticalRotation;
	float horizontalRotation;
private:
    int serial_num;
	Vector3D position;
	Quaternion rotation;
    void Move(Vector3D _inputDirection);
    float Mathf_Clamp(float input, float min, float max);

};
*/
