using System;
using System.Collections;
using System.Collections.Generic;

using UnityEngine.SceneManagement;
using System.Text;
using TCP;
using UnityEngine;

public class LoginState : State
{ 
    public LoginState()
    {
        m_SubState = SUBSTATE.LOGININFO;
    }

    enum SUBSTATE : int
    {
        LOGININFO,
    }
    public int id;
    SUBSTATE m_SubState;

    [HideInInspector] public bool Login_Error;
    [HideInInspector] public bool Overlap_Error = true;

    byte[] PackingData(string _id)
    {
        int size = _id.Length + (sizeof(int));
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(_id.Length), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(Encoding.Default.GetBytes(_id.ToCharArray()), 0, data.buffer, data.offset, _id.Length);
        data.size += _id.Length;
        data.offset += _id.Length;

        return data.buffer;
    }

    byte[] PackingData(string _id, string _pw)
    {
        int size = _id.Length + _pw.Length + (sizeof(int) * 2);
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(_id.Length), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(Encoding.Default.GetBytes(_id.ToCharArray()), 0, data.buffer, data.offset, _id.Length);
        data.size += _id.Length;
        data.offset += _id.Length;

        Buffer.BlockCopy(BitConverter.GetBytes(_pw.Length), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(Encoding.Default.GetBytes(_pw.ToCharArray()), 0, data.buffer, data.offset, _pw.Length);
        data.size += _pw.Length;

        return data.buffer;
    }

    byte[] PackingData(string _id, string _pw, string name)
    {
        int size = _id.Length + _pw.Length  + name.Length + (sizeof(int) * 3);
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(_id.Length), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(Encoding.Default.GetBytes(_id.ToCharArray()), 0, data.buffer, data.offset, _id.Length);
        data.size += _id.Length;
        data.offset += _id.Length;

        Buffer.BlockCopy(BitConverter.GetBytes(_pw.Length), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(Encoding.Default.GetBytes(_pw.ToCharArray()), 0, data.buffer, data.offset, _pw.Length);
        data.size += _pw.Length;
        data.offset += _pw.Length;

        Buffer.BlockCopy(BitConverter.GetBytes(name.Length), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(Encoding.Default.GetBytes(name.ToCharArray()), 0, data.buffer, data.offset, name.Length);
        data.size += name.Length;

        return data.buffer;
    }

    void UnPackingData(RecvBuffer buffer, out int _result, out string _msg)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        _result = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        int len = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        _msg = Encoding.Default.GetString(data.buffer, data.offset, len);
    }

    public void Register_OverlapCheck(string id)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.LOGIN_STATE | (UInt64)STATE.MENU_SELECT_STATE | (UInt64)PROTOCOL.JOIN_OVERLAP;

        TCPClient.Instance.PackingData(Protocol, PackingData(id));
    }

    public void Register_Info(string id, string pw, string name)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.LOGIN_STATE | (UInt64)STATE.MENU_SELECT_STATE | (UInt64)PROTOCOL.JOIN_INFO;

        TCPClient.Instance.PackingData(Protocol, PackingData(id, pw, name));
    }

    public void Login_Info(string id, string pw)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.LOGIN_STATE | (UInt64)STATE.MENU_SELECT_STATE | (UInt64)PROTOCOL.LOGIN_INFO;

        TCPClient.Instance.PackingData(Protocol, PackingData(id, pw));
    }

    public void LogOut()
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.LOGIN_STATE | (UInt64)STATE.MENU_SELECT_STATE | (UInt64)PROTOCOL.LOGOUT;
        TCPClient.Instance.PackingData(Protocol);
    }

    public override void RecvProcess()
    {
        UInt64 Protocol = TCPClient.Instance.GetProtocol() & (UInt64)FULL_CODE.PROTOCOL;
        Debug.Log(string.Format("temp = {0:x}", Protocol));

        int result;
        string msg;

        switch((PROTOCOL)Protocol)
        {
            case PROTOCOL.JOIN_OVERLAPRESULT:
                UnPackingData(TCPClient.Instance.UnPackingData(), out result, out msg);
                if (result == (int)RESULT.ERROR_JOIN_EXISTS)
                {
                    RegisterManager.Instance.Register_Error("ID 중복 확인", "해당 ID 는 사용 중인 ID 입니다.");
                    Overlap_Error = true;
                }
                else
                {
                    RegisterManager.Instance.Overlap_Success();
                    Overlap_Error = false;   
                }
                break;

            case PROTOCOL.JOIN_RESULT:
                UnPackingData(TCPClient.Instance.UnPackingData(), out result, out msg);
                if (result == (int)RESULT.JOIN_SUCCESS)
                {
                    RegisterManager.Instance.m_Success.ModalWindowIn();
                    print("Register OK");
                }
                else
                {
                    RegisterManager.Instance.Register_Error("계정 생성 실패" ,"서버에서 오류가 발생하였습니다.");
                    print("Register Error");
                }
                print(msg);
                break;

            case PROTOCOL.LOGIN_RESULT:
                var m_login = GameObject.Find("Manager").GetComponent<LoginManager>();
                UnPackingData(TCPClient.Instance.UnPackingData(), out result, out msg);
                if (result == (int)RESULT.LOGIN_SUCCESS)
                {
                    Static_Data.m_id = m_login.m_IDField.text;
                    Static_Data.m_name = msg;

                    m_login.m_IDField.text = string.Empty;
                    m_login.m_PWField.text = string.Empty;

                    TitleManager.Instance.Login();

                    //TCPClient.Instance.SetState(TCPClient.m_Match);
                }
                else
                {
                    Login_Error = true;

                    if (result == (int)RESULT.ERROR_LOGIN_EXISTS)
                        LoginManager.Instance.Login_Error("이미 접속 중인 계정입니다.");
                    else
                        LoginManager.Instance.Login_Error("없는 계정이거나, PassWord 가 일치하지 않습니다.");

                }
                break;
        }
    }
}
