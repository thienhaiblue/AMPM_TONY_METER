/************************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.vn
----------------------------------
AMPM JSC
Add: 634/6 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN
**************************************************************************/

#include <tonymeter_process_function.h>
#include "tony_meter.h"
#include "process_tonymeter.h"

volatile	uint8_t		TonyBufferProcess[TONYMETER_ATCOMMAND_MAX_LENGTH];
static		uint8_t*	tonymeterarg_arr[TONYMETER_ATCOMMAND_MAX_ARGUMENT];
static 		uint8_t		tonymeterarg_count;
static 		void tonymeter_atcommand_split(uint8_t* inputData, uint8_t* argCount, uint8_t** argData);


const STR_INFO_Type RandomFeedBack_CallBack[TONYMETER_MAX_RANDOM_FEEDBACK] =
{
		{(uint8_t*)"+OVERLOADALERT:", OverCurrent_Alert_Callback },
		{(uint8_t*)"+UNDERVOLTALERT:", UnderVolt_Alert_Callback },
		{(uint8_t*)"+OVERVOLTALERT:", OverVolt_Alert_Callback },
		{(uint8_t*)"+RESIDUALALERT:", Residual_Current_Alert_Callback },
#if (TONYMETER_NOLOAD_ENABLE == 1)
		{(uint8_t*)"+NOLOADALERT:", No_Load_Alert_Callback },
#endif
		{(uint8_t*)"+TIMEOUTNOTIFY:", Timeout_Alert_Callback },
};



/* Define TonyMeter AT Command ReadID */
const static TONYMETER_AT_CMD_PACKET_TYPE TonyMeterAtCmd_ReadID[1] =
{
		{ (uint8_t*)TONY_READ_ID_AT_CMD, NULL, (uint8_t*)"+ID:", TonyMeterGetID_Callback,(uint8_t*)"\r\n", (uint8_t*)"ERROR" }
};


/* Define TonyMeter AT Command ReadAC Parameter */
const static TONYMETER_AT_CMD_PACKET_TYPE TonyMeterAtCmd_ReadAC[TONYMETER_MAX_CHANNEL] =
{
		{ (uint8_t*)TONY_READ_AC0_AT_CMD, NULL, (uint8_t*)"+READ:", TonyMeterReadAC_Callback, (uint8_t*) "\r\n", (uint8_t*)"ERROR" },
		{ (uint8_t*)TONY_READ_AC1_AT_CMD, NULL, (uint8_t*)"+READ:", TonyMeterReadAC_Callback, (uint8_t*) "\r\n", (uint8_t*)"ERROR" },
		{ (uint8_t*)TONY_READ_AC2_AT_CMD, NULL, (uint8_t*)"+READ:", TonyMeterReadAC_Callback, (uint8_t*) "\r\n", (uint8_t*)"ERROR" },
		{ (uint8_t*)TONY_READ_AC3_AT_CMD, NULL, (uint8_t*)"+READ:", TonyMeterReadAC_Callback, (uint8_t*) "\r\n", (uint8_t*)"ERROR" },

};


/* Define TonyMeter AT Command Read Residual Current ADC Parameter */
const static TONYMETER_AT_CMD_PACKET_TYPE TonyMeterAtCmd_ReadResidualCurrent[TONYMETER_MAX_CHANNEL] =
{
		{ (uint8_t*)TONY_READ_RESIDUAL0_AT_CMD, NULL, (uint8_t*)"+RESIDUAL:", TonyMeterReadResidualCurrent_Callback, (uint8_t*) "\r\n", (uint8_t*)"ERROR" },
		{ (uint8_t*)TONY_READ_RESIDUAL1_AT_CMD, NULL, (uint8_t*)"+RESIDUAL:", TonyMeterReadResidualCurrent_Callback, (uint8_t*) "\r\n", (uint8_t*)"ERROR" },
		{ (uint8_t*)TONY_READ_RESIDUAL2_AT_CMD, NULL, (uint8_t*)"+RESIDUAL:", TonyMeterReadResidualCurrent_Callback, (uint8_t*) "\r\n", (uint8_t*)"ERROR" },
		{ (uint8_t*)TONY_READ_RESIDUAL3_AT_CMD, NULL, (uint8_t*)"+RESIDUAL:", TonyMeterReadResidualCurrent_Callback, (uint8_t*) "\r\n", (uint8_t*)"ERROR" },

};

