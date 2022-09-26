
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	//ʵ��
	for(Index = 0; Index < MAX_AXIS; Index++)
	{
		//��ȡ���ָ�룬Ȼ�����������
		if(Axis1Obj[Index] == 0) continue;
		
		switch (Index)
		{
			case 0://0�ŵ�Դ��
				/*general automat parameter*/
				break;
			case 1://������ camprofile
				/*general automat parameter*/
				break;
			case 2://����
				break;
			case 3://�ɴ� camprofile
				break;
			case 4://��׼1
				AutData[Index].Master              = Axis1Obj[2];	//this specified master axis reference
				AutData[Index].MaxMasterVelocity   = 600;
				AutData[Index].MasterParID		   = 428;     		//the specified ParID,if it has.the involving axis unit is axis unit
				AutData[Index].AddSlaveParID	   = 412;
			
				/*Automat STATE 0 Basis State*/
				AutData[Index].State[0].Event[0].Type      = ncST_END;
				AutData[Index].State[0].Event[0].Attribute = ncST_END;
				AutData[Index].State[0].Event[0].NextState = 2;

				/*Automat STATE 1 Standstill*/
				AutData[Index].State[1].CamProfileIndex    = 0xFFFF;
				AutData[Index].State[1].MasterCompDistance = 90;
				AutData[Index].State[1].SlaveCompDistance  = 360;
				AutData[Index].State[1].CompMode           = ncONLYCOMP_DIRECT;
				AutData[Index].State[1].MasterFactor       = 90;
				AutData[Index].State[1].SlaveFactor        = 360;

//				AutData[Index].State[1].Event[0].Type      = ncSIGNAL1;
//				AutData[Index].State[1].Event[0].Attribute = ncAT_ONCE;
//				AutData[Index].State[1].Event[0].NextState = 2;

				AutData[Index].State[1].Event[1].Type      = ncST_END;
				AutData[Index].State[1].Event[1].Attribute = ncST_END;
				AutData[Index].State[1].Event[1].NextState = 1;

				/*Automat STATE 2 Standstill*/
				AutData[Index].State[2].CamProfileIndex    = 1;
				AutData[Index].State[2].MasterCompDistance = 0; //because use specified ParID,so the Unit is Axis Unit.
				AutData[Index].State[2].SlaveCompDistance  = 0;
				AutData[Index].State[2].CompMode           = ncOFF;
				AutData[Index].State[2].MasterFactor       = 1000;
				AutData[Index].State[2].SlaveFactor        = 1000;
//				AutData[Index].State[2].MasterFactor       = 1;
//				AutData[Index].State[2].SlaveFactor        = 1;

				AutData[Index].State[2].Event[0].Type      = ncTRIGGER1+ncP_EDGE;
				AutData[Index].State[2].Event[0].Attribute = ncST_END;
				AutData[Index].State[2].Event[0].NextState = 3;

				AutData[Index].State[2].Event[1].Type      = ncST_END;
				AutData[Index].State[2].Event[1].Attribute = ncST_END;
				AutData[Index].State[2].Event[1].NextState = 2;
				break;
			case 5://��׼2
				AutData[Index].Master              = Axis1Obj[2];	//this specified master axis reference
				AutData[Index].MaxMasterVelocity   = 600;
				AutData[Index].MasterParID		   = 428;     		//the specified ParID,if it has.the involving axis unit is axis unit
				AutData[Index].AddSlaveParID	   = 412;
			
				/*Automat STATE 0 Basis State*/
				AutData[Index].State[0].Event[0].Type      = ncST_END;
				AutData[Index].State[0].Event[0].Attribute = ncST_END;
				AutData[Index].State[0].Event[0].NextState = 1;

				/*Automat STATE 1 Standstill*/
				AutData[Index].State[1].CamProfileIndex    = 0xFFFF;
				AutData[Index].State[1].MasterCompDistance = 0;
				AutData[Index].State[1].SlaveCompDistance  = 0;
				AutData[Index].State[1].CompMode           = ncOFF;
				AutData[Index].State[1].MasterFactor       = 8000;
				AutData[Index].State[1].SlaveFactor        = 0;

				AutData[Index].State[1].Event[0].Type      = ncSIGNAL1;
				AutData[Index].State[1].Event[0].Attribute = ncAT_ONCE;
				AutData[Index].State[1].Event[0].NextState = 2;

				AutData[Index].State[1].Event[1].Type      = ncST_END;
				AutData[Index].State[1].Event[1].Attribute = ncST_END;
				AutData[Index].State[1].Event[1].NextState = 1;

				/*Automat STATE 2 Standstill*/
				AutData[Index].State[2].CamProfileIndex    = 1;
				AutData[Index].State[2].MasterCompDistance = 0; //because use specified ParID,so the Unit is Axis Unit.
				AutData[Index].State[2].SlaveCompDistance  = 0;
				AutData[Index].State[2].CompMode           = ncOFF;
				AutData[Index].State[2].MasterFactor       = 1000;
				AutData[Index].State[2].SlaveFactor        = 1000;


				AutData[Index].State[2].Event[0].Type      = ncTRIGGER1+ncP_EDGE;
				AutData[Index].State[2].Event[0].Attribute = ncST_END;
				AutData[Index].State[2].Event[0].NextState = 3;

				AutData[Index].State[2].Event[1].Type      = ncST_END;
				AutData[Index].State[2].Event[1].Attribute = ncST_END;
				AutData[Index].State[2].Event[1].NextState = 1;
				break;
			case 6://ѹ����
				break;
			case 7://�ղ���
				//ע������ŷ�ʹ�õ���PLCopen��λ������ص�λ
				AutData[Index].Master              = Axis1Obj[2];	//this specified master axis reference
				AutData[Index].MaxMasterVelocity   = 3927;
				AutData[Index].MasterParID		   = 428;     		//the specified ParID,if it has.the involving axis unit is axis unit
				AutData[Index].AddMasterParID      = 543;
				AutData[Index].AddSlaveParID	   = 412;
			
				/*Automat STATE 0 Basis State*/
				AutData[Index].State[0].Event[0].Type      = ncST_END;
				AutData[Index].State[0].Event[0].Attribute = ncST_END;
				AutData[Index].State[0].Event[0].NextState = 1;

				/*Automat STATE 1 Standstill*/
				AutData[Index].State[1].CamProfileIndex    = 0xFFFF;
				AutData[Index].State[1].MasterCompDistance = 0;
				AutData[Index].State[1].SlaveCompDistance  = 0;
				AutData[Index].State[1].CompMode           = ncOFF;
				AutData[Index].State[1].MasterFactor       = 8000;
				AutData[Index].State[1].SlaveFactor        = 0;

				AutData[Index].State[1].Event[0].Type      = ncSIGNAL1;
				AutData[Index].State[1].Event[0].Attribute = ncAT_ONCE;
				AutData[Index].State[1].Event[0].NextState = 2;

				AutData[Index].State[1].Event[1].Type      = ncST_END;
				AutData[Index].State[1].Event[1].Attribute = ncST_END;
				AutData[Index].State[1].Event[1].NextState = 1;

				/*Automat STATE 2 Standstill*/
				AutData[Index].State[2].CamProfileIndex    = 0xFFFF;
				AutData[Index].State[2].MasterCompDistance = 160000; //because use specified ParID,so the Unit is Axis Unit.
				AutData[Index].State[2].SlaveCompDistance  = 10;
				AutData[Index].State[2].CompMode           = ncONLYCOMP_DIRECT;
				AutData[Index].State[2].MasterFactor       = 50000;
				AutData[Index].State[2].SlaveFactor        = 0;


				AutData[Index].State[2].Event[0].Type      = ncTRIGGER1+ncP_EDGE;
				AutData[Index].State[2].Event[0].Attribute = ncST_END;
				AutData[Index].State[2].Event[0].NextState = 3;

				AutData[Index].State[2].Event[1].Type      = ncST_END;
				AutData[Index].State[2].Event[1].Attribute = ncST_END;
				AutData[Index].State[2].Event[1].NextState = 1;
				break;
			case 8://�Ų�1
				/*general automat parameter*/
				AutData[Index].Master              = Axis1Obj[2];	//this specified master axis reference
				AutData[Index].MaxMasterVelocity   = 3927;
				AutData[Index].MasterParID		   = 428;     		//the specified ParID,if it has.
				AutData[Index].AddMasterParID      = 543;
				AutData[Index].AddSlaveParID	   = 412;
			
				/*Automat STATE 0 Basis State*/
				AutData[Index].State[0].Event[0].Type      = ncST_END;
				AutData[Index].State[0].Event[0].Attribute = ncST_END;
				AutData[Index].State[0].Event[0].NextState = 1;

				/*Automat STATE 1 Standstill*/
				AutData[Index].State[1].CamProfileIndex    = 0xFFFF;
				AutData[Index].State[1].MasterCompDistance = 0;
				AutData[Index].State[1].SlaveCompDistance  = 0;
				AutData[Index].State[1].CompMode           = ncOFF;
				AutData[Index].State[1].MasterFactor       = 8000;
				AutData[Index].State[1].SlaveFactor        = 0;

				AutData[Index].State[1].Event[0].Type      = ncSIGNAL1;
				AutData[Index].State[1].Event[0].Attribute = ncAT_ONCE;
				AutData[Index].State[1].Event[0].NextState = 2;

				AutData[Index].State[1].Event[1].Type      = ncST_END;
				AutData[Index].State[1].Event[1].Attribute = ncST_END;
				AutData[Index].State[1].Event[1].NextState = 1;

				/*Automat STATE 2 Standstill*/
				AutData[Index].State[2].CamProfileIndex    = 0xFFFF;
				AutData[Index].State[2].MasterCompDistance = 160000;//Plcopen unit no using for Specified ParID �����ֵΪ�ᵥλ��
				AutData[Index].State[2].SlaveCompDistance  = 10; //Plcopen unit
				AutData[Index].State[2].CompMode           = ncONLYCOMP_DIRECT;
				AutData[Index].State[2].MasterFactor       = 50000; //��ĵ�λ
				AutData[Index].State[2].SlaveFactor        = 0;//�ᵥλ

				AutData[Index].State[2].Event[0].Type      = ncTRIGGER1+ncP_EDGE;
				AutData[Index].State[2].Event[0].Attribute = ncST_END;
				AutData[Index].State[2].Event[0].NextState = 2;

				AutData[Index].State[2].Event[1].Type      = ncST_END;
				AutData[Index].State[2].Event[1].Attribute = ncST_END;
				AutData[Index].State[2].Event[1].NextState = 1;
				break;
			case 9://�Ų�2
				AutData[Index].Master              = Axis1Obj[2];	//this specified master axis reference
				AutData[Index].MaxMasterVelocity   = 3927;
				AutData[Index].MasterParID		   = 428;     		//the specified ParID,if it has.
				AutData[Index].AddMasterParID      = 543;
				AutData[Index].AddSlaveParID	   = 412;
			
				/*Automat STATE 0 Basis State*/
				AutData[Index].State[0].Event[0].Type      = ncST_END;
				AutData[Index].State[0].Event[0].Attribute = ncST_END;
				AutData[Index].State[0].Event[0].NextState = 1;

				/*Automat STATE 1 Standstill*/
				AutData[Index].State[1].CamProfileIndex    = 0xFFFF;
				AutData[Index].State[1].MasterCompDistance = 0;
				AutData[Index].State[1].SlaveCompDistance  = 0;
				AutData[Index].State[1].CompMode           = ncOFF;
				AutData[Index].State[1].MasterFactor       = 8000;
				AutData[Index].State[1].SlaveFactor        = 0;

				AutData[Index].State[1].Event[0].Type      = ncSIGNAL1;
				AutData[Index].State[1].Event[0].Attribute = ncAT_ONCE;
				AutData[Index].State[1].Event[0].NextState = 2;

				AutData[Index].State[1].Event[1].Type      = ncST_END;
				AutData[Index].State[1].Event[1].Attribute = ncST_END;
				AutData[Index].State[1].Event[1].NextState = 1;

				/*Automat STATE 2 Standstill*/
				AutData[Index].State[2].CamProfileIndex    = 0xFFFF;
				AutData[Index].State[2].MasterCompDistance = 160000;//Plcopen unit no using for Specified ParID �����ֵΪ�ᵥλ��
				AutData[Index].State[2].SlaveCompDistance  = 10; //Plcopen unit
				AutData[Index].State[2].CompMode           = ncONLYCOMP_DIRECT;
				AutData[Index].State[2].MasterFactor       = 50000; //��ĵ�λ
				AutData[Index].State[2].SlaveFactor        = 0;//�ᵥλ

				AutData[Index].State[2].Event[0].Type      = ncTRIGGER1+ncP_EDGE;
				AutData[Index].State[2].Event[0].Attribute = ncST_END;
				AutData[Index].State[2].Event[0].NextState = 2;

				AutData[Index].State[2].Event[1].Type      = ncST_END;
				AutData[Index].State[2].Event[1].Attribute = ncST_END;
				AutData[Index].State[2].Event[1].NextState = 1;
				break;
			default:

				break;
		}
     	
	}
	
	//�����������
	for (Index = 0; Index < MAX_VAXIS; Index++)
	{
		switch(Index)
		{
			case 0:
				/*general automat parameter*/
				break;
			case 1:
				/*general automat parameter*/

				break;
			case 2:
				break;
			case 3:
				break;
			default:

				break;
		}
	}

}

void _CYCLIC ProgramCyclic(void)
{

}

void _EXIT ProgramExit(void)
{

}

