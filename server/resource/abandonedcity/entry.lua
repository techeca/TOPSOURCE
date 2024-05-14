--���ļ��У����ǿ��ܱ����ִ�еĺ�������������Ҫ���ϵ�ͼ��ǰ׺����after_destroy_entry_testpk
--���ļ�ÿ������ַ�����Ϊ255���������飬�������̽��

function config_entry(entry) 
    SetMapEntryEntiID(entry, 2492,4) --���õ�ͼ���ʵ��ı�ţ��ñ�Ŷ�Ӧ��characterinfo.txt��������

end 

function after_create_entry(entry) 
    local copy_mgr = GetMapEntryCopyObj(entry, 0) --��������������󣬴˺���������ʽ��ڵĵ�ͼ�б�����ã�������ʽ��ڵĵ�ͼ���������ս����Ҫ���øýӿ�

    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) --ȡ��ͼ��ڵ�λ����Ϣ����ͼ�������꣬Ŀ���ͼ����
    Notice("Announcement: Magical Ocean ["..posx..","..posy.."] emerges a portal to [Forsaken City].") --֪ͨ������������������

end

function after_destroy_entry_abandonedcity(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
    Notice("Announcement: Magical Ocean ["..posx..","..posy.."] nearby has ceased. Remember to take note of the announcement!") 

end

function after_player_login_abandonedcity(entry, player_name)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) --ȡ��ͼ��ڵ�λ����Ϣ����ͼ�������꣬Ŀ���ͼ����
    ChaNotice(player_name, "Announcement: Magical Ocean ["..posx..","..posy.."] emerges a portal to [Forsaken City].") --֪ͨ������������������

end

--���ڼ���������
--����ֵ��0�����������������1���ɹ����롣
function check_can_enter_abandonedcity(role, copy_mgr)

	local Cha = TurnToCha(role)
  
	if Lv(Cha) >=30 and Lv(Cha) <=45 then
	
		local Num
		Num = CheckBagItem(Cha, 1812)
        	
		if Num >= 1 then
			return 1
	
		else
			SystemNotice(role, "Does not possess Ancient Generator.Unable to pass through Portal to Forsaken City")
			return 0
		end
		
	else
		SystemNotice(role, "Characters need to be between Lv 30 to 45 to enter Forsaken City")
		return 0    
	end
end



function begin_enter_abandonedcity(role, copy_mgr) 

	local Cha = TurnToCha(role)	
	local Dbag = 0
	Dbag = DelBagItem(Cha, 1812, 1)

	if Dbag == 1 then
		SystemNotice(role,"Entering [Forsaken City]") 
		MoveCity(role, "Forsaken City")

	else
	
		SystemNotice(role, "Acquire Ancient Generator failed. Unable to pass through Portal to Forsaken City")
	end


end