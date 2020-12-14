# Team-OverDepth
--------------------------------------------
팀명 : OverDepth  
게임명 : OverDepth  
개발 기간 : 2020.03.01 - 2020.12.03  
개발 환경 : Unity3D / Unity C# / C++ / Github Desktop / Trello  
개발 인원 : 11명 ( 기획 3 / 프로그래밍 4 / 그래픽 4 )  
  
```  
최대 2인까지 협동 가능한 3D 멀티 액션 게임으로 OverDepth 만의 특수한 기믹과 강력한 몬스터들을 협력하여 공략해 나가는 게임입니다.  
G-Star 2020 출품, 교내 경진대회 출품 및 최우수상 수상, GGC ( Global Game Challenge 2020 ) 출품 등의 성과를 내었습니다.  
```  
  
+ 담당 업무 : 서버 / 클라이언트 프로그래머  
  + 서버 세부 업무 : 라이브러리 설계, MYSQL 연동, 본 게임과의 연동 등 전체적인 모든 서버 시스템  
  + 클라이언트 세부 엄무 : 게임 UI, 옵션, 타이틀 화면과 같은 게임 시스템 기능 및 플레이어 서버 동기화와 보간 기능  
  
  
# 게임 소개
-------------------------------------------  
  
게임 영상 링크 : https://www.youtube.com/watch?v=y0tODYMJPVg  
  
<img src = "https://user-images.githubusercontent.com/63215359/102127979-86a36500-3e90-11eb-8acd-45a96c9f0246.PNG" width="350" height="300">
<img src = "https://user-images.githubusercontent.com/63215359/102128119-bc484e00-3e90-11eb-9ab5-afeb920b8e88.PNG" width="350" height="300">
<img src = "https://user-images.githubusercontent.com/63215359/102128137-c23e2f00-3e90-11eb-8afb-220d5221c968.PNG" width="350" height="300">
  
```  
OverDepth 는 최대 2인 협동이 가능한 3D 멀티 액션 게임 입니다.  
Soul Like 류의 게임 중 대표격인 블러드본, 다크소울과 같은 게임에 영향이 많이 받았습니다.  
  
게임 시작 시 플레이 할 캐릭터를 선택하고 다른 유저와의 매칭을 대기하게 됩니다.  
2인이 서로 매칭이 되었을 경우 게임을 진행할 수 있으며 서로 협동하여 각종 퍼즐과 몬스터를 공략하고,  
최종적으로 마지막 보스를 제거하면 게임을 클리어 할 수 있습니다.  
```  

# 담당 게임 기능
------------------------------------------  
  
+ 서버  
  + 서버 라이브러리  
  + 플레이어 동기화  
  + 서버-클라이언트 보간  
+ 클라이언트  
  + 각종 유저 편의 기능  
    
## 서버
-----------------------------------------
### 서버 라이브러리
-----------------------------------------
  
서버 라이브러리는 IOCP 를 기반으로 설계되었으며 DB 는 MYSQL 과 연동하고 있습니다.  
  
#### 1. 프로토콜
  
프로토콜은 바이트 연산을 이용하여 그 기능과 역할을 세분화하여 관리하기 용이하도록 구현하였고, 기본적으로 Main, Sub, Protocol 로 나누어 사용하고 있습니다.  
  
Main 의 역할은 그 기능의 큰 틀로 보통 클래스명을 사용하고 합니다. ( MonsterClass, PlayerClass . . . )  
Sub 의 역할은 클래스 내에 어떤 멤버 함수를 사용할 것인가에 대한 기능으로 그 기준은 사용자의 재량에 따릅니다. ( Attack, Move 또는 몬스터 종류별 함수 등 . . . )  
Protocol 의 역할은 어떤 기능을 사용할 것인지를 선택하는 기능으로 가장 핵심적인 역할을 담당하고 있습니다.
  
헤더 파일  
```  
#pragma once
#include"Global.h"

class CProtocol
{
private:
    //1$3 = 일치
    //2&6 = 일치
    //2&7 = 일치 
    //& 연산을 하기 때문에 1이 아닌것은 다 0으로 처리 하기 때문.
    //1, 3, 7, f, 3f, 7f, ff
    DECLARE_SINGLETONE(CProtocol);
    CProtocol() = default;
    ~CProtocol() = default;
    enum class FULL_CODE :unsigned __int64
    {
        MAIN = 0xff00000000000000,
        SUB =  0x00ff000000000000,
        PROTOCOL = 0x0000ff0000000000
    };
public:
    void ProtocolMaker(unsigned __int64& Full_Code, unsigned __int64 input);
    bool ProtocolUnpacker(unsigned __int64 _full_code, unsigned __int64 main, unsigned __int64 sub, unsigned __int64 protocol);
};
```  
  
구현부
  
```  
#include "CProtocol.h"
IMPLEMENT_SINGLETON(CProtocol)

void CProtocol::ProtocolMaker(unsigned __int64& Full_Code, unsigned __int64 input)
{
    Full_Code = Full_Code | input;
}

bool CProtocol::ProtocolUnpacker(unsigned __int64 _full_code, unsigned __int64 main, unsigned __int64 sub, unsigned __int64 protocol)
{
    unsigned __int64 temp = 0;
    int variable_count = 0;//입력받은 매개변수 개수
    if (main != NULL)
    {
        temp = _full_code & (unsigned __int64)FULL_CODE::MAIN;
        variable_count++;
    }
    if (sub != NULL)
    {
        temp = _full_code & (unsigned __int64)FULL_CODE::SUB;
        variable_count++;
    }
    if (protocol != NULL)
    {
        temp = _full_code & (unsigned __int64)FULL_CODE::PROTOCOL;
        variable_count++;
    }
    if (variable_count == 2)
    {
        printf("입력 가능 매개변수의 수를 초과하셨습니다.");
        return false;
    }
    if (temp == main|| temp == sub || temp == protocol)
    {
        return true;
    }
    else
    {
        return false;
    }

}
```  
  
### 플레이어 동기화
-----------------------------------------
  
### 서버-클라이언트 보간
-----------------------------------------
  
## 클라이언트
-----------------------------------------
### 각종 유저 편의 기능
  
-----------------------------------------
