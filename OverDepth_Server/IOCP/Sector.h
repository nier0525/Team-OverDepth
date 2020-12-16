#pragma once
#include "Global.h"

class CSector
{
private:
	// list<CClientSection*>* mClientList;
	// Monster, Object List . . .

	list<char>* mTest;

	// 인접 Sector 리스트
	list<CSector*>* mAlmostSector;
	// Sector ID
	int mSectorID;
public:
	CSector()
	{
		mTest = new list<char>();
		mAlmostSector = new list<CSector*>();

		mSectorID = 0;
	}

	CSector(int id)
	{
		mTest = new list<char>();
		mAlmostSector = new list<CSector*>();

		mSectorID = id;
	}

	~CSector()
	{
		delete mTest;
		delete mAlmostSector;
	}

	// bool Send(char* buf, int size)

	// Client , Monster, Object 등의 오버로딩 정의가 필요함
	// 지금은 굳이 하지 않았음.
	void OnEnter(char str)
	{
		printf("%d Enter\n\n", mSectorID);
		mTest->push_back(str);
	}

	void OnExit(char str)
	{
		printf("%d Exit\n\n", mSectorID);
		mTest->remove(str);
	}

	void GetData()
	{
		for (list<char>::iterator data = mTest->begin(); data != mTest->end(); data++)
		{
			printf("%c\n", *data);
		}
	}

	char GetChar()
	{
		for (list<char>::iterator data = mTest->begin(); data != mTest->end(); data++)
		{
			return *data;
		}
	}

	void Insert_AlmostSector(CSector* sector)
	{
		mAlmostSector->push_back(sector);
	}

	void Show_AlmostSector()
	{
		printf("%d Almost Sector List\n", mSectorID);
		for (list<CSector*>::iterator data = mAlmostSector->begin(); data != mAlmostSector->end(); data++)
		{
			CSector* sector = *data;
			printf("%d\t", sector->GetID());
		}
		printf("\n");
	}

	list<CSector*>* GetAlmostSectorList() { return mAlmostSector; }
	int GetID() { return mSectorID; }

};