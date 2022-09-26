
/*****************************************************************************************************
*做主从轴跟随camautomate,
*1.在最开始，函数声明。在初始化程序调用耦合函数
		Automat(1);
*2.程序末尾耦合函数的定义，也即是AutData结构参数的定义填写，其中包括了跟随的主轴的指针
如果在定义中不给定MasterParID的话，从轴就是跟随的主轴的位置发生器。
		AutData.Master              = Axis1Obj[Index];
		AutData.MaxMasterVelocity   = 1000;
		AutData.MasterParID			= 412;
		
		AutData.State[0].Event[0].Type      = ncST_END;
		AutData.State[0].Event[0].Attribute = ncST_END;
		AutData.State[0].Event[0].NextState = 1;
		......
*3.为了适应多轴控制，耦合函数定义添加了函数参数，轴的索引.
*
******************************************************************************************************/
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif
#include<string.h>

/*prototypes for automat initialization functions*/

void _INIT AxisCtrlInit(void)
{
	////////////////////////////////////////////////////////////////////////////////////////////
	// 初始化轴控指针
	////////////////////////////////////////////////////////////////////////////////////////////
	Axis1Obj[0] = (UDINT)&gAxis01;	//
	Axis1Obj[1] = (UDINT)&gAxis02;	//

	
	////////////////////////////////////////////////////////////////////////////////////////////
	//凸轮曲线初始化例子 参数块下载
	//1号轴的第1条曲线。
	//   strcpy((void*)AxisFun.Axis[1].CamName.down_cam_name[0],"CamProf1");  */
	//   brsstrcpy((UDINT)AxisFun.Axis[1].CamName.down_cam_name[0],(UDINT)"ToothProf");
	//	strcpy((void*)AxisFun.Axis[0].ParaName.down_para_name[0],"Para1");
	//	strcpy((void*)AxisFun.Axis[1].ParaName.down_para_name[0],"Para2");
	////////////////////////////////////////////////////////////////////////////////////////////
	strcpy((void*)AxisFun.Axis[0].CamName.down_cam_name[0],""); 
	strcpy((void*)AxisFun.Axis[1].CamName.down_cam_name[0],""); 
	

	strcpy((void*)AxisFun.Axis[0].ParaName.down_para_name[0],"");
	strcpy((void*)AxisFun.Axis[1].ParaName.down_para_name[0],"");
	////////////////////////////////////////////////////////////////////////////////////////////
	//初始化轴控CAMAUTOMAT，函数传递参数为轴的索引值。
	////////////////////////////////////////////////////////////////////////////////////////////

	//初始化轴控参数值
   
	////////////////////////////////////////////////////////////////////////////////////////////
	//上电和回原点方式处理
	//
	////////////////////////////////////////////////////////////////////////////////////////////
	
	brsmemset((UDINT)&CamNr,0,sizeof(CamNr)); //清除计数数组数据
	brsmemset((UDINT)&ParNr,0,sizeof(ParNr)); //清除计数数组数据
	
	for (Index = 0; Index < MAX_AXIS; Index++)
	{		
		AxisStep[Index] = STATE_WAIT;  /* start step */	
		////////////////////////////////////////////////////////////////////////////////////////////
		//初始化基本参数
		////////////////////////////////////////////////////////////////////////////////////////////
		if(Axis1Obj[Index] == 0) continue;
		pAxis = (ACP10AXIS_typ*)Axis1Obj[Index];
		
		
		////////////////////////////////////////////////////////////////////////////////////////////
		//初始化mcHOME_RESTORE_POS 回原点方式操作，当需要使用HOME_RESTORE_MODE时，
		//MC_BR_InitEndlessPosition需要初始化调用
		//初始化调用示例：
		//
		//	MC_BR_InitEndlessPosition_0[Index].Axis         = Axis1Obj[Index];
		//	MC_BR_InitEndlessPosition_0[Index].DataAddress  = (UDINT)&(MC_ENDLESS_POS_DATA[Index]);
		//	MC_BR_InitEndlessPosition_0[Index].Execute      = 1;
		//	MC_BR_InitEndlessPosition(&MC_BR_InitEndlessPosition_0[Index]);
		//
        ////////////////////////////////////////////////////////////////////////////////////////////
		
		
				
		AxisFun.Axis[Index].Parameter.HomeMode = mcHOME_DIRECT; //程序初始化回原点模式
		AxisFun.Axis[Index].Parameter.Direction= mcPOSITIVE_DIR;//绝对运动方向初始化
		
		
		
		////////////////////////////////////////////////////////////////////////////////////////////
        //
		//初始化轴控参数值
		//初始化轴控PLCopen速度，默认Factor=1，否则还需要除以Factor因子
		//		if(AxisFun.Axis[Index].Parameter.Velocity 		<= 0) 
		//			AxisFun.Axis[Index].Parameter.Velocity 		= pAxis->move.basis.parameter.v_pos/Factor;
		////////////////////////////////////////////////////////////////////////////////////////////

		if(AxisFun.Axis[Index].Parameter.Velocity 		<= 0.0) 
			AxisFun.Axis[Index].Parameter.Velocity 		= pAxis->move.basis.parameter.v_pos/1; 
		if(AxisFun.Axis[Index].Parameter.Acceleration 	<= 0.0)
			AxisFun.Axis[Index].Parameter.Acceleration	= pAxis->move.basis.parameter.a1_pos/1;
		if(AxisFun.Axis[Index].Parameter.Deceleration	<= 0.0)
			AxisFun.Axis[Index].Parameter.Deceleration	= pAxis->move.basis.parameter.a2_pos/1;
		if(AxisFun.Axis[Index].Parameter.JogVelocity	<= 0.0)
			AxisFun.Axis[Index].Parameter.JogVelocity	= pAxis->move.basis.parameter.v_pos/10;	
		if(AxisFun.Axis[Index].Parameter.Override_v		<= 0.0)
			AxisFun.Axis[Index].Parameter.Override_v	= pAxis->move.basis.override.v/10000.0;
		if(AxisFun.Axis[Index].Parameter.Override_a		<= 0.0)
			AxisFun.Axis[Index].Parameter.Override_a	= pAxis->move.basis.override.a/10000.0;
	}	
}

