using System;
using System.Collections;
using System.Collections.Generic;

using UnityEngine.SceneManagement;
using System.Text;
using TCP;
using UnityEngine;


public class MatchState : State
{
    public byte[] PackingData(int serial)
    {
        int size = sizeof(int) + sizeof(int);
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        return data.buffer;
    }

    void UnPackingData(RecvBuffer buffer, out int _result)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        _result = BitConverter.ToInt32(data.buffer, data.offset);
    }

    void UnPackingData(RecvBuffer buffer, out int _num, int dummey)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);
        data.offset += sizeof(int);

        _num = BitConverter.ToInt32(data.buffer, data.offset);
    }

    void UnPackingData(RecvBuffer buffer, out string _msg)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);
        data.offset += sizeof(int);

        int len = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        _msg = Encoding.Default.GetString(data.buffer, data.offset, len);
    }

    void UnPackingData(RecvBuffer buffer, out int _num, out string _msg)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);
        data.offset += sizeof(int);

        _num = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        int len = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        _msg = Encoding.Default.GetString(data.buffer, data.offset, len);
    }


    public void Match_Message()
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.LOBBY_STATE | (UInt64)STATE.ENTER;

        TCPClient.Instance.PackingData(Protocol);
    }

    public void Match_Leave()
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.LOBBY_STATE | (UInt64)STATE.LEAVE;

        TCPClient.Instance.PackingData(Protocol);
    }

    public void ValveComplete()
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.LOBBY_STATE | (UInt64)STATE.VALVE;

        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number));
    }

    public override void RecvProcess()
    {
        UInt64 Sub = TCPClient.Instance.GetProtocol() & (UInt64)FULL_CODE.SUB;
        UInt64 Protocol = TCPClient.Instance.GetProtocol() & (UInt64)FULL_CODE.PROTOCOL;
        Debug.Log(string.Format("temp = {0:x}", Protocol));

        int result;
        string msg;
        int num;

        RecvBuffer buffer;

        buffer = TCPClient.Instance.UnPackingData();
        UnPackingData(buffer, out result);

        switch ((LOBBY_RESULT)result)
        {
            case LOBBY_RESULT.NOT_READY:
                UnPackingData(buffer, out msg);
                print(msg);
                break;

            case LOBBY_RESULT.READY:
                UnPackingData(buffer, out num, out msg);
                Static_Data.m_number = num;
                Static_Data.m_FixNumber = num;
                print(num);

                var CharacterManager = GameObject.Find("CharacterSelect").GetComponent<CharacterSelectManager>();
                CharacterManager.MatchComplete();
                break;

            case LOBBY_RESULT.UPDATE_NUMBER:
                UnPackingData(buffer, out num, 0);

                print("Update Number : " + num);
                Static_Data.m_number = num;

                var ohter = GameObject.Find("Other");
                ohter.SetActive(false);
                break;

            case LOBBY_RESULT.VALVE_COMPLETE:
                var Player = GameObject.Find("GameManager").GetComponent<GameManager>().ClientPlayer.GetComponent<PlayerControl>();
                try
                {
                    Player.SteamSceneStart();
                }
                catch
                {
                    print("Not Player Control");
                }
                break;
        }

        /*
        switch ((STATE)Sub)
        {
            case STATE.ENTER:
                buffer = TCPClient.Instance.UnPackingData();
                UnPackingData(buffer, out result);

                switch ((LOBBY_RESULT)result)
                {
                    case LOBBY_RESULT.NOT_READY:
                        UnPackingData(buffer, out msg);
                        print(msg);
                        break;

                    case LOBBY_RESULT.READY:
                        UnPackingData(buffer, out num, out msg);
                        print(msg);
                        Static_Data.m_number = num;

                        var CharacterManager = GameObject.Find("CharacterSelect").GetComponent<CharacterSelectManager>();
                        CharacterManager.MatchComplete();
                        break;

                    case LOBBY_RESULT.UPDATE_NUMBER:
                        UnPackingData(buffer, out num, 0);

                        print("Update Number : " + num);
                        Static_Data.m_number = num;
                        break;
                }
                break;

            default:
                buffer = TCPClient.Instance.UnPackingData();
                UnPackingData(buffer, out result);

                switch ((LOBBY_RESULT)result)
                {
                    case LOBBY_RESULT.UPDATE_NUMBER:
                        UnPackingData(buffer, out num, 0);

                        print("Update Number : " + num);
                        Static_Data.m_number = num;
                        break;
                }
                break;
        }
        */
    }
}
