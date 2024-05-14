------------------------------------------------------------
--TemplateSdk.lua Created by bt 2005.1.27.
--
--����ϵͳ�ű�ģ�庯��
print( "Loading TemplateSDK.lua" )
print( "------------------------------------" )
------------------------------------------------------------

function MisKillMonster( Talk, Param, Prize )

	--Created By BT 2005.01.28

	DefineMission( Param.CurMissionID, Param.MissionName, Param.CurMissionID )

	MisBeginTalk( Talk.Begin )
	MisBeginCondition( NoMission, Param.CurMissionID )
	MisBeginCondition( NoRecord, Param.CurMissionID )

	if Param.NeedRecord ~= nil then
	   MisBeginCondition( HasRecord, Param.NeedRecord )
	end

	MisBeginCondition( LvCheck, ">", Param.MissionMinLv )
	MisBeginCondition( LvCheck, "<", Param.MissionMaxLv )

	MisBeginAction( AddMission, Param.CurMissionID )

	MisBeginAction( AddTrigger, Param.CurTriggerID1, TE_KILL, Param.Kill_Number1, Param.CurMonsterID1 )
	MisNeed( MIS_NEED_KILL, Param.CurMonsterID1, Param.Kill_Number1 )

	if Param.CurMonsterID2 ~= nil and Param.CurTriggerID2 ~= nil and Param.Kill_Number2 ~= nil then
	   MisBeginAction( AddTrigger, Param.CurTriggerID2, TE_KILL, Param.Kill_Number2, Param.CurMonsterID2 )
	   MisNeed( MIS_NEED_KILL, Param.CurMonsterID2, Param.Kill_Number2 )
	end
	if Param.CurMonsterID3 ~= nil and Param.CurTriggerID3 ~= nil and Param.Kill_Number3 ~= nil then
	   MisBeginAction( AddTrigger, Param.CurTriggerID3, TE_KILL, Param.Kill_Number3, Param.CurMonsterID3 )
	   MisNeed( MIS_NEED_KILL, Param.CurMonsterID3, Param.Kill_Number3 )
	end
	if Param.CurMonsterID4 ~= nil and Param.CurTriggerID4 ~= nil and Param.Kill_Number4 ~= nil then
	   MisBeginAction( AddTrigger, Param.CurTriggerID4, TE_KILL, Param.Kill_Number4, Param.CurMonsterID4 )
	   MisNeed( MIS_NEED_KILL, Param.CurMonsterID4, Param.Kill_Number4 )
	end

	if Prize.Mode == nil then
	   MisPrizeSelOne()
	else
	   MisPrizeSelAll()
	end

	if Prize.Select1 ~= nil and Prize.ID1 ~= nil and Prize.Num1 ~= nil then
	   MisPrize( Prize.Select1, Prize.ID1, Prize.Num1 )
	end
	if Prize.Select2 ~= nil and Prize.ID2 ~= nil and Prize.Num2 ~= nil then
	   MisPrize( Prize.Select2, Prize.ID2, Prize.Num2 )
	end
	if Prize.Select3 ~= nil and Prize.ID3 ~= nil and Prize.Num3 ~= nil then
	   MisPrize( Prize.Select3, Prize.ID3, Prize.Num3 )
	end
	if Prize.Select4 ~= nil and Prize.ID4 ~= nil and Prize.Num4 ~= nil then
	   MisPrize( Prize.Select4, Prize.ID4, Prize.Num4 )
	end
	if Prize.Select5 ~= nil and Prize.ID5 ~= nil and Prize.Num5 ~= nil then
	   MisPrize( Prize.Select5, Prize.ID5, Prize.Num5 )
	end
	if Prize.Select6 ~= nil and Prize.ID6 ~= nil and Prize.Num6 ~= nil then
	   MisPrize( Prize.Select6, Prize.ID6, Prize.Num6 )
	end
	if Prize.Select7 ~= nil and Prize.ID7 ~= nil and Prize.Num7 ~= nil then
	   MisPrize( Prize.Select7, Prize.ID7, Prize.Num7 )
	end
	if Prize.Select8 ~= nil and Prize.ID8 ~= nil and Prize.Num8 ~= nil then
	   MisPrize( Prize.Select8, Prize.ID8, Prize.Num8 )
	end

	MisResultTalk( Talk.Result )
	MisHelpTalk( Talk.Help )

	MisResultCondition( HasMission, Param.CurMissionID )
	MisResultCondition( NoRecord, Param.CurMissionID )
	if Param.NeedRecord ~= nil then
	   MisResultCondition( HasRecord, Param.NeedRecord )
	end
	MisResultCondition( HasFlag, Param.CurMissionID, 79 + Param.Kill_Number1 )
	if Param.CurMonsterID2 ~= nil and Param.CurTriggerID2 ~= nil and Param.Kill_Number2 ~= nil then
	   MisResultCondition( HasFlag, Param.CurMissionID, 99 + Param.Kill_Number2 )
	end

	MisResultAction( AddExp, Param.MaxAddExp, Param.MinAddExp )
	MisResultAction( ClearMission, Param.CurMissionID )
	if Param.SetMisRecord ~= nil then
	   MisResultAction( SetRecord, Param.CurMissionID )
	end

	InitTrigger()
	TriggerCondition( 1, IsMonster, Param.CurMonsterID1 )
	TriggerAction( 1, AddNextFlag, Param.CurMissionID, 80, Param.Kill_Number1 )
	RegCurTrigger( Param.CurTriggerID1 )

	if Param.CurMonsterID2 ~= nil and Param.CurTriggerID2 ~= nil and Param.Kill_Number2 ~= nil then
	   InitTrigger()
	   TriggerCondition( 1, IsMonster, Param.CurMonsterID2 )
	   TriggerAction( 1, AddNextFlag, Param.CurMissionID, 100, Param.Kill_Number2 )
	   RegCurTrigger( Param.CurTriggerID2 )
	end

