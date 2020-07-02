#include <process_tonymeter.h>
#include <process_tonymeter.h>
#include "application_config.h"
#include "hardware_config.h"
#include "tony_meter.h"
#include "tonymeter_process_function.h"
/************************** Constant Definitions *****************************/
#define TONY_DBG  PRINTF
/**************************** Type Definitions *******************************/
typedef enum
{
	TONY_STATE_IDLE = 0U, /*!< */
} tony_state_t;


/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
static uint8_t tony_onIdle(tony_control_t *me, const evt_t *e);
static void tony_configParameter(void);

/************************** Variable Definitions *****************************/
tony_control_t ao_tony_meter;

sRingBufferHandle_Typedef sTonyMeterATCmdRingbuff;

/************************** Functions *****************************************/
void process_tonymeter_init(void)
{
	/* Wait Tonymeter Start-up */
	printf("\r\nWait TonyMeter Start-up ...");
	printf_process();
	uint16_t time = 4000;
	while (time--)
	{
		if((time % 1000) == 0)
		{
			printf("\r\nRemaining time %ds...",time/1000);
			printf_process();
		}
		HAL_Delay(1);
	}

	MX_USART4_UART_Init();

	assert_param(tonymeter_init() == HAL_OK);
	ao_tony_meter.deferSig = TONY_INVALID_SIG;

	assert_param(ringBuffer_init(&sTonyMeterATCmdRingbuff, eRingBuffer_Type32bit, 10 * TONYMETER_MAX_CHANNEL) == eResult_OK);

	ao_tony_meter.alertStatus = eAlert_Status_None;
	for(uint8_t index = 0; index < TONYMETER_MAX_CHANNEL; ++index)
	{
		ao_tony_meter.errorCode[index] = 0;
		ao_tony_meter.relayStatus[index] = false;
	}

	printf_process();

	/* Send command to config TonyMeter */
	tony_configParameter();

	TonyMeter_Start();

	TONY_DBG("\r\nTonymeter init done !");
}