/* Define TonyMeter AT Command resetEnergyCounter */
const uint8_t* TONY_RESET_ENERGY_AT_CMD_ARR[TONYMETER_MAX_CHANNEL] =
{
		(uint8_t*)TONY_RESET_ENERGY0_AT_CMD,
		(uint8_t*)TONY_RESET_ENERGY1_AT_CMD,
		(uint8_t*)TONY_RESET_ENERGY2_AT_CMD,
		(uint8_t*)TONY_RESET_ENERGY3_AT_CMD
};
const static TONYMETER_AT_CMD_PACKET_TYPE TonyMeterAtCmd_ResetEnergyCounter[TONYMETER_MAX_CHANNEL] =
{
		{ (uint8_t*)TONY_RESET_ENERGY0_AT_CMD, NULL, (uint8_t*)TONY_RETURN_OK_AT_CMD, NULL, NULL, NULL },
		{ (uint8_t*)TONY_RESET_ENERGY1_AT_CMD, NULL, (uint8_t*)TONY_RETURN_OK_AT_CMD, NULL, NULL, NULL },
		{ (uint8_t*)TONY_RESET_ENERGY2_AT_CMD, NULL, (uint8_t*)TONY_RETURN_OK_AT_CMD, NULL, NULL, NULL },
		{ (uint8_t*)TONY_RESET_ENERGY3_AT_CMD, NULL, (uint8_t*)TONY_RETURN_OK_AT_CMD, NULL, NULL, NULL },
};



/* Define TonyMeter AT Command Set Relay */
const uint8_t*	TONY_SET_RELAY_AT_CMD_ARR[TONYMETER_MAX_CHANNEL][2] =
{
		{
				(uint8_t*)TONY_SET_RELAY00_AT_CMD,
				(uint8_t*)TONY_SET_RELAY01_AT_CMD,
		},
		{
				(uint8_t*)TONY_SET_RELAY10_AT_CMD,
				(uint8_t*)TONY_SET_RELAY11_AT_CMD,
		},
		{
				(uint8_t*)TONY_SET_RELAY20_AT_CMD,
				(uint8_t*)TONY_SET_RELAY21_AT_CMD,
		},
		{
				(uint8_t*)TONY_SET_RELAY30_AT_CMD,
				(uint8_t*)TONY_SET_RELAY31_AT_CMD,
		},
};

const static TONYMETER_AT_CMD_PACKET_TYPE TonyMeterAtCmd_SetRelay[TONYMETER_MAX_CHANNEL][2] =
{
		{
				{ (uint8_t*)TONY_SET_RELAY00_AT_CMD, NULL, (uint8_t*)TONY_RETURN_OK_AT_CMD, NULL, NULL, NULL },
				{ (uint8_t*)TONY_SET_RELAY01_AT_CMD, NULL, (uint8_t*)TONY_RETURN_OK_AT_CMD, NULL, NULL, NULL },
		},

		{
				{ (uint8_t*)TONY_SET_RELAY10_AT_CMD, NULL, (uint8_t*)TONY_RETURN_OK_AT_CMD, NULL, NULL, NULL },
				{ (uint8_t*)TONY_SET_RELAY11_AT_CMD, NULL, (uint8_t*)TONY_RETURN_OK_AT_CMD, NULL, NULL, NULL },
		},

		{
				{ (uint8_t*)TONY_SET_RELAY20_AT_CMD, NULL, (uint8_t*)TONY_RETURN_OK_AT_CMD, NULL, NULL, NULL },
				{ (uint8_t*)TONY_SET_RELAY21_AT_CMD, NULL, (uint8_t*)TONY_RETURN_OK_AT_CMD, NULL, NULL, NULL },
		},

		{
				{ (uint8_t*)TONY_SET_RELAY30_AT_CMD, NULL, (uint8_t*)TONY_RETURN_OK_AT_CMD, NULL, NULL, NULL },
				{ (uint8_t*)TONY_SET_RELAY31_AT_CMD, NULL, (uint8_t*)TONY_RETURN_OK_AT_CMD, NULL, NULL, NULL },
		},
};



