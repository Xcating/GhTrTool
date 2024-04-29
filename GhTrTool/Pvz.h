#pragma once
#include <filesystem>
#include "json.hpp"
#define GAME_NAME L"Plants Vs Zombies GhTr ~ Perfect Voyage ver.0.16m"

class GhTrManager
{
public:
    GhTrManager();
    ~GhTrManager();
    std::filesystem::path GetConfigFilePath();
    bool CheckLawn(DWORD dwPid);
    void WriteConfig();
    // 修改阳光的值
    void ModifySunValue(DWORD dwSun);
    //修改卡槽数量
    void ModifyCardNumber(DWORD dwSP);
    //修改卡槽内容
    void ModifyCardData(DWORD dwID,DWORD dwNum);
    //修改存档难度
    void DifficultySwitcher(DWORD dwDiff);
    //修改存档难度
    void ConvertToWiki(CString RawData);
    //种植植物
    void Plant(DWORD dwXP, DWORD dwYP,DWORD dwID);
    //检查pid合法性
    bool CheckGamePid(DWORD dw_pid, bool is_message);
    // 获取游戏的 PID
    DWORD GetGamePid();
private:
    nlohmann::json ReadConfigFile(const std::filesystem::path& configFilePath);
    void WriteConfigFile(const std::filesystem::path& configFilePath, const nlohmann::json& configJson);
    DWORD ReadTheMemory(HANDLE hProcess, DWORD address);
    bool WriteByteMemory(HANDLE hProcess, DWORD address, BYTE value);
public:
    // 种植不减阳光
    void PlantNoSubSun(bool isFeatureEnabled);
    // 种植免冷却
    void NoCd(bool isFeatureEnabled);
    // 修改背景id
    void ModifyBGIdValue(DWORD dwBGId);
    // 重复建造、无需荷叶
    void Build(bool isFeatureEnabled);
    // 自动收集阳光
    void Auto(bool isFeatureEnabled);
    // 帧伤
    void Card(bool isFeatureEnabled);
    // 加速阳光金币
    void Fast(bool isFeatureEnabled);
    // 更好的高级暂停
    void TheWorld(bool isFeatureEnabled);
    // 没有技能冷却
    void NoModelCD(bool isFeatureEnabled);
    // 障碍物自动消失
    void NoBuildTime(bool isFeatureEnabled);
    // 豌豆类子弹帧伤
    void PeaSDamage(bool isFeatureEnabled);
    //小推车不动
    void Mowers(bool isFeatureEnabled);
    //没有阳光限制
    void NoSunMax(bool isFeatureEnabled);
    // 僵尸死亡掉落卡片
    void ZombieDC(bool isFeatureEnabled);
    // 植物不被魅惑
    void NotSubvert(bool isFeatureEnabled);
    // 植物无敌模式
    void GodMode(bool isFeatureEnabled);
    //荷鲁斯刀刀暴击
    void LoursMC(bool isFeatureEnabled);
    // 荣光拒绝骄傲
    void Point(bool isFeatureEnabled);
    // 进入关卡修改荣光
    void Point2(bool isFeatureEnabled);
    // 导藓批量种植
    void DX(bool isFeatureEnabled);
    // 樱桃瞬炸
    void CherryFast(bool isFeatureEnabled);
    // 樱桃不炸
    void CherryNo(bool isFeatureEnabled);
    //猫丝子无延迟
    void MeowFast(bool isFeatureEnabled);
    //召唤奖杯过关
    void SummonCup();
    //忽略阳光种植
    void IgnoreSun(bool isFeatureEnabled);
    //光菱角帧伤
    void LingSDamage(bool isFeatureEnabled);
    //苹果鼓瑟手无冷却
    void ApplayerNoCD(bool isFeatureEnabled);
    //苹果鼓瑟手无延迟
    void ApplayerNoLag(bool isFeatureEnabled);
    //车前草无冷却
    void PlantageNoCD(bool isFeatureEnabled);
    //向日葵产出无冷却
    void SunFlowerNoCD(bool isFeatureEnabled);
    //豌豆无冷却
    void PeaNoCD(bool isFeatureEnabled);
    //超级闪电芦苇
    void SuperReed(bool isFeatureEnabled);
    //牡丹无冷却
    void PowerFlowerNoCD(bool isFeatureEnabled);
    //牡丹永远最大
    void AwayMax(bool isFeatureEnabled);
    //牡丹永远最大
    void ItemNoDie(bool isFeatureEnabled);
    //阳光无延迟生成
    void SunNoDelay(bool isFeatureEnabled);
    int GenerateValidRandomID(const std::vector<int>& invalidIDs);
    //一键布阵
    void BuildTheArray();
    //清空僵尸
    void ClearPlant();
    //清空植物
    void ClearZombie();
    //清空子弹
    void ClearBullet();
    //修复崩溃bug
    void FixCrashBug();
    //切换至导向线
    void ToDaoXiangJi();
    //切换是否可以创建Ub存档
    void ArrUb();
    //显示难度提示信息
    void ShowDiffBox(DWORD dwDiff);
    //切换至红针线
    void ToHongZhen();
    //Ub不碎档
    void NoUbBroken(bool isFeatureEnabled);
};

