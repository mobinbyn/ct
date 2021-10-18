#include "stm32f10x_adc.h"
#include "math.h"
#include "ct.h"


extern double gCT_OverLoad;
extern double gCT_UnderLoad;
extern uint16_t gCT_UnbalanceCofficient;


static volatile uint16_t sampleI;
static double filteredI, offsetI, sqI, sumI;



/* Private function prototype --------------------------------------------------*/
static uint16_t ADC_Read(uint8_t channel);


/**
 * @brief
 * 
 * @param void: None
 * 
 * @return None 
 */ 
void CT_Init(void)
{
    /* PCLK2 is the APB2 clock */
    /* ADCCLK = PCLK2/6 = 72/6 = 12MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    /* Enable ADC1 clock so that we can talk to it */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    /* Put everything back to power-on defaults */
    ADC_DeInit(ADC1);
	
    //* --------------------------- ADC1 Configuration ---------------------------*/
    CT_TypeDef CT_InitStruct;
    ADC_InitTypeDef ADC_InitStruct;
    CT_SettingTypeDef CT_SettingInitStruct;
    /* Set the status to normal when the device starts up  */
    CT_Status_t CT_StatusInitStruct = CT_STATUS_NORMAL;
	
    
    /* ADC1 operate independently */
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    /* Disable the scan conversion so we do one at a time */
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    /* Don't do contimuous conversions - do them on demand */
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    /* Start conversin by software, not an external trigger */
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    /* Conversions are 12 bit - put them in the lower 12 bits of the result */
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    /* Say how many channels would be used by the sequencer */
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    

    /* Now do the setup */
    ADC_Init(ADC_CT, CT_InitStruct.ADC_InitStruct);
    /* Enable ADC1 */
    ADC_Cmd(ADC_CT, ENABLE);

    /* Enable ADC1 reset calibaration register */
    ADC_ResetCalibration(ADC_CT);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC_CT));
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC_CT);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC_CT));
   
    // TODO Load from EEPROM
    /* Set overload velue from eeprom */
    CT_SettingInitStruct.OverLoad = gCT_OverLoad;
    /* Set underload velue from eeprom */
    CT_SettingInitStruct.UnderLoad = gCT_UnderLoad;
    /* Set UnbalanceCoefficient velue from eeprom */
    CT_SettingInitStruct.UnbalanceCoefficient = gCT_UnbalanceCofficient;
    
    CT_InitStruct.ADC_InitStruct = &ADC_InitStruct;
    CT_InitStruct.Setting = &CT_SettingInitStruct;
    CT_InitStruct.Status = CT_StatusInitStruct;
		
    // TODO Enable ADC Watchdog here
    
}



/**
 * @brief 
 * 
 * @param uint: 
 * 
 * @return
 * 
 */
double CT_CalcIrms(uint8_t Channel_Number, unsigned int Number_of_Samples)
{
    static int SupplyVoltage = 3300;

	
    for(unsigned int n = 0; n < Number_of_Samples; n++)
    {	
        sampleI = ADC_Read(Channel_Number);
			
        // Digital low pass filter extracts the 2.5 V or 1.65 V dc offset,
        //  then subtract this - signal is now centered on 0 counts.
        offsetI = (offsetI + (sampleI - offsetI) / ADC_COUNTS);
        filteredI = sampleI - offsetI;

        // Root-mean-square method current
        // 1) square current values
        sqI = filteredI * filteredI;
        // 2) sum
        sumI += sqI;
    }

    double I_RATIO = ICAL * ((SupplyVoltage / 1000.0) / (ADC_COUNTS));
    double Irms = I_RATIO * sqrt(sumI / Number_of_Samples);

    // Reset accumulators
    sumI = 0;

    return Irms;
}



/**
 * @brief 
 * 
 * @param CT: 
 * 
 * @return
 * 
 */
CT_Status_t CT_GetStatus(CT_TypeDef *CT)
{
    /** Return CT status. */
    return CT->Status;
}



/**
 * @brief 
 * 
 * @param CT: 
 * 
 * @return
 * 
 */
uint8_t CT_IsNormal(CT_TypeDef *CT)
{
    if(CT->Status == CT_STATUS_NORMAL)
        return 1;
    else
        return 0;
}



static uint16_t ADC_Read(uint8_t channel)
{
    ADC_RegularChannelConfig(ADC_CT, channel, 1, ADC_CT_SMPL);
    // Start the conversion
    ADC_SoftwareStartConvCmd(ADC_CT, ENABLE);
    // Wait until conversion completion
    while(ADC_GetFlagStatus(ADC_CT, ADC_FLAG_EOC) == RESET);
    // Get the conversion value
    return ADC_GetConversionValue(ADC_CT);
}
