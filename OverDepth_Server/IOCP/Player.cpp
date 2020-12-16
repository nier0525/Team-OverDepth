#include "Player.h"
#include <math.h>

Player::Player()
{
    Serial_Num = 0;
    HP = 1000;

    position = Vector3();
    rotation = Quaternion();
}

Player::~Player()
{

}

/*
void Player::Move(Vector3D _inputDirection)
{
    Vector3D _forward;
    _forward.x = 2 * (rotation.x * rotation.z + rotation.w * rotation.y);
    _forward.y = 2 * (rotation.y * rotation.z - rotation.w * rotation.x);
    _forward.z = 1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y);

    Vector3D vec_temp;
    //cross_product
    vec_temp.x = _forward.y * 0 - _forward.z * 1;
    vec_temp.y = _forward.z * 0 - _forward.x * 0;
    vec_temp.z = _forward.x * 1 - _forward.y * 0;

    float temp1 = (vec_temp.x * vec_temp.x) + (vec_temp.y * vec_temp.y) + (vec_temp.z * vec_temp.z);
    float temp2 = sqrt(temp1);
    Vector3D _right = vec_temp / temp2;
    //Console.WriteLine(Vector3.Cross(_forward, new Vector3(0, 1, 0)));
    //Vector3 _right = Vector3.Normalize(Vector3.Cross(_forward, new Vector3(0, 1, 0)));

    Vector3D _moveDirection = _right * _inputDirection.x + _forward * _inputDirection.y;
    position += _moveDirection * moveSpeed;
}

float Player::Mathf_Clamp(float input, float min, float max)
{
    if (input > min || input < max)
    {
        return input;
    }
    else if (input < min)
    {
        return min;
    }
    else
    {
        return max;
    }
}

Vector3D Player::Character_Forward(float rotVertical, Vector3D m_CamForward, float rotHorizontal, Vector3D Cam_right)
{
    return ((m_CamForward * rotVertical) + ( Cam_right * rotHorizontal));
}

Player::Player()
{
    moveSpeed = 5.0f / 30.0f;
}

void Player::Update_Character_Vector3D(bool* _input)
{
    Vector3D temp;
    temp.x = 0.0f;
    temp.y = 0.0f;
    temp.z = 0.0f;
    if (input[0])//W
    {   
        temp.y += 1;
    }
    if (input[1])//S
    {
        temp.y -= 1;
    }
    if (input[2])//A
    {
        temp.x += 1;
    }
    if (input[3])//D
    {
        temp.x -= 1;
    }
    Move(temp);
}

Vector3D* Player::Get_Position()
{
    return &this->position;
}

void Player::Set_Position(Vector3D _pos)
{
    this->position.x = _pos.x;
    this->position.y = _pos.y;
    this->position.z = _pos.z;
}

Quaternion* Player::Get_Rotation()
{
    return &this->rotation;
}

void Player::Set_Rotation(Quaternion* _rot)
{
    this->rotation.x = _rot->x;
    this->rotation.y = _rot->y;
    this->rotation.z = _rot->z;
    this->rotation.w = _rot->w;
}

void Player::Set_Rotation(Quaternion _rot)
{
    this->rotation.x = _rot.x;
    this->rotation.y = _rot.y;
    this->rotation.z = _rot.z;
    this->rotation.w = _rot.w;
}

int Player::Get_Serial_num()
{
    return this->serial_num;
}

void Player::Set_Serial_num(int _num)
{
    this->serial_num = _num;
}
*/