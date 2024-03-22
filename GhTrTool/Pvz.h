#pragma once
#include <filesystem>
#include "json.hpp"
#define GAME_NAME L"Plants Vs Zombies GhTr ~ Perfect Voyage ver.0.16l"

class CPvz
{
public:
    CPvz();
    ~CPvz();
    std::filesystem::path GetConfigFilePath();
    BOOL check_battlefield(DWORD dwPid);
    VOID WriteConfig();
    // 修改阳光的值
    VOID ModifySunValue(DWORD dwSun);
    //修改卡槽数量
    VOID SeedPacket(DWORD dwSP);
    //修改卡槽内容
    VOID ModifySeedPacket(DWORD dwID,DWORD dwNum);
    //修改存档难度
    VOID DifficultySwitcher(DWORD dwDiff);
    //修改存档难度
    VOID ConvertToWiki(CString RawData);
    //种植植物
    VOID Plant(DWORD dwXP, DWORD dwYP,DWORD dwID);
    //检查pid合法性
    BOOL check_dwPid(DWORD dwPid, BOOL isMessage);
    // 获取游戏的 PID
    DWORD GetGamePid();
private:
    nlohmann::json ReadConfigFile(const std::filesystem::path& configFilePath);
    void WriteConfigFile(const std::filesystem::path& configFilePath, const nlohmann::json& configJson);
    DWORD ReadMemory(HANDLE hProcess, DWORD address);
    BOOL WriteByteMemory(HANDLE hProcess, DWORD address, DWORD value);
public:
    // 种植不减阳光
    VOID SunNop(bool isFeatureEnabled);
    // 种植免冷却
    VOID NoCd(bool isFeatureEnabled);
    // 修改背景id
    VOID ModifyBGIdValue(DWORD dwBGId);
    // 重复建造、无需荷叶
    VOID Build(bool isFeatureEnabled);
    // 自动收集阳光
    VOID Auto(bool isFeatureEnabled);
    // 帧伤
    VOID Card(bool isFeatureEnabled);
    // 加速阳光金币
    VOID Fast(bool isFeatureEnabled);
    // 更好的高级暂停
    VOID TheWorld(bool isFeatureEnabled);
    // 没有技能冷却
    VOID NoModelCD(bool isFeatureEnabled);
    // 障碍物自动消失
    VOID NoBuildTime(bool isFeatureEnabled);
    // 豌豆类子弹帧伤
    VOID PeaSDamage(bool isFeatureEnabled);
    //小推车不动
    VOID Mowers(bool isFeatureEnabled);
    //没有阳光限制
    VOID NoSunMax(bool isFeatureEnabled);
    // 僵尸死亡掉落卡片
    VOID ZombieDC(bool isFeatureEnabled);
    // 植物不被魅惑
    VOID NotSubvert(bool isFeatureEnabled);
    // 植物无敌模式
    VOID GodMode(bool isFeatureEnabled);
    //荷鲁斯刀刀暴击
    VOID LoursMC(bool isFeatureEnabled);
    // 荣光拒绝骄傲
    VOID Point(bool isFeatureEnabled);
    // 进入关卡修改荣光
    VOID Point2(bool isFeatureEnabled);
    // 导藓批量种植
    VOID DX(bool isFeatureEnabled);
    // 樱桃瞬炸
    VOID CherryFast(bool isFeatureEnabled);
    // 樱桃不炸
    VOID CherryNo(bool isFeatureEnabled);
    //猫丝子无延迟
    VOID MeowFast(bool isFeatureEnabled);
    //召唤奖杯过关
    VOID SummonCup();
    //忽略阳光种植
    VOID IgnoreSun(bool isFeatureEnabled);
    //光菱角帧伤
    VOID LingSDamage(bool isFeatureEnabled);
    //苹果鼓瑟手无冷却
    VOID ApplayerNoCD(bool isFeatureEnabled);
    //苹果鼓瑟手无延迟
    VOID ApplayerNoLag(bool isFeatureEnabled);
    //车前草无冷却
    VOID PlantageNoCD(bool isFeatureEnabled);
    //向日葵产出无冷却
    VOID SunFlowerNoCD(bool isFeatureEnabled);
    //豌豆无冷却
    VOID PeaNoCD(bool isFeatureEnabled);
    //超级闪电芦苇
    VOID SuperReed(bool isFeatureEnabled);
    //牡丹无冷却
    VOID PowerFlowerNoCD(bool isFeatureEnabled);
    //牡丹永远最大
    VOID AwayMax(bool isFeatureEnabled);
    //牡丹永远最大
    VOID ItemNoDie(bool isFeatureEnabled);
    //阳光无延迟生成
    VOID SunNoDelay(bool isFeatureEnabled);
    int GenerateValidRandomID(const std::vector<int>& invalidIDs);
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
    //显示难度提示信息
    void ShowDiffBox(DWORD dwDiff);
    //切换至红针线
    VOID ToHongZhen();
    //Ub不碎档
    VOID NoUbBroken(bool isFeatureEnabled);
};

