#pragma once

#define GAME_NAME L"Plants Vs Zombies GhTr ~ Perfect Voyage ver.0.16l"

class CPvz
{
public:
    CPvz();
    ~CPvz();
    VOID WriteConfig();
    // 修改阳光的值
    VOID ModifySunValue(DWORD dwSun);
    //修改卡槽数量
    VOID SeedPacket(DWORD dwSP);
    //修改卡槽内容
    VOID ModifySeedPacket(DWORD dwID,DWORD dwNum);
    //修改存档难度
    VOID DifficultySwitcher(DWORD dwDiff);
    //种植植物
    VOID Plant(DWORD dwXP, DWORD dwYP,DWORD dwID);

private:
    // 获取游戏的 PID
    DWORD GetGamePid();
public:
    // 种植不减阳光
    VOID SunNop(bool dwSwitch);
    // 种植免冷却
    VOID NoCd(bool dwSwitch);
    // 修改背景id
    VOID ModifyBGIdValue(DWORD dwBGId);
    // 重复建造、无需荷叶
    VOID Build(bool dwSwitch);
    // 自动收集阳光
    VOID Auto(bool dwSwitch);
    // 帧伤
    VOID Card(bool dwSwitch);
    // 加速阳光金币
    VOID Fast(bool dwSwitch);
    // 更好的高级暂停
    VOID TheWorld(bool dwSwitch);
    // 没有技能冷却
    VOID NoModelCD(bool dwSwitch);
    // 障碍物自动消失
    VOID NoBuildTime(bool dwSwitch);
    // 豌豆类子弹帧伤
    VOID PeaSDamage(bool dwSwitch);
    //小推车不动
    VOID Mowers(bool dwSwitch);
    //没有阳光限制
    VOID NoSunMax(bool dwSwitch);
    // 僵尸死亡掉落卡片
    VOID ZombieDC(bool dwSwitch);
    // 植物不被魅惑
    VOID NotSubvert(bool dwSwitch);
    // 植物无敌模式
    VOID GodMode(bool dwSwitch);
    //荷鲁斯刀刀暴击
    VOID LoursMC(bool dwSwitch);
    // 荣光拒绝骄傲
    VOID Point(bool dwSwitch);
    // 进入关卡修改荣光
    VOID Point2(bool dwSwitch);
    // 导藓批量种植
    VOID DX(bool dwSwitch);
    // 樱桃瞬炸
    VOID CherryFast(bool dwSwitch);
    // 樱桃不炸
    VOID CherryNo(bool dwSwitch);
    //猫丝子无延迟
    VOID MeowFast(bool dwSwitch);
    //召唤奖杯过关
    VOID SummonCup();
    //忽略阳光种植
    VOID IgnoreSun(bool dwSwitch);
    //光菱角帧伤
    VOID LingSDamage(bool dwSwitch);
    //苹果鼓瑟手无冷却
    VOID ApplayerNoCD(bool dwSwitch);
    //苹果鼓瑟手无延迟
    VOID ApplayerNoLag(bool dwSwitch);
    //车前草无冷却
    VOID PlantageNoCD(bool dwSwitch);
    //向日葵产出无冷却
    VOID SunFlowerNoCD(bool dwSwitch);
    //豌豆无冷却
    VOID PeaNoCD(bool dwSwitch);
    //超级闪电芦苇
    VOID SuperReed(bool dwSwitch);
    //牡丹无冷却
    VOID PowerFlowerNoCD(bool dwSwitch);
    //牡丹永远最大
    VOID AwayMax(bool dwSwitch);
    //牡丹永远最大
    VOID ItemNoDie(bool dwSwitch);
    //阳光无延迟生成
    VOID SunNoDelay(bool dwSwitch);
    //一键布阵
    VOID BuildTheArray();
    //清空僵尸
    VOID ClearPlant();
    //清空植物
    VOID ClearZombie();
    //清空子弹
    VOID ClearBullet();
    //修复崩溃bug
    VOID FixCrashBug();
    //切换至导向线
    VOID ToDaoXiangJi();
    //切换至红针线
    VOID ToHongZhen();
};

