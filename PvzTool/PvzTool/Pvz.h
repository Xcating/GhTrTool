#pragma once

#define GAME_NAME L"Plants vs. Zombies"

class CPvz
{
public:
    CPvz();
    ~CPvz();
    // �޸������ֵ
    VOID ModifySunValue(DWORD dwSun);

private:
    // ��ȡ��Ϸ�� PID
    DWORD GetGamePid();
public:
    // ��ֲ��������
    VOID SunNop();
    // ��ֲ����ȴ
    VOID NoCd();
    // �޸Ľ��
    VOID ModifyCoinValue(DWORD dwCoin);
    // �ظ����죬���軨�衢�����Ҷ
    VOID Build();
    // �Զ��ռ�����
    VOID Auto();
    // ���ٹ���
    VOID Speed();
    // ����������
    VOID More();
    // ȫ����ʬ
    VOID AllScreen();
    // ȡ��ȫ����ʬ
    VOID UnAllScreen();
    // ��ʬ����״̬
    VOID Cool();
    // ��ʬ����״̬
    VOID Stop();
    // ��ʬ����
    VOID Dead();
    // ����
    VOID Attract();
};