/* Define TonyMeter AT Command Set Channel Status */
const uint8_t* TONY_ENABLE_AT_CMD_ARR[TONYMETER_MAX_CHANNEL][2] =
{
		{
				(uint8_t*)TONY_ENABLE00_AT_CMD,
				(uint8_t*)TONY_ENABLE01_AT_CMD,
		},
		{
				(uint8_t*)TONY_ENABLE10_AT_CMD,
				(uint8_t*)TONY_ENABLE11_AT_CMD,
		},
		{
				(uint8_t*)TONY_ENABLE20_AT_CMD,
				(uint8_t*)TONY_ENABLE21_AT_CMD,
		},
		{
				(uint8_t*)TONY_ENABLE30_AT_CMD,
				(uint8_t*)TONY_ENABLE31_AT_CMD,
		},
};


/* Define TonyMeter AT Command change Relay Pin */
const uint8_t* TONY_SET_RELAYPIN_AT_CMD_ARR[TONYMETER_MAX_CHANNEL] =
{
		(uint8_t*)TONY_SET_RELAYPIN0_AT_CMD,
		(uint8_t*)TONY_SET_RELAYPIN1_AT_CMD,
		(uint8_t*)TONY_SET_RELAYPIN2_AT_CMD,
		(uint8_t*)TONY_SET_RELAYPIN3_AT_CMD,
};


/* Define TonyMeter AT Command Set ADC */
const uint8_t* TONY_SET_ADC_AT_CMD_ARR[TONYMETER_MAX_CHANNEL] =
{
		(uint8_t*)TONY_SET_ADC0_AT_CMD,
		(uint8_t*)TONY_SET_ADC1_AT_CMD,
		(uint8_t*)TONY_SET_ADC2_AT_CMD,
		(uint8_t*)TONY_SET_ADC3_AT_CMD,
};


/* Define TonyMeter AT Command change Relay Polarity */
const uint8_t* TONY_RELAY_POLARITY_AT_CMD_ARR[TONYMETER_MAX_CHANNEL] =
{
		(uint8_t*)TONY_RELAY_POLARITY0_AT_CMD,
		(uint8_t*)TONY_RELAY_POLARITY1_AT_CMD,
		(uint8_t*)TONY_RELAY_POLARITY2_AT_CMD,
		(uint8_t*)TONY_RELAY_POLARITY3_AT_CMD,
};


/* Define TonyMeter AT Command change Relay On Delay */
const uint8_t* TONY_ONDELAY_AT_CMD_ARR[TONYMETER_MAX_CHANNEL] =
{
		(uint8_t*)TONY_ONDELAY0_AT_CMD,
		(uint8_t*)TONY_ONDELAY1_AT_CMD,
		(uint8_t*)TONY_ONDELAY2_AT_CMD,
		(uint8_t*)TONY_ONDELAY3_AT_CMD,
};


/* Define TonyMeter AT Command change Relay Off Delay */
const uint8_t* TONY_OFFDELAY_AT_CMD_ARR[TONYMETER_MAX_CHANNEL] =
{
		(uint8_t*)TONY_OFFDELAY0_AT_CMD,
		(uint8_t*)TONY_OFFDELAY1_AT_CMD,
		(uint8_t*)TONY_OFFDELAY2_AT_CMD,
		(uint8_t*)TONY_OFFDELAY3_AT_CMD,
};


/* Define TonyMeter AT Command Over Load protect */
const uint8_t* TONY_OVERLOAD_AT_CMD_ARR[TONYMETER_MAX_CHANNEL] =
{
		(uint8_t*)TONY_OVERLOAD0_AT_CMD,
		(uint8_t*)TONY_OVERLOAD1_AT_CMD,
		(uint8_t*)TONY_OVERLOAD2_AT_CMD,
		(uint8_t*)TONY_OVERLOAD3_AT_CMD,
};

