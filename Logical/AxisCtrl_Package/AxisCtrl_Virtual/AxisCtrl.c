
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT AxisCtrlInit(void)
{
	// 获取虚轴指针 
	VAxis1Obj[0] = (UDINT)&gVAxis01;
	VAxis1Obj[1] = (UDINT)&gVAxis02;

	//init the slave reference point.
	
	brsmemset((UDINT)&CamNr,0,sizeof(CamNr)); //清除计数数组数据
	brsmemset((UDINT)&ParNr,0,sizeof(ParNr)); //清除计数数组数据
	
	brsstrcpy((UDINT)VAxisFun.Axis[0].CamName.down_cam_name[0],(UDINT)"");
	brsstrcpy((UDINT)VAxisFun.Axis[1].CamName.down_cam_name[0],(UDINT)"");
	for (Index = 0; Index < MAX_VAXIS; Index++)
	{
		
		VAxisStep[Index] = STATE_WAIT;  /* start step */
		//设定虚轴参数
		//初始化基本参数
		if (0 == VAxis1Obj[Index]) continue;
		pVAxis = (ACP10AXIS_typ*)VAxis1Obj[Index];
		
		VAxisFun.Axis[Index].Parameter.HomeMode		= mcHOME_DIRECT;
		VAxisFun.Axis[Index].Parameter.Direction	= mcPOSITIVE_DIR;	
//初始化轴控PLCopen速度，默认Factor=1，否则还需要除以Factor因子
//		if(VAxisFun.Axis[Index].Parameter.Velocity 		<= 0) 
//			VAxisFun.Axis[Index].Parameter.Velocity 		= pVAxis->move.basis.parameter.v_pos/Factor;	
		if(VAxisFun.Axis[Index].Parameter.Velocity 		<= 0.0) 
			VAxisFun.Axis[Index].Parameter.Velocity 	= pVAxis->move.basis.parameter.v_pos/1000; 
		if(VAxisFun.Axis[Index].Parameter.Acceleration 	<= 0.0)
			VAxisFun.Axis[Index].Parameter.Acceleration	= pVAxis->move.basis.parameter.a1_pos/1000;
		if(VAxisFun.Axis[Index].Parameter.Deceleration	<= 0.0)
			VAxisFun.Axis[Index].Parameter.Deceleration	= pVAxis->move.basis.parameter.a1_neg/1000;
		if(VAxisFun.Axis[Index].Parameter.JogVelocity	<= 0.0)
			VAxisFun.Axis[Index].Parameter.JogVelocity	= pVAxis->move.basis.parameter.v_pos/10000;	
		if(VAxisFun.Axis[Index].Parameter.Override_v	<= 0.0)
			VAxisFun.Axis[Index].Parameter.Override_v	= pVAxis->move.basis.override.v/10000.0;
		if(VAxisFun.Axis[Index].Parameter.Override_a	<= 0.0)
			VAxisFun.Axis[Index].Parameter.Override_a	= pVAxis->move.basis.override.a/10000.0;
		
	}
}

