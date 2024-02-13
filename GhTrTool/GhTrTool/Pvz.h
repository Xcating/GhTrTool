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
    // 修改金币
    VOID ModifyCoinValue(DWORD dwCoin);
    // 重复建造，无需花盆、无需荷叶
    VOID Build();
    // 自动收集阳光
    VOID Auto();
    // 僵尸掉卡
    VOID Card();
    // 加速阳光金币
    VOID More();
    // 全屏僵尸
    VOID AllScreen();
    // 取消全屏僵尸
    VOID UnAllScreen();
    // 僵尸冰冻状态
    VOID Cool();
    // 僵尸奶油状态
    VOID Stop();
    // 僵尸死亡
    VOID Dead();
    // 吸怪
    VOID Attract();
};