/* Define TonyMeter AT Command Over Load protect */
const uint8_t* TONY_NOLOAD_AT_CMD_ARR[TONYMETER_MAX_CHANNEL] =
{
		(uint8_t*)TONY_NOLOAD0_AT_CMD,
		(uint8_t*)TONY_NOLOAD1_AT_CMD,
		(uint8_t*)TONY_NOLOAD2_AT_CMD,
		(uint8_t*)TONY_NOLOAD3_AT_CMD,
};


/* Define TonyMeter AT Command UnderVolt protect */
const uint8_t* TONY_UNDERVOL_AT_CMD_ARR[1] =
{
		(uint8_t*)TONY_UNDERVOLT_AT_CMD,
};

/* Define TonyMeter AT Command OverVolt protect */
const uint8_t* TONY_OVERVOLT_AT_CMD_ARR[1] =
{
		(uint8_t*)TONY_OVERVOLT_AT_CMD,
};

/* Define TonyMeter AT Command Residual Current protect */
const uint8_t* TONY_RESIDUAL_CURRENT_AT_CMD_ARR[TONYMETER_MAX_CHANNEL] =
{
		(uint8_t*)TONY_RESIDUAL_CURRENT0_AT_CMD,
		(uint8_t*)TONY_RESIDUAL_CURRENT1_AT_CMD,
		(uint8_t*)TONY_RESIDUAL_CURRENT2_AT_CMD,
		(uint8_t*)TONY_RESIDUAL_CURRENT3_AT_CMD,
};


/* Define TonyMeter AT Command Residual Current protect */
const uint8_t* TONY_READ_RESIDUAL_CURRENT_AT_CMD_ARR[TONYMETER_MAX_CHANNEL] =
{
		(uint8_t*)TONY_READ_RESIDUAL0_AT_CMD,
		(uint8_t*)TONY_READ_RESIDUAL1_AT_CMD,
		(uint8_t*)TONY_READ_RESIDUAL2_AT_CMD,
		(uint8_t*)TONY_READ_RESIDUAL3_AT_CMD,
};



const TONYMETER_CMD_PROCESS_TYPE TonyMeter_ReadIDProcess[1]		=
{
		{ NULL, (void *)&TonyMeterAtCmd_ReadID[0], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 }
};

