using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.Net;
using System.Net.Sockets;

namespace TCP
{
    public class SendBuffer
    {
        public byte[] buffer;
        public int offset;
        public int size;
    }

    public class RecvBuffer
    {
        public byte[] buffer;
        public int offset;
        public int size;

        public RecvBuffer()
        {
            buffer = null;
            offset = 0;
            size = 0;
        }
    }
    
    public class TCPSocket : MonoBehaviour
    {
        public TcpClient Sock;
        public NetworkStream Net;

        public bool Connecting(string ip, int port)
        {
            try
            {
                Sock = new TcpClient(ip, port);
                Net = Sock.GetStream();
            }
            catch (SocketException e)
            {
                print("Socket Error : " + e.Message.ToString());
                return false;
            }
            catch (Exception e)
            {
                print("ETC Error : " + e.Message.ToString());
                return false;
            }
            return true;
        }

        public void Send(SendBuffer ptr)
        {
            Net.Write(ptr.buffer, 0, ptr.size);
        }

        public int Recv(RecvBuffer ptr)
        {
            return Net.Read(ptr.buffer, ptr.offset, ptr.size);
        }

        public void Release()
        {
            Net.Close();
            Sock.Close();
        }
    }
}