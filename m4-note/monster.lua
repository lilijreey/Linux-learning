-- Generated automatically by m4 template file, Do not modify this file by hand. 

function  MonsterKilled_00051022(grpId, count)
    local player = GetPlayer()
    local task = player:getTaskMgr()
    local container = player:getItemContainer()
    
    if task:hasCompletedTaskNode(40001) and task:hasCompletedTaskNode(40001, 5) and not task:hasCompletedTaskNode(40001,6) then
	updateTaskNode(player, 40001, 6, {1,0} )
	player:playInstanceOK(1) -- 1 is 副本id
    end	
end

function  MonsterKilled_00050013(grpId, count)
    local player = GetPlayer()
    local task = player:getTaskMgr()
    local container = player:getItemContainer()
    
    if task:hasCompletedTaskNode(40001) and not task:hasCompletedTaskNode(40001, 1) then
	updateTaskNode(player, 40001, 1, {1,0} )
    end 
    if task:hasCompletedTaskNode(40001) and task:hasCompletedTaskNode(40001, 2) and not task:hasCompletedTaskNode(40001,3) then
	local currentValues = getTaskNode(player,40001,3);
	if table.getn(currentValues)==2 then
	    if currentValues[1]<3 then
		currentValues[1]=currentValues[1]+1;
	    else
		currentValues[2]=1;
		Log("Evan 40001 =3")
	    end
	    updateTaskNode(player, 40001,3, currentValues);
	end	
    end
end


function  MonsterKilled_00050006(grpId, count)
    local player = GetPlayer()
    local task = player:getTaskMgr()
    local container = player:getItemContainer()
    
    if task:hasCompletedTaskNode(40001) and not task:hasCompletedTaskNode(40001, 1) then
	updateTaskNode(player, 40001, 1, {1,0} )
    end
end

function  MonsterKilled_00050010(grpId, count)
    local player = GetPlayer()
    local task = player:getTaskMgr()
    local container = player:getItemContainer()
    
    if task:hasCompletedTaskNode(40001) and not task:hasCompletedTaskNode(40001, 1) then
	updateTaskNode(player, 40001, 1, {1,0} )
    end
end

function  MonsterKilled_00050011(grpId, count)
    local player = GetPlayer()
    local task = player:getTaskMgr()
    local container = player:getItemContainer()
    
    if task:hasCompletedTaskNode(40001) and not task:hasCompletedTaskNode(40001, 1) then
	updateTaskNode(player, 40001, 1, {1,0} )
    end
end

function  MonsterKilled_00050012(grpId, count)
    local player = GetPlayer()
    local task = player:getTaskMgr()
    local container = player:getItemContainer()
    
    if task:hasCompletedTaskNode(40001) and not task:hasCompletedTaskNode(40001, 1) then
	updateTaskNode(player, 40001, 1, {1,0} )
    end
end



local MonsterKillFunTable = {
    [50001] = MonsterKilled_00050001,
    [50002] = MonsterKilled_00050002,
    [50003] = MonsterKilled_00050003,
    [50004] = MonsterKilled_00050004,
    [50005] = MonsterKilled_00050005,
    [50006] = MonsterKilled_00050006,
    [50007] = MonsterKilled_00050007,
    [50008] = MonsterKilled_00050008,
    [50009] = MonsterKilled_00050009,
    [50010] = MonsterKilled_00050010,
    [50011] = MonsterKilled_00050011,
    [50013] = MonsterKilled_00050013,
    [50014] = MonsterKilled_00050014,
    [50015] = MonsterKilled_00050015,
    [51001] = MonsterKilled_00051001,
    [51002] = MonsterKilled_00051002,
    [51003] = MonsterKilled_00051003,
    [51004] = MonsterKilled_00051004,
    [51007] = MonsterKilled_00051007,
    [51022] = MonsterKilled_00051022,
    [51037] = MonsterKilled_00051037,
    --[50012] = MonsterKilled_00050012,
};

function MonsterKilled( grpId, id, count )
    if MonsterKillFunTable ~= nil and MonsterKillFunTable[id] ~= nil then
	return MonsterKillFunTable[id]( grpId, count );
    end
    return false;
end         
