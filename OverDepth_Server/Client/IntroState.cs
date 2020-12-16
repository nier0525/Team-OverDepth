using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TCP;
using System.Net.Sockets;

public class IntroState : State
{
    public void Intro_Message()
    {
        try
        {
            UInt64 Protocol = (UInt64)CLASS_STATE.INIT_STATE;        
            Debug.Log(string.Format("temp = {0:x}", Protocol));

            TCPClient.Instance.PackingData(Protocol);
            //TCPClient.Instance.SetState(TCPClient.m_Login);
        }
        catch(SocketException e)
        {
            print("Socket Error : " + e.Message.ToString());
        }
    }

    public override void RecvProcess()
    {

    }
}
