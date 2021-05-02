/*
* This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AI_FORMATION_MGR_H
#define AI_FORMATION_MGR_H

#include "Common.h"

struct AIFormationSettings
{
    uint8 FormationType;
    float FormationRadius;
};

struct AIFormationMemberData
{
    uint32 LeaderSpawnId;
    uint8 FormationIndex;
};

class TC_GAME_API AIFormationMgr
{
private:
    AIFormationMgr() { }
    ~AIFormationMgr() { }

public:
    static AIFormationMgr* instance();

    void LoadAIFormations();

    AIFormationSettings const* GetAIFormationSettingsForSpawnId(uint32 spawnId) const;
    AIFormationMemberData const* GetAIFormationMemberDataForSpawnId(uint32 spawnId) const;
    std::vector<ObjectGuid::LowType> const* GetAIFormationMemberGUIDsForSpawnId(uint32 spawnId) const;

private:
    std::unordered_map<uint32 /*leaderSpawnId*/, AIFormationSettings> _aiFormationSettingsMap;
    std::unordered_map<uint32 /*memberSpawnId*/, AIFormationMemberData> _aiFormationMemberDataMap;
    std::unordered_map < uint32 /*leaderSpawnId*/, std::vector<ObjectGuid::LowType>> _aiFormationMembersSpawnIdMap;
};

#define sAIFormationMgr AIFormationMgr::instance()

#endif // AI_FORMATION_MGR_H
