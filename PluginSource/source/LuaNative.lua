--[[
    auto generated LuaNative Api
]]
local LuaNative = {}
    
---Wrapper of CaFtCreateBattle
---@param buffer string
---@param len number
---@return number result
CaFtCreateBattle = CaFtCreateBattle or function(buffer, len)
	LogError('CaFtCreateBattle not found')
end
---Wrapper of CaFtDestroyBattle
---@return boolean result
CaFtDestroyBattle = CaFtDestroyBattle or function()
	LogError('CaFtDestroyBattle not found')
end
---Wrapper of CaFtLoadConfig
---@param config_type number
---@param content string
---@param len number
---@return number result
CaFtLoadConfig = CaFtLoadConfig or function(config_type, content, len)
	LogError('CaFtLoadConfig not found')
end
---Wrapper of CaFtUnloadAllConfigs
CaFtUnloadAllConfigs = CaFtUnloadAllConfigs or function()
	LogError('CaFtUnloadAllConfigs not found')
end
---Wrapper of CaFtPointFree
---@param map number
---@param x number
---@param y number
---@return boolean result
CaFtPointFree = CaFtPointFree or function(map, x, y)
	LogError('CaFtPointFree not found')
end
---Wrapper of CaFtEnterBattle
---@param team number
---@return number result
CaFtEnterBattle = CaFtEnterBattle or function(team)
	LogError('CaFtEnterBattle not found')
end
---Wrapper of CaFtStartBattle
---@param team number
---@return number result
---@return string out_buffer
CaFtStartBattle = CaFtStartBattle or function(team)
	LogError('CaFtStartBattle not found')
end
---Wrapper of CaFtPrepareBattle
---@param team number
---@param buffer string
---@param len number
---@return number result
---@return string out_buffer
CaFtPrepareBattle = CaFtPrepareBattle or function(team, buffer, len)
	LogError('CaFtPrepareBattle not found')
end
---Wrapper of CaFtMoveTo
---@param buffer string
---@param len number
---@return number result
---@return string out_buffer
CaFtMoveTo = CaFtMoveTo or function(buffer, len)
	LogError('CaFtMoveTo not found')
end
---Wrapper of CaFtGetPathToPoint
---@param uuid number
---@param target_x number
---@param target_y number
---@return number result
---@return table out_buffer
CaFtGetPathToPoint = CaFtGetPathToPoint or function(uuid, target_x, target_y)
	LogError('CaFtGetPathToPoint not found')
end
---Wrapper of CaFtGetPathToEnemy
---@param uuid number
---@param target_id number
---@param skill_id number
---@param v_x number
---@param v_y number
---@return number result
---@return table out_buffer
CaFtGetPathToEnemy = CaFtGetPathToEnemy or function(uuid, target_id, skill_id, v_x, v_y)
	LogError('CaFtGetPathToEnemy not found')
end
---Wrapper of CaFtGetPathToSkillTarget
---@param uuid number
---@param skill_id number
---@param target_x number
---@param target_y number
---@param v_x number
---@param v_y number
---@return number result
---@return table out_buffer
CaFtGetPathToSkillTarget = CaFtGetPathToSkillTarget or function(uuid, skill_id, target_x, target_y, v_x, v_y)
	LogError('CaFtGetPathToSkillTarget not found')
end
---Wrapper of CaFtGetTargetDistance
---@param uuid number
---@param target_id number
---@return number result
CaFtGetTargetDistance = CaFtGetTargetDistance or function(uuid, target_id)
	LogError('CaFtGetTargetDistance not found')
end
---Wrapper of CaFtGetDistanceTo
---@param uuid number
---@param target_x number
---@param target_y number
---@param path_distance boolean
---@return number result
CaFtGetDistanceTo = CaFtGetDistanceTo or function(uuid, target_x, target_y, path_distance)
	LogError('CaFtGetDistanceTo not found')
