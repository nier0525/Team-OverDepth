using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;

namespace TCP
{
    public class TCPClient : Packing
    {
        public static TCPClient Instance;

        State m_State;

        [HideInInspector] public static IntroState m_Intro;
        [HideInInspector] public static LoginState m_Login;
        [HideInInspector] public static MatchState m_Match;
        [HideInInspector] public static Player_State m_Player;
        [HideInInspector] public static MonsterState m_Monster;

        // Start is called before the first frame update
        void Awake()
        {
            DontDestroyOnLoad(this);

            if (Instance != null)
            {
                GameObject obj = GameObject.Find("ServerManager");
                DestroyImmediate(obj);
            }
            else
            {
                Instance = this;
                gameObject.name = "ServerManager_Orizin";
                Begin();

                m_Intro = new IntroState();
                m_Login = new LoginState();
                m_Match = new MatchState();
                m_Player = new Player_State();
                m_Monster = new MonsterState();

                m_Intro.Intro_Message();
            }
        }

        // Update is called once per frame
        void Update()
        {
            if (m_RecvBuffer.Count != 0)
            {
                var Class_State = GetProtocol() & (UInt64)FULL_CODE.MAIN;

                switch((CLASS_STATE)Class_State)
                {
                    case CLASS_STATE.INIT_STATE:
                        SetState(m_Intro);
                        break;

                    case CLASS_STATE.LOGIN_STATE:
                        SetState(m_Login);
                        break;

                    case CLASS_STATE.LOBBY_STATE:
                        SetState(m_Match);
                        break;

                    case CLASS_STATE.PLAYER_STATE:
                        SetState(m_Player);
                        break;

                    case CLASS_STATE.MONSTER_STATE:
                        SetState(m_Monster);
                        break;
                }

                m_State.RecvProcess();
            }
        }

        void SetState(State _state)
        {
            m_State = _state;
        }

        private void OnApplicationQuit()
        {
            //if (Static_Data.m_state == "TCPClientTest")
            //{
            //    m_Match.Match_Leave();
            //}
            End();
        }
    }
}
