#pragma once

#define GAME_NAME L"Plants Vs Zombies GhTr ~ Perfect Voyage ver.0.16k"

class CPvz
{
public:
    CPvz();
    ~CPvz();
    // 修改阳光的值
    VOID ModifySunValue(DWORD dwSun);

private:
    // 获取游戏的 PID
    DWORD GetGamePid();
public:
    // 种植不减阳光
    VOID SunNop();
    // 种植免冷却
    VOID NoCd();
    // 修改背景id
    VOID ModifyBGIdValue(DWORD dwBGId);
    // 重复建造、无需荷叶
    VOID Build();
    // 自动收集阳光
    VOID Auto();
    // 帧伤
    VOID Card();
    // 加速阳光金币
    VOID Fast();
    // 更好的高级暂停
    VOID TheWorld();
    // 没有技能冷却
    VOID NoModelCD();
    // 障碍物自动消失
    VOID NoBuildTIme();
    // 豌豆类子弹帧伤
    VOID PeaSDamage();
    //小推车不动
    VOID Mowers();
    //没有阳光限制
    VOID NoSunMax();
    // 僵尸死亡掉落卡片
    VOID ZombieDC();
    // 植物不被魅惑
    VOID NotSubvert();
    // 植物无敌模式
    VOID GodMode();
    //荷鲁斯刀刀暴击
    VOID LoursMC();
    // 荣光拒绝骄傲
    VOID Point();
    // 进入关卡修改荣光
    VOID Point2();
    // 导藓批量种植
    VOID DX();
    // 樱桃瞬炸
    VOID CherryFast();
    // 樱桃不炸
    VOID CherryNo();
    //猫丝子无延迟
    VOID MeowFast();
    //召唤奖杯过关
    VOID SummonCup();
};