end
---Wrapper of CaFtBattleAiUpdate
---@return boolean result
CaFtBattleAiUpdate = CaFtBattleAiUpdate or function()
	LogError('CaFtBattleAiUpdate not found')
end
---Wrapper of CaFtCastSkill
---@param buffer string
---@param len number
---@return number result
---@return string out_buffer
CaFtCastSkill = CaFtCastSkill or function(buffer, len)
	LogError('CaFtCastSkill not found')
end
---Wrapper of CaFtBattleUpdateTurn
CaFtBattleUpdateTurn = CaFtBattleUpdateTurn or function()
	LogError('CaFtBattleUpdateTurn not found')
end
---Wrapper of CaFtTakeRecords
---@return number result
---@return string out_buffer
CaFtTakeRecords = CaFtTakeRecords or function()
	LogError('CaFtTakeRecords not found')
end
---Wrapper of CaFtGetRoleAttribute
---@param uuid number
---@param attr number
---@return number result
CaFtGetRoleAttribute = CaFtGetRoleAttribute or function(uuid, attr)
	LogError('CaFtGetRoleAttribute not found')
end
---Wrapper of CaFtSkillPredict
---@param buffer string
---@param len number
---@return number result
---@return string out_buffer
CaFtSkillPredict = CaFtSkillPredict or function(buffer, len)
	LogError('CaFtSkillPredict not found')
end
---Wrapper of CaFtReqEvent
---@param buffer string
---@param len number
---@return number result
---@return string out_buffer
CaFtReqEvent = CaFtReqEvent or function(buffer, len)
	LogError('CaFtReqEvent not found')
end
---Wrapper of CaFtCheckSkillValid
---@param uuid number
---@param skill_id number
---@param cast_pos number
---@param target_id number
---@param point_target boolean
---@return number result
CaFtCheckSkillValid = CaFtCheckSkillValid or function(uuid, skill_id, cast_pos, target_id, point_target)
	LogError('CaFtCheckSkillValid not found')
end
---Wrapper of CaFtStatusSynchronize
---@param buffer string
---@param len number
---@return number result
CaFtStatusSynchronize = CaFtStatusSynchronize or function(buffer, len)
	LogError('CaFtStatusSynchronize not found')
end
---Wrapper of CaFtGetValidTargets
---@param uuid number
---@param skill_id number
---@return number result
---@return table targets
CaFtGetValidTargets = CaFtGetValidTargets or function(uuid, skill_id)
	LogError('CaFtGetValidTargets not found')
end
---Wrapper of CaSetRustLogLevel
---@param log_level number
---@return boolean result
CaSetRustLogLevel = CaSetRustLogLevel or function(log_level)
	LogError('CaSetRustLogLevel not found')
end
---Wrapper of CaMainUpdate
CaMainUpdate = CaMainUpdate or function()
	LogError('CaMainUpdate not found')
end
---Wrapper of CaRsOnApplicationQuit
CaRsOnApplicationQuit = CaRsOnApplicationQuit or function()
	LogError('CaRsOnApplicationQuit not found')
end
---Wrapper of CaTimestampMills
---@return number result
CaTimestampMills = CaTimestampMills or function()
	LogError('CaTimestampMills not found')
end
---Wrapper of CaReleaseVector
---@param ptr table
---@param size number
---@param type_size number
---@param cap number
CaReleaseVector = CaReleaseVector or function(ptr, size, type_size, cap)
	LogError('CaReleaseVector not found')
end
---Wrapper of CaExpInverseDirection
---@param dx number
---@param dy number
---@return number x
---@return number y
CaExpInverseDirection = CaExpInverseDirection or function(dx, dy)
	LogError('CaExpInverseDirection not found')
end
---Wrapper of CaIsBaseDir
---@param x number
---@param y number
---@return boolean result
CaIsBaseDir = CaIsBaseDir or function(x, y)
	LogError('CaIsBaseDir not found')