void _CYCLIC AxisCtrlCyclic(void)
{
	// Insert code here 
	for (Index = 0; Index < MAX_VAXIS; Index++)
	{
		pVAxis = (ACP10AXIS_typ*)VAxis1Obj[Index];
		if(pVAxis == 0) continue;
		/***************************************************************
        Control Sequence
		***************************************************************/
		
		/* status information is read before the step sequencer to attain a shorter reaction time */
		/************************ MC_READSTATUS *************************/
		MC_ReadStatus_0[Index].Enable = !MC_ReadStatus_0[Index].Error;
		MC_ReadStatus_0[Index].Axis = VAxis1Obj[Index];
		MC_ReadStatus(&MC_ReadStatus_0[Index]);
		
		VAxisFun.Axis[Index].AxisState.Disabled             = MC_ReadStatus_0[Index].Disabled;
		VAxisFun.Axis[Index].AxisState.StandStill           = MC_ReadStatus_0[Index].StandStill;
		VAxisFun.Axis[Index].AxisState.Stopping             = MC_ReadStatus_0[Index].Stopping;
		VAxisFun.Axis[Index].AxisState.Homing               = MC_ReadStatus_0[Index].Homing;
		VAxisFun.Axis[Index].AxisState.DiscreteMotion       = MC_ReadStatus_0[Index].DiscreteMotion;
		VAxisFun.Axis[Index].AxisState.ContinuousMotion     = MC_ReadStatus_0[Index].ContinuousMotion;
		VAxisFun.Axis[Index].AxisState.SynchronizedMotion   = MC_ReadStatus_0[Index].SynchronizedMotion;
		VAxisFun.Axis[Index].AxisState.ErrorStop            = MC_ReadStatus_0[Index].Errorstop;

		/********************MC_BR_READDRIVESTATUS***********************/
		MC_BR_ReadDriveStatus_0[Index].Enable 				= !MC_BR_ReadDriveStatus_0[Index].Error;
		MC_BR_ReadDriveStatus_0[Index].Axis 				= VAxis1Obj[Index];
		MC_BR_ReadDriveStatus_0[Index].AdrDriveStatus 		= (UDINT)&(VAxisFun.Axis[Index].Status.DriveStatus);
		MC_BR_ReadDriveStatus(&MC_BR_ReadDriveStatus_0[Index]);

		/******************** MC_READACTUALPOSITION *********************/
		MC_ReadActualPosition_0[Index].Enable 				= (!MC_ReadActualPosition_0[Index].Error);
		MC_ReadActualPosition_0[Index].Axis 				= VAxis1Obj[Index];
		MC_ReadActualPosition(&MC_ReadActualPosition_0[Index]);
		if(MC_ReadActualPosition_0[Index].Valid == 1)
		{
			VAxisFun.Axis[Index].Status.ActPosition = MC_ReadActualPosition_0[Index].Position;
		}

		/******************** MC_READACTUALVELOCITY *********************/
		MC_ReadActualVelocity_0[Index].Enable 				= (!MC_ReadActualVelocity_0[Index].Error);
		MC_ReadActualVelocity_0[Index].Axis 				= VAxis1Obj[Index];; 
		MC_ReadActualVelocity(&MC_ReadActualVelocity_0[Index]);
		if(MC_ReadActualVelocity_0[Index].Valid == 1)
		{
			VAxisFun.Axis[Index].Status.ActVelocity 		= MC_ReadActualVelocity_0[Index].Velocity;
		}

		/************************ MC_READAXISERROR **********************/
		MC_ReadAxisError_0[Index].Enable 					= !MC_ReadAxisError_0[Index].Error;
		MC_ReadAxisError_0[Index].Axis 						= VAxis1Obj[Index];
		MC_ReadAxisError_0[Index].DataAddress 				= (UDINT)&(VAxisFun.Axis[Index].Status.ErrorText);
		MC_ReadAxisError_0[Index].DataLength 				= sizeof(VAxisFun.Axis[Index].Status.ErrorText);
		brsstrcpy((UDINT)&MC_ReadAxisError_0[Index].DataObjectName,(UDINT)"acp10etxen");
		MC_ReadAxisError(&MC_ReadAxisError_0[Index]);

		/**************** CHECK FOR GENERAL AXIS ERROR ******************/
		if ((MC_ReadAxisError_0[Index].AxisErrorID != 0) && (MC_ReadAxisError_0[Index].Valid == 1))
		{
			VAxisStep[Index] = STATE_ERROR_AXIS;
		}
		
		/* central monitoring of stop command attains a shorter reaction time in case of emergency stop */
		/******************CHECK for STOP COMMAND************************/
		if (VAxisFun.Axis[Index].Command.Stop == 1)
		{
			if ((VAxisStep[Index] >= STATE_READY) && (VAxisStep[Index] <= STATE_ERROR))
			{
				/* reset all FB execute inputs we use */
				MC_Home_0[Index].Execute = 0;
				MC_Stop_0[Index].Execute = 0;
				MC_MoveAbsolute_0[Index].Execute = 0;
				MC_MoveAdditive_0[Index].Execute = 0;
				MC_MoveVelocity_0[Index].Execute = 0;
				MC_Halt_0[Index].Execute = 0;
				MC_ReadAxisError_0[Index].Acknowledge = 0;
				MC_Reset_0[Index].Execute = 0;

				/* reset user commands */
				brsmemset((UDINT)&VAxisFun.Axis[Index].Command,0,sizeof(VAxisFun.Axis[Index].Command));
				VAxisStep[Index] = STATE_STOP;
			}
		}
		switch(VAxisStep[Index])
		{
			/******************** WAIT *************************/
			case STATE_WAIT:  /* STATE: Wait */
				//虚轴没有上电动作
				if (VAxisFun.Axis[Index].Status.DriveStatus.NetworkInit)
				{
					VAxisStep[Index] = STATE_HOME;
				}
				/* reset all FB execute inputs we use */
				MC_Home_0[Index].Execute = 0;
				MC_Stop_0[Index].Execute = 0;
				MC_MoveAbsolute_0[Index].Execute = 0;
				MC_MoveAdditive_0[Index].Execute = 0;
				MC_MoveVelocity_0[Index].Execute = 0;
				MC_ReadAxisError_0[Index].Acknowledge = 0;
				MC_Reset_0[Index].Execute = 0;

				/* reset user commands */
				brsmemset((UDINT)&VAxisFun.Axis[Index].Command,0,sizeof(VAxisFun.Axis[Index].Command));
				AxisFun.Axis[Index].Status.ErrorID = 0;
				if(VAxisFun.Axis[Index].Status.DriveStatus.ControllerReady)
				{
					VAxisStep[Index] = STATE_DOWNLOAD_CAM_PROFILE;
				}
				break;
			case STATE_DOWNLOAD_CAM_PROFILE:
				if((CamNr[Index] >= MAX_CAM_NUM)
					||( brsstrcmp((UDINT)VAxisFun.Axis[Index].CamName.down_cam_name[CamNr[Index]],(UDINT)"") == 0 )  )
				{
					VAxisStep[Index] = STATE_DOWNLOAD_PARA_TABLE;
					break;
				}			
				brsstrcpy((UDINT)&MC_BR_DownloadCamProfileObj_0[Index].DataObjectName,
				(UDINT)&VAxisFun.Axis[Index].CamName.down_cam_name[CamNr[Index]]);
				MC_BR_DownloadCamProfileObj_0[Index].Index 		= CamNr[Index]+1;
				MC_BR_DownloadCamProfileObj_0[Index].Periodic	= mcNON_PERIODIC;
				MC_BR_DownloadCamProfileObj_0[Index].Execute    = 1;
				VAxisStep[Index] = STATE_WAIT_DOWNLOAD_CAM_PROFILE;
				break;
			case STATE_WAIT_DOWNLOAD_CAM_PROFILE:
				if(MC_BR_DownloadCamProfileObj_0[Index].Done)
				{					
					MC_BR_DownloadCamProfileObj_0[Index].Execute    = 0;
					CamNr[Index]++;
					VAxisStep[Index] = STATE_DOWNLOAD_CAM_PROFILE;				
				}
				
				if(MC_BR_DownloadCamProfileObj_0[Index].Error)
				{
					MC_BR_DownloadCamProfileObj_0[Index].Execute    = 0;
					CamNr[Index] = 0;
					VAxisFun.Axis[Index].Status.ErrorID = MC_BR_DownloadCamProfileObj_0[Index].ErrorID;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			case STATE_DOWNLOAD_PARA_TABLE:
				if((ParNr[Index] >= MAX_PARA_NUM)
					||( brsstrcmp((UDINT)VAxisFun.Axis[Index].ParaName.down_para_name[ParNr[Index]],(UDINT)"") == 0 ) )
				{
					VAxisStep[Index] = STATE_CHECK_POWER;
					break;
				}
				brsstrcpy((UDINT)&MC_BR_InitParTabObj_0[Index].DataObjectName,
				(UDINT)&VAxisFun.Axis[Index].ParaName.down_para_name[ParNr[Index]]);
				MC_BR_InitParTabObj_0[Index].Execute	= 1;
				VAxisStep[Index] = STATE_WAIT_DOWNLOAD_PARA_TABLE;
				break;
			case STATE_WAIT_DOWNLOAD_PARA_TABLE:
				if(MC_BR_InitParTabObj_0[Index].Done)
				{
					MC_BR_InitParTabObj_0[Index].Execute = 0;
					ParNr[Index]++;
					VAxisStep[Index] = STATE_DOWNLOAD_PARA_TABLE;
				}
				
				if(MC_BR_InitParTabObj_0[Index].Error)
				{
					MC_BR_InitParTabObj_0[Index].Execute = 0;
					ParNr[Index] = 0;
					VAxisFun.Axis[Index].Status.ErrorID = MC_BR_InitParTabObj_0[Index].ErrorID;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			case STATE_CHECK_POWER:
				if(VAxisFun.Axis[Index].Status.DriveStatus.ControllerStatus)
				{
					MC_Power_0[Index].Enable = 0;
				}
				else if(VAxisFun.Axis[Index].Status.DriveStatus.ControllerStatus == 0)
				{
					VAxisStep[Index] = STATE_GLOBAL_INIT;
				}
				break;
			case STATE_GLOBAL_INIT: 
				MC_BR_InitAxisPar_0[Index].Execute = 1;
				VAxisStep[Index] = STATE_WAIT_GLOBAL_INIT;
				break;
			case STATE_WAIT_GLOBAL_INIT:
				if(MC_BR_InitAxisPar_0[Index].Done)
				{
					MC_BR_InitAxisPar_0[Index].Execute = 0;	
					VAxisStep[Index] = STATE_INIT_POWER_ON;
				}
				
				if(MC_BR_InitAxisPar_0[Index].Error)
				{
					MC_BR_InitAxisPar_0[Index].Execute = 0;
					VAxisFun.Axis[Index].Status.ErrorID = MC_BR_InitAxisPar_0[Index].ErrorID;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			case STATE_INIT_POWER_ON:
                MC_Power_0[Index].Enable = 1;
				if (MC_Power_0[Index].Status == 1)
				{
    				if(pVAxis->nc_obj_inf.hardware.acp_typ == ncACP_TYP_PS 
    				|| pVAxis->nc_obj_inf.hardware.acp_typ == ncACP_TYP_PPS)
    				{
        				VAxisStep[Index] = STATE_READY;
    				}
    				else
    				{
        				VAxisStep[Index] = STATE_HOME;
    				}
				}
				/* if a power error occured go to error state */
				if (MC_Power_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_Power_0[Index].ErrorID;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			/******************** HOME **********************/
			case STATE_HOME:  /* STATE: start homing process */

                MC_Home_0[Index].Position = VAxisFun.Axis[Index].Parameter.HomePosition;
                MC_Home_0[Index].HomingMode = VAxisFun.Axis[Index].Parameter.HomeMode;
                MC_Home_0[Index].Execute = 1;

				if (MC_Home_0[Index].Done == 1)
				{
					MC_Home_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_READY;
				}
				/* if a homing error occured go to error state */
				if (MC_Home_0[Index].Error == 1)
				{
					MC_Home_0[Index].Execute = 0;
					VAxisFun.Axis[Index].Status.ErrorID = MC_Home_0[Index].ErrorID;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			/******************** READY **********************/
			case STATE_READY:  /* STATE: Waiting for commands */
				if (VAxisFun.Axis[Index].Command.Stop == 1)
				{
					VAxisFun.Axis[Index].Command.Stop = 0;
					VAxisStep[Index] = STATE_STOP;
				}
				else if(VAxisFun.Axis[Index].Command.Power)
				{
					VAxisFun.Axis[Index].Command.Power = 0;
					VAxisStep[Index] = STATE_POWER_ON;
				}
				else if(AxisFun.Axis[Index].Command.PowerOff)
				{
					VAxisFun.Axis[Index].Command.PowerOff = 0;
					VAxisStep[Index] =	STATE_POWER_OFF;
				}
				if (VAxisFun.Axis[Index].Command.Home == 1)
				{
					VAxisFun.Axis[Index].Command.Home = 0;
					VAxisStep[Index] = STATE_USER_HOME;
				}
				else if (VAxisFun.Axis[Index].Command.Stop == 1)
				{
					VAxisStep[Index] = STATE_STOP;
				}
				else if (VAxisFun.Axis[Index].Command.MoveJogPos == 1)
				{
					VAxisStep[Index] = STATE_JOG_POSITIVE;
				}
				else if (VAxisFun.Axis[Index].Command.MoveJogNeg == 1)
				{
					VAxisStep[Index] = STATE_JOG_NEGATIVE;
				}
				else if (VAxisFun.Axis[Index].Command.MoveAbsolute == 1)
				{
					VAxisFun.Axis[Index].Command.MoveAbsolute = 0;
					VAxisStep[Index] = STATE_MOVE_ABSOLUTE;
				}
				else if (VAxisFun.Axis[Index].Command.MoveAdditive == 1)
				{
					VAxisFun.Axis[Index].Command.MoveAdditive = 0;
					VAxisStep[Index] = STATE_MOVE_ADDITIVE;
				}
				else if (VAxisFun.Axis[Index].Command.MoveVelocity == 1)
				{
					VAxisFun.Axis[Index].Command.MoveVelocity = 0;
					VAxisStep[Index] = STATE_MOVE_VELOCITY;
				}
				else if(VAxisFun.Axis[Index].Command.StartSlave == 1)
				{
					VAxisFun.Axis[Index].Command.StartSlave = 0;
					VAxisStep[Index] = STATE_AUTOMAT_INIT;
				}
				else if(VAxisFun.Axis[Index].Command.StopSlave == 1)
				{
					VAxisFun.Axis[Index].Command.StopSlave = 0;	
					VAxisStep[Index] = STATE_AUTOMAT_STOP;
				}							
				else if (VAxisFun.Axis[Index].Command.Halt == 1)
				{
					VAxisFun.Axis[Index].Command.Halt = 0;
					VAxisStep[Index] = STATE_HALT;   
				}
                else if(VAxisFun.Axis[Index].Command.ReadParID == 1)
                {
                    VAxisFun.Axis[Index].Command.ReadParID = 0;	
                    VAxisStep[Index] = STATE_READ_PARIDINFO;
                }
                else if(VAxisFun.Axis[Index].Command.WriteParID == 1)
                {
                    VAxisFun.Axis[Index].Command.WriteParID = 0; 
                    VAxisStep[Index] = STATE_WRITE_PARIDINFO;     
                }
                
				break;
			/******************** POWER ON **********************/
			case STATE_POWER_ON:  /* STATE: Power on */
				MC_Power_0[Index].Enable = 1;
				if (MC_Power_0[Index].Status == 1)
				{
				/*AxisStep[Index] = STATE_READY;*/
				VAxisStep[Index] = STATE_HOME;
				}
				/* if a power error occured go to error state */
				if (MC_Power_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_Power_0[Index].ErrorID;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			case STATE_POWER_OFF:
				MC_Power_0[Index].Enable = 0;
				if(MC_Power_0[Index].Status == 0)
				{
					VAxisStep[Index] = STATE_READY;
				}				
				if(MC_Power_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_Power_0[Index].ErrorID;
					VAxisStep[Index] = STATE_ERROR;				
				}
				break;
			/*************************user self define home***************************/
			case STATE_USER_HOME:
				MC_Home_0[Index].Position = VAxisFun.Axis[Index].Parameter.HomePosition;
				MC_Home_0[Index].HomingMode = VAxisFun.Axis[Index].Parameter.HomeMode;
				MC_Home_0[Index].Execute = 1;
				if (MC_Home_0[Index].Done == 1)
				{
					MC_Home_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_READY;
				}
				/* if a homing error occured go to error state */
				if (MC_Home_0[Index].Error == 1)
				{
					MC_Home_0[Index].Execute = 0;
					VAxisFun.Axis[Index].Status.ErrorID = MC_Home_0[Index].ErrorID;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			/************************HALT MOVEMENT**************************/    
			case STATE_HALT:  /* STATE: Stop the active movement */
				MC_Halt_0[Index].Deceleration = VAxisFun.Axis[Index].Parameter.Deceleration;
				MC_Halt_0[Index].Execute = 1;
				if (MC_Halt_0[Index].Done == 1)
				{
					MC_Halt_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_READY;  
				}
				/*check if error occured */
				if (MC_Halt_0[Index].Error == 1)
				{
					MC_Halt_0[Index].Execute = 0;
					VAxisFun.Axis[Index].Status.ErrorID = MC_Halt_0[Index].ErrorID;
					VAxisStep[Index] = STATE_ERROR;   
				}
				break;
			
			/*********************** STOP MOVEMENT *************************/
			case STATE_STOP: /* STATE: Stop movement as long as command is set */
				MC_Stop_0[Index].Deceleration = VAxisFun.Axis[Index].Parameter.Deceleration;
				MC_Stop_0[Index].Execute = 1;
				/* if axis is stopped go to ready state */
				if ((MC_Stop_0[Index].Done == 1) && (VAxisFun.Axis[Index].Command.Stop == 0))
				{
					MC_Stop_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_READY;
				}
				/* check if error occured */
				if (MC_Stop_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_Stop_0[Index].ErrorID;
					MC_Stop_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;

			/******************** START JOG MOVEMENT POSITVE **********************/
			case STATE_JOG_POSITIVE:  /* STATE: Start jog movement in positive direction */
				MC_MoveVelocity_0[Index].Velocity      = VAxisFun.Axis[Index].Parameter.JogVelocity;
				MC_MoveVelocity_0[Index].Acceleration  = VAxisFun.Axis[Index].Parameter.Acceleration;
				MC_MoveVelocity_0[Index].Deceleration  = VAxisFun.Axis[Index].Parameter.Deceleration;
				MC_MoveVelocity_0[Index].Direction     = mcPOSITIVE_DIR;
				MC_MoveVelocity_0[Index].Execute = 1;
				/* check if jog movement should be stopped */
				if (VAxisFun.Axis[Index].Command.MoveJogPos == 0)
				{
					MC_MoveVelocity_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_HALT;
				}
				/* check if error occured */
				if (MC_MoveVelocity_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_MoveVelocity_0[Index].ErrorID;
					MC_MoveVelocity_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;

			/******************** START JOG MOVEMENT NEGATIVE **********************/
			case STATE_JOG_NEGATIVE:  /* STATE: Start jog movement in negative direction */
				MC_MoveVelocity_0[Index].Velocity      = VAxisFun.Axis[Index].Parameter.JogVelocity;
				MC_MoveVelocity_0[Index].Acceleration  = VAxisFun.Axis[Index].Parameter.Acceleration;
				MC_MoveVelocity_0[Index].Deceleration  = VAxisFun.Axis[Index].Parameter.Deceleration;
				MC_MoveVelocity_0[Index].Direction     = mcNEGATIVE_DIR;
				MC_MoveVelocity_0[Index].Execute = 1;        
				/* check if jog movement should be stopped */
				if (VAxisFun.Axis[Index].Command.MoveJogNeg == 0)
				{
					MC_MoveVelocity_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_HALT;
				}
				/* check if error occured */
				if (MC_MoveVelocity_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_MoveVelocity_0[Index].ErrorID;
					MC_MoveVelocity_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			/******************** START ABSOLUTE MOVEMENT **********************/
			case STATE_MOVE_ABSOLUTE:  /* STATE: Start absolute movement */
				MC_MoveAbsolute_0[Index].Position      = VAxisFun.Axis[Index].Parameter.Position;
				MC_MoveAbsolute_0[Index].Velocity      = VAxisFun.Axis[Index].Parameter.Velocity;
				MC_MoveAbsolute_0[Index].Acceleration  = VAxisFun.Axis[Index].Parameter.Acceleration;
				MC_MoveAbsolute_0[Index].Deceleration  = VAxisFun.Axis[Index].Parameter.Deceleration;
				MC_MoveAbsolute_0[Index].Direction     = VAxisFun.Axis[Index].Parameter.Direction;
				MC_MoveAbsolute_0[Index].Execute = 1;
				/* check if commanded position is reached */
				if (VAxisFun.Axis[Index].Command.Halt == 1)
				{
					VAxisFun.Axis[Index].Command.Halt = 0;
					MC_MoveAbsolute_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_HALT;   
				}
				else if (MC_MoveAbsolute_0[Index].Done == 1)
				{
					MC_MoveAbsolute_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_READY;
				}
				/* check if error occured */
				if (MC_MoveAbsolute_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_MoveAbsolute_0[Index].ErrorID;
					MC_MoveAbsolute_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			/******************** START ADDITIVE MOVEMENT **********************/
			case STATE_MOVE_ADDITIVE:  /* STATE: Start additive movement */
				MC_MoveAdditive_0[Index].Distance      = VAxisFun.Axis[Index].Parameter.Distance;
				MC_MoveAdditive_0[Index].Velocity      = VAxisFun.Axis[Index].Parameter.Velocity;
				MC_MoveAdditive_0[Index].Acceleration  = VAxisFun.Axis[Index].Parameter.Acceleration;
				MC_MoveAdditive_0[Index].Deceleration  = VAxisFun.Axis[Index].Parameter.Deceleration;
				MC_MoveAdditive_0[Index].Execute = 1;
				/* check if commanded distance is reached */
				if (VAxisFun.Axis[Index].Command.Halt == 1)
				{
					VAxisFun.Axis[Index].Command.Halt = 0;
					MC_MoveAdditive_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_HALT;   
				}
				else if (MC_MoveAdditive_0[Index].Done == 1)
				{
					MC_MoveAdditive_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_READY;
				}
				/* check if error occured */
				if (MC_MoveAdditive_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_MoveAdditive_0[Index].ErrorID;
					MC_MoveAdditive_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			/******************** START VELOCITY MOVEMENT **********************/
			case STATE_MOVE_VELOCITY:  /* STATE: Start velocity movement */
				MC_MoveVelocity_0[Index].Velocity      = VAxisFun.Axis[Index].Parameter.Velocity;
				MC_MoveVelocity_0[Index].Acceleration  = VAxisFun.Axis[Index].Parameter.Acceleration;
				MC_MoveVelocity_0[Index].Deceleration  = VAxisFun.Axis[Index].Parameter.Deceleration;
				MC_MoveVelocity_0[Index].Direction     = VAxisFun.Axis[Index].Parameter.Direction;
				MC_MoveVelocity_0[Index].Execute = 1;
				/* check if commanded velocity is reached */
				if (VAxisFun.Axis[Index].Command.Halt == 1)
				{
					VAxisFun.Axis[Index].Command.Halt = 0;
					MC_MoveVelocity_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_HALT;    
				}
				else if (MC_MoveVelocity_0[Index].InVelocity == 1)
				{
					MC_MoveVelocity_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_READY;
				}
				/* check if error occured */
				if (MC_MoveVelocity_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_MoveVelocity_0[Index].ErrorID;
					MC_MoveVelocity_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			/******************** INITIALIZE AUTOMAT PARAMETER **********************/
			case STATE_AUTOMAT_INIT:/* STATE: Initialize Automat parameter */	
				MC_BR_AutControl_0[Index].Enable 			= 1;
				MC_BR_AutControl_0[Index].AdrAutData		= (UDINT)&VAutData[Index];
				MC_BR_AutControl_0[Index].InitAutData		= 1;
				if (MC_BR_AutControl_0[Index].AutDataInitialized)
				{
					MC_BR_AutControl_0[Index].InitAutData 	= 0;
					VAxisStep[Index] 						= STATE_AUTOMAT_START;
				}
				if(MC_BR_AutControl_0[Index].Error)
				{
					MC_BR_AutControl_0[Index].Enable 		= 0;
					MC_BR_AutControl_0[Index].Start			= 0;
					VAxisFun.Axis[Index].Status.ErrorID 	= MC_BR_AutControl_0[Index].ErrorID;
					VAxisStep[Index] 						= STATE_ERROR;
				}
				break;
			/******************** ENABLE CAM-AUTOMAT **********************/
			case STATE_AUTOMAT_START:/* STATE: Start Automat */
				MC_BR_AutControl_0[Index].Enable = 1;
				MC_BR_AutControl_0[Index].Start = 1;
				/* wait for Automat stop */
				if (VAxisFun.Axis[Index].Command.Halt == 1)
				{
					VAxisFun.Axis[Index].Command.Halt 	= 0;
					MC_BR_AutControl_0[Index].Enable 	= 0;
					MC_BR_AutControl_0[Index].Start 	= 0;
					VAxisStep[Index] 					= STATE_HALT;   
				}
				else if(MC_BR_AutControl_0[Index].Running) //耦合的命令也是凸轮的初始化命令
				{
					VAxisStep[Index] = STATE_READY;
				}
				/* check if error occured */
				if (MC_BR_AutControl_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_BR_AutControl_0[Index].ErrorID;
					MC_BR_AutControl_0[Index].Enable = 0;
					MC_BR_AutControl_0[Index].Start = 0;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			/******************** STOP CAM-AUTOMAT**********************/
			case STATE_AUTOMAT_STOP:/* STATE: Stop CAM-Automat*/
				MC_BR_AutControl_0[Index].Deceleration = VAxisFun.Axis[Index].Parameter.Deceleration;
				MC_BR_AutControl_0[Index].Stop = 1;
				/* check if automat is stopped */
				if (MC_BR_AutControl_0[Index].Running == 0)
				{
					MC_BR_AutControl_0[Index].Enable = 0;
					MC_BR_AutControl_0[Index].Stop = 0;
					VAxisStep[Index] = STATE_READY;
				}
				/* check if error occured */
				if (MC_BR_AutControl_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_BR_AutControl_0[Index].ErrorID;
					MC_BR_AutControl_0[Index].Enable = 0;
					MC_BR_AutControl_0[Index].Stop = 0;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			
			
			/******************** SELECT CAM TABLE **********************/
			case STATE_CAM_SELECT:  /* STATE: Select the CAM */
				strcpy((void*)&MC_CamTableSelect_0[Index].CamTable,(void*)VAxisFun.Axis[Index].CamName.down_cam_name[0]);
				MC_CamTableSelect_0[Index].Periodic = mcPERIODIC;
				MC_CamTableSelect_0[Index].Execute = 1;
				/* wait for CAM table selected */
				if (MC_CamTableSelect_0[Index].Done == 1)
				{
					CamTableID[Index] = MC_CamTableSelect_0[Index].CamTableID;
					MC_CamTableSelect_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_CAM_START;
				}
				/* check if error occured */
				if (MC_CamTableSelect_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_CamTableSelect_0[Index].ErrorID;
					MC_CamTableSelect_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;
			/******************** START CAM MOVEMENT **********************/
			case STATE_CAM_START:  /* STATE: Start electronic CAM coupling */
				MC_CamIn_0[Index].MasterOffset 	= VAxisFun.Axis[Index].CamPara.MasterOffset;
				MC_CamIn_0[Index].SlaveOffset 	= VAxisFun.Axis[Index].CamPara.SlaveOffset;
				MC_CamIn_0[Index].MasterScaling = VAxisFun.Axis[Index].CamPara.MasterScaling;
				MC_CamIn_0[Index].SlaveScaling 	= VAxisFun.Axis[Index].CamPara.SlaveScaling;
				MC_CamIn_0[Index].StartMode 	= VAxisFun.Axis[Index].CamPara.StartMode;
				MC_CamIn_0[Index].CamTableID 	= CamTableID[Index];
				MC_CamIn_0[Index].Execute 		= 1;
				/* wait for CAM stop */
				if (VAxisFun.Axis[Index].Command.Halt == 1)
				{
					VAxisFun.Axis[Index].Command.Halt = 0;
					MC_CamIn_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_HALT;   
				}
				else if (MC_CamIn_0[Index].InSync == 1)
				{
					MC_CamIn_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_READY;
				}
				/* check if error occured */
				if (MC_CamIn_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_CamIn_0[Index].ErrorID;
					MC_CamIn_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_ERROR;
				}				
				break;
			/******************** STOP CAM MOVEMENT **********************/
			case STATE_CAM_STOP:  /* STATE: Stop electronic CAM coupling */
				MC_CamOut_0[Index].Execute = 1;
				/* check if coupling is stopped */
				if (MC_CamOut_0[Index].Done == 1)
				{
					MC_CamOut_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_READY;
				}
				/* check if error occured */
				if (MC_CamOut_0[Index].Error == 1)
				{
					VAxisFun.Axis[Index].Status.ErrorID = MC_CamOut_0[Index].ErrorID;
					MC_CamOut_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_ERROR;
				}
				break;

            
            case STATE_READ_PARIDINFO:
                MC_BR_GetParIDInfo_0[Index].Execute = 1;
                MC_BR_GetParIDInfo_0[Index].ParID	= VAxisFun.Axis[Index].Parameter.ParID;
                if(MC_BR_GetParIDInfo_0[Index].Done)
                {
                    MC_BR_GetParIDInfo_0[Index].Execute = 0;
                    DataType[Index]	= MC_BR_GetParIDInfo_0[Index].ParIDInfo.DataType;
                    VAxisStep[Index] = STATE_READ_PARID;
                }
				
                if(MC_BR_GetParIDInfo_0[Index].Error)
                {
                    VAxisFun.Axis[Index].Status.ErrorID = MC_BR_GetParIDInfo_0[Index].ErrorID;
                    MC_BR_GetParIDInfo_0[Index].Execute = 0;
                    VAxisStep[Index] = STATE_ERROR;
                }
                break;
            case STATE_READ_PARID:
                MC_BR_ReadParID_0[Index].Execute        = 1;
                MC_BR_ReadParID_0[Index].DataAddress    = (UDINT)&VAxisFun.Axis[Index].Status.ReadIDValue;
                MC_BR_ReadParID_0[Index].DataType	    = DataType[Index];
                MC_BR_ReadParID_0[Index].ParID		    = VAxisFun.Axis[Index].Parameter.ParID;
                if(MC_BR_ReadParID_0[Index].Done)
                {
                    MC_BR_ReadParID_0[Index].Execute    = 0;
                    VAxisStep[Index] = STATE_READY;
                }
                if(MC_BR_ReadParID_0[Index].Error)
                {
                    VAxisFun.Axis[Index].Status.ErrorID  = MC_BR_ReadParID_0[Index].ErrorID;
                    MC_BR_ReadParID_0[Index].Execute    = 0;
                    VAxisStep[Index] = STATE_ERROR;
                }
                break;
            case STATE_WRITE_PARIDINFO:
                MC_BR_GetParIDInfo_0[Index].Execute = 1;
                MC_BR_GetParIDInfo_0[Index].ParID	= VAxisFun.Axis[Index].Parameter.ParID;
                if(MC_BR_GetParIDInfo_0[Index].Done)
                {
                    MC_BR_GetParIDInfo_0[Index].Execute = 0;
                    DataType[Index]	= MC_BR_GetParIDInfo_0[Index].ParIDInfo.DataType;
                    VAxisStep[Index] = STATE_WRITE_PARID;
                }
				
                if(MC_BR_GetParIDInfo_0[Index].Error)
                {
                    VAxisFun.Axis[Index].Status.ErrorID = MC_BR_GetParIDInfo_0[Index].ErrorID;
                    MC_BR_GetParIDInfo_0[Index].Execute = 0;
                    VAxisStep[Index] = STATE_ERROR;
                }
                break;
            case STATE_WRITE_PARID:
                MC_BR_WriteParID_0[Index].Execute       = 1;
                MC_BR_WriteParID_0[Index].DataAddress   =(UDINT)&VAxisFun.Axis[Index].Parameter.WriteIDValue;
                MC_BR_WriteParID_0[Index].DataType      =  DataType[Index];
                MC_BR_WriteParID_0[Index].ParID         =VAxisFun.Axis[Index].Parameter.ParID;
                if(MC_BR_WriteParID_0[Index].Done)
                {
                    MC_BR_WriteParID_0[Index].Execute       = 0;
                    VAxisStep[Index] = STATE_READY;
                }
                if(MC_BR_WriteParID_0[Index].Error)
                {
                    VAxisFun.Axis[Index].Status.ErrorID  = MC_BR_WriteParID_0[Index].ErrorID;
                    MC_BR_WriteParID_0[Index].Execute       = 0;
                    VAxisStep[Index] = STATE_ERROR;
                }
                break;		
			/******************** FB-ERROR OCCURED *************************/
			case STATE_ERROR:  /* STATE: Error */
				/* check if FB indicates an axis error */
				if (MC_ReadAxisError_0[Index].AxisErrorCount != 0)
				{
					VAxisStep[Index] = STATE_ERROR_AXIS;
				}
				else
				{
					if (VAxisFun.Axis[Index].Command.ErrorAcknowledge == 1)
					{
						VAxisFun.Axis[Index].Command.ErrorAcknowledge = 0;
						VAxisFun.Axis[Index].Status.ErrorID = 0;
						/* reset axis if it is in axis state ErrorStop */
						if ((MC_ReadStatus_0[Index].Errorstop == 1) && (MC_ReadStatus_0[Index].Valid == 1))
						{
							VAxisStep[Index] = STATE_ERROR_RESET;
						}
						else
						{
							VAxisStep[Index] = STATE_WAIT;
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
						VAxisFun.Axis[Index].Status.ErrorID = MC_ReadAxisError_0[Index].AxisErrorID;
					}
					MC_ReadAxisError_0[Index].Acknowledge = 0;
					if (VAxisFun.Axis[Index].Command.ErrorAcknowledge == 1)
					{
						VAxisFun.Axis[Index].Command.ErrorAcknowledge = 0;
						/* acknowledge axis error */
						if (MC_ReadAxisError_0[Index].AxisErrorID != 0)
						{
							MC_ReadAxisError_0[Index].Acknowledge = 1;
						}
					}
					if (MC_ReadAxisError_0[Index].AxisErrorCount == 0)
					{
						VAxisFun.Axis[Index].Status.ErrorID = 0;
						/* reset axis if it is in axis state ErrorStop */
						if ((MC_ReadStatus_0[Index].Errorstop == 1) && (MC_ReadStatus_0[Index].Valid == 1))
						{
							VAxisStep[Index] = STATE_ERROR_RESET;
						}
						else
						{
							VAxisStep[Index] = STATE_WAIT;
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
					VAxisStep[Index] = STATE_WAIT;
				}
				else if (MC_Reset_0[Index].Error == 1)
				{
					MC_Reset_0[Index].Execute = 0;
					VAxisStep[Index] = STATE_ERROR;   
				}
				break;
    
			/******************** SEQUENCE END *************************/

		}
		/***************************************************************
		        Function Block Calls
		***************************************************************/
		MC_BR_InitParTabObj_0[Index].Axis = VAxis1Obj[Index];
		MC_BR_InitParTabObj(&MC_BR_InitParTabObj_0[Index]);
		
		MC_BR_DownloadCamProfileObj_0[Index].Axis = VAxis1Obj[Index];
		MC_BR_DownloadCamProfileObj(&MC_BR_DownloadCamProfileObj_0[Index]);
		
		MC_BR_InitAxisPar_0[Index].Axis = VAxis1Obj[Index];
		MC_BR_InitAxisPar(&MC_BR_InitAxisPar_0[Index]);

		/************************** MC_POWER ****************************/
		MC_Power_0[Index].Axis = VAxis1Obj[Index];  /* pointer to axis */
		MC_Power(&MC_Power_0[Index]);

		/************************** MC_HOME *****************************/
		MC_Home_0[Index].Axis = VAxis1Obj[Index];
		MC_Home(&MC_Home_0[Index]);

		/********************** MC_MOVEABSOLUTE *************************/
		MC_MoveAbsolute_0[Index].Axis = VAxis1Obj[Index];
		MC_MoveAbsolute(&MC_MoveAbsolute_0[Index]);

		/********************** MC_MOVEADDITIVE *************************/
		MC_MoveAdditive_0[Index].Axis = VAxis1Obj[Index];
		MC_MoveAdditive(&MC_MoveAdditive_0[Index]);

		/********************** MC_MOVEVELOCITY *************************/
		MC_MoveVelocity_0[Index].Axis = VAxis1Obj[Index];
		MC_MoveVelocity(&MC_MoveVelocity_0[Index]);
		
		/**********************MC_BR_INITAUTDATA************************/
		MC_BR_InitAutData_0[Index].Slave = VAxis1Obj[Index];
		MC_BR_InitAutData(&MC_BR_InitAutData_0[Index]);

		/********************** MC_AUTCONTROL *************************/
		MC_BR_AutControl_0[Index].Slave 		= VAxis1Obj[Index];
		MC_BR_AutControl_0[Index].Signal1		= VAxisFun.Axis[Index].Command.Signal1;
		MC_BR_AutControl_0[Index].Signal2		= VAxisFun.Axis[Index].Command.Signal2;
		MC_BR_AutControl_0[Index].Signal3		= VAxisFun.Axis[Index].Command.Signal3;
		MC_BR_AutControl_0[Index].Signal4		= VAxisFun.Axis[Index].Command.Signal4;
		MC_BR_AutControl(&MC_BR_AutControl_0[Index]);
		VAxisFun.Axis[Index].Status.ActualStateIndex = MC_BR_AutControl_0[Index].ActualStateIndex;
		VAxisFun.Axis[Index].Status.ActualCamType	= MC_BR_AutControl_0[Index].ActualCamType;
		
		/********************** MC_CAMTABLESELECT ***********************/
		
		MC_CamTableSelect_0[Index].Slave = VAxis1Obj[Index];
		MC_CamTableSelect(&MC_CamTableSelect_0[Index]);

		/********************** MC_CAMIN ********************************/
		MC_CamIn_0[Index].Slave 			= VAxis1Obj[Index];
		MC_CamIn(&MC_CamIn_0[Index]);

		/********************** MC_CAMOUT *******************************/
		MC_CamOut_0[Index].Slave 			= VAxis1Obj[Index];
		MC_CamOut(&MC_CamOut_0[Index]);
		
		/************************** MC_STOP *****************************/
		MC_Stop_0[Index].Axis = VAxis1Obj[Index];
		MC_Stop(&MC_Stop_0[Index]);

		/**************************MC_HALT*******************************/
		MC_Halt_0[Index].Axis = VAxis1Obj[Index];
		MC_Halt(&MC_Halt_0[Index]);

		/************************** MC_RESET ****************************/
		MC_Reset_0[Index].Axis = VAxis1Obj[Index];
		MC_Reset(&MC_Reset_0[Index]);
		
		/************************MC_SetOverride**************************/
		MC_SetOverride_0[Index].Enable 		= !MC_SetOverride_0[Index].Error
                                              &&(!VAxisFun.Axis[Index].AxisState.Stopping)&&(!VAxisFun.Axis[Index].AxisState.ErrorStop)
                                              &&(!VAxisFun.Axis[Index].AxisState.Homing)  &&(!VAxisFun.Axis[Index].AxisState.Disabled);
		MC_SetOverride_0[Index].Axis 		= VAxis1Obj[Index];
		MC_SetOverride_0[Index].VelFactor 	= VAxisFun.Axis[Index].Parameter.Override_v;
		MC_SetOverride_0[Index].AccFactor 	= VAxisFun.Axis[Index].Parameter.Override_a;
		MC_SetOverride(&MC_SetOverride_0[Index]);
        
        /******Read PLCopen axis paramter*********************************/
        MC_ReadParameter_0[Index].Enable 	= (!MC_ReadParameter_0[Index].Error);
        MC_ReadParameter_0[Index].Axis 		=  VAxis1Obj[Index];		
        MC_ReadParameter_0[Index].ParameterNumber = mcAXIS_FACTOR;
        MC_ReadParameter(&MC_ReadParameter_0[Index]);
	}
}

void _EXIT AxisCtrlExit(void)
{
	// Insert code here 

}

