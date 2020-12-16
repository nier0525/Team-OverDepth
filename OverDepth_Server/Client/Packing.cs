using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.Threading;
using System.Net.Sockets;
using UnityEngine.Playables;

namespace TCP
{
    public class Packing : TCPSocket
    {
        /// <summary>Full Protocol from Server</summary>
        public enum FULL_CODE : UInt64
        {
            MAIN = 0xff00000000000000,
            SUB = 0x00ff000000000000,
            PROTOCOL = 0x0000ff0000000000
        };

        /// <summary>Sent from server to client.</summary>
        public enum CLASS_STATE : UInt64
        {
            NO_STATE = 0x0000000000000000,
            INIT_STATE = 0x0100000000000000,
            LOGIN_STATE = 0x0200000000000000,
            CHAT_STATE = 0x0300000000000000,
            LOBBY_STATE = 0x0400000000000000,
            PLAYER_STATE = 0x0500000000000000,
            MONSTER_STATE = 0x0600000000000000,
        }

        /// <summary>Sent from client to server.</summary>
        public enum PROTOCOL : UInt64
        {
            // Login Protocol
            JOIN_INFO = 0x0000010000000000,
            JOIN_RESULT = 0x0000020000000000,
            LOGIN_INFO = 0x0000030000000000,
            LOGIN_RESULT = 0x0000040000000000,
            JOIN_OVERLAP = 0x0000050000000000,
            JOIN_OVERLAPRESULT = 0x0000060000000000,
            LOGOUT = 0x0000070000000000,

            // Chat
            SEND = 0x0000010000000000,
            RECV = 0x0000020000000000,

            // Player
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

            // Monster
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
            M_SETSTONE = 0x0000110000000000,
            M_FINDSTONE = 0x0000120000000000,
        };

        public enum STATE : UInt64
        {
            // Login
            MENU_SELECT_STATE = 0x0001000000000000,
            SEND_RESULT_STATE = 0x0002000000000000,

            // Chat
            RECV_STATE = 0x0001000000000000,
            SEND_STATE = 0x0002000000000000,

            // Match
            ENTER = 0x0001000000000000,
            LEAVE = 0x0002000000000000,
            VALVE = 0x0003000000000000,

            // Player & Monster
            MOVEMENT = 0x0001000000000000,
            FOCUS = 0x0002000000000000,
            ANIMATION = 0x0003000000000000,
            UPDATE = 0x0004000000000000,

            MERMAN = 0x0001000000000000,
            FANATIC_SWORD = 0x0002000000000000,
            FANATIC_MASE = 0x0003000000000000,
            FANATIC_GUN = 0x0004000000000000,
            TITANICHYDRA = 0x0005000000000000,
        };

        public enum M_TYPE
        {

        }

        public enum LOBBY_RESULT
        {
            NOT_READY,
            READY,
            UPDATE_NUMBER,
            VALVE_COMPLETE,
        };

        public enum INFO_RESULT
        {
            NODATA = -1,
            JOIN_SUCCESS = 1,
            ERROR_JOIN_EXISTS,
            LOGIN_SUCCESS,
            ERROR_LOGIN_ID,
            ERROR_LOGIN_PW,
            ERROR_LOGIN_EXISTS
        };


        public enum RESULT
        {
            NODATA = -1,
            JOIN_SUCCESS = 1,
            ERROR_JOIN_EXISTS,
            LOGIN_SUCCESS,
            ERROR_LOGIN_ID,
            ERROR_LOGIN_PW,
            ERROR_LOGIN_EXISTS
        };

        public Queue<SendBuffer> m_SendBuffer;
        public Queue<RecvBuffer> m_RecvBuffer;

        Thread SendThread;
        Thread RecvThread;

        public AutoResetEvent m_SendEvent;

        static public object m_lock;
        public static List<string> ip = new List<string>();