end
---Wrapper of CaGetHexId
---@param x number
---@param y number
---@param map number
---@return number result
CaGetHexId = CaGetHexId or function(x, y, map)
	LogError('CaGetHexId not found')
end
---Wrapper of CaGetHexPosition
---@param id number
---@param map number
---@return number x
---@return number y
CaGetHexPosition = CaGetHexPosition or function(id, map)
	LogError('CaGetHexPosition not found')
end
---Wrapper of CaGetMapSize
---@param map number
---@return number result
CaGetMapSize = CaGetMapSize or function(map)
	LogError('CaGetMapSize not found')
end
---Wrapper of CaExpDestroyMap
CaExpDestroyMap = CaExpDestroyMap or function()
	LogError('CaExpDestroyMap not found')
end
---Wrapper of CaExpCreateMap
---@param width number
---@param height number
---@param barrier_ptr table
---@param barrier_count number
---@return boolean result
CaExpCreateMap = CaExpCreateMap or function(width, height, barrier_ptr, barrier_count)
	LogError('CaExpCreateMap not found')
end
---Wrapper of CaExpFindPath
---@param start_x number
---@param start_y number
---@param target_x number
---@param target_y number
---@param distance number
---@return number result
---@return table out_buffer
CaExpFindPath = CaExpFindPath or function(start_x, start_y, target_x, target_y, distance)
	LogError('CaExpFindPath not found')
end
---Wrapper of CaExpUpdateGridState
---@param grid number
---@param block boolean
---@param explore_map boolean
CaExpUpdateGridState = CaExpUpdateGridState or function(grid, block, explore_map)
	LogError('CaExpUpdateGridState not found')
end
---Wrapper of CaExpGetDistance
---@param x number
---@param y number
---@param target_x number
---@param target_y number
---@param path_distance boolean
---@return number result
CaExpGetDistance = CaExpGetDistance or function(x, y, target_x, target_y, path_distance)
	LogError('CaExpGetDistance not found')
end
---Wrapper of CaExpRemoveFogs
---@param list table
---@param list_count number
CaExpRemoveFogs = CaExpRemoveFogs or function(list, list_count)
	LogError('CaExpRemoveFogs not found')
end
---Wrapper of CaExpAddFogs
---@param list table
---@param list_count number
CaExpAddFogs = CaExpAddFogs or function(list, list_count)
	LogError('CaExpAddFogs not found')
end
---Wrapper of CaExpAddAllFogsExceptDistance
---@param x number
---@param y number
---@param distance number
---@return table out_buffer
CaExpAddAllFogsExceptDistance = CaExpAddAllFogsExceptDistance or function(x, y, distance)
	LogError('CaExpAddAllFogsExceptDistance not found')
end
---Wrapper of CaExpClearAllFogs
CaExpClearAllFogs = CaExpClearAllFogs or function()
	LogError('CaExpClearAllFogs not found')
end
---Wrapper of CaGetSessionState
---@param session_id number
---@return number result
CaGetSessionState = CaGetSessionState or function(session_id)
	LogError('CaGetSessionState not found')
end
---native 接口测试
function LuaNative:Test()
    CaSetRustLogLevel(4);
    CaMainUpdate()
    Log("LuaNative CaIsBaseDir", CaIsBaseDir(1, 0))
    local x, y = CaExpInverseDirection(1, 0)
    Log("LuaNative CaExpInverseDirection", x, y)
    Log("LuaNative CaExpCreateMap", CaExpCreateMap(10, 10, {11, 42, 32, 14, 56}, 5))
    local tb = {}
    for i = 1, 120 do
        table.insert(tb, i)
    end
    Log("CaExpAddExploredPoints")
    --CaExpAddExploredPoints(tb, 120);
    local ret, point = CaExpFindPath(1, 1, 3, 4, 2)
    Log("LuaNative CaExpFindPath", ret);
    LogTable(point, "path")
    Log("LuaNative CaTimestampMills", CaTimestampMills())
end
Normalize(LuaNative)
return LuaNative