const TONYMETER_CMD_PROCESS_TYPE TonyMeter_ResetEnergyCounterProcess[TONYMETER_MAX_CHANNEL]		=
{
		{ NULL, (void *)&TonyMeterAtCmd_ResetEnergyCounter[0], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		{ NULL, (void *)&TonyMeterAtCmd_ResetEnergyCounter[1], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		{ NULL, (void *)&TonyMeterAtCmd_ResetEnergyCounter[2], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		{ NULL, (void *)&TonyMeterAtCmd_ResetEnergyCounter[3], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 }
};

const TONYMETER_CMD_PROCESS_TYPE TonyMeter_ReadACProcess[TONYMETER_MAX_CHANNEL]		=
{
		{ NULL, (void *)&TonyMeterAtCmd_ReadAC[0], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		{ NULL, (void *)&TonyMeterAtCmd_ReadAC[1], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		{ NULL, (void *)&TonyMeterAtCmd_ReadAC[2], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		{ NULL, (void *)&TonyMeterAtCmd_ReadAC[3], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 }
};


const TONYMETER_CMD_PROCESS_TYPE TonyMeter_ReadResidualCurrentProcess[TONYMETER_MAX_CHANNEL]		=
{
		{ NULL, (void *)&TonyMeterAtCmd_ReadResidualCurrent[0], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		{ NULL, (void *)&TonyMeterAtCmd_ReadResidualCurrent[1], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		{ NULL, (void *)&TonyMeterAtCmd_ReadResidualCurrent[2], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		{ NULL, (void *)&TonyMeterAtCmd_ReadResidualCurrent[3], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 }
};

const TONYMETER_CMD_PROCESS_TYPE TonyMeter_SetRelayProcess[TONYMETER_MAX_CHANNEL][2]		=
{
		{
				{ NULL, (void *)&TonyMeterAtCmd_SetRelay[0][0], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
				{ NULL, (void *)&TonyMeterAtCmd_SetRelay[0][1], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		},

		{
				{ NULL, (void *)&TonyMeterAtCmd_SetRelay[1][0], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
				{ NULL, (void *)&TonyMeterAtCmd_SetRelay[1][1], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		},

		{
				{ NULL, (void *)&TonyMeterAtCmd_SetRelay[2][0], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
				{ NULL, (void *)&TonyMeterAtCmd_SetRelay[2][1], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		},

		{
				{ NULL, (void *)&TonyMeterAtCmd_SetRelay[3][0], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
				{ NULL, (void *)&TonyMeterAtCmd_SetRelay[3][1], TonyMeter_SendCallbackDefault, TonyMeter_RecvCallbackDefault, NULL, TONYMETER_TRY_AGAIN_NUMBER_DEFAULT, TONYMETER_RECEIVE_TIMEOUT_DEFAULT, 100 },
		},
};




static void tonymeter_atcommand_split(uint8_t* inputData, uint8_t* argCount, uint8_t** argData)
{
	uint8_t* 	readp;
	uint8_t    	inside_token = 0;

	if((inputData != NULL) && (argCount != NULL) && (argData != NULL))
	{
		*argCount = 0;
		for (readp = inputData; *readp; readp++)
		{
			if (!inside_token)
			{
				if ((*readp == ':') || (*readp == ',') || (*readp == '\r') || (*readp == '\n'))
				{
					/* :,\r\n is not copied */
				}
				else
				{
					/* Start of token */
					inside_token = 1;
					argData[*argCount] = readp;
					(*argCount)++;
				}
			}
			else
			{
				/* inside token */
				if ((*readp == ':') || (*readp == ',') || (*readp == '\r') || (*readp == '\n'))
				{
					/* End of token */
					inside_token = 0;

					/* Assign null for terminate token */
					*readp =  0;
				}
				else
				{
					/* Continuation of token */
				}
			}
		}

		if (inside_token)
		{
			/* End of input line terminates a token */
			*readp = 0;
			readp++;
		}

		/* Null-terminate just to be nice */
		argData[*argCount] = 0;
	}
	else
	{
		assert_param(false);
	}
}


/*------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------- TonyMeter Read Info Callback ---------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/

uint32_t TonyMeterGetID_Callback(uint16_t cnt,uint8_t c)
{
	TonyBufferProcess[cnt] = c;

	if((c == '\r') || (c == '\n') || cnt >= sizeof(TonyBufferProcess))
	{
		TonyBufferProcess[cnt] = '\0';
		tonymeter_atcommand_split((uint8_t*)TonyBufferProcess, (uint8_t*)&tonymeterarg_count, (uint8_t**)tonymeterarg_arr);
		return 0;
	}
	return 0xff;
}


uint32_t TonyMeterReadAC_Callback(uint16_t cnt,uint8_t c)
{
	TonyBufferProcess[cnt] = c;

	if((c == '\r') || (c == '\n') || cnt >= sizeof(TonyBufferProcess))
	{
		TonyBufferProcess[cnt] = '\0';
		tonymeter_atcommand_split((uint8_t*)TonyBufferProcess, (uint8_t*)&tonymeterarg_count, (uint8_t**)tonymeterarg_arr);

		uint16_t voltage_V, current_mA, power_W, energy_Wh;

		uint8_t eChannel = atoi((const char *)tonymeterarg_arr[0]);
		voltage_V = atoi((const char *)tonymeterarg_arr[1])/100;		/*	(V)		*/
		current_mA = atoi((const char *)tonymeterarg_arr[2]); 			/*	(mA) 	*/
		power_W = atoi((const char *)tonymeterarg_arr[3])/100;			/*	(W) 	*/
		energy_Wh = atoi((const char *)tonymeterarg_arr[4]);			/*	(Wh) 	*/

		printf("\r\n[Channel, Vol, Cur, Pow, Energy] = %d  %d(V),  %d(mA),  %d(W),  %d(Wh)",eChannel, voltage_V, current_mA, power_W, energy_Wh);

		switch (eChannel)
		{
		case 0:

			break;
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		default:
			printf("\r\nRead AC Callback not handle this case %d !!!", eChannel);
			assert_param(false);
			break;
		}

		return 0;
	}
	return 0xff;
}


uint32_t TonyMeterReadResidualCurrent_Callback(uint16_t cnt,uint8_t c)
{
	TonyBufferProcess[cnt] = c;

	if((c == '\r') || (c == '\n') || cnt >= sizeof(TonyBufferProcess))
	{
		TonyBufferProcess[cnt] = '\0';
		tonymeter_atcommand_split((uint8_t*)TonyBufferProcess, (uint8_t*)&tonymeterarg_count, (uint8_t**)tonymeterarg_arr);

		uint8_t eChannel = atoi((const char *)tonymeterarg_arr[0]);
		uint16_t current_mA = atoi((const char *)tonymeterarg_arr[1]); 			/*	(mA) 	*/

		printf("\r\nResidual current channel %d = %d mA",eChannel, current_mA);
		return 0;
	}
	return 0xff;
}


/*------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------- TonyMeter Alert Callback -------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/


uint32_t OverCurrent_Alert_Callback(uint16_t cnt,uint8_t c)
{
	TonyBufferProcess[cnt] = c;

	if((c == '\r') || (c == '\n') || cnt >= sizeof(TonyBufferProcess))
	{
		TonyBufferProcess[cnt] = '\0';
		tonymeter_atcommand_split((uint8_t*)TonyBufferProcess, (uint8_t*)&tonymeterarg_count, (uint8_t**)tonymeterarg_arr);
		printf("\r\nOver Current Alert at channel = %s",tonymeterarg_arr[0]);

		ao_tony_meter.alertStatus = eAlert_Status_OverCurrent;
		ao_tony_meter.alertChannel = (Alert_Channel)atoi((const char*)tonymeterarg_arr[0]);
		ao_tony_meter.relayStatus[ao_tony_meter.alertChannel] = false;

		/* Set signal status off for channel is protected */
		switch(ao_tony_meter.alertChannel)
		{
		case eAlert_Channel_0:
			ao_tony_meter.errorCode[eAlert_Channel_0] |= (1 << eAlert_Status_OverCurrent_Bit);
			break;
		case eAlert_Channel_1:
			ao_tony_meter.errorCode[eAlert_Channel_1] |= (1 << eAlert_Status_OverCurrent_Bit);
			break;
		case eAlert_Channel_2:
			ao_tony_meter.errorCode[eAlert_Channel_2] |= (1 << eAlert_Status_OverCurrent_Bit);
			break;
		case eAlert_Channel_3:
			ao_tony_meter.errorCode[eAlert_Channel_3] |= (1 << eAlert_Status_OverCurrent_Bit);
			break;
		case eAlert_Channel_all:
			break;
		}

		return 0;
	}
	return 0xff;
}

uint32_t UnderVolt_Alert_Callback(uint16_t cnt,uint8_t c)
{
	TonyBufferProcess[cnt] = c;

	if((c == '\r') || (c == '\n') || cnt >= sizeof(TonyBufferProcess))
	{
		TonyBufferProcess[cnt] = '\0';
		printf("\r\nUnder Volt Alert !!!");
		ao_tony_meter.alertStatus = eAlert_Status_UnderVol;
		ao_tony_meter.alertChannel = eAlert_Channel_all;

		for(uint8_t index = 0; index < TONYMETER_MAX_CHANNEL; ++index)
		{
			ao_tony_meter.errorCode[index] |= (1 << eAlert_Status_UnderVol_Bit);
			ao_tony_meter.relayStatus[index] = false;
		}
		return 0;
	}
	return 0xff;
}


uint32_t OverVolt_Alert_Callback(uint16_t cnt,uint8_t c)
{
	TonyBufferProcess[cnt] = c;

	if((c == '\r') || (c == '\n') || cnt >= sizeof(TonyBufferProcess))
	{
		TonyBufferProcess[cnt] = '\0';
		printf("\r\nOver Volt Alert !!!");

		ao_tony_meter.alertStatus = eAlert_Status_OverVol;
		ao_tony_meter.alertChannel = eAlert_Channel_all;

		for(uint8_t index = 0; index < TONYMETER_MAX_CHANNEL; ++index)
		{
			ao_tony_meter.errorCode[index] |= (1 << eAlert_Status_OverVol_Bit);
			ao_tony_meter.relayStatus[index] = false;
		}

		return 0;
	}
	return 0xff;
}



uint32_t Residual_Current_Alert_Callback(uint16_t cnt,uint8_t c)
{
	TonyBufferProcess[cnt] = c;

	if((c == '\r') || (c == '\n') || cnt >= sizeof(TonyBufferProcess))
	{
		TonyBufferProcess[cnt] = '\0';
		tonymeter_atcommand_split((uint8_t*)TonyBufferProcess, (uint8_t*)&tonymeterarg_count, (uint8_t**)tonymeterarg_arr);
		printf("\r\nResidual current alert channel %s",tonymeterarg_arr[0]);

		ao_tony_meter.alertStatus = eAlert_Status_ResidualCurrent;
		ao_tony_meter.alertChannel = atoi((const char*)tonymeterarg_arr[0]);
		ao_tony_meter.relayStatus[ao_tony_meter.alertChannel] = false;

		/* Set signal status off for channel is protected */
		switch(ao_tony_meter.alertChannel)
		{
		case eAlert_Channel_0:

			ao_tony_meter.errorCode[eAlert_Channel_0] |= (1 << eAlert_Status_ResidualCurrent_Bit);

			break;
		case eAlert_Channel_1:

			ao_tony_meter.errorCode[eAlert_Channel_1] |= (1 << eAlert_Status_ResidualCurrent_Bit);

			break;
		case eAlert_Channel_2:

			ao_tony_meter.errorCode[eAlert_Channel_2] |= (1 << eAlert_Status_ResidualCurrent_Bit);

			break;
		case eAlert_Channel_3:

			ao_tony_meter.errorCode[eAlert_Channel_3] |= (1 << eAlert_Status_ResidualCurrent_Bit);

			break;
		case eAlert_Channel_all:
			break;
		}
	}
	return 0xff;
}


uint32_t No_Load_Alert_Callback(uint16_t cnt,uint8_t c)
{
	TonyBufferProcess[cnt] = c;

	if((c == '\r') || (c == '\n') || cnt >= sizeof(TonyBufferProcess))
	{
		TonyBufferProcess[cnt] = '\0';
		tonymeter_atcommand_split((uint8_t*)TonyBufferProcess, (uint8_t*)&tonymeterarg_count, (uint8_t**)tonymeterarg_arr);
		printf("\r\nNo Load alert channel %s !!!",tonymeterarg_arr[0]);

		/* Set signal status off for channel is protected */
		switch(atoi((const char*)tonymeterarg_arr[0]))
		{
		case 0:

			break;
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		}
		return 0;
	}
	return 0xff;
}


uint32_t Timeout_Alert_Callback(uint16_t cnt,uint8_t c)
{
	TonyBufferProcess[cnt] = c;

	if((c == '\r') || (c == '\n') || cnt >= sizeof(TonyBufferProcess))
	{
		TonyBufferProcess[cnt] = '\0';
		tonymeter_atcommand_split((uint8_t*)TonyBufferProcess, (uint8_t*)&tonymeterarg_count, (uint8_t**)tonymeterarg_arr);
		printf("\r\nTimeout alert channel %s !!!",tonymeterarg_arr[0]);

		/* Set signal status off for channel is protected */
		switch(atoi((const char*)tonymeterarg_arr[0]))
		{
		case 0:

			break;
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		}
		return 0;
	}
	return 0xff;
}
