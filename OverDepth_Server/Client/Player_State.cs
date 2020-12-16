using System;
using System.Text;
using TCP;
using UnityEngine;

public class Player_State : State
{
    /*
    // ----------------------------
    // 한빛으로 도망간 자의 흔적
    // ----------------------------
    void UnPackingData(RecvBuffer buffer, out int id, out int serial, out Vector3 position, out Quaternion rotation)
    {

        Debug.Log("UNPACKINHGG");
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        id = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(string.Format("temp = {0:x}", serial));

        position.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.x);

        position.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(position.y);

        position.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.z);

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.w);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.x);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(rotation.y);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        //Debug.Log(rotation.z);
    }
    void UnPackingData(RecvBuffer buffer, out int serial, out Vector3 position)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(string.Format("temp = {0:x}", serial));

        position.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.x);

        position.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.y);

        position.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.z);


    }
    void UnPackingData(RecvBuffer buffer, out int serial, out Quaternion rotation)
    {

        Debug.Log("UNPACKINHGG");
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        Debug.Log(string.Format("temp = {0:x}", serial));

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        Debug.Log(rotation.w);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        Debug.Log(rotation.x);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        Debug.Log(rotation.y);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        Debug.Log(rotation.z);
    }
    byte[] PackingData(bool[] _inputs, Quaternion rotation)
    {

        int size = _inputs.Length + (sizeof(bool) * 4) + (sizeof(float) * 4);
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(_inputs.Length), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        foreach (bool input in _inputs)
        {
            Buffer.BlockCopy(BitConverter.GetBytes(input), 0, data.buffer, data.offset, sizeof(bool));
            data.size += sizeof(bool);
            data.offset += sizeof(bool);
        }
        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);

        return data.buffer;
    }
    // ----------------------------
    */

    public void Player_BeginInfoUpdate(int hp)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.UPDATE | (UInt64)PROTOCOL.BEGININFO;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, hp, Static_Data.m_jab));
    }

    public void Player_TransformUpdate(Vector3 position, Quaternion rotation)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.UPDATE | (UInt64)PROTOCOL.TRANSFROM;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, position, rotation));
    }

    public void Player_HPUpdate(UInt64 PROTOCOL, int HP)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.UPDATE | PROTOCOL;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, HP));
    }

    public void Player_UpdateRotation(Quaternion rotation)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.UPDATE | (UInt64)PROTOCOL.ROTATION;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, rotation));
    }

    public void Player_Movement(Vector3 position, Quaternion rotation, float anim)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.MOVEMENT | (UInt64)PROTOCOL.MOVE;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, position, rotation, anim));
    }

    public void Player_FocusMovement(Vector3 position, Quaternion rotation, float forward, float right)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.MOVEMENT | (UInt64)PROTOCOL.FOCUS;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, position, rotation, forward, right));
    }

    public void Player_Animation_Active(UInt64 _Protocol, string anim)
    {
        var Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.ANIMATION | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, anim));
    }

    public void Player_Animation_Active(UInt64 _Protocol, Vector3 position, Quaternion rotation, string anim)
    {
        var Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.ANIMATION | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, position, rotation, anim));
    }

    public Vector3 prev_position = Vector3.zero;
    public Quaternion prev_rotation = Quaternion.identity;
    public int prev_hp = 0;
    public string nickname;

    public override void RecvProcess()
    {
        UInt64 State = TCPClient.Instance.GetProtocol() & (UInt64)FULL_CODE.SUB;
        UInt64 Protocol = TCPClient.Instance.GetProtocol() & (UInt64)FULL_CODE.PROTOCOL;

        var buffer = TCPClient.Instance.UnPackingData();
        if (buffer.size == 0)
        {
            print("Queue Empty");
            return;
        }

        Debug.Log(string.Format("temp = {0:x}", Protocol));

        int num = 0;
        Vector3 position = Vector3.zero;
        Quaternion rotation = Quaternion.identity;

        string string_anim = string.Empty;
        float float_anim = 0f;
        int value = 0;

        try
        {
            var ohter = GameObject.Find("Other").GetComponent<OtherControl>();

            switch ((STATE)State)
            {
                case STATE.MOVEMENT:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.MOVE:
                            UnPackingData(buffer, out num, out position, out rotation, out float_anim);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_Movement(position, rotation, float_anim);
                            break;

                        case PROTOCOL.FOCUS:
                            print("Focus Move");
                            float forward, right;
                            UnPackingData(buffer, out num, out position, out rotation, out forward, out right);

                            ohter.Other_FocusMovement(position, rotation, forward, right);
                            break;
                    }
                    break;

                case STATE.UPDATE:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.BEGININFO:
                            UnPackingData(buffer, out num, out Static_Data.m_OhterName, out Static_Data.m_OhterJab);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                            {
                                ohter.Other_InfoSetting(Static_Data.m_OhterName);
                            }
                            break;

                        case PROTOCOL.HP:
                            UnPackingData(buffer, out num, out value);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_HP(value);
                            break;

                        case PROTOCOL.TRANSFROM:
                            UnPackingData(buffer, out num, out position, out rotation);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_UpdateTransform(position, rotation);
                            break;
                    }
                    break;

                case STATE.ANIMATION:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.RESURRECTION:
                        case PROTOCOL.GUN:
                        case PROTOCOL.SERUM:
                            UnPackingData(buffer, out num, out position, out rotation, out string_anim);
                            print(string_anim);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_Animation_Active(position, rotation, string_anim);
                            break;

                        case PROTOCOL.HIT:
                            UnPackingData(buffer, out num, out string_anim);
                            print(string_anim);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_Hit(string_anim);
                            break;

                        case PROTOCOL.CHARGEDOWN:
                            UnPackingData(buffer, out num, out string_anim);
                            print(string_anim);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_ChargeDown();
                            break;

                        case PROTOCOL.CHARGEUP:
                            UnPackingData(buffer, out num, out string_anim);
                            print(string_anim);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_ChargeUp();
                            break;

                        case PROTOCOL.EVADE:
                        case PROTOCOL.ATTACK:
                        case PROTOCOL.DIE:
                            UnPackingData(buffer, out num, out string_anim);
                            print(string_anim);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_Animation_Active(string_anim);
                            break;
                    }
                    break;
            }
        }
        catch
        {
            // 아직 플레이어가 로딩 중인 경우
            print("Not Loading Complete");

            switch ((STATE)State)
            {
                case STATE.MOVEMENT:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.MOVE:
                            UnPackingData(buffer, out num, out prev_position, out prev_rotation, out float_anim);
                            break;
                    }
                    break;

                case STATE.UPDATE:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.BEGININFO:
                            UnPackingData(buffer, out num, out Static_Data.m_OhterName, out Static_Data.m_OhterJab);
                            break;

                        case PROTOCOL.TRANSFROM:
                            UnPackingData(buffer, out num, out prev_position, out prev_rotation);
                            break;

                        case PROTOCOL.HP:
                            UnPackingData(buffer, out num, out prev_hp);
                            break;
                    }
                    break;
            }
        }
    }
}
