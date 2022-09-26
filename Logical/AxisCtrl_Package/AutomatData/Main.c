
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	//实轴
	for(Index = 0; Index < MAX_AXIS; Index++)
	{
		//获取轴的指针，然后进行评估。
		if(Axis1Obj[Index] == 0) continue;
		
		switch (Index)
		{
			case 0://0号电源轴
				/*general automat parameter*/
				break;
			case 1://牙排轴 camprofile
				/*general automat parameter*/
				break;
			case 2://主轴
				break;
			case 3://飞达 camprofile
				break;
			case 4://套准1
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
			case 5://套准2
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
			case 6://压力轴
				break;
			case 7://收箔轴
				//注意各个伺服使用的是PLCopen单位还是轴控单位
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
			case 8://放箔1
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
				AutData[Index].State[2].MasterCompDistance = 160000;//Plcopen unit no using for Specified ParID 这个数值为轴单位，
				AutData[Index].State[2].SlaveCompDistance  = 10; //Plcopen unit
				AutData[Index].State[2].CompMode           = ncONLYCOMP_DIRECT;
				AutData[Index].State[2].MasterFactor       = 50000; //轴的单位
				AutData[Index].State[2].SlaveFactor        = 0;//轴单位

				AutData[Index].State[2].Event[0].Type      = ncTRIGGER1+ncP_EDGE;
				AutData[Index].State[2].Event[0].Attribute = ncST_END;
				AutData[Index].State[2].Event[0].NextState = 2;

				AutData[Index].State[2].Event[1].Type      = ncST_END;
				AutData[Index].State[2].Event[1].Attribute = ncST_END;
				AutData[Index].State[2].Event[1].NextState = 1;
				break;
			case 9://放箔2
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
				AutData[Index].State[2].MasterCompDistance = 160000;//Plcopen unit no using for Specified ParID 这个数值为轴单位，
				AutData[Index].State[2].SlaveCompDistance  = 10; //Plcopen unit
				AutData[Index].State[2].CompMode           = ncONLYCOMP_DIRECT;
				AutData[Index].State[2].MasterFactor       = 50000; //轴的单位
				AutData[Index].State[2].SlaveFactor        = 0;//轴单位

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
	
	//虚轴耦合数据
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