        public void Begin()
        {
            //ip.Add("192.168.0.216");
            //ip.Add("127.0.0.1");

            // 로컬
            ip.Add("182.172.23.98");

            // AWS -----------------
            // 서울
            ip.Add("54.180.123.104");

            // 오하이오 ( 미국 )
            ip.Add("3.137.204.197");

            // 일본
            ip.Add("13.112.127.182");

            bool connected = false;

            foreach (var item in ip)
            {
                if (Connecting(item, 9050))
                {
                    connected = true;
                    break;
                }
            }

            if (!connected)
                Application.Quit();

            //if (!Connecting(ip, 9050))
            //{

            //    if (!Connecting(ip, 9050))
            //        Application.Quit();
            //}

            m_lock = new object();
            m_SendEvent = new AutoResetEvent(false);

            m_SendBuffer = new Queue<SendBuffer>();
             m_RecvBuffer= new Queue<RecvBuffer>();

            SendThread = new Thread(new ThreadStart(SendProcess));
            RecvThread = new Thread(new ThreadStart(RecvProcess));

            SendThread.Start();
            RecvThread.Start();
        }

        public void End()
        {
            m_SendBuffer.Clear();
            m_RecvBuffer.Clear();

            SendThread.Abort();
            RecvThread.Abort();

            Release();
        }

        void SendProcess()
        {
            while (true)
            {
                try
                {
                    if (m_SendBuffer.Count == 0)
                        m_SendEvent.WaitOne();

                    lock (m_lock)
                        Send(m_SendBuffer.Dequeue());
                }
                catch
                {
                    //print("Send Error");
                    continue;
                }
            }
        }

        void RecvProcess()
        {
            while (true)
            {
                try
                {
                    RecvBuffer data = new RecvBuffer();

                    data.buffer = new byte[4];
                    data.size = sizeof(int);
                    data.offset = 0;

                    int retval = Recv(data);
                    data.size = BitConverter.ToInt32(data.buffer, 0);
                    data.buffer = new byte[data.size];

                    while (data.size > 0)
                    {
                        retval = Recv(data);
                        if (retval == 0)
                            break;

                        data.size -= retval;
                        data.offset += retval;
                    }

                    data.size = data.offset;

                    lock (m_lock)
                        m_RecvBuffer.Enqueue(data);
                }
                catch
                {
                    //print("Recv Error");
                    continue;
                }
            }

        }

        public void PackingData(UInt64 Protocol)
        {
            lock (m_lock)
            {
                SendBuffer data = new SendBuffer();
                data.buffer = new byte[4096];
                data.size = 0;

                data.offset = sizeof(int);

                Buffer.BlockCopy(BitConverter.GetBytes(Protocol), 0, data.buffer, data.offset, sizeof(UInt64));
                data.size += sizeof(UInt64);

                Buffer.BlockCopy(BitConverter.GetBytes(data.size), 0, data.buffer, 0, sizeof(int));
                data.size += sizeof(int);

                m_SendBuffer.Enqueue(data);
                m_SendEvent.Set();
            }
        }

        public void PackingData(UInt64 Protocol, byte[] buffer)
        {
            lock (m_lock)
            {

                SendBuffer data = new SendBuffer();
                data.buffer = new byte[4096];
                data.size = 0;

                data.offset = sizeof(int);

                Buffer.BlockCopy(BitConverter.GetBytes(Protocol), 0, data.buffer, data.offset, sizeof(UInt64));
                data.offset += sizeof(UInt64);
                data.size += sizeof(UInt64);

                if (buffer.Length != 0)
                {
                    Buffer.BlockCopy(buffer, 0, data.buffer, data.offset, buffer.Length);
                    data.size += buffer.Length;
                }

                Buffer.BlockCopy(BitConverter.GetBytes(data.size), 0, data.buffer, 0, sizeof(int));
                data.size += sizeof(int);

                m_SendBuffer.Enqueue(data);
                m_SendEvent.Set();
            }
        }


        public UInt64 GetProtocol()
        {
            lock (m_lock)
            {
                try
                {
                    return BitConverter.ToUInt64(m_RecvBuffer.Peek().buffer, 0);
                }
                catch
                {
                    return 0;
                }
            }
        }

        public RecvBuffer UnPackingData()
        {
            lock (m_lock)
            {
                try
                {
                    return m_RecvBuffer.Dequeue();
                }
                catch
                {
                    return new RecvBuffer();
                }
            }
        }
    }
}