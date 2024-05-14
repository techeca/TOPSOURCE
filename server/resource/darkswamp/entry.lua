--���ļ��У����ǿ��ܱ����ִ�еĺ�������������Ҫ���ϵ�ͼ��ǰ׺����after_destroy_entry_testpk
--���ļ�ÿ������ַ�����Ϊ255���������飬�������̽��

function config_entry(entry) 
    SetMapEntryEntiID(entry, 2492,4) --���õ�ͼ���ʵ��ı�ţ��ñ�Ŷ�Ӧ��characterinfo.txt��������
end 

function after_create_entry(entry) 
    local copy_mgr = GetMapEntryCopyObj(entry, 0) --��������������󣬴˺���������ʽ��ڵĵ�ͼ�б�����ã�������ʽ��ڵĵ�ͼ���������ս����Ҫ���øýӿ�

    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) --ȡ��ͼ��ڵ�λ����Ϣ����ͼ�������꣬Ŀ���ͼ����
    Notice("Announcement: Magical Ocean ["..posx..","..posy.."] emerges a portal that leads to [Dark Swamp].") --֪ͨ������������������

end

function after_destroy_entry_darkswamp(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
    Notice("Announcement: Magical Ocean ["..posx..","..posy.."] nearby has ceased. Remember to take note of the announcement!") 

end

function after_player_login_darkswamp(entry, player_name)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) --ȡ��ͼ��ڵ�λ����Ϣ����ͼ�������꣬Ŀ���ͼ����
    ChaNotice(player_name, "Announcement: Magical Ocean ["..posx..","..posy.."] emerges a portal that leads to [Dark Swamp].") --֪ͨ������������������

end


--���ڼ���������
--����ֵ��0�����������������1���ɹ����롣
function check_can_enter_darkswamp( role, copy_mgr )
    local Cha = TurnToCha(role)
  
	if Lv(Cha) >=40 and Lv(Cha) <=55 then
		
		return 1

	else
		SystemNotice(role, "Characters need to be between Lv 40 to 55 to enter Dark Swamp")
		return 0
	end
end


function begin_enter_darkswamp(role, copy_mgr) 

	SystemNotice(role,"Entering [Dark Swamp]") 
	MoveCity(role, "Dark Swamp")

end 