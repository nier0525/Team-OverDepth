using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;
using TCP;
using UnityEngine;

public class MonsterState : State
{
    public void Monster_BeginInfoUpdate(int hp)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)STATE.UPDATE | (UInt64)PROTOCOL.BEGININFO;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, hp, Static_Data.m_jab));
    }

    public void Monster_TransformUpdate(Vector3 position, Quaternion rotation)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)STATE.UPDATE | (UInt64)PROTOCOL.TRANSFROM;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, position, rotation));
    }

    public void Monster_HPUpdate(UInt64 PROTOCOL, int HP)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)STATE.UPDATE | PROTOCOL;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, HP));
    }

    //public void Monster_UpdateRotation(Quaternion rotation)
    //{
    //    UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)STATE.UPDATE | (UInt64)PROTOCOL.ROTATION;
    //    TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, rotation));
    //}

    public void Monster_Movement(Vector3 position, Quaternion rotation, float anim)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)STATE.MOVEMENT | (UInt64)PROTOCOL.MOVE;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, position, rotation, anim));
    }

    public void Monster_Movement(Vector3 position, Quaternion rotation, float forward, float right)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)STATE.MOVEMENT | (UInt64)PROTOCOL.FOCUS;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, position, rotation, forward, right));
    }

    public void Monster_Animation_Active(UInt64 _Protocol, string anim)
    {
        var Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)STATE.ANIMATION | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, anim));
    }

    public void Monster_Animation_Active(UInt64 _Protocol, Vector3 position, Quaternion rotation, string anim)
    {
        var Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)STATE.ANIMATION | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, position, rotation, anim));
    }

    // 몬스터 타입, 몬스터 인덱스(아이디), 현재 상태, 위치, 회전값, 애니메이션
    public void Monster_Animation(STATE MTYPE, int index, UInt64 _Protocol, Vector3 position, Quaternion rotation, int anim)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)MTYPE | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(index, position, rotation, anim));
    }

    // 몬스터 타입, 몬스터 인덱스(아이디), 현재 상태, 위치, 회전값, 애니메이션
    public void Monster_Animation(STATE MTYPE, int index, UInt64 _Protocol, Vector3 position, Quaternion rotation, float v, float h, int anim)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)MTYPE | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(index, position, rotation, v, h, anim));
    }

    // 몬스터 타입, 몬스터 인덱스(아이디), 현재 상태, 위치, 회전값, vertical, horizontal
    public void Monster_Movement(STATE MTYPE, int index, UInt64 _Protocol, Vector3 position, Quaternion rotation, float forward, float right, float speed)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)MTYPE | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(index, position, rotation, forward, right, speed));
    }

    // 몬스터 타입, 몬스터 인덱스(아이디), 현재 상태, 위치, 회전값, vertical, horizontal
    public void Monster_Movement(STATE MTYPE, int index, UInt64 _Protocol, Vector3 position, MoveType moveType, float speed)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)MTYPE | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(index, position, (int)moveType, speed));
    }

    // 속도값 보내기
    public void Monster_SetSpeed(STATE MTYPE, int index, UInt64 _Protocol, float Speed)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)MTYPE | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(index, Speed));
    }

    // 몬스터 상태 변경
    public void Monster_ChangeState(STATE MTYPE, int index, UInt64 _Protocol, MONSTER_STATE nowState, MONSTER_STATE desiredState)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)MTYPE | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(index, (int)nowState, (int)desiredState));
    }

    // 타겟 지정
    // 플레이어의 classNumber를 받아서 타겟을 판별
    public void Monster_SetTarget(STATE MTYPE, int index, UInt64 _Protocol, MONSTER_STATE nowState, int classNumber)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)MTYPE | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(index, (int)nowState, classNumber));
    }

    // 페이즈와 스탠스(2족, 4족) 정보
    public void Monster_BattleInfo(STATE MTYPE, int index, UInt64 _Protocol, int battlePhase)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)MTYPE | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(index, battlePhase));
    }

    // 몬스터 체력 정보
    public void Monster_HPInfo(STATE MTYPE, int index, UInt64 _Protocol, int curHP)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)MTYPE | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(index, curHP));
    }

    public void Monster_Die(STATE state, int index, Vector3 position, Quaternion rotation, int anim)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)state | (UInt64)PROTOCOL.M_DEATH;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, index, position, rotation, anim));
    }

    public void Monster_BeginInfoUpdate(STATE state, int index, int hp)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)state | (UInt64)PROTOCOL.M_BEGIN;
        TCPClient.Instance.PackingData(Protocol, PackingData(index, hp));
    }

    public void Monster_Hit(STATE state, int index, Vector3 pos, Quaternion rot, int anim, int hp)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)state | (UInt64)PROTOCOL.M_ONHIT;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, index, pos, rot, anim, hp));
    }

    public void Monster_Hit(STATE state, int index, Vector3 pos, Quaternion rot, int anim, int hp, int hitType)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)state | (UInt64)PROTOCOL.M_ONHIT;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, index, pos, rot, anim, hp, hitType));
    }

    public void Monster_Die(STATE state, int index, Vector3 position, Quaternion rotation)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.MONSTER_STATE | (UInt64)state | (UInt64)PROTOCOL.M_DEATH;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, index, position, rotation));
    }

    // Recv

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

        int index = 0; // 각 몬스터 별 식별 번호
        Vector3 position = Vector3.zero; // 위치값
        Quaternion rotation = Quaternion.identity; // 회전값
        int state = 0; // 상태값
        int anim = 0;
        int hp = 0;
        int hitType = 0;
        int m_MonsterState;

        try
        {
            switch((STATE)State)
            {
                case STATE.MERMAN:
                    switch ((PROTOCOL)Protocol)
                    {
                        case
                        PROTOCOL.M_DEATH:
                            UnPackingData(buffer, out index, out position, out rotation, out anim);
                            print("Mermen Die");

                            try
                            {
                                var monster = Static_Data.MermanList[index];
                                monster.GetComponent<MermanController>().Death_State(position, rotation, anim);

                                print("1");
                            }
                            catch
                            {
                                Static_Data.MonsterDataPack pack = new Static_Data.MonsterDataPack();
                                pack.index = index;
                                pack.position = position;
                                pack.rotation = rotation;
                                pack.anim = anim;

                                Static_Data.Die_MermanList.Add(pack);

                                print("2");
                            }
                            break;
                    }
                    break;

                case STATE.FANATIC_SWORD:
                    switch((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.M_MOVE:       // 이동
                            float forward, right, speed;
                            int moveType;
                            UnPackingData(buffer, out index, out position, out moveType, out speed);

                            try
                            {
                                var monster = Static_Data.FanaticSwordList[index];
                                monster.GetComponent<FanaticController>().TCP_RecvTransform(position, rotation, speed);
                                monster.GetComponent<FanaticController>().TCP_RecvMoveType(moveType);
                            }
                            catch
                            {
                                print("M_MOVE / No connection found with Server");
                            }

                            break;
                        case PROTOCOL.M_ATTACK:
                            UnPackingData(buffer, out index, out position, out rotation, out forward, out right, out anim);

                            try
                            {
                                var monster = Static_Data.FanaticSwordList[index];
                                monster.GetComponent<FanaticController>().TCP_RecvAnimation(anim);
                                monster.GetComponent<FanaticController>().TCP_RecvTransform(position, rotation);
                                monster.GetComponent<FanaticController>().TCP_RecvMoveValue(forward, right);
                            }
                            catch
                            {
                                print("M_ATTACK / No connection found with Server");
                            }

                            break;
                        case PROTOCOL.M_ONHIT:
                            UnPackingData(buffer, out index, out position, out rotation, out anim, out hp, out hitType);
                            print("스워드 온히트 언팩킹");

                            try
                            {
                                print("스워드 온히트 받음1");
                                var monster = Static_Data.FanaticSwordList[index];
                                print("스워드 온히트 받음2");
                                monster.GetComponent<FanaticController>().OnHit_State(position, rotation, anim, hp, hitType);
                                print("스워드 온히트 받음3");
                            }
                            catch { print("스워드 온히트 못받음"); }

                            break;
                        case PROTOCOL.M_DEATH:
                            UnPackingData(buffer, out index, out position, out rotation);

                            try
                            {
                                var monster = Static_Data.FanaticSwordList[index];
                                monster.GetComponent<FanaticController>().Death_State(position, rotation);
                            }
                            catch { }

                            break;
                        case PROTOCOL.M_SET_TARGET:     // 타겟 지정하기
                            int classNumber;
                            UnPackingData(buffer, out index, out m_MonsterState, out classNumber);

                            try
                            {
                                var monster = Static_Data.FanaticSwordList[index];
                                monster.GetComponent<FanaticController>().TCP_RecvTarget(classNumber);
                                monster.GetComponent<FanaticController>().TCP_isConnected = true;
                            }
                            catch
                            {
                                print("M_SET_TARGET / No connection found with Server");
                                var monster = Static_Data.FanaticSwordList[index];
                                monster.GetComponent<FanaticController>().TCP_isConnected = false;
                            }

                            break;
                        case PROTOCOL.M_CHANGE_STATE:   // 상태변경하기
                            int desiredState;
                            UnPackingData(buffer, out index, out m_MonsterState, out desiredState);

                            try
                            {
                                var monster = Static_Data.FanaticSwordList[index];
                                monster.GetComponent<FanaticController>().TCP_RecvChangeState(m_MonsterState, desiredState);
                                monster.GetComponent<FanaticController>().TCP_isConnected = true;
                            }
                            catch
                            {
                                print("M_CHANGE_STATE / No connection found with Server");
                                var monster = Static_Data.FanaticSwordList[index];
                                monster.GetComponent<FanaticController>().TCP_isConnected = false;
                            }

                            break;
                    }
                    break;

                case STATE.FANATIC_MASE:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.M_MOVE:       // 이동
                            float forward, right, speed;
                            int moveType;
                            UnPackingData(buffer, out index, out position, out moveType, out speed);

                            try
                            {
                                var monster = Static_Data.FanaticMaceList[index];
                                monster.GetComponent<FanaticMaceController>().TCP_RecvTransform(position, rotation, speed);
                                monster.GetComponent<FanaticMaceController>().TCP_RecvMoveType(moveType);
                            }
                            catch
                            {
                                print("M_MOVE / No connection found with Server");
                            }

                            break;
                        case PROTOCOL.M_ATTACK:
                            UnPackingData(buffer, out index, out position, out rotation, out forward, out right, out anim);

                            try
                            {
                                var monster = Static_Data.FanaticMaceList[index];
                                monster.GetComponent<FanaticMaceController>().TCP_RecvAnimation(anim);
                                monster.GetComponent<FanaticMaceController>().TCP_RecvTransform(position, rotation);
                                monster.GetComponent<FanaticMaceController>().TCP_RecvMoveValue(forward, right);
                            }
                            catch
                            {
                                print("M_ATTACK / No connection found with Server");
                            }

                            break;
                        case PROTOCOL.M_ONHIT:
                            UnPackingData(buffer, out index, out position, out rotation, out anim, out hp, out hitType);
                            print("메이스 온히트 언팩킹");

                            try
                            {
                                print("메이스 온히트 받음1");
                                var monster = Static_Data.FanaticMaceList[index];
                                print("메이스 온히트 받음2");
                                monster.GetComponent<FanaticMaceController>().OnHit_State(position, rotation, anim, hp, hitType);
                                print("메이스 온히트 받음3");
                            }
                            catch { print("메이스 온히트 못받음"); }

                            break;
                        case PROTOCOL.M_DEATH:
                            UnPackingData(buffer, out index, out position, out rotation);

                            try
                            {
                                var monster = Static_Data.FanaticMaceList[index];
                                monster.GetComponent<FanaticMaceController>().Death_State(position, rotation);
                            }
                            catch { }

                            break;
                        case PROTOCOL.M_SET_TARGET:     // 타겟 지정하기
                            int classNumber;
                            UnPackingData(buffer, out index, out m_MonsterState, out classNumber);

                            try
                            {
                                var monster = Static_Data.FanaticMaceList[index];
                                monster.GetComponent<FanaticMaceController>().TCP_RecvTarget(classNumber);
                                monster.GetComponent<FanaticMaceController>().TCP_isConnected = true;
                            }
                            catch
                            {
                                print("M_SET_TARGET / No connection found with Server");
                                var monster = Static_Data.FanaticMaceList[index];
                                monster.GetComponent<FanaticMaceController>().TCP_isConnected = false;
                            }

                            break;
                        case PROTOCOL.M_CHANGE_STATE:   // 상태변경하기
                            int desiredState;
                            UnPackingData(buffer, out index, out m_MonsterState, out desiredState);

                            try
                            {
                                var monster = Static_Data.FanaticMaceList[index];
                                monster.GetComponent<FanaticMaceController>().TCP_RecvChangeState(m_MonsterState, desiredState);
                                monster.GetComponent<FanaticMaceController>().TCP_isConnected = true;
                            }
                            catch
                            {
                                print("M_CHANGE_STATE / No connection found with Server");
                                var monster = Static_Data.FanaticMaceList[index];
                                monster.GetComponent<FanaticMaceController>().TCP_isConnected = false;
                            }

                            break;
                    }
                    break;

                case STATE.FANATIC_GUN:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.M_MOVE:       // 이동
                            float forward, right, speed;
                            int moveType;
                            UnPackingData(buffer, out index, out position, out moveType, out speed);

                            try
                            {
                                var monster = Static_Data.FanaticGunnerList[index];
                                monster.GetComponent<FanaticGunnerController>().TCP_RecvTransform(position, rotation, speed);
                                monster.GetComponent<FanaticGunnerController>().TCP_RecvMoveType(moveType);
                            }
                            catch
                            {
                                print("M_MOVE / No connection found with Server");
                            }

                            break;
                        case PROTOCOL.M_ATTACK:     // 공격, 애니메이션 실행
                            UnPackingData(buffer, out index, out position, out rotation, out forward, out right, out anim);

                            try
                            {
                                var monster = Static_Data.FanaticGunnerList[index];
                                monster.GetComponent<FanaticGunnerController>().TCP_RecvAnimation(anim);
                                monster.GetComponent<FanaticGunnerController>().TCP_RecvTransform(position, rotation);
                                monster.GetComponent<FanaticGunnerController>().TCP_RecvMoveValue(forward, right);
                            }
                            catch
                            {
                                print("M_ATTACK / No connection found with Server");
                            }

                            break;
                        case PROTOCOL.M_ONHIT:
                            UnPackingData(buffer, out index, out position, out rotation, out anim, out hp);
                            try
                            {
                                var monster = Static_Data.FanaticGunnerList[index];
                                monster.GetComponent<FanaticGunnerController>().OnHit_State(position, rotation, anim, hp);
                            }
                            catch { }
                            break;

                        case PROTOCOL.M_DEATH:
                            UnPackingData(buffer, out index, out position, out rotation);

                            try
                            {
                                var monster = Static_Data.FanaticGunnerList[index];
                                monster.GetComponent<FanaticGunnerController>().Death_State(position, rotation);
                            }
                            catch { }
                            break;
                        case PROTOCOL.M_SET_TARGET:     // 타겟 지정하기
                            int classNumber;
                            UnPackingData(buffer, out index, out m_MonsterState, out classNumber);

                            try
                            {
                                var monster = Static_Data.FanaticGunnerList[index];
                                monster.GetComponent<FanaticGunnerController>().TCP_RecvTarget(classNumber);
                                monster.GetComponent<FanaticGunnerController>().TCP_isConnected = true;
                            }
                            catch
                            {
                                print("M_SET_TARGET / No connection found with Server");
                                var monster = Static_Data.FanaticGunnerList[index];
                                monster.GetComponent<FanaticGunnerController>().TCP_isConnected = false;
                            }

                            break;
                        case PROTOCOL.M_CHANGE_STATE:   // 상태변경하기
                            int desiredState;
                            UnPackingData(buffer, out index, out m_MonsterState, out desiredState);

                            try
                            {
                                var monster = Static_Data.FanaticGunnerList[index];
                                monster.GetComponent<FanaticGunnerController>().TCP_RecvChangeState(m_MonsterState, desiredState);
                                monster.GetComponent<FanaticGunnerController>().TCP_isConnected = true;
                            }
                            catch
                            {
                                print("M_CHANGE_STATE / No connection found with Server");
                                var monster = Static_Data.FanaticGunnerList[index];
                                monster.GetComponent<FanaticGunnerController>().TCP_isConnected = false;
                            }

                            break;
                    }
                    break;

                case STATE.TITANICHYDRA:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.M_MOVE:       // 이동
                            float forward, right, speed;
                            int moveType;
                            UnPackingData(buffer, out index, out position, out moveType, out speed);

                            try
                            {
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_RecvTransform(position, rotation, speed);
                                monster.GetComponent<TitanicHydraController>().TCP_RecvMoveType(moveType);
                            }
                            catch
                            {
                                print("M_MOVE / No connection found with Server");
                            }

                            break;
                        case PROTOCOL.M_ATTACK:     // 공격, 애니메이션 실행
                            UnPackingData(buffer, out index, out position, out rotation, out forward, out right, out anim);

                            try
                            {
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_RecvAnimation(anim);
                                monster.GetComponent<TitanicHydraController>().TCP_RecvTransform(position, rotation);
                                monster.GetComponent<TitanicHydraController>().TCP_RecvMoveValue(forward, right);
                            }
                            catch
                            {
                                print("M_ATTACK / No connection found with Server");
                            }

                            break;
                        case PROTOCOL.M_ONHIT:      // 피격
                            int serial;
                            UnPackingData(buffer, out index, out position, out rotation, out anim, out hp, out serial);
                            try
                            {
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_RecvTransform(position, rotation);
                                monster.GetComponent<TitanicHydraController>().TCP_RecvHPInfo(hp);
                                monster.GetComponent<TitanicHydraController>().TCP_RecvDamageType(hp, serial);
                            }
                            catch 
                            {
                                print("M_ONHIT / No connection found with Server");
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_isConnected = false;
                            }

                            break;
                        case PROTOCOL.M_DEATH:      // 죽음
                            break;
                        case PROTOCOL.M_SET_TARGET:     // 타겟 지정하기
                            int classNumber;
                            UnPackingData(buffer, out index, out m_MonsterState, out classNumber);

                            try
                            {
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_RecvTarget((MONSTER_STATE)m_MonsterState, classNumber);
                                monster.GetComponent<TitanicHydraController>().TCP_isConnected = true;
                            }
                            catch
                            {
                                print("M_SET_TARGET / No connection found with Server");
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_isConnected = false;
                            }

                            break;
                        case PROTOCOL.M_CHANGE_STATE:   // 상태변경하기
                            int desiredState;
                            UnPackingData(buffer, out index, out m_MonsterState, out desiredState);

                            
                            try
                            {
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_RecvChangeState(m_MonsterState, desiredState);
                                monster.GetComponent<TitanicHydraController>().TCP_isConnected = true;
                            }
                            catch
                            {
                                print("M_CHANGE_STATE / No connection found with Server");
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_isConnected = false;
                            }

                            break;
                        case PROTOCOL.M_BATTLEINFO:     // 페이즈와 현재 스탠스 지정
                            int battlePhase;
                            UnPackingData(buffer, out index, out battlePhase);

                            try
                            {
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_RecvBattleInfo(battlePhase);
                                monster.GetComponent<TitanicHydraController>().TCP_isConnected = true;
                            }
                            catch
                            {
                                print("M_BATTLEINFO / No connection found with Server");
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_isConnected = false;
                            }

                            break;
                        case PROTOCOL.M_SETSTONE:
                            int currentStone;
                            UnPackingData(buffer, out index, out currentStone);

                            try
                            {
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_RecvSetStone(currentStone);
                                monster.GetComponent<TitanicHydraController>().TCP_isConnected = true;
                            }
                            catch
                            {
                                print("M_SETSTONE / No connection found with Server");
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_isConnected = false;
                            }

                            break;
                        case PROTOCOL.M_FINDSTONE:      // 돌 찾으러가기
                            int isFindStone;
                            UnPackingData(buffer, out index, out isFindStone);

                            try
                            {
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_RecvFindStone(isFindStone);
                                monster.GetComponent<TitanicHydraController>().TCP_isConnected = true;
                            }
                            catch
                            {
                                print("M_FINDSTONE / No connection found with Server");
                                var monster = Static_Data.TitanicHydra;
                                monster.GetComponent<TitanicHydraController>().TCP_isConnected = false;
                            }

                            break;
                    }
                    break;
            }
        }
        catch
        {
            print("Not Loading Completed");
        }
    }
}