void _CYCLIC AxisCtrlCyclic(void)
{
	////////////////////////////////////////////////////////////////////////////////////////////
	//CamIn功能块调用准备示例
	//主轴的选定
	//参数的给定
	//当Scaling等于Factor时候，那么CamProfile曲线使用的是PLCopen单位。
	// a.
	//	MC_CamTableSelect_0[1].Master 					= Axis1Obj[2];							//指定主轴
	//	MC_CamIn_0[1].Master 							= Axis1Obj[2];							//指定主轴
	//	AxisFun.Axis[1].CamPara.MasterScaling			= 1000; 								//曲线用整数 factor 不缩放
	//	AxisFun.Axis[1].CamPara.SlaveScaling			= 1000; 								//曲线用整数
	//	AxisFun.Axis[1].CamPara.StartMode				= mcDIRECT;								//7--mcDIRECT 0--mcABSOLUTE
	//	AxisFun.Axis[1].CamPara.MasterOffset			= AxisFun.Axis[2].Status.ActPosition;	//牙排原点偏移;
	//	AxisFun.Axis[1].CamPara.SlaveOffset				= AxisFun.Axis[1].Status.ActPosition;	//
	
	//  b. 
	//	MC_CamTableSelect_0[4].Master 					= Axis1Obj[2];
	//	MC_CamIn_0[4].Master 							= Axis1Obj[2];
	//	AxisFun.Axis[4].CamPara.MasterScaling			= 1000;                					//曲线用整数 factor 不缩放
	//	AxisFun.Axis[4].CamPara.SlaveScaling			= 1000; 								//曲线用整数
	//	AxisFun.Axis[4].CamPara.StartMode				= mcABSOLUTE;
	//	AxisFun.Axis[4].CamPara.MasterOffset			= -90;
	//	AxisFun.Axis[4].CamPara.SlaveOffset				= 0;
	//
	////////////////////////////////////////////////////////////////////////////////////////////

	
	
	
	
	
	////////////////////////////////////////////////////////////////////////////////////////////
    //MC_BR_OFFSET功能块调用准备示例
    //参数的给定准备
	//
	//	AxisFun.Axis[1].Parameter.ShiftAcceleration     = 884;	                                //884 --50
	//	AxisFun.Axis[1].Parameter.AppDistance           = 120;
	//	AxisFun.Axis[1].Parameter.ShiftMode				= mcABSOLUTE_NO_RESET;    //mcMASTER_POSITION_BASED + mcMASTER_POSITION or mcTIME_BASED
	//
	//	AxisFun.Axis[5].Parameter.AppMode				= mcTIME_BASED;				            //基于主轴位置 mcMASTER_POSITION_BASED--2 mcTIME_BASED--1
	//	AxisFun.Axis[5].Parameter.ShiftVelocity         = 126;									//平移的速度  126 --150
	//	AxisFun.Axis[5].Parameter.ShiftAcceleration     = 2332;	                                //2332 --150
	//	//AxisFun.Axis[5].Parameter.AppDistance           = 60;                                  //根据选定的模式mcMASTER_POSITION_BASED + mcMASTER_POSITION，参数才有效。
	//	AxisFun.Axis[5].Parameter.ShiftMode				= mcABSOLUTE_NO_RESET;	  //mcMASTER_POSITION_BASED + mcMASTER_POSITION
	////////////////////////////////////////////////////////////////////////////////////////////
	
	 
	
	for (Index = 0; Index < MAX_AXIS; Index++)		
	{
		////////////////////////////////////////////////////////////////////////////////////////////
		//循环调用时候指针识别
		////////////////////////////////////////////////////////////////////////////////////////////
		if(Axis1Obj[Index] == 0) continue;
		pAxis = (ACP10AXIS_typ*)Axis1Obj[Index];	
		
		
		
        ////////////////////////////////////////////////////////////////////////////////////////////
		//初始化完成之后，根据标志实际需要切换回原点方式。
		//		if (RestoreInitOk[Index])
		//		{
		//			//			if(Index == 2 || Index == 3 )				//主轴和飞达轴   测试使用
		//			if(Index == 2 || Index == 3 )				//主轴和飞达轴
		//				AxisFun.Axis[Index].Parameter.HomeMode = mcHOME_RESTORE_POS;
		//			else if(Index == 1 || Index == 6)			//牙排和压力轴
		//				AxisFun.Axis[Index].Parameter.HomeMode = mcHOME_ABSOLUTE_CORR;
		//		}
		//		else
		//		{
		//			if(Index == 1 || Index == 6 )				//牙排和压力轴
		//				AxisFun.Axis[Index].Parameter.HomeMode = mcHOME_ABSOLUTE_CORR;
		//			else
		//				AxisFun.Axis[Index].Parameter.HomeMode = mcHOME_ABSOLUTE;
		//		}
		////////////////////////////////////////////////////////////////////////////////////////////
		
		
		
		
		/********************************************************************************************
        Control Sequence
		********************************************************************************************/		
		/* status information is read before the step sequencer to attain a shorter reaction time */
		/************************ MC_READSTATUS *************************/
		MC_ReadStatus_0[Index].Enable 						= !MC_ReadStatus_0[Index].Error;
		MC_ReadStatus_0[Index].Axis 						= Axis1Obj[Index];
		MC_ReadStatus(&MC_ReadStatus_0[Index]);
		
		AxisFun.Axis[Index].AxisState.Disabled             = MC_ReadStatus_0[Index].Disabled;
		AxisFun.Axis[Index].AxisState.StandStill           = MC_ReadStatus_0[Index].StandStill;
		AxisFun.Axis[Index].AxisState.Stopping             = MC_ReadStatus_0[Index].Stopping;
		AxisFun.Axis[Index].AxisState.Homing               = MC_ReadStatus_0[Index].Homing;
		AxisFun.Axis[Index].AxisState.DiscreteMotion       = MC_ReadStatus_0[Index].DiscreteMotion;
		AxisFun.Axis[Index].AxisState.ContinuousMotion     = MC_ReadStatus_0[Index].ContinuousMotion;
		AxisFun.Axis[Index].AxisState.SynchronizedMotion   = MC_ReadStatus_0[Index].SynchronizedMotion;
		AxisFun.Axis[Index].AxisState.ErrorStop            = MC_ReadStatus_0[Index].Errorstop;

		/********************MC_BR_READDRIVESTATUS***********************/
		MC_BR_ReadDriveStatus_0[Index].Enable 				= !MC_BR_ReadDriveStatus_0[Index].Error;
		MC_BR_ReadDriveStatus_0[Index].Axis 			   	= Axis1Obj[Index];
		MC_BR_ReadDriveStatus_0[Index].AdrDriveStatus      	= (UDINT)&(AxisFun.Axis[Index].Status.DriveStatus);
		MC_BR_ReadDriveStatus(&MC_BR_ReadDriveStatus_0[Index]);

		/******************** MC_READACTUALPOSITION *********************/
		MC_ReadActualPosition_0[Index].Enable 	           	= (!MC_ReadActualPosition_0[Index].Error);
		MC_ReadActualPosition_0[Index].Axis 	           	= Axis1Obj[Index];
		MC_ReadActualPosition(&MC_ReadActualPosition_0[Index]);
		if(MC_ReadActualPosition_0[Index].Valid == 1)
		{
			AxisFun.Axis[Index].Status.ActPosition = MC_ReadActualPosition_0[Index].Position;
		}

		/******************** MC_READACTUALVELOCITY *********************/
		MC_ReadActualVelocity_0[Index].Enable 				= (!MC_ReadActualVelocity_0[Index].Error);
		MC_ReadActualVelocity_0[Index].Axis 				= Axis1Obj[Index];; 
		MC_ReadActualVelocity(&MC_ReadActualVelocity_0[Index]);
		if(MC_ReadActualVelocity_0[Index].Valid == 1)
		{
			AxisFun.Axis[Index].Status.ActVelocity = MC_ReadActualVelocity_0[Index].Velocity;
		}

		/************************ MC_READAXISERROR **********************/
		MC_ReadAxisError_0[Index].Enable 					= !MC_ReadAxisError_0[Index].Error;
		MC_ReadAxisError_0[Index].Axis 						= Axis1Obj[Index];
		MC_ReadAxisError_0[Index].DataAddress 				= (UDINT)&(AxisFun.Axis[Index].Status.ErrorText);
		MC_ReadAxisError_0[Index].DataLength 				= sizeof(AxisFun.Axis[Index].Status.ErrorText);
		brsstrcpy((UDINT)&MC_ReadAxisError_0[Index].DataObjectName,(UDINT)"acp10etxen");
		MC_ReadAxisError(&MC_ReadAxisError_0[Index]);

		/**************** CHECK FOR GENERAL AXIS ERROR ******************/
		if ((MC_ReadAxisError_0[Index].AxisErrorID != 0) && (MC_ReadAxisError_0[Index].Valid == 1))
		{
			AxisStep[Index] = STATE_ERROR_AXIS;
		}
		
		/* central monitoring of stop command attains a shorter reaction time in case of emergency stop */
		/******************CHECK for STOP COMMAND************************/
		if (AxisFun.Axis[Index].Command.Stop == 1)
		{
			/* reset all FB execute inputs we use */
			MC_BR_DownloadCamProfileObj_0[Index].Execute    = 0;
			MC_BR_InitParTabObj_0[Index].Execute 			= 0;
			MC_BR_InitAxisPar_0[Index].Execute 				= 0;
			MC_Home_0[Index].Execute 						= 0;
			MC_Stop_0[Index].Execute 						= 0;
			MC_MoveAbsolute_0[Index].Execute				= 0;
			MC_MoveAdditive_0[Index].Execute 				= 0;
			MC_MoveVelocity_0[Index].Execute 				= 0;
			MC_Halt_0[Index].Execute 						= 0;
			MC_ReadAxisError_0[Index].Acknowledge 			= 0;
			MC_Reset_0[Index].Execute 						= 0;
                
			MC_BR_AutControl_0[Index].Enable 				= 0;
			MC_BR_AutControl_0[Index].Start 				= 0;
			MC_BR_AutControl_0[Index].Stop 					= 0;
			MC_BR_AutControl_0[Index].InitAutData 			= 0;
			MC_CamTableSelect_0[Index].Execute 				= 0;
			MC_CamIn_0[Index].Execute 						= 0;
			MC_CamOut_0[Index].Execute 						= 0;
//			MC_BR_Offset_0[Index].Enable 	 				= 0;//20180522 ????
			MC_BR_Offset_0[Index].InitData   				= 0;
			MC_BR_ReadParID_0[Index].Execute                = 0;
			MC_BR_WriteParID_0[Index].Execute               = 0;
			/* reset user commands */
			brsmemset((UDINT)&AxisFun.Axis[Index].Command,0,sizeof(AxisFun.Axis[Index].Command));
			AxisStep[Index] = STATE_STOP;
		}

			switch(AxisStep[Index])
			{
				/******************** WAIT *************************/
				case STATE_WAIT:  /* STATE: Wait */
					/* reset all FB execute inputs we use */
					MC_BR_DownloadCamProfileObj_0[Index].Execute    = 0;
					MC_BR_InitParTabObj_0[Index].Execute 			= 0;
					MC_BR_InitAxisPar_0[Index].Execute 				= 0;	
					MC_Home_0[Index].Execute 						= 0;
					MC_Stop_0[Index].Execute 						= 0;
					MC_MoveAbsolute_0[Index].Execute 				= 0;
					MC_MoveAdditive_0[Index].Execute 				= 0;
					MC_MoveVelocity_0[Index].Execute 				= 0;
					MC_ReadAxisError_0[Index].Acknowledge 			= 0;
					MC_Reset_0[Index].Execute 						= 0;
				
					MC_BR_AutControl_0[Index].Enable 				= 0;
					MC_BR_AutControl_0[Index].Start 				= 0;
					MC_BR_AutControl_0[Index].Stop 					= 0;
					MC_BR_AutControl_0[Index].InitAutData 			= 0;
					MC_CamTableSelect_0[Index].Execute 				= 0;
					MC_CamIn_0[Index].Execute 						= 0;
					MC_CamOut_0[Index].Execute 						= 0;
					MC_BR_ReadParID_0[Index].Execute                = 0;
					MC_BR_WriteParID_0[Index].Execute               = 0;
					/* reset user commands */
					brsmemset((UDINT)&AxisFun.Axis[Index].Command,0,sizeof(AxisFun.Axis[Index].Command));
					AxisFun.Axis[Index].Status.ErrorID = 0;
				
					if(AxisFun.Axis[Index].Status.DriveStatus.ControllerReady)
					{
						AxisStep[Index] = STATE_DOWNLOAD_CAM_PROFILE;
					}
					break;
				case STATE_DOWNLOAD_CAM_PROFILE:
					if((CamNr[Index] >= MAX_CAM_NUM)
						||( AxisFun.Axis[Index].CamName.down_cam_name[CamNr[Index]][0] == '\0')  )
					{
						AxisStep[Index] = STATE_DOWNLOAD_PARA_TABLE;
						break;
					}
				
					brsstrcpy((UDINT)&MC_BR_DownloadCamProfileObj_0[Index].DataObjectName,
					(UDINT)&AxisFun.Axis[Index].CamName.down_cam_name[CamNr[Index]]);
					MC_BR_DownloadCamProfileObj_0[Index].Index 		= CamNr[Index]+1;
					MC_BR_DownloadCamProfileObj_0[Index].Periodic	= mcNON_PERIODIC;
					MC_BR_DownloadCamProfileObj_0[Index].Execute    = 1;
					AxisStep[Index] = STATE_WAIT_DOWNLOAD_CAM_PROFILE;
				
					break;
				case STATE_WAIT_DOWNLOAD_CAM_PROFILE:
					if(MC_BR_DownloadCamProfileObj_0[Index].Done)
					{					
						MC_BR_DownloadCamProfileObj_0[Index].Execute    = 0;
						CamNr[Index]++;
						AxisStep[Index] = STATE_DOWNLOAD_CAM_PROFILE;				
					}
				
					if(MC_BR_DownloadCamProfileObj_0[Index].Error)
					{
						MC_BR_DownloadCamProfileObj_0[Index].Execute    = 0;
						CamNr[Index] = 0;
						AxisFun.Axis[Index].Status.ErrorID = MC_BR_DownloadCamProfileObj_0[Index].ErrorID;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				case STATE_DOWNLOAD_PARA_TABLE:
					if((ParNr[Index] >= MAX_PARA_NUM)
						||(AxisFun.Axis[Index].ParaName.down_para_name[ParNr[Index]][0] == '\0') )
					{
						AxisStep[Index] = STATE_CHECK_POWER;
						break;
					}
					brsstrcpy((UDINT)&MC_BR_InitParTabObj_0[Index].DataObjectName,
					(UDINT)&AxisFun.Axis[Index].ParaName.down_para_name[ParNr[Index]]);
					MC_BR_InitParTabObj_0[Index].Execute	= 1;
					AxisStep[Index] = STATE_WAIT_DOWNLOAD_PARA_TABLE;
					break;
				case STATE_WAIT_DOWNLOAD_PARA_TABLE:
					if(MC_BR_InitParTabObj_0[Index].Done)
					{
						MC_BR_InitParTabObj_0[Index].Execute = 0;
						ParNr[Index]++;
						AxisStep[Index] = STATE_DOWNLOAD_PARA_TABLE;
					}
				
					if(MC_BR_InitParTabObj_0[Index].Error)
					{
						MC_BR_InitParTabObj_0[Index].Execute = 0;
						ParNr[Index] = 0;
						AxisFun.Axis[Index].Status.ErrorID = MC_BR_InitParTabObj_0[Index].ErrorID;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				case STATE_CHECK_POWER:
					if(AxisFun.Axis[Index].Status.DriveStatus.ControllerStatus)
					{
						MC_Power_0[Index].Enable = 0;
					}
					else if(AxisFun.Axis[Index].Status.DriveStatus.ControllerStatus == 0)
					{
						AxisStep[Index] = STATE_GLOBAL_INIT;
					}
					MC_Power_0[Index].Enable = 0;
					break;
				case STATE_GLOBAL_INIT://需要初始化的参数都可以在这里修改				
					MC_BR_InitAxisPar_0[Index].Execute = 1;
					AxisStep[Index] = STATE_WAIT_GLOBAL_INIT;
					break;
				case STATE_WAIT_GLOBAL_INIT:
					if(MC_BR_InitAxisPar_0[Index].Done)
					{
						MC_BR_InitAxisPar_0[Index].Execute = 0;	
						AxisStep[Index] = STATE_INIT_POWER_ON;
					}
				
					if(MC_BR_InitAxisPar_0[Index].Error)
					{
						MC_BR_InitAxisPar_0[Index].Execute = 0;
						AxisFun.Axis[Index].Status.ErrorID = MC_BR_InitAxisPar_0[Index].ErrorID;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				case STATE_INIT_POWER_ON:
					MC_Power_0[Index].Enable = 1;
					if (MC_Power_0[Index].Status == 1)
					{
					if(pAxis->nc_obj_inf.hardware.acp_typ == ncACP_TYP_PS 
					|| pAxis->nc_obj_inf.hardware.acp_typ == ncACP_TYP_PPS)
					{
					AxisStep[Index] = STATE_READY;
					}
					else
					{
					AxisStep[Index] = STATE_HOME;
					}
					}
					/* if a power error occured go to error state */
					if (MC_Power_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_Power_0[Index].ErrorID;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				/******************** HOME **********************/
				case STATE_HOME:  /* STATE: start homing process */
				//Notice :when the Power on again frequently,maybe it will wrong.SO it need add limited condictions.
//				if(gEmergencyLightOutut && gEmergency && gSafeDoor )  
//				{
//					MC_Home_0[Index].Position 	= AxisFun.Axis[Index].Parameter.HomePosition;
//					MC_Home_0[Index].HomingMode = AxisFun.Axis[Index].Parameter.HomeMode;
//					MC_Home_0[Index].Execute 	= 1;
//					if (MC_Home_0[Index].Done == 1)
//					{
//						MC_Home_0[Index].Execute = 0;
//						AxisStep[Index] = STATE_READY;
//					}
//					/* if a homing error occured go to error state */
//					if (MC_Home_0[Index].Error == 1)
//					{
//						MC_Home_0[Index].Execute = 0;
//						AxisFun.Axis[Index].Status.ErrorID = MC_Home_0[Index].ErrorID;
//						AxisStep[Index] = STATE_ERROR;
//					}
//				}
//				
				
				MC_Home_0[Index].Position 	= AxisFun.Axis[Index].Parameter.HomePosition;
				MC_Home_0[Index].HomingMode = AxisFun.Axis[Index].Parameter.HomeMode;
				MC_Home_0[Index].Execute 	= 1;
				if (MC_Home_0[Index].Done == 1)
				{
					MC_Home_0[Index].Execute = 0;
					AxisStep[Index] = STATE_READY;
				}
				/* if a homing error occured go to error state */
				if (MC_Home_0[Index].Error == 1)
				{
					MC_Home_0[Index].Execute = 0;
					AxisFun.Axis[Index].Status.ErrorID = MC_Home_0[Index].ErrorID;
					AxisStep[Index] = STATE_ERROR;
				}

					break;

				/******************** READY **********************/
				case STATE_READY:  /* STATE: Waiting for commands */
					if (AxisFun.Axis[Index].Command.Stop == 1)
					{
						AxisFun.Axis[Index].Command.Stop = 0;
						AxisStep[Index] = STATE_STOP;
					}
					else if(AxisFun.Axis[Index].Command.Power)
					{
						AxisFun.Axis[Index].Command.Power = 0;
						AxisStep[Index] = STATE_POWER_ON;
					}
					else if(AxisFun.Axis[Index].Command.PowerOff)
					{
						AxisFun.Axis[Index].Command.PowerOff = 0;
						AxisStep[Index] =	STATE_POWER_OFF;
					}
					else if (AxisFun.Axis[Index].Command.Home == 1)
					{
						AxisFun.Axis[Index].Command.Home = 0;
						AxisStep[Index] = STATE_USER_HOME;
					}
					else if (AxisFun.Axis[Index].Command.MoveJogPos == 1)
					{
						AxisStep[Index] = STATE_JOG_POSITIVE;
					}
					else if (AxisFun.Axis[Index].Command.MoveJogNeg == 1)
					{
						AxisStep[Index] = STATE_JOG_NEGATIVE;
					}
				/*permitted State:
				 Standstill,Discrete Motion,Continuous Motion,Synchronized Motion
				*/
					else if (AxisFun.Axis[Index].Command.MoveAbsolute == 1)
					{
						AxisFun.Axis[Index].Command.MoveAbsolute = 0;
						AxisStep[Index] = STATE_MOVE_ABSOLUTE;
					}						
					else if (AxisFun.Axis[Index].Command.MoveAdditive == 1)
					{
						AxisFun.Axis[Index].Command.MoveAdditive = 0;
						AxisStep[Index] = STATE_MOVE_ADDITIVE;
					}
					else if (AxisFun.Axis[Index].Command.MoveVelocity == 1)
					{
						AxisFun.Axis[Index].Command.MoveVelocity = 0;
						AxisStep[Index] = STATE_MOVE_VELOCITY;
					}
					else if(AxisFun.Axis[Index].Command.StartSlave == 1)
					{
						AxisFun.Axis[Index].Command.StartSlave = 0;
						AxisStep[Index] = STATE_AUTOMAT_INIT;
					}
					else if(AxisFun.Axis[Index].Command.StopSlave == 1)
					{
						AxisFun.Axis[Index].Command.StopSlave = 0;	
						AxisStep[Index] = STATE_AUTOMAT_STOP;
					}
					else if (AxisFun.Axis[Index].Command.Halt == 1)
					{
						AxisFun.Axis[Index].Command.Halt = 0;
						AxisStep[Index] = STATE_HALT;   
					}
					else if(AxisFun.Axis[Index].Command.CamStart == 1)
					{
						AxisFun.Axis[Index].Command.CamStart = 0;
						AxisStep[Index] = STATE_CAM_SELECT;
					}
					else if(AxisFun.Axis[Index].Command.CamStop == 1)
					{
						AxisFun.Axis[Index].Command.CamStop = 0;
						AxisStep[Index] = STATE_CAM_STOP;
					}
					else if(AxisFun.Axis[Index].Command.SLOffset == 1)
					{
						AxisFun.Axis[Index].Command.SLOffset = 0;
						AxisStep[Index] = STATE_BR_OFFSET_INIT;
					}	
					else if(AxisFun.Axis[Index].Command.ReadParID == 1)
					{
						AxisFun.Axis[Index].Command.ReadParID = 0;	
						AxisStep[Index] = STATE_READ_PARIDINFO;
					}
					else if(AxisFun.Axis[Index].Command.WriteParID == 1)
					{
						AxisFun.Axis[Index].Command.WriteParID = 0; 
						AxisStep[Index] = STATE_WRITE_PARIDINFO;     
					}
					break;
				/******************** POWER ON **********************/
				case STATE_POWER_ON:  /* STATE: Power on */
					MC_Power_0[Index].Enable = 1;
					if (MC_Power_0[Index].Status == 1)
					{
					AxisStep[Index] = STATE_READY;
					}
					/* if a power error occured go to error state */
					if (MC_Power_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_Power_0[Index].ErrorID;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				case STATE_POWER_OFF:
					MC_Power_0[Index].Enable = 0;
					if(MC_Power_0[Index].Status == 0)
					{
						AxisStep[Index] = STATE_READY;
					}				
					if(MC_Power_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_Power_0[Index].ErrorID;
						AxisStep[Index] = STATE_ERROR;				
					}
					break;
				/*************************user self define home***************************/
				case STATE_USER_HOME:
					MC_Home_0[Index].Position 	= AxisFun.Axis[Index].Parameter.HomePosition;
					MC_Home_0[Index].HomingMode = AxisFun.Axis[Index].Parameter.HomeMode;
					MC_Home_0[Index].Execute 	= 1;
					if (AxisFun.Axis[Index].Command.Halt == 1)
					{
						AxisFun.Axis[Index].Command.Halt = 0;
						MC_Home_0[Index].Execute = 0;
						AxisStep[Index] = STATE_HALT;
					}
					if (MC_Home_0[Index].Done == 1)
					{
						MC_Home_0[Index].Execute = 0; 
						AxisStep[Index] = STATE_READY;
					}
					/* if a homing error occured go to error state */
					if (MC_Home_0[Index].Error == 1)
					{
						MC_Home_0[Index].Execute = 0;
						AxisFun.Axis[Index].Status.ErrorID = MC_Home_0[Index].ErrorID;
						AxisStep[Index] = STATE_ERROR;				    
					}
					break;
				/************************HALT MOVEMENT**************************/    
				case STATE_HALT:  /* STATE: Stop the active movement */
					MC_Halt_0[Index].Deceleration = AxisFun.Axis[Index].Parameter.Deceleration;
					MC_Halt_0[Index].Execute = 1;
					if (MC_Halt_0[Index].Done == 1)
					{
						MC_Halt_0[Index].Execute = 0;
						AxisStep[Index] = STATE_READY; 
					}
					/*check if error occured */
					if (MC_Halt_0[Index].Error == 1)
					{
						MC_Halt_0[Index].Execute = 0;
						AxisFun.Axis[Index].Status.ErrorID = MC_Halt_0[Index].ErrorID;
						AxisStep[Index] = STATE_ERROR; 
					}
					break;
			
				/*********************** STOP MOVEMENT *************************/
				case STATE_STOP: /* STATE: Stop movement as long as command is set */
					MC_Stop_0[Index].Deceleration = AxisFun.Axis[Index].Parameter.Deceleration;
					MC_Stop_0[Index].Execute = 1;
					/* if axis is stopped go to ready state */
					/*if ((MC_Stop_0[Index].Done == 1) && (AxisFun.Axis[Index].Command.Stop == 0))*/
					if (MC_Stop_0[Index].Done == 1)
					{
						MC_Stop_0[Index].Execute = 0;
						AxisStep[Index] = STATE_READY;
					}
					/* check if error occured */
					if (MC_Stop_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_Stop_0[Index].ErrorID;
						MC_Stop_0[Index].Execute = 0;
						AxisStep[Index] = STATE_ERROR;
					}
					break;

				/******************** START JOG MOVEMENT POSITVE **********************/
				case STATE_JOG_POSITIVE:  /* STATE: Start jog movement in positive direction */
					MC_MoveVelocity_0[Index].Velocity      = AxisFun.Axis[Index].Parameter.JogVelocity;
					MC_MoveVelocity_0[Index].Acceleration  = AxisFun.Axis[Index].Parameter.Acceleration;
					MC_MoveVelocity_0[Index].Deceleration  = AxisFun.Axis[Index].Parameter.Deceleration;
					MC_MoveVelocity_0[Index].Direction     = mcPOSITIVE_DIR;
					MC_MoveVelocity_0[Index].Execute = 1;
					/* check if jog movement should be stopped */
					if (AxisFun.Axis[Index].Command.MoveJogPos == 0)
					{
						MC_MoveVelocity_0[Index].Execute = 0;
						AxisStep[Index] = STATE_HALT;
					}
					/* check if error occured */
					if (MC_MoveVelocity_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_MoveVelocity_0[Index].ErrorID;
						MC_MoveVelocity_0[Index].Execute = 0;
						AxisStep[Index] = STATE_ERROR;
						AxisFun.Axis[Index].Command.MoveJogPos = 0;
					}
					break;

				/******************** START JOG MOVEMENT NEGATIVE **********************/
				case STATE_JOG_NEGATIVE:  /* STATE: Start jog movement in negative direction */
					MC_MoveVelocity_0[Index].Velocity      = AxisFun.Axis[Index].Parameter.JogVelocity;
					MC_MoveVelocity_0[Index].Acceleration  = AxisFun.Axis[Index].Parameter.Acceleration;
					MC_MoveVelocity_0[Index].Deceleration  = AxisFun.Axis[Index].Parameter.Deceleration;
					MC_MoveVelocity_0[Index].Direction     = mcNEGATIVE_DIR;
					MC_MoveVelocity_0[Index].Execute = 1;        
					/* check if jog movement should be stopped */
					if (AxisFun.Axis[Index].Command.MoveJogNeg == 0)
					{
						MC_MoveVelocity_0[Index].Execute = 0;
						AxisStep[Index] = STATE_HALT;
					}
					/* check if error occured */
					if (MC_MoveVelocity_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_MoveVelocity_0[Index].ErrorID;
						MC_MoveVelocity_0[Index].Execute = 0;
						AxisStep[Index] = STATE_ERROR;
						AxisFun.Axis[Index].Command.MoveJogNeg = 0;
					}
					break;
				/******************** START ABSOLUTE MOVEMENT **********************/
				case STATE_MOVE_ABSOLUTE:  /* STATE: Start absolute movement */
					MC_MoveAbsolute_0[Index].Position      = AxisFun.Axis[Index].Parameter.Position;
					MC_MoveAbsolute_0[Index].Velocity      = AxisFun.Axis[Index].Parameter.Velocity;
					MC_MoveAbsolute_0[Index].Acceleration  = AxisFun.Axis[Index].Parameter.Acceleration;
					MC_MoveAbsolute_0[Index].Deceleration  = AxisFun.Axis[Index].Parameter.Deceleration;
					MC_MoveAbsolute_0[Index].Direction     = AxisFun.Axis[Index].Parameter.Direction;
					MC_MoveAbsolute_0[Index].Execute = 1;
					/* check if commanded position is reached */
					if (AxisFun.Axis[Index].Command.Halt == 1)
					{
						AxisFun.Axis[Index].Command.Halt = 0;
						MC_MoveAbsolute_0[Index].Execute = 0;
						AxisStep[Index] = STATE_HALT;   
					}
					else if (MC_MoveAbsolute_0[Index].Done == 1)
					{
						MC_MoveAbsolute_0[Index].Execute = 0;
						AxisStep[Index] = STATE_READY;
					}
					/* check if error occured */
					if (MC_MoveAbsolute_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_MoveAbsolute_0[Index].ErrorID;
						MC_MoveAbsolute_0[Index].Execute = 0;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				/******************** START ADDITIVE MOVEMENT **********************/
				case STATE_MOVE_ADDITIVE:  /* STATE: Start additive movement */
					MC_MoveAdditive_0[Index].Distance      = AxisFun.Axis[Index].Parameter.Distance;
					MC_MoveAdditive_0[Index].Velocity      = AxisFun.Axis[Index].Parameter.Velocity;
					MC_MoveAdditive_0[Index].Acceleration  = AxisFun.Axis[Index].Parameter.Acceleration;
					MC_MoveAdditive_0[Index].Deceleration  = AxisFun.Axis[Index].Parameter.Deceleration;
					MC_MoveAdditive_0[Index].Execute = 1;
					/* check if commanded distance is reached */
					if (AxisFun.Axis[Index].Command.Halt == 1)
					{
						AxisFun.Axis[Index].Command.Halt = 0;
						MC_MoveAdditive_0[Index].Execute = 0;
						AxisStep[Index] = STATE_HALT;   
					}
					else if (MC_MoveAdditive_0[Index].Done == 1)
					{
						MC_MoveAdditive_0[Index].Execute = 0;
						AxisStep[Index] = STATE_READY;
					}
					/* check if error occured */
					if (MC_MoveAdditive_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_MoveAdditive_0[Index].ErrorID;
						MC_MoveAdditive_0[Index].Execute = 0;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				/******************** START VELOCITY MOVEMENT **********************/
				case STATE_MOVE_VELOCITY:  /* STATE: Start velocity movement */
					MC_MoveVelocity_0[Index].Velocity      = AxisFun.Axis[Index].Parameter.Velocity;
					MC_MoveVelocity_0[Index].Acceleration  = AxisFun.Axis[Index].Parameter.Acceleration;
					MC_MoveVelocity_0[Index].Deceleration  = AxisFun.Axis[Index].Parameter.Deceleration;
					MC_MoveVelocity_0[Index].Direction     = mcPOSITIVE_DIR;
					MC_MoveVelocity_0[Index].Execute = 1;
					/* check if commanded velocity is reached */
					if (AxisFun.Axis[Index].Command.Halt == 1)
					{
						AxisFun.Axis[Index].Command.Halt = 0;
						MC_MoveVelocity_0[Index].Execute = 0;
						AxisStep[Index] = STATE_HALT;    
					}
					else if (MC_MoveVelocity_0[Index].InVelocity == 1)
					{
						MC_MoveVelocity_0[Index].Execute = 0;
						AxisStep[Index] = STATE_READY;
						AxisFun.Axis[Index].Command.MoveVelocity = 0;//速度到达时命令复位
					}
					/* check if error occured */
					if (MC_MoveVelocity_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_MoveVelocity_0[Index].ErrorID;
						MC_MoveVelocity_0[Index].Execute = 0;
						AxisStep[Index] = STATE_ERROR;
						AxisFun.Axis[Index].Command.MoveVelocity = 0;//错误了也要复位
					}
					break;
				/******************** INITIALIZE AUTOMAT PARAMETER **********************/
				case STATE_AUTOMAT_INIT:/* STATE: Initialize Automat parameter */
					MC_BR_AutControl_0[Index].Enable 			= 1;
					MC_BR_AutControl_0[Index].AdrAutData		= (UDINT)&AutData[Index];
					MC_BR_AutControl_0[Index].InitAutData		= 1;
					if (MC_BR_AutControl_0[Index].AutDataInitialized)
					{
						MC_BR_AutControl_0[Index].InitAutData 	= 0;
						AxisStep[Index] 						= STATE_AUTOMAT_START;
					}
					if(MC_BR_AutControl_0[Index].Error)
					{
						MC_BR_AutControl_0[Index].Enable 		= 0;
						MC_BR_AutControl_0[Index].Start			= 0;
						AxisFun.Axis[Index].Status.ErrorID 	 	= MC_BR_AutControl_0[Index].ErrorID;
						AxisStep[Index] 						= STATE_ERROR;
					}
					break;
				/******************** ENABLE CAM-AUTOMAT **********************/
				case STATE_AUTOMAT_START:/* STATE: Start Automat */
					MC_BR_AutControl_0[Index].Enable 		= 1;
					MC_BR_AutControl_0[Index].Start 		= 1;
			
					/* wait for Automat stop */
					if (AxisFun.Axis[Index].Command.Halt == 1)
					{
						AxisFun.Axis[Index].Command.Halt 	= 0;
						MC_BR_AutControl_0[Index].Enable 	= 0;
						MC_BR_AutControl_0[Index].Start 	= 0;
						AxisStep[Index] 					= STATE_HALT;   
					}
					else if(MC_BR_AutControl_0[Index].Running) //耦合的命令也是凸轮的初始化命令
					{
						AxisStep[Index] = STATE_READY;
					}
					/* check if error occured */
					if (MC_BR_AutControl_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_BR_AutControl_0[Index].ErrorID;
						MC_BR_AutControl_0[Index].Enable 	= 0;
						MC_BR_AutControl_0[Index].Start 	= 0;
						AxisStep[Index] 					= STATE_ERROR;
					}
					break;
				/******************** STOP CAM-AUTOMAT**********************/
				case STATE_AUTOMAT_STOP:/* STATE: Stop CAM-Automat*/
					MC_BR_AutControl_0[Index].Deceleration = AxisFun.Axis[Index].Parameter.Deceleration;
					MC_BR_AutControl_0[Index].Stop = 1;
					/* check if automat is stopped */
					if (MC_BR_AutControl_0[Index].Running == 0)
					{
						MC_BR_AutControl_0[Index].Enable = 0;
						MC_BR_AutControl_0[Index].Stop = 0;
						AxisStep[Index] = STATE_READY;
					}
					/* check if error occured */
					if (MC_BR_AutControl_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_BR_AutControl_0[Index].ErrorID;
						MC_BR_AutControl_0[Index].Enable = 0;
						MC_BR_AutControl_0[Index].Stop = 0;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				/******************** SELECT CAM TABLE **********************/
				case STATE_CAM_SELECT:  /* STATE: Select the CAM */
					strcpy((void*)&MC_CamTableSelect_0[Index].CamTable,(void*)AxisFun.Axis[Index].CamName.down_cam_name[0]);
					MC_CamTableSelect_0[Index].Periodic = mcPERIODIC;
					MC_CamTableSelect_0[Index].Execute = 1;
					/* wait for CAM table selected */
					if (MC_CamTableSelect_0[Index].Done == 1)
					{
						CamTableID[Index] = MC_CamTableSelect_0[Index].CamTableID;
						MC_CamTableSelect_0[Index].Execute = 0;
						AxisStep[Index] = STATE_CAM_START;
					}
					/* check if error occured */
					if (MC_CamTableSelect_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_CamTableSelect_0[Index].ErrorID;
						MC_CamTableSelect_0[Index].Execute = 0;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				/******************** START CAM MOVEMENT **********************/
				case STATE_CAM_START:  /* STATE: Start electronic CAM coupling */
					MC_CamIn_0[Index].MasterOffset 	= AxisFun.Axis[Index].CamPara.MasterOffset;
					MC_CamIn_0[Index].SlaveOffset 	= AxisFun.Axis[Index].CamPara.SlaveOffset;
					MC_CamIn_0[Index].MasterScaling = AxisFun.Axis[Index].CamPara.MasterScaling;
					MC_CamIn_0[Index].SlaveScaling 	= AxisFun.Axis[Index].CamPara.SlaveScaling;
					MC_CamIn_0[Index].StartMode 	= AxisFun.Axis[Index].CamPara.StartMode;
					MC_CamIn_0[Index].CamTableID 	= CamTableID[Index];
					MC_CamIn_0[Index].Execute 		= 1;
					/* wait for CAM stop */
					if (AxisFun.Axis[Index].Command.Halt == 1)
					{
						AxisFun.Axis[Index].Command.Halt = 0;
						MC_CamIn_0[Index].Execute = 0;
						AxisStep[Index] = STATE_HALT;   
					}
					else if (AxisFun.Axis[Index].AxisState.SynchronizedMotion == 1)  //modify 20180524 //(MC_CamIn_0[Index].InSync == 1)
					{
						MC_CamIn_0[Index].Execute = 0;
						AxisStep[Index] = STATE_READY;
					}
					/* check if error occured */
					if (MC_CamIn_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_CamIn_0[Index].ErrorID;
						MC_CamIn_0[Index].Execute = 0;
						AxisStep[Index] = STATE_ERROR;
					}				
					break;
				/******************** STOP CAM MOVEMENT **********************/
				case STATE_CAM_STOP:  /* STATE: Stop electronic CAM coupling */
					MC_CamOut_0[Index].Execute = 1;
					/* check if coupling is stopped */
					if (MC_CamOut_0[Index].Done == 1)
					{
						MC_CamOut_0[Index].Execute = 0;
						AxisStep[Index] = STATE_READY;
					}
					/* check if error occured */
					if (MC_CamOut_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_CamOut_0[Index].ErrorID;
						MC_CamOut_0[Index].Execute = 0;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				case STATE_BR_OFFSET_INIT:
					MC_BR_Offset_0[Index].Enable 		= 1;
					MC_BR_Offset_0[Index].InitData      = 0;
					MC_BR_Offset_0[Index].Shift  		= AxisFun.Axis[Index].Parameter.Shift;
					MC_BR_Offset_0[Index].Velocity		= AxisFun.Axis[Index].Parameter.ShiftVelocity;
					MC_BR_Offset_0[Index].Acceleration	= AxisFun.Axis[Index].Parameter.ShiftAcceleration;
					MC_BR_Offset_0[Index].ApplicationMode= AxisFun.Axis[Index].Parameter.AppMode;
					MC_BR_Offset_0[Index].ShiftMode		= AxisFun.Axis[Index].Parameter.ShiftMode;
					AxisStep[Index] 					= STATE_BR_WAIT_OFFSET_INIT;
				case STATE_BR_WAIT_OFFSET_INIT:
					if(MC_BR_Offset_0[Index].Busy && MC_BR_Offset_0[Index].Active)
					{
						MC_BR_Offset_0[Index].InitData  = 1;
						AxisStep[Index] 				= STATE_BR_OFFSET;
					}
					if(MC_BR_Offset_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_BR_Offset_0[Index].ErrorID;
						MC_BR_Offset_0[Index].Enable  	   = 0;
						MC_BR_Offset_0[Index].InitData     = 0;
						AxisStep[Index]                    = STATE_ERROR;
					}
					break;
				case STATE_BR_OFFSET:
					if(MC_BR_Offset_0[Index].ShiftAttained)
					{
						MC_BR_Offset_0[Index].InitData    = 0;
						AxisStep[Index] 				  = STATE_READY;
					}
					if(MC_BR_Offset_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_BR_Offset_0[Index].ErrorID;
						MC_BR_Offset_0[Index].Enable  	   = 0;
						MC_BR_Offset_0[Index].InitData     = 0;
						AxisStep[Index]                    = STATE_ERROR;
					}
				case STATE_WAIT_BR_OFFSET:
					if(MC_BR_Offset_0[Index].ShiftAttained)
					{
						MC_BR_Offset_0[Index].InitData    = 0;
						AxisStep[Index] 				  = STATE_READY;
					}
					if(MC_BR_Offset_0[Index].Error == 1)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_BR_Offset_0[Index].ErrorID;
						MC_BR_Offset_0[Index].Enable  	   = 0;
						MC_BR_Offset_0[Index].InitData     = 0;
						AxisStep[Index]                    = STATE_ERROR;
					}
					break;
				case STATE_READ_PARIDINFO:
					MC_BR_GetParIDInfo_0[Index].Execute = 1;
					MC_BR_GetParIDInfo_0[Index].ParID	= AxisFun.Axis[Index].Parameter.ParID;
					if(MC_BR_GetParIDInfo_0[Index].Done)
					{
						MC_BR_GetParIDInfo_0[Index].Execute = 0;
						DataType[Index]	= MC_BR_GetParIDInfo_0[Index].ParIDInfo.DataType;
						AxisStep[Index] = STATE_READ_PARID;
					}
				
					if(MC_BR_GetParIDInfo_0[Index].Error)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_BR_GetParIDInfo_0[Index].ErrorID;
						MC_BR_GetParIDInfo_0[Index].Execute = 0;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				case STATE_READ_PARID:
					MC_BR_ReadParID_0[Index].Execute        = 1;
					MC_BR_ReadParID_0[Index].DataAddress    = (void*)&AxisFun.Axis[Index].Status.ReadIDValue;
					MC_BR_ReadParID_0[Index].DataType	    = DataType[Index];
					MC_BR_ReadParID_0[Index].ParID		    = AxisFun.Axis[Index].Parameter.ParID;
					if(MC_BR_ReadParID_0[Index].Done)
					{
						MC_BR_ReadParID_0[Index].Execute    = 0;
						AxisStep[Index] = STATE_READY;
					}
					if(MC_BR_ReadParID_0[Index].Error)
					{
						AxisFun.Axis[Index].Status.ErrorID  = MC_BR_ReadParID_0[Index].ErrorID;
						MC_BR_ReadParID_0[Index].Execute    = 0;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				case STATE_WRITE_PARIDINFO:
					MC_BR_GetParIDInfo_0[Index].Execute = 1;
					MC_BR_GetParIDInfo_0[Index].ParID	= AxisFun.Axis[Index].Parameter.ParID;
					if(MC_BR_GetParIDInfo_0[Index].Done)
					{
						MC_BR_GetParIDInfo_0[Index].Execute = 0;
						DataType[Index]	= MC_BR_GetParIDInfo_0[Index].ParIDInfo.DataType;
						AxisStep[Index] = STATE_WRITE_PARID;
					}
				
					if(MC_BR_GetParIDInfo_0[Index].Error)
					{
						AxisFun.Axis[Index].Status.ErrorID = MC_BR_GetParIDInfo_0[Index].ErrorID;
						MC_BR_GetParIDInfo_0[Index].Execute = 0;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				case STATE_WRITE_PARID:
					MC_BR_WriteParID_0[Index].Execute       = 1;
					MC_BR_WriteParID_0[Index].DataAddress   =(void*)&AxisFun.Axis[Index].Parameter.WriteIDValue;
					MC_BR_WriteParID_0[Index].DataType      =  DataType[Index];
					MC_BR_WriteParID_0[Index].ParID         =AxisFun.Axis[Index].Parameter.ParID;
					if(MC_BR_WriteParID_0[Index].Done)
					{
						MC_BR_WriteParID_0[Index].Execute       = 0;
						AxisStep[Index] = STATE_READY;
					}
					if(MC_BR_WriteParID_0[Index].Error)
					{
						AxisFun.Axis[Index].Status.ErrorID  = MC_BR_WriteParID_0[Index].ErrorID;
						MC_BR_WriteParID_0[Index].Execute       = 0;
						AxisStep[Index] = STATE_ERROR;
					}
					break;
				/******************** FB-ERROR OCCURED *************************/
				case STATE_ERROR:  /* STATE: Error */
					/* check if FB indicates an axis error */
					if (MC_ReadAxisError_0[Index].AxisErrorCount != 0)
					{
						AxisStep[Index] = STATE_ERROR_AXIS;
					}
					else
					{
						if (AxisFun.Axis[Index].Command.ErrorAcknowledge == 1)
						{
							AxisFun.Axis[Index].Command.ErrorAcknowledge = 0;
							AxisFun.Axis[Index].Status.ErrorID = 0;
							/* reset axis if it is in axis state ErrorStop */
							if ((MC_ReadStatus_0[Index].Errorstop == 1) && (MC_ReadStatus_0[Index].Valid == 1))
							{
								AxisStep[Index] = STATE_ERROR_RESET;
							}
							else
							{
								AxisStep[Index] = STATE_WAIT;							
							}
						}
					}
					break;
				/******************** AXIS-ERROR OCCURED *************************/
				case STATE_ERROR_AXIS:  /* STATE: Axis Error */
					if (MC_ReadAxisError_0[Index].Valid == 1)
					{
						if (MC_ReadAxisError_0[Index].AxisErrorID != 0)
						{
							AxisFun.Axis[Index].Status.ErrorID = MC_ReadAxisError_0[Index].AxisErrorID;
						}
						MC_ReadAxisError_0[Index].Acknowledge = 0;
						if (AxisFun.Axis[Index].Command.ErrorAcknowledge == 1)
						{
							AxisFun.Axis[Index].Command.ErrorAcknowledge = 0;
							/* acknowledge axis error */
							if (MC_ReadAxisError_0[Index].AxisErrorID != 0)
							{
								MC_ReadAxisError_0[Index].Acknowledge = 1;
							}
						}
						if (MC_ReadAxisError_0[Index].AxisErrorCount == 0)
						{
							AxisFun.Axis[Index].Status.ErrorID = 0;
							/* reset axis if it is in axis state ErrorStop */
							if ((MC_ReadStatus_0[Index].Errorstop == 1) && (MC_ReadStatus_0[Index].Valid == 1))
							{
								AxisStep[Index] = STATE_ERROR_RESET;
							}
							else
							{
								AxisStep[Index] = STATE_WAIT;
							}
						}
					}
					break;

				/******************** RESET DONE *************************/
				case STATE_ERROR_RESET:  /* STATE: Wait for reset done */
					MC_Reset_0[Index].Execute = 1;
					/* reset MC_Power.Enable if this FB is in Error*/
					if(MC_Power_0[Index].Error == 1)
					{
						MC_Power_0[Index].Enable = 0;
					}
					if(MC_Reset_0[Index].Done == 1)
					{
						MC_Reset_0[Index].Execute = 0;
						AxisStep[Index] = STATE_WAIT;
					}
					else if (MC_Reset_0[Index].Error == 1)
					{
						MC_Reset_0[Index].Execute = 0;
						AxisStep[Index] = STATE_ERROR;   
					}
					break;
    
				/******************** SEQUENCE END *************************/

			}
		
		/***************************************************************
		        Function Block Calls
		***************************************************************/
		//错误ID传递
		ErrorID[Index] = (UDINT)AxisFun.Axis[Index].Status.ErrorID;
		
		MC_BR_InitParTabObj_0[Index].Axis = Axis1Obj[Index];
		MC_BR_InitParTabObj(&MC_BR_InitParTabObj_0[Index]);
		
		MC_BR_DownloadCamProfileObj_0[Index].Axis = Axis1Obj[Index];
		MC_BR_DownloadCamProfileObj(&MC_BR_DownloadCamProfileObj_0[Index]);
		
		MC_BR_InitAxisPar_0[Index].Axis = Axis1Obj[Index];
		MC_BR_InitAxisPar(&MC_BR_InitAxisPar_0[Index]);

		/************************** MC_POWER ****************************/
		MC_Power_0[Index].Axis = Axis1Obj[Index];  /* pointer to axis */
		MC_Power(&MC_Power_0[Index]);

		/************************** MC_HOME *****************************/
		MC_Home_0[Index].Axis = Axis1Obj[Index];
		MC_Home(&MC_Home_0[Index]);

		/********************** MC_MOVEABSOLUTE *************************/
		MC_MoveAbsolute_0[Index].Axis = Axis1Obj[Index];
		MC_MoveAbsolute(&MC_MoveAbsolute_0[Index]);

		/********************** MC_MOVEADDITIVE *************************/
		MC_MoveAdditive_0[Index].Axis = Axis1Obj[Index];
		MC_MoveAdditive(&MC_MoveAdditive_0[Index]);

		/********************** MC_MOVEVELOCITY *************************/
		MC_MoveVelocity_0[Index].Axis = Axis1Obj[Index];
		MC_MoveVelocity(&MC_MoveVelocity_0[Index]);
		
		/**********************MC_BR_INITAUTDATA************************/
		MC_BR_InitAutData_0[Index].Slave = Axis1Obj[Index];
		MC_BR_InitAutData(&MC_BR_InitAutData_0[Index]);

		/********************** MC_AUTCONTROL *************************/
		MC_BR_AutControl_0[Index].Slave 		= Axis1Obj[Index];
		MC_BR_AutControl_0[Index].Signal1		= AxisFun.Axis[Index].Command.Signal1;
		MC_BR_AutControl_0[Index].Signal2		= AxisFun.Axis[Index].Command.Signal2;
		MC_BR_AutControl_0[Index].Signal3		= AxisFun.Axis[Index].Command.Signal3;
		MC_BR_AutControl_0[Index].Signal4		= AxisFun.Axis[Index].Command.Signal4;		
		MC_BR_AutControl(&MC_BR_AutControl_0[Index]);
		AxisFun.Axis[Index].Status.ActualStateIndex = MC_BR_AutControl_0[Index].ActualStateIndex;
		AxisFun.Axis[Index].Status.ActualCamType	= MC_BR_AutControl_0[Index].ActualCamType;
		/********************** MC_CAMTABLESELECT ***********************/
		
		MC_CamTableSelect_0[Index].Slave = Axis1Obj[Index];
		MC_CamTableSelect(&MC_CamTableSelect_0[Index]);

		/********************** MC_CAMIN ********************************/
		MC_CamIn_0[Index].Slave 			= Axis1Obj[Index];
		MC_CamIn(&MC_CamIn_0[Index]);

		/********************** MC_CAMOUT *******************************/
		MC_CamOut_0[Index].Slave 			= Axis1Obj[Index];
		MC_CamOut(&MC_CamOut_0[Index]);
		/**********************MC_BR_Offset***************************/
		MC_BR_Offset_0[Index].Slave			= Axis1Obj[Index];
		MC_BR_Offset(&MC_BR_Offset_0[Index]);
		/**********************MC_BR_AutoCamDwell************************/
		MC_BR_AutoCamDwell(&MC_BR_AutoCamDwell_0[Index]);
		/************************** MC_STOP *****************************/
		MC_Stop_0[Index].Axis = Axis1Obj[Index];
		MC_Stop(&MC_Stop_0[Index]);

		/**************************MC_HALT*******************************/
		MC_Halt_0[Index].Axis = Axis1Obj[Index];
		MC_Halt(&MC_Halt_0[Index]);
		
		/************************** MC_RESET ****************************/
		MC_Reset_0[Index].Axis = Axis1Obj[Index];
		MC_Reset(&MC_Reset_0[Index]);
		
		/**************************MC_BR_GetParIDInfo********************/
		MC_BR_GetParIDInfo(&MC_BR_GetParIDInfo_0[Index]); 
		
		/*************************MC_BR_ReadParID************************/
		MC_BR_ReadParID_0[Index].Axis = Axis1Obj[Index];
		MC_BR_ReadParID(&MC_BR_ReadParID_0[Index]);
		
		/*************************MC_BR_WriteParID***********************/
		MC_BR_WriteParID_0[Index].Axis = Axis1Obj[Index];
		MC_BR_WriteParID(&MC_BR_WriteParID_0[Index]);
		
		/************************MC_SetOverride**************************/
		/******state Stopping Errorstop Homing Disable Not permetted*****/
		MC_SetOverride_0[Index].Enable 		= (!MC_SetOverride_0[Index].Error) 
			&& (!AxisFun.Axis[Index].AxisState.Stopping)&&(!AxisFun.Axis[Index].AxisState.ErrorStop)
			&& (!AxisFun.Axis[Index].AxisState.Homing)  &&(!AxisFun.Axis[Index].AxisState.Disabled);
		MC_SetOverride_0[Index].Axis 		= Axis1Obj[Index];
		MC_SetOverride_0[Index].VelFactor 	= AxisFun.Axis[Index].Parameter.Override_v;//注意单位
		MC_SetOverride_0[Index].AccFactor 	= AxisFun.Axis[Index].Parameter.Override_a;//注意单位
		MC_SetOverride(&MC_SetOverride_0[Index]);
		/******Read PLCopen axis paramter*********************************/
		MC_ReadParameter_0[Index].Enable 	= (!MC_ReadParameter_0[Index].Error);
		MC_ReadParameter_0[Index].Axis 		=  Axis1Obj[Index];		
		MC_ReadParameter_0[Index].ParameterNumber = mcAXIS_FACTOR;
		MC_ReadParameter(&MC_ReadParameter_0[Index]);
        
		MC_WriteParameter_0[Index].Execute  =(!MC_WriteParameter_0[Index].Error);
		MC_WriteParameter_0[Index].Axis     = Axis1Obj[Index];	
		MC_WriteParameter_0[Index].ParameterNumber = mcDEFAULT_MOVE_PARAMETERS;
		MC_WriteParameter_0[Index].Value    = mcBASIS_PARAMETERS;
		MC_WriteParameter(&MC_WriteParameter_0[Index]);
		//mcLIMIT_PARAMETERS--1 limit.parameter
		//mcBASIS_PARAMETERS-- 2 move.basis.parameter
		//mcFB_INPUTS -255 only use the function value
	}
}

void _EXIT AxisCtrlExit(void)
{
	// Insert code here 

}