end


function DefSendItemMission( id, name, misid, npcid, areaid, sid )
	--����һ����������ú���˵����COMPLETE_SHOW������������������ֻ���ڽ�ɫ��ȫ�����������Խ���ʱ�Ż���ʾ����ҽ�ɫ��
	DefineMission( id, name, misid, COMPLETE_SHOW )
	
	MisBeginCondition( AlwaysFailure )
	
	MisResultTalk("Thank you for sending my parcel over!")
	
	--��д�������������ж���ϰ���������˵��
	MisResultCondition( HasRandMissionNpc, misid, npcid, areaid )
	MisResultCondition( NoRandNpcItemFlag, misid, npcid )
	
	--������ɶ�����Ϣ��������������Ʒ��ȡ����TakeRandNpcItem��Ϣ˵��,�ú��������У�
	MisResultAction( TakeRandNpcItem, misid, npcid, GetNpcName( npcid ) )
	MisResultAction( RefreshCompleteFlag, sid )
end



--����תְ������Ϣ
function ConvertProfession()
	--������תְ���� 

	--��ʿתְ
	AddPfTable( 1, 8 )		--תְΪ�޽�ʿ
	AddPfTable( 1, 9 )		--תְΪ˫��ʿ
	AddPfTable( 1, 10 )		--תְΪ����ʿ
	
	--����תְ
	AddPfTable( 2, 11 )		--תְΪѵ��ʦ
	AddPfTable( 2, 12 )		--תְΪ�ѻ���
	
	--ˮ��תְ
	AddPfTable( 3, 15 )		--תְΪ����
	
	--ð����תְ
	AddPfTable( 4, 16 ) 		--תְΪ����ʿ
	
	--��Ըʹתְ
	AddPfTable( 5, 13 )		--תְΪʥְ��
	AddPfTable( 5, 14 )		--תְΪ��ӡʦ
	
	--��ʦתְ
	AddPfTable( 6, 18 )		--תְΪ����ʦ
	
	--����תְ
	AddPfTable( 7, 17 )		--תְΪ����
	
	--��ɫ��������
	--������תְ
	AddCatTable( 1, 1 )		--תְΪ��ʿ
	AddCatTable( 1, 2 )		--תְΪ����
	AddCatTable( 1, 4 )		--תְΪð����
	AddCatTable( 1, 7 )		--תְΪ����
	AddCatTable( 1, 9 )		--תְΪ˫��ʿ
	AddCatTable( 1, 10 )	--תְΪ����ʿ
	AddCatTable( 1, 11 )	--תְΪѱ��ʦ
	AddCatTable( 1, 12 )	--תְΪ�ѻ���
	AddCatTable( 1, 16 )	--תְΪ����ʿ
	AddCatTable( 1, 17 )	--תְΪ������
	
	--������תְ
	AddCatTable( 2, 1 )		--תְΪ��ʿ
	AddCatTable( 2, 3 )		--תְΪˮ��
	AddCatTable( 2, 6 )		--תְΪ��ʦ
	AddCatTable( 2, 7 )		--תְΪ����
	AddCatTable( 2, 8 )		--תְΪ�޽�ʿ
	AddCatTable( 2, 10 )	--תְΪ����ʿ
	AddCatTable( 2, 17 )	--תְΪ������
	
	--Ů����һתְ
	AddCatTable( 3, 2 )		--תְΪ����
	AddCatTable( 3, 3 )		--תְΪˮ��
	AddCatTable( 3, 4 )		--תְΪð����
	AddCatTable( 3, 5 )		--תְΪ��Ըʹ
	AddCatTable( 3, 7 )		--תְΪ����
	AddCatTable( 3, 11 )	--תְΪѵ��ʦ
	AddCatTable( 3, 12 )	--תְΪ�ѻ���
	AddCatTable( 3, 13 )	--תְΪʥְ��
	AddCatTable( 3, 14 )	--תְΪ��ӡʦ
	AddCatTable( 3, 15 )	--תְΪ����
	AddCatTable( 3, 16 )	--תְΪ����ʿ
	AddCatTable( 3, 17 )	--תְΪ������
	
	
	--Ů���Ƕ�תְ
	AddCatTable( 4, 4 )		--תְΪð����
	AddCatTable( 4, 5 )		--תְΪ��Ըʹ
	AddCatTable( 4, 6 )		--תְΪ��ʦ
	AddCatTable( 4, 7 )		--תְΪ����
	AddCatTable( 4, 13 )	--תְΪʥְ��
	AddCatTable( 4, 14 )	--תְΪ��ӡʦ
	AddCatTable( 4, 16 )	--תְΪ����ʿ
	AddCatTable( 4, 17 )	--תְΪ������
	AddCatTable( 4, 18 )	--תְΪ����ʦ
end
ConvertProfession()























