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
    void ModifyEnableFrameDamageNumber(DWORD dwSP);
    //修改卡槽内容
    void ModifyEnableFrameDamageData(DWORD dwID,DWORD dwNum);
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
    void PlantWithoutSunReduction(bool isFeatureEnabled);
    // 种植免冷却
    void DisableCooldowns(bool isFeatureEnabled);
    // 修改背景id
    void ChangeBackgroundID(DWORD dwBGId);
    // 重复建造、无需荷叶
    void Build(bool isFeatureEnabled);
    // 自动收集阳光
    void EnableAutoCollectSun(bool isFeatureEnabled);
    // 帧伤
    void EnableFrameDamage(bool isFeatureEnabled);
    // 加速阳光金币
    void Fast(bool isFeatureEnabled);
    // 更好的高级暂停
    void TheWorld(bool isFeatureEnabled);
    // 没有技能冷却
    void DisableSkillCooldown(bool isFeatureEnabled);
    // 障碍物自动消失
    void InstantObstacleDisappearance(bool isFeatureEnabled);
    // 豌豆类子弹帧伤
    void EnablePeashooterFrameDamage(bool isFeatureEnabled);
    //小推车不动
    void DisableLawnMowerMovement(bool isFeatureEnabled);
    //没有阳光限制
    void RemoveSunProductionLimit(bool isFeatureEnabled);
    // 僵尸死亡掉落卡片
    void ZombiesDropCardsOnDeath(bool isFeatureEnabled);
    // 植物不被魅惑
    void PreventPlantCharm(bool isFeatureEnabled);
    // 植物无敌模式
    void MakePlantsInvincible(bool isFeatureEnabled);
    //荷鲁斯刀刀暴击
    void EnableCriticalHitsForLorus(bool isFeatureEnabled);
    // 荣光拒绝骄傲
    void RefusePrideEffect(bool isFeatureEnabled);
    // 进入关卡修改荣光
    void RefusePrideEffect2(bool isFeatureEnabled);
    // 导藓批量种植
    void DX(bool isFeatureEnabled);
    // 樱桃瞬炸
    void InstantCherryBombExplosion(bool isFeatureEnabled);
    // 樱桃不炸
    void PreventCherryBombExplosion(bool isFeatureEnabled);
    //猫丝子无延迟
    void EnableInstantCascutaProjectile(bool isFeatureEnabled);
    //召唤奖杯过关
    void CompleteLevelWithTrophy();
    //忽略阳光种植
    void PlantWithoutSunCost(bool isFeatureEnabled);
    //光菱角帧伤
    void EnableOpticaltropFrameDamage(bool isFeatureEnabled);
    //苹果鼓瑟手无冷却
    void ApplayerDisableCooldowns(bool isFeatureEnabled);
    //苹果鼓瑟手无延迟
    void RemoveApplayerDelay(bool isFeatureEnabled);
    //车前草无冷却
    void PlantageDisableCooldowns(bool isFeatureEnabled);
    //向日葵产出无冷却
    void SunFlowerDisableCooldowns(bool isFeatureEnabled);
    //豌豆无冷却
    void PeaDisableCooldowns(bool isFeatureEnabled);
    //超级闪电芦苇
    void EnhanceLightningReedAbility(bool isFeatureEnabled);
    //牡丹无冷却
    void PowerFlowerDisableCooldowns(bool isFeatureEnabled);
    //牡丹永远最大
    void MaintainMaximumPowerPlantSize(bool isFeatureEnabled);
    //牡丹永远最大
    void PreventItemDeterioration(bool isFeatureEnabled);
    //阳光无延迟生成
    void InstantSunGeneration(bool isFeatureEnabled);
    int GenerateValidRandomID(const std::vector<int>& invalidIDs);
    //一键布阵
    void DeployFormationInstantly();
    //清空僵尸
    void RemoveAllPlants();
    //清空植物
    void RemoveAllZombies();
    //清空子弹
    void RemoveAllProjectiles();
    //修复崩溃bug
    void CorrectCrashIssue();
    //切换至导向线
    void SwitchToHomingThistleMode();
    //切换是否可以创建Ub存档
    void EnableCreationOfUbSaves();
    //显示难度提示信息
    void ShowDifficultyNotification(DWORD dwDiff);
    //切换至红针线
    void SwitchToRedStingerMode();
    //Ub不碎档
    void DisableUbSaveDestroy(bool isFeatureEnabled);
};

