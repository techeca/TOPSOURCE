print('Loading guildvars.lua')

Guild_ReqId = 1780
Guild1_Gold = 10000000
Guild2_Gold = Guild1_Gold

emGldPermSpeak = 1		
emGldPermMgr = 2	
emGldPermViewBank = 4	
emGldPermDepoBank = 8	
emGldPermTakeBank = 16
emGldPermRecruit = 32	
emGldPermKick = 64		
emGldPermMotto = 128		
emGldPermAttr = 256	
emGldPermEnter = 512
emGldPermPlace = 1024
emGldPermRem = 2048
emGldPermDisband = 4096
emGldPermLeader = 8192

emGldPermMax = 0xFFFFFFFF
emGldPermDefault = 1
emGldPermNum = 14

function PushToGuildBank(role,item)
	local pkt = GetPacket()
	WriteCmd(pkt, 5531);
	WriteString(pkt, CreateItemString(item));
	SendPacket(role, pkt);
end

function hasGuildPerm(role,...)
	local perms = {...}
	local perm = 0
	for i,v in pairs(perms) do
		perm = bitwiseOr(perm,v)
	end
	local chaPerm = GetChaGuildPermission(role)
	local hasPerm = bitwiseAnd(chaPerm,perm)
	return hasPerm == perm
end

function bitwiseOr(a,b)
	local a2 = toBits(a, 32)
	local b2 = toBits(b, 32)
	local c = ""
	for i = 1,32 do
		if a2:sub(i,i) == "1" or b2:sub(i,i) == "1" then
			c = c.."1"
		else
			c = c.."0"
		end
	end
	return tonumber(c,2)
end

function bitwiseAnd(a,b)
	local a2 = toBits(a, 32)
	local b2 = toBits(b, 32)
	
	local c = ""
	
	for i = 1,32 do
		if a2:sub(i,i) == b2:sub(i,i) then
			c = c..a2:sub(i,i)
		else
			c = c.."0"
		end
	end
	return tonumber(c,2)
end

--https://stackoverflow.com/questions/9079853/lua-print-integer-as-a-binary/26702880#26702880
function toBits(num, bits)
    -- returns a table of bits
    local t={} -- will contain the bits
    for b=bits,1,-1 do
        rest=math.fmod(num,2)
        t[b]=rest
        num=math.floor((num-rest)/2)
    end
    if num==0 then 
		return table.concat(t) 
	else
		return emGldPermMax 
	end
end
