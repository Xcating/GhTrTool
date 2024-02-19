#pragma once

#define GAME_NAME L"Plants Vs Zombies GhTr ~ Perfect Voyage ver.0.16k"

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
    // �޸ı���id
    VOID ModifyBGIdValue(DWORD dwBGId);
    // �ظ����졢�����Ҷ
    VOID Build();
    // �Զ��ռ�����
    VOID Auto();
    // ֡��
    VOID Card();
    // ����������
    VOID Fast();
    // ���õĸ߼���ͣ
    VOID TheWorld();
    // û�м�����ȴ
    VOID NoModelCD();
    // �ϰ����Զ���ʧ
    VOID NoBuildTIme();
    // �㶹���ӵ�֡��
    VOID PeaSDamage();
    //С�Ƴ�����
    VOID Mowers();
    //û����������
    VOID NoSunMax();
    // ��ʬ�������俨Ƭ
    VOID ZombieDC();
    // ֲ�ﲻ���Ȼ�
    VOID NotSubvert();
    // ֲ���޵�ģʽ
    VOID GodMode();
    //��³˹��������
    VOID LoursMC();
    // �ٹ�ܾ�����
    VOID Point();
    // ����ؿ��޸��ٹ�
    VOID Point2();
    // ��޺������ֲ
    VOID DX();
    // ӣ��˲ը
    VOID CherryFast();
    // ӣ�Ҳ�ը
    VOID CherryNo();
    //è˿�����ӳ�
    VOID MeowFast();
    //�ٻ���������
    VOID SummonCup();
};