static void tony_configParameter(void)
{
	for(uint8_t index = 0; index < TONYMETER_MAX_CHANNEL; index++)
	{
		/* Disable Channel */
		TonyMeter_SendCommandBlocking((uint8_t*)TONY_RETURN_OK_AT_CMD,
				(uint8_t*)TONY_RETURN_ERROR_AT_CMD,
				TONYMETER_RECEIVE_TIMEOUT_DEFAULT,
				1,
				(uint8_t*)TONY_ENABLE_AT_CMD_ARR[index][eTonymeter_State_Disable]);

		/* Config ADC Channel */
		TonyMeter_SendCommandBlocking((uint8_t*)TONY_RETURN_OK_AT_CMD,
				(uint8_t*)TONY_RETURN_ERROR_AT_CMD,
				TONYMETER_RECEIVE_TIMEOUT_DEFAULT,
				1,
				(uint8_t*)TONY_SET_ADC_AT_CMD_ARR[index]);

		/* Set Relay Pin */
		TonyMeter_SendCommandBlocking((uint8_t*)TONY_RETURN_OK_AT_CMD,
				(uint8_t*)TONY_RETURN_ERROR_AT_CMD,
				TONYMETER_RECEIVE_TIMEOUT_DEFAULT,
				1,
				(uint8_t*)TONY_SET_RELAYPIN_AT_CMD_ARR[index]);

		/* Set Relay Polarity */
		TonyMeter_SendCommandBlocking((uint8_t*)TONY_RETURN_OK_AT_CMD,
				(uint8_t*)TONY_RETURN_ERROR_AT_CMD,
				TONYMETER_RECEIVE_TIMEOUT_DEFAULT,
				1,
				(uint8_t*)TONY_RELAY_POLARITY_AT_CMD_ARR[index]);

		/* Set Relay ONDELAY */
		TonyMeter_SendCommandBlocking((uint8_t*)TONY_RETURN_OK_AT_CMD,
				(uint8_t*)TONY_RETURN_ERROR_AT_CMD,
				TONYMETER_RECEIVE_TIMEOUT_DEFAULT,
				1,
				(uint8_t*)TONY_ONDELAY_AT_CMD_ARR[index]);

		/* Set Relay OFFDELAY */
		TonyMeter_SendCommandBlocking((uint8_t*)TONY_RETURN_OK_AT_CMD,
				(uint8_t*)TONY_RETURN_ERROR_AT_CMD,
				TONYMETER_RECEIVE_TIMEOUT_DEFAULT,
				1,
				(uint8_t*)TONY_OFFDELAY_AT_CMD_ARR[index]);

		/* Set OverLoad Protect */
		TonyMeter_SendCommandBlocking((uint8_t*)TONY_RETURN_OK_AT_CMD,
				(uint8_t*)TONY_RETURN_ERROR_AT_CMD,
				TONYMETER_RECEIVE_TIMEOUT_DEFAULT,
				1,
				(uint8_t*)TONY_OVERLOAD_AT_CMD_ARR[index]);

		/* Set NoLoad Protect */
#if (TONYMETER_NOLOAD_ENABLE == 1)
		/* Set NoLoad Protect */
		TonyMeter_SendCommandBlocking((uint8_t*)TONY_RETURN_OK_AT_CMD,
				(uint8_t*)TONY_RETURN_ERROR_AT_CMD,
				TONYMETER_RECEIVE_TIMEOUT_DEFAULT,
				1,
				(uint8_t*)TONY_NOLOAD_AT_CMD_ARR[index]);
#endif
		if(index == 0)
		{
			/* Set UnderVolt Protect */
			TonyMeter_SendCommandBlocking((uint8_t*)TONY_RETURN_OK_AT_CMD,
					(uint8_t*)TONY_RETURN_ERROR_AT_CMD,
					TONYMETER_RECEIVE_TIMEOUT_DEFAULT,
					1,
					(uint8_t*)TONY_UNDERVOL_AT_CMD_ARR[index]);

			/* Set OverVolt Protect */
			TonyMeter_SendCommandBlocking((uint8_t*)TONY_RETURN_OK_AT_CMD,
					(uint8_t*)TONY_RETURN_ERROR_AT_CMD,
					TONYMETER_RECEIVE_TIMEOUT_DEFAULT,
					1,
					(uint8_t*)TONY_OVERVOLT_AT_CMD_ARR[index]);
		}
#if (TONYMETER_RESIDUAL_CURRENT_PROTECT_ENABLE == 1)
		/* Set Residual Current Protect */
			TonyMeter_SendCommandBlocking((uint8_t*)TONY_RETURN_OK_AT_CMD,
					(uint8_t*)TONY_RETURN_ERROR_AT_CMD,
					TONYMETER_RECEIVE_TIMEOUT_DEFAULT,
					1,						
					(uint8_t*)TONY_RESIDUAL_CURRENT_AT_CMD_ARR[index]);
#endif
		/* ENABLE Channel */
		TonyMeter_SendCommandBlocking((uint8_t*)TONY_RETURN_OK_AT_CMD,
				(uint8_t*)TONY_RETURN_ERROR_AT_CMD,
				TONYMETER_RECEIVE_TIMEOUT_DEFAULT,
				1,
				(uint8_t*)TONY_ENABLE_AT_CMD_ARR[index][eTonymeter_State_Enable]);
		printf_process();
	}
}

