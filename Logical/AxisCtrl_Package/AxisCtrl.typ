
TYPE
	basic_command_typ : 	STRUCT  (*command structure*)
		Power : BOOL; (*switch on the controller*)
		PowerOff : BOOL;
		Home : BOOL; (*reference the axis*)
		MoveAbsolute : BOOL; (*move to an defined position*)
		MoveAdditive : BOOL; (*move a defiened distance*)
		MoveVelocity : BOOL; (*start a movement with a defiened velocity*)
		Halt : BOOL; (*stop every active movement once*)
		Stop : BOOL; (*stop every active movement as long as set*)
		MoveJogPos : BOOL; (*move in positive direction as long as is set*)
		MoveJogNeg : BOOL; (*move in negative direction as long as is set*)
		StartSlave : BOOL; (*start coupling beetween master and slave*)
		StopSlave : BOOL; (*stop coupling between master and slave*)
		Signal1 : BOOL;
		Signal2 : BOOL;
		Signal3 : BOOL;
		Signal4 : BOOL;
		CamStart : BOOL;
		CamStop : BOOL;
		ReadParID : BOOL;
		WriteParID : BOOL;
		SLOffset : BOOL;
		ErrorAcknowledge : BOOL; (*reset active errors*)
	END_STRUCT;
	cam_parameter_typ : 	STRUCT 
		MasterOffset : REAL; (*offset of master*)
		SlaveOffset : REAL; (*offset of slave*)
		MasterScaling : REAL; (*scaling of master axis *)
		SlaveScaling : REAL; (*scaling of slave axis*)
		StartMode : USINT; (*start mode*)
	END_STRUCT;
	basic_parameter_typ : 	STRUCT  (*parameter structure*)
		Position : REAL; (*target-position for MoveAbsolute-Command*)
		Distance : REAL; (*distance for MoveAdditive-Command*)
		Velocity : REAL; (*velocity for MoveVelocity-Command*)
		Direction : USINT; (*direction for commanded movements*)
		Acceleration : REAL; (*acceleration for commanded movements*)
		Deceleration : REAL; (*deceleration for commanded movements*)
		HomePosition : REAL; (*target-position for referencing the axis*)
		HomeMode : USINT; (*homing mode*)
		JogVelocity : REAL; (*velocity for jogging movement*)
		Override_v : REAL;
		Override_a : REAL;
		ParID : UINT;
		WriteIDValue : DINT;
		Shift : REAL;
		ShiftVelocity : REAL;
		ShiftAcceleration : REAL;
		AppDistance : REAL;
		AppMode : UINT;
		ShiftMode : UINT;
	END_STRUCT;
	basic_status_typ : 	STRUCT  (*status structure*)
		ErrorID : UINT; (*ErrorID of any occured error*)
		ErrorText : ARRAY[0..3]OF STRING[79]; (*Error Text*)
		ActPosition : REAL; (*actual position of the axis*)
		ActVelocity : REAL; (*actual velocity of the axis*)
		ActualStateIndex : USINT; (*act state index*)
		ActualCamType : USINT; (*cam type  or compensation type*)
		ReadIDValue : DINT;
		DriveStatus : MC_DRIVESTATUS_TYP; (*actual status of the axis*)
	END_STRUCT;
	basic_axisState_typ : 	STRUCT  (*axis state structure*)
		Disabled : BOOL; (*if set, axis is in state Disabled*)
		StandStill : BOOL; (*if set, axis is in state StandsStill*)
		Homing : BOOL; (*if set, axis is in state Homing*)
		Stopping : BOOL; (*if set, axis is in state Stopping*)
		DiscreteMotion : BOOL; (*if set, axis is in state DiscreteMotion*)
		ContinuousMotion : BOOL; (*if set, axis is in state ContinuousMotion*)
		SynchronizedMotion : BOOL; (*if set, axis is in state SynchronizedMotion*)
		ErrorStop : BOOL; (*if set, axis is in state ErrorStop*)
	END_STRUCT;
	basic_cam_typ : 	STRUCT 
		down_cam_name : ARRAY[0..MAX_CAM_NUM_MINUS]OF STRING[MAX_OBJ_LEN];
	END_STRUCT;
	basic_para_typ : 	STRUCT 
		down_para_name : ARRAY[0..MAX_PARA_NUM_MINUS]OF STRING[MAX_OBJ_LEN];
	END_STRUCT;
	basic_typ : 	STRUCT  (*control structure*)
		Command : basic_command_typ; (*command structure*)
		Parameter : basic_parameter_typ; (*parameter structure*)
		Status : basic_status_typ; (*status structure*)
		AxisState : basic_axisState_typ; (*axis state structure*)
		CamPara : cam_parameter_typ;
		CamName : basic_cam_typ;
		ParaName : basic_para_typ;
	END_STRUCT;
	axis_typ : 	STRUCT 
		Axis : ARRAY[0..MAX_AXIS_MINUS]OF basic_typ;
	END_STRUCT;
	axis_step : 
		(
		STATE_WAIT := 0,
		STATE_DOWNLOAD_CAM_PROFILE := 10,
		STATE_WAIT_DOWNLOAD_CAM_PROFILE := 20,
		STATE_DOWNLOAD_PARA_TABLE := 30,
		STATE_WAIT_DOWNLOAD_PARA_TABLE := 40,
		STATE_CHECK_POWER := 50,
		STATE_GLOBAL_INIT := 60,
		STATE_WAIT_GLOBAL_INIT := 70,
		STATE_INIT_POWER_ON := 80,
		STATE_HOME := 90,
		STATE_READY := 100,
		STATE_POWER_OFF := 110,
		STATE_POWER_ON := 120,
		STATE_USER_HOME := 130,
		STATE_HALT := 140,
		STATE_STOP := 150,
		STATE_JOG_POSITIVE := 160,
		STATE_JOG_NEGATIVE := 170,
		STATE_MOVE_ABSOLUTE := 180,
		STATE_MOVE_ADDITIVE := 190,
		STATE_MOVE_VELOCITY := 200,
		STATE_AUTOMAT_INIT := 210,
		STATE_AUTOMAT_START := 220,
		STATE_AUTOMAT_STOP := 230,
		STATE_CAM_SELECT := 240,
		STATE_CAM_START := 250,
		STATE_CAM_STOP := 260,
		STATE_BR_OFFSET_INIT := 270,
		STATE_BR_WAIT_OFFSET_INIT := 280,
		STATE_BR_OFFSET := 290,
		STATE_WAIT_BR_OFFSET := 292,
		STATE_READ_PARIDINFO := 300,
		STATE_READ_PARID := 310,
		STATE_WRITE_PARIDINFO,
		STATE_WRITE_PARID,
		STATE_ERROR_AXIS := 350,
		STATE_ERROR := 360,
		STATE_ERROR_RESET := 370,
		STATE_BR_OFFSET_DSC := 380
		);
END_TYPE
