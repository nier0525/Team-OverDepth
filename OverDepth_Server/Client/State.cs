using System.Collections;
using System.Collections.Generic;
using System;
using System.Text;
using TCP;
using UnityEngine;

public abstract class State : TCPClient
{
    public abstract void RecvProcess();
    //public abstract void SendProcess();

    // -------------------------------언팩킹---------------------------------
    
    public void UnPackingData(RecvBuffer buffer, out int serial, out int value)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        value = BitConverter.ToInt32(data.buffer, data.offset);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out float value)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        value = BitConverter.ToSingle(data.buffer, data.offset);
    }

    // 타겟 언팩킹
    public void UnPackingData(RecvBuffer buffer, out int serial, out int value, out int classNumber)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        value = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        classNumber = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out int value, out int classNumber, out int hitType)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        value = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        classNumber = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        hitType = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
    }

    // 애니메이션
    public void UnPackingData(RecvBuffer buffer, out int serial, out string anim)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        int len = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        anim = Encoding.Default.GetString(data.buffer, data.offset, len);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out string anim, out string anim2)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        int len = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        anim = Encoding.Default.GetString(data.buffer, data.offset, len);
        data.offset += len;

        int len2 = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        anim2 = Encoding.Default.GetString(data.buffer, data.offset, len2);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out Quaternion rotation)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.w);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.x);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(rotation.y);

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out Quaternion rotation, out string anim)
    {
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.w);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.x);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(rotation.y);

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);

        int len = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        anim = Encoding.Default.GetString(data.buffer, data.offset, len);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out Vector3 position, out Quaternion rotation)
    {
        // Debug.Log("UNPACKINHGG");
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(string.Format("temp = {0:x}", serial));

        position.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.x);

        position.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(position.y);

        position.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.z);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.w);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.x);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(rotation.y);

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        //Debug.Log(rotation.z);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out Vector3 position, out Quaternion rotation, out int anim)
    {
        // Debug.Log("UNPACKINHGG");
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(string.Format("temp = {0:x}", serial));

        position.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.x);

        position.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(position.y);

        position.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.z);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.w);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.x);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(rotation.y);

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.z);

        anim = BitConverter.ToInt32(data.buffer, data.offset);
    }

    // 무브타입, 스피드 언팩킹
    public void UnPackingData(RecvBuffer buffer, out int serial, out Vector3 position, out int moveType, out float speed)
    {
        // Debug.Log("UNPACKINHGG");
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(string.Format("temp = {0:x}", serial));

        position.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.x);

        position.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(position.y);

        position.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.z);

        moveType = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(rotation.w);

        speed = BitConverter.ToSingle(data.buffer, data.offset);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out Vector3 position, out Quaternion rotation, out int anim, out int hp)
    {
        // Debug.Log("UNPACKINHGG");
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(string.Format("temp = {0:x}", serial));

        position.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.x);

        position.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(position.y);

        position.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.z);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.w);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.x);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(rotation.y);

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.z);

        anim = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        hp = BitConverter.ToInt32(data.buffer, data.offset);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out Vector3 position, out Quaternion rotation, out int anim, out int hp, out int hitType)
    {
        // Debug.Log("UNPACKINHGG");
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(string.Format("temp = {0:x}", serial));

        position.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.x);

        position.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(position.y);

        position.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.z);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.w);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.x);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(rotation.y);

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.z);

        anim = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        hp = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        hitType = BitConverter.ToInt32(data.buffer, data.offset);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out Vector3 position, out Quaternion rotation, out float anim)
    {
        // Debug.Log("UNPACKINHGG");
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(string.Format("temp = {0:x}", serial));

        position.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.x);

        position.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(position.y);

        position.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.z);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.w);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.x);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(rotation.y);

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.z);

        anim = BitConverter.ToSingle(data.buffer, data.offset);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out Vector3 position, out Quaternion rotation, out float forward, out float right)
    {
        // Debug.Log("UNPACKINHGG");
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(string.Format("temp = {0:x}", serial));

        position.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.x);

        position.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(position.y);

        position.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.z);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.w);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.x);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(rotation.y);

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.z);

        forward = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);

        right = BitConverter.ToSingle(data.buffer, data.offset);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out Vector3 position, out Quaternion rotation, out float forward, out float right, out float speed)
    {
        // Debug.Log("UNPACKINHGG");
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(string.Format("temp = {0:x}", serial));

        position.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.x);

        position.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(position.y);

        position.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.z);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.w);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.x);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(rotation.y);

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.z);

        forward = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);

        right = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);

        speed = BitConverter.ToSingle(data.buffer, data.offset);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out Vector3 position, out Quaternion rotation, out float forward, out float right, out int anim)
    {
        // Debug.Log("UNPACKINHGG");
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(string.Format("temp = {0:x}", serial));

        position.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.x);

        position.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(position.y);

        position.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.z);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.w);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.x);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(rotation.y);

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.z);

        forward = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);

        right = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);

        anim = BitConverter.ToInt32(data.buffer, data.offset);
    }

    public void UnPackingData(RecvBuffer buffer, out int serial, out Vector3 position, out Quaternion rotation, out string Anim)
    {
        // Debug.Log("UNPACKINHGG");
        RecvBuffer data = buffer;
        data.offset = sizeof(UInt64);

        ////serial
        serial = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);
        //Debug.Log(string.Format("temp = {0:x}", serial));

        position.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.x);

        position.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(position.y);

        position.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(position.z);

        rotation.x = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.w);

        rotation.y = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.x);

        rotation.z = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        // Debug.Log(rotation.y);

        rotation.w = BitConverter.ToSingle(data.buffer, data.offset);
        data.offset += sizeof(float);
        //Debug.Log(rotation.z);

        int len = BitConverter.ToInt32(data.buffer, data.offset);
        data.offset += sizeof(int);

        Anim = Encoding.Default.GetString(data.buffer, data.offset, len);
    }


    // -------------------------------팩킹---------------------------------

    public byte[] PackingData(int serial, int value)
    {
        int size = sizeof(int) + sizeof(int);
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(value), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        return data.buffer;
    }

    public byte[] PackingData(int serial, float value)
    {
        int size = sizeof(int) + sizeof(float);
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(value), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        return data.buffer;
    }

    public byte[] PackingData(int serial, int value, int classNumber)
    {
        int size = sizeof(int) + sizeof(int) + sizeof(int);
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(value), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(classNumber), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        return data.buffer;
    }

    public byte[] PackingData(int serial, int value, int classNumber, int hitType)
    {
        int size = sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int);
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(value), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(classNumber), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(hitType), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        return data.buffer;
    }

    public byte[] PackingData(int serial, int value, string anim)
    {
        int size = sizeof(int) + sizeof(int) + (sizeof(int) + anim.Length);
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(value), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(anim.Length), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(Encoding.Default.GetBytes(anim.ToCharArray()), 0, data.buffer, data.offset, anim.Length);
        data.size += anim.Length;

        return data.buffer;
    }

    public byte[] PackingData(int serial, string anim)
    {
        int size = sizeof(int) + sizeof(int) + anim.Length;
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(anim.Length), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(Encoding.Default.GetBytes(anim.ToCharArray()), 0, data.buffer, data.offset, anim.Length);
        data.size += anim.Length;

        return data.buffer;
    }

    public byte[] PackingData(int serial, Quaternion rotation)
    {
        int size = sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4);
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        // serial

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        // Rotation

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);

        return data.buffer;
    }

    public byte[] PackingData(int serial, Vector3 position, Quaternion rotation)
    {
        int size = sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4);
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        // serial

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        // position

        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        // Rotation

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);

        return data.buffer;
    }

    public byte[] PackingData(int serial, int index, Vector3 position, Quaternion rotation)
    {
        int size = sizeof(int) + sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4);
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        // serial

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(index), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        // position

        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        // Rotation

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);

        return data.buffer;
    }

    public byte[] PackingData(int serial, Vector3 position, Quaternion rotation, int anim)
    {
        int size = sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4) + (sizeof(int));
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(anim), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);

        //print(anim);
        return data.buffer;
    }

    public byte[] PackingData(int serial, Vector3 position, Quaternion rotation, float anim)
    {
        int size = sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4) + (sizeof(float));
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(anim), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);

        //print(anim);
        return data.buffer;
    }

    // 무브타입, 스피드 팩킹
    public byte[] PackingData(int serial, Vector3 position, int moveType, float speed)
    {
        int size = sizeof(int) + (sizeof(float) * 3) + sizeof(int) + (sizeof(float));
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(moveType), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(speed), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);

        //print(anim);
        return data.buffer;
    }

    public byte[] PackingData(int serial, int index, Vector3 position, Quaternion rotation, float anim)
    {
        int size = sizeof(int) + sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4) + (sizeof(float));
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(index), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(anim), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);

        //print(anim);
        return data.buffer;
    }

    public byte[] PackingData(int serial, int index, Vector3 position, Quaternion rotation, int anim)
    {
        int size = sizeof(int) + sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4) + (sizeof(int));
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(index), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(anim), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);

        //print(anim);
        return data.buffer;
    }

    public byte[] PackingData(int serial, Vector3 position, Quaternion rotation, int state, int anim)
    {
        int size = sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4) + (sizeof(int) + sizeof(int));
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(state), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(anim), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);

        //print(anim);
        return data.buffer;
    }

    public byte[] PackingData(int serial, int index, Vector3 position, Quaternion rotation, int state, int anim)
    {
        int size = sizeof(int) + sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4) + (sizeof(int) + sizeof(int));
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(index), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);


        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(state), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(anim), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);

        //print(anim);
        return data.buffer;
    }

    public byte[] PackingData(int serial, int index, Vector3 position, Quaternion rotation, int state, int anim, int hitType)
    {
        int size = sizeof(int) + sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4) + (sizeof(int) + sizeof(int) + sizeof(int));
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(index), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);


        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(state), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(anim), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(hitType), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);

        //print(anim);
        return data.buffer;
    }

    public byte[] PackingData(int serial, Vector3 position, Quaternion rotation, float forward, float right)
    {
        int size = sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4) + (sizeof(float)) + (sizeof(float));
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(forward), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(right), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);

        //print(anim);
        return data.buffer;
    }

    public byte[] PackingData(int serial, Vector3 position, Quaternion rotation, float forward, float right, float speed)
    {
        int size = sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4) + (sizeof(float)) + (sizeof(float)) + (sizeof(float));
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(forward), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(right), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(speed), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);

        //print(anim);
        return data.buffer;
    }

    public byte[] PackingData(int serial, Vector3 position, Quaternion rotation, float forward, float right, int anim)
    {
        int size = sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4) + (sizeof(float)) + (sizeof(float)) + (sizeof(int));
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(forward), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(right), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(anim), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);

        //print(anim);
        return data.buffer;
    }

    public byte[] PackingData(int serial, Vector3 position, Quaternion rotation, string Anim)
    {
        int size = sizeof(int) + (sizeof(float) * 3) + (sizeof(float) * 4) + sizeof(int) + Anim.Length;
        SendBuffer data = new SendBuffer();
        data.buffer = new byte[size];
        data.size = 0;
        data.offset = 0;

        Buffer.BlockCopy(BitConverter.GetBytes(serial), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(BitConverter.GetBytes(position.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(position.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.x), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.y), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.z), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(rotation.w), 0, data.buffer, data.offset, sizeof(float));
        data.size += sizeof(float);
        data.offset += sizeof(float);

        Buffer.BlockCopy(BitConverter.GetBytes(Anim.Length), 0, data.buffer, data.offset, sizeof(int));
        data.size += sizeof(int);
        data.offset += sizeof(int);

        Buffer.BlockCopy(Encoding.Default.GetBytes(Anim.ToCharArray()), 0, data.buffer, data.offset, Anim.Length);
        data.size += Anim.Length;

        return data.buffer;
    }
}
