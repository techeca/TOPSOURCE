--���ļ��У����ǿ��ܱ����ִ�еĺ�������������Ҫ���ϵ�ͼ��ǰ׺����after_destroy_entry_testpk
--���ļ�ÿ������ַ�����Ϊ255���������飬�������̽��

function config_entry(entry) 
    SetMapEntryEntiID(entry, 2492,1) --���õ�ͼ���ʵ��ı�ţ��ñ�Ŷ�Ӧ��characterinfo.txt��������

end 

function after_create_entry(entry) 

    local copy_mgr = GetMapEntryCopyObj(entry, 0) --��������������󣬴˺���������ʽ��ڵĵ�ͼ�б�����ã�������ʽ��ڵĵ�ͼ���������ս����Ҫ���øýӿ�
    local EntryName = "Gate to Hell"
    SetMapEntryEventName( entry, EntryName )
    
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) --ȡ��ͼ��ڵ�λ����Ϣ����ͼ�������꣬Ŀ���ͼ����
    Notice("�����㲥�������Ų��"["..posx..","..posy.."]"����ͨ�����Ӻڰ������Ļ�Ȫ��ڣ�") --֪ͨ������������������

end

function after_destroy_entry_hell3(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
    --Notice("Announcement: Challenge for today has ended.") 

end

function after_player_login_hell3(entry, player_name)

end

--���ڼ���������
--����ֵ��0�����������������1���ɹ����롣
function check_can_enter_hell3( role, copy_mgr )

if CRY[9]==0 then
	SystemNotice(role,"The power of Darkness has sealed the gateway. It will be impossible for you to pass.")
	return 0
end
return 1

end

function begin_enter_hell3(role, copy_mgr) 
    

		SystemNotice(role,"An unknown gravity pulls you towards the endless darkness. A darker Abaddon awaits you.")
		MoveCity(role, "Abaddon 3")
	

end 