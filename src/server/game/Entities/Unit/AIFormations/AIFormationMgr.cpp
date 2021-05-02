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

#include "AIFormationMgr.h"
#include "Containers.h"
#include "Log.h"

AIFormationMgr* AIFormationMgr::instance()
{
    static AIFormationMgr instance;
    return &instance;
}

void AIFormationMgr::LoadAIFormations()
{
    uint32 oldMSTime = getMSTime();
    _aiFormationSettingsMap.clear();
    _aiFormationMemberDataMap.clear();

    QueryResult result = WorldDatabase.Query("SELECT FormationLeaderSpawnId, FormationType, FormationRadius FROM creature_ai_formation_settings");

    if (!result)
        return;

    _aiFormationSettingsMap.reserve(result->GetRowCount());
    do
    {
        Field* fields = result->Fetch();

        uint32 leaderSpawnId = fields[0].GetUInt32();
        AIFormationSettings& formationSettings = _aiFormationSettingsMap[leaderSpawnId];
        formationSettings.FormationType = fields[1].GetUInt8();
        formationSettings.FormationRadius = fields[2].GetFloat();

    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u AI formation settings in %u ms", uint32(_aiFormationSettingsMap.size()), GetMSTimeDiffToNow(oldMSTime));

    oldMSTime = getMSTime();

    result = WorldDatabase.Query("SELECT FormationLeaderSpawnId, FormationMemberSpawnId, FormationIndex FROM creature_ai_formation_members");

    if (!result)
        return;

    _aiFormationMemberDataMap.reserve(result->GetRowCount());
    do
    {
        Field* fields = result->Fetch();

        uint32 leaderSpawnId = fields[0].GetUInt32();
        uint32 memberSpawnId = fields[1].GetUInt32();
        uint8 FormationIndex = fields[2].GetUInt8();

        AIFormationMemberData& memberData = _aiFormationMemberDataMap[memberSpawnId];
        memberData.LeaderSpawnId = leaderSpawnId;
        memberData.FormationIndex = FormationIndex;

        std::vector<ObjectGuid::LowType>& memberGuids = _aiFormationMembersSpawnIdMap[leaderSpawnId];
        memberGuids.push_back(memberSpawnId);

    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u AI formation member data in %u ms", uint32(_aiFormationMemberDataMap.size()), GetMSTimeDiffToNow(oldMSTime));
}

AIFormationSettings const* AIFormationMgr::GetAIFormationSettingsForSpawnId(uint32 spawnId) const
{
    return Trinity::Containers::MapGetValuePtr(_aiFormationSettingsMap, spawnId);
}

AIFormationMemberData const* AIFormationMgr::GetAIFormationMemberDataForSpawnId(uint32 spawnId) const
{
    return Trinity::Containers::MapGetValuePtr(_aiFormationMemberDataMap, spawnId);
}

std::vector<ObjectGuid::LowType> const* AIFormationMgr::GetAIFormationMemberGUIDsForSpawnId(uint32 spawnId) const
{
    return Trinity::Containers::MapGetValuePtr(_aiFormationMembersSpawnIdMap, spawnId);
}
