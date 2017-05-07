changecom('--')dnl
define(`DEF_MONSTER_KILLED_FUNC_BEGIN',`function  MonsterKilled_000`$1'(grpId, count)')dnl
dnl
define(`GET_PLAYER',`local `$1' = GetPlayer()')dnl
dnl
define(`GET_TASK',`local `$1' = `$2':getTaskMgr()')dnl --TODO check arguments
dnl
define(`GET_TIEM_CONTAINER',`local `$1' = `$2':getItemContainer()')dnl
dnl
define(`COMMON_FUNC_DEF_BEGIN', `DEF_MONSTER_KILLED_FUNC_BEGIN(`$1')
    GET_PLAYER(`player')
    GET_TASK(`task',`player')
    GET_TIEM_CONTAINER(`container',`player')')dnl
define(`MY_DEFINE', `define(`$1',`$2')')dnl
dnl
define(`IF_TASK_FIRST_NODE_NOT_COMPLETED',`if task:hasCompletedTaskNode(`$1') and not task:hasCompletedTaskNode(`$1', 1) then')dnl 
dnl
dnl 已经完成了指定任务，指定节点以前的所有任务
define(`IF_TASK_COMPLETED_BEFORE_NODE',`if task:hasCompletedTaskNode(`$1') and task:hasCompletedTaskNode(`$1', decr(`$2')) and not task:hasCompletedTaskNode(`$1',`$2') then')dnl 
define(`END', `end')dnl
dnl
dnl
dnl
dnl# __file__ , __line__ 内置宏
define(`LOG', print("__file__:__line__" ..`$1'))dnl
-- Generated automatically by m4 template file, Do not modify this file by hand. 



COMMON_FUNC_DEF_BEGIN(51022)
    MY_DEFINE(TASK_ID, 40001)
    IF_TASK_COMPLETED_BEFORE_NODE(TASK_ID, 6)
	updateTaskNode(player, 40001, 6, {1,0} )
	player:playInstanceOK(1) -- 1 is 副本id
    end	
end

COMMON_FUNC_DEF_BEGIN(50013)
    MY_DEFINE(TASK_ID, 30014)
    IF_TASK_FIRST_NODE_NOT_COMPLETED(TASK_ID)
	updateTaskNode(player, TASK_ID, 1, {1,0} )
    end 
dnl
    IF_TASK_COMPLETED_BEFORE_NODE(TASK_ID, 3)
	local currentValues = getTaskNode(player,TASK_ID,3);
	if table.getn(currentValues)==2 then
	    if currentValues[1]<3 then
		currentValues[1]=currentValues[1]+1;
	    else
		currentValues[2]=1;
		Log("Evan TASK_ID =3")
	    end
	    updateTaskNode(player, TASK_ID,3, currentValues);
	end	
    END
end


COMMON_FUNC_DEF_BEGIN(50006)
    MY_DEFINE(TASK_ID, 30007)
    IF_TASK_FIRST_NODE_NOT_COMPLETED(TASK_ID)
	updateTaskNode(player, TASK_ID, 1, {1,0} )
    END
END

COMMON_FUNC_DEF_BEGIN(50010)
    MY_DEFINE(TASK_ID, 30011)
    IF_TASK_FIRST_NODE_NOT_COMPLETED(TASK_ID)
	updateTaskNode(player, TASK_ID, 1, {1,0} )
    END
END

COMMON_FUNC_DEF_BEGIN(50011)
    MY_DEFINE(TASK_ID, 30012)
    IF_TASK_FIRST_NODE_NOT_COMPLETED(TASK_ID)
	updateTaskNode(player, TASK_ID, 1, {1,0} )
    END
END

COMMON_FUNC_DEF_BEGIN(50012)
    MY_DEFINE(TASK_ID, 30006)
    IF_TASK_FIRST_NODE_NOT_COMPLETED(TASK_ID)
	updateTaskNode(player, TASK_ID, 1, {1,0} )
    END
END


define(`REGISTER_MK_FUNC', `[`$1'] = MonsterKilled_000`$1'')dnl

local MonsterKillFunTable = {
    REGISTER_MK_FUNC(50001),
    REGISTER_MK_FUNC(50002),
    REGISTER_MK_FUNC(50003),
    REGISTER_MK_FUNC(50004),
    REGISTER_MK_FUNC(50005),
    REGISTER_MK_FUNC(50006),
    REGISTER_MK_FUNC(50007),
    REGISTER_MK_FUNC(50008),
    REGISTER_MK_FUNC(50009),
    REGISTER_MK_FUNC(50010),
    REGISTER_MK_FUNC(50011),
    REGISTER_MK_FUNC(50013),
    REGISTER_MK_FUNC(50014),
    REGISTER_MK_FUNC(50015),
    REGISTER_MK_FUNC(51001),
    REGISTER_MK_FUNC(51002),
    REGISTER_MK_FUNC(51003),
    REGISTER_MK_FUNC(51004),
    REGISTER_MK_FUNC(51007),
    REGISTER_MK_FUNC(51022),
    REGISTER_MK_FUNC(51037),
    --REGISTER_MK_FUNC(50012),
};

function MonsterKilled( grpId, id, count )
    LOG(33)
    if MonsterKillFunTable ~= nil and MonsterKillFunTable[id] ~= nil then
	return MonsterKillFunTable[id]( grpId, count );
    end
    return false;
end         
