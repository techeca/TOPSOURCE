print("loading antidupe by Billy because Kongs one does literally nothing.")

--dofile(GetResPath('../lua/plugin/antidupe.lua')) in addHelpNpc.lua

local ADmaps = {
	"garner",
	"teampk",
	"puzzleworld",
	"puzzleworld2",
	"lonetower",
	"PKmap",
	"darkblue",
	"garner2",
	"eastgoaf",
	"magicsea",
	"abandonedcity",
	"abandonedcity2",
	"abandonedcity3",
	"darkswamp",
	"jialebi",
}

function GetChaTrueName(role)
	return GetChaDefaultName(TurnToCha(role))
end

function LogData(files,strings)
	local filename = files
	local file = "GSLog/"..files
	local LogFile = io.open(file,'a')
	LogFile:write("["..os.date().."]\t"..strings.."\n")
	LogFile:close()	
end

function LuaAll(cmd,role)
	local packet = GetPacket()
	WriteCmd( packet, 4015 )
	WriteDword( packet, 0 )
	WriteString(packet,cmd)
	SendPacket( role, packet )
end

function CheckDuper(role)
	local cmd = string.format("DetectDuper('%s','%s')",GetChaTrueName(role),GetChaMapName(role))
	LuaAll(cmd,role)
end

function DetectDuper(name,map)
	local role = GetPlayerByName(name) 
	if role and GetChaMapName(role) ~= map then
		PunishDuper(role)
	end
end

function DeleteClone(role)
	GoTo(role,0,0,"garner")
	KickCha(role)
end

function PunishDuper(role)
	BanActRole(role)
	local cmd = string.format("DeleteClone(GetPlayerByName('%s'))",GetChaTrueName(role))
	LuaAll(cmd,role)
	LogData("dupe.log",string.format("%s was found dupeing. ",GetChaTrueName(role)))
end

for i,v in pairs(ADmaps) do
	if not _G["map_run_"..v] then
		_G["map_run_"..v] = function(role) end
	end
	
	local AfterEnter = string.format("after_enter_%s",v)
	
	if not _G[AfterEnter] then
		_G[AfterEnter] = function(role) end
	end
	
	local hookName = "HOOK"..AfterEnter
	_G[hookName] = _G[hookName] or _G[AfterEnter] 
	_G[AfterEnter] = function(role)
		CheckDuper(role)
		_G[hookName](role)
	end
	

end

function CreateClone(role) 
	if type(role) == "string" then
		role = GetPlayerByName(role)
	end
	local packet = GetPacket() 
	WriteCmd( packet, 516 )
	WriteString( packet, 'e')
	WriteString( packet, 'e')
	SendPacket( role, packet ) 
end 