static uint8_t tony_onIdle(tony_control_t* me, const evt_t* e)
{
	uint8_t u8State = TONY_STATE_IDLE;
	//printf("\r\ntonymeter curr_state = %d, evt = %d",u8State, e->sig);

	if (me != NULL)
	{

		ASSERT_NONVOID((e != NULL), u8State);

		switch (e->sig)
		{
		case ENTRY_SIG:
		{

			/* Check and get older signal from other state before start
			 * If have, process this signal */
			if (me->deferSig != TONY_INVALID_SIG)
			{
				me->deferSig = TONY_INVALID_SIG;
			}
			TONY_DBG("\r\nTonymeter on standby");
		}
		break;
		case EXIT_SIG:
			break;
		case TONY_SEND_PING_SIG:
			for(uint8_t index = 0; index < MCU_MAX_GPIO_OUTPUT; ++index)
			{
				HAL_GPIO_WritePin((GPIO_TypeDef*)MCU_GPIO_OUTPUT_PORT[index], MCU_GPIO_OUTPUT_PIN[index], GPIO_PIN_SET);
			}
			break;
		case TONY_READ_AC_PARAM_SIG:
		{
			if(ao_tony_meter.alertStatus == eAlert_Status_None)
			{
				printf("\r\n--->");
				TonyMeter_SendCommandNow(&TonyMeter_ReadACProcess[0]);
				TonyMeter_SendCommandNow(&TonyMeter_ReadACProcess[1]);
				TonyMeter_SendCommandNow(&TonyMeter_ReadACProcess[2]);
				TonyMeter_SendCommandNow(&TonyMeter_ReadACProcess[3]);
			}

//			TonyMeter_SendCommandNow(&TonyMeter_ReadResidualCurrentProcess[0]);
//			TonyMeter_SendCommandNow(&TonyMeter_ReadResidualCurrentProcess[1]);
//			TonyMeter_SendCommandNow(&TonyMeter_ReadResidualCurrentProcess[2]);
//			TonyMeter_SendCommandNow(&TonyMeter_ReadResidualCurrentProcess[3]);

		}
		break;

		break;
		case TONY_RECEIVE_GW_TIMESYNC_SIG:
		{
			uint32_t epochTime = 0;

			//			time_t rawtime = epochTime;
			//			struct tm  ts;
			//			char       buf[80];
			//
			//			// Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
			//			ts = *localtime(&rawtime);
			//			strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
			//			printf("\r\n%s", buf);
		}
		break;
		case TONY_RECEIVE_GW_CONTROL_CP_SIG:
		{
			uint8_t signalValue[TONYMETER_MAX_CHANNEL];
			printf("\r\nGW_CONTROL_CP [%d %d %d %d]",signalValue[0],signalValue[1],signalValue[2],signalValue[3]);

			for(uint8_t index = 0; index < TONYMETER_MAX_CHANNEL; ++index)
			{
				if(ao_tony_meter.relayStatus[index] != (bool)signalValue[index])
				{
					printf("\r\nOutlet %d set relay %d",index, (bool)signalValue[index]);
					ao_tony_meter.relayStatus[index] = (bool)signalValue[index];
					TonyMeter_SendCommandNow(&TonyMeter_SetRelayProcess[index][(TonyMeter_State)ao_tony_meter.relayStatus[index]]);

					/* Set OK */
					switch(index)
					{
					case 0:
						break;
					}
				}
			}
		}
		break;
		case TONY_WORKING_STATUS_SIG:

			break;
		case TONY_ALERT_STATUS_TIMEOUT_SIG:
			/* Alert is timeout, clear status then update signal */
			switch(ao_tony_meter.alertStatus)
			{
			case eAlert_Status_None:
				break;
			case eAlert_Status_OverCurrent:
				ao_tony_meter.errorCode[ao_tony_meter.alertChannel] &= ~(1 << eAlert_Status_OverCurrent_Bit);
				break;
			case eAlert_Status_OverVol:
				for(uint8_t index = 0; index < eAlert_Channel_all; ++index)
				{
					ao_tony_meter.errorCode[index] &= ~(1 << eAlert_Status_OverVol_Bit);
				}
				break;
			case eAlert_Status_UnderVol:
				for(uint8_t index = 0; index < eAlert_Channel_all; ++index)
				{
					ao_tony_meter.errorCode[index] &= ~(1 << eAlert_Status_UnderVol_Bit);
				}
				break;
			case eAlert_Status_ResidualCurrent:
				ao_tony_meter.errorCode[ao_tony_meter.alertChannel] &= ~(1 << eAlert_Status_ResidualCurrent_Bit);
				break;
			}

			ao_tony_meter.alertStatus = eAlert_Status_None;
			printf("\r\nAlert Status None !!!");
			break;
			default:
				printf("\r\ntony_onIdle not handle this event %d", e->sig);
				break;
		}
	}
	else
	{
		assert_param(false);
	}

	return u8State;
}


