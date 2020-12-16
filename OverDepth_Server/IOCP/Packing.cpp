#include "Packing.h"
#include"Crypt.h"

CPacking::CPacking()
{
	recvbytes = 0;
	comp_recvbytes = 0;
	ZeroMemory(recvbuf, sizeof(recvbuf));

	recv_sizeflag = false;

	sendbuf = new queue<QueueSendData*>();
}

CPacking::CPacking(SOCKET _sock) : CTCPSocket(_sock)
{
	recvbytes = 0;
	comp_recvbytes = 0;
	ZeroMemory(recvbuf, sizeof(recvbuf));

	recv_sizeflag = false;

	sendbuf = new queue<QueueSendData*>();
}

CPacking::~CPacking()
{
	delete sendbuf;
}

void CPacking::PackingData(unsigned __int64 protocol, const char* buf, int _size)
{
	CLock lock;
	char encrypt[4096];
	ZeroMemory(encrypt, sizeof(encrypt));
	QueueSendData* data = new QueueSendData;
	ZeroMemory(data, sizeof(QueueSendData));
	char* ptr = data->sendbuf;
	int size = 0;

	ptr = ptr + sizeof(size);



	memcpy(ptr, &protocol, sizeof(unsigned __int64));
	ptr = ptr + sizeof(unsigned __int64);
	size = size + sizeof(unsigned __int64);

	if (buf != nullptr)
	{
		memcpy(ptr, buf, _size);
		size = size + _size;
	}

	CCrypt::Decrypt((BYTE*)buf, (BYTE*)encrypt, size);
	//데이터 첫번째 전체 패킷 길이를 제외한 packing이 다된 상태에서 암호화를 진행 해야 하므로.

	ptr = data->sendbuf;
	memcpy(ptr, &size, sizeof(int));

	size = size + sizeof(int);
	data->sendbytes = size;

	sendbuf->push(data);
}

bool CPacking::Send()
{
	CLock lock;
	if (sendbuf->size() > 1)
	{
		printf("sendbuf->size() = %d\n", sendbuf->size());
		return false;
	}

	return WSA_Send(sendbuf->front()->sendbuf + sendbuf->front()->comp_sendbytes, sendbuf->front()->sendbytes - sendbuf->front()->comp_sendbytes);
}

int CPacking::CompleteSend(int _completebyte)
{
	CLock lock;

	sendbuf->front()->comp_sendbytes += _completebyte;

	if (sendbuf->front()->comp_sendbytes == sendbuf->front()->sendbytes)
	{
		sendbuf->pop();

		if (sendbuf->empty())
		{
			return SOC_TRUE;
		}
		else
		{
			if (!Send()) return SOC_ERROR;
			return SOC_FALSE;
		}
	}

	if (!Send()) return SOC_ERROR;

	return SOC_FALSE;
}
//이부분 수정
unsigned __int64 CPacking::GetProtocol()
{
	//CLock lock;

	unsigned __int64 protocol;
	memcpy(&protocol, recvbuf, sizeof(unsigned __int64));
	return protocol;
}

const char* CPacking::UnPackData()
{
	//CLock lock;
	//이부분 수정
	const char* ptr = recvbuf + sizeof(unsigned __int64);
	return ptr;
}

bool CPacking::Recv()
{
	//CLock lock;

	int len;
	if (recv_sizeflag)
	{
		len = recvbytes - comp_recvbytes;
	}
	else
	{
		len = sizeof(int) - comp_recvbytes;
	}

	return WSA_Recv(recvbuf + comp_recvbytes, len);
}

int CPacking::CompleteRecv(int _completebyte)
{
	//CLock lock;
	char decrypt[4096];
	ZeroMemory(decrypt, sizeof(decrypt));
	if (!recv_sizeflag)
	{
		comp_recvbytes += _completebyte;

		if (comp_recvbytes == sizeof(int))
		{
			memcpy(&recvbytes, recvbuf, sizeof(int));
			comp_recvbytes = 0;
			recv_sizeflag = true;
		}

		if (!Recv()) return SOC_ERROR;

		return SOC_FALSE;
	}

	comp_recvbytes += _completebyte;

	if (comp_recvbytes == recvbytes)
	{
		//CCrypt::Decrypt((BYTE*)recvbuf, (BYTE*)decrypt, recvbytes);//데이터가 다 받아진 상태에서 복호화를 진행 해야 하므로.
		comp_recvbytes = 0;
		recvbytes = 0;
		recv_sizeflag = false;

		return SOC_TRUE;
	}
	else
	{
		if (!Recv()) return SOC_ERROR;

		return SOC_FALSE;
	}
}