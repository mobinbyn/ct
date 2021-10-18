#ifndef _CT_H
#define _CT_H

/**
 *  Detailed Description
 *  
 *  This library has been compiled to read current using CT.
 *  In this library we have used standard-peripheral-library functions.
 *  
 *  @author:        Mobin Babaeian
 *  @version:       V1.0
 *  @date:          21-Septemver-2021
 *  @attention:     
 * 
 **/


#include "stm32f10x_adc.h"


/**
 * @defgroup The status of each type of CT is one of the following
 */ 
typedef enum
{
    CT_STATUS_NORMAL = 0,                                   /*!< It is normal                                       */
    CT_STATUS_UNDER_LOAD,                                   /*!< The current flows slightly beyond the set limit    */
    CT_STATUS_OVER_LOAD,                                    /*!< Alot of current passes through                     */
    CT_STATUS_UNBALANCE                                     /*!< The currents between the two phases are different  */
} CT_Status_t;


/**
 * @defgroup Contain ct load setting value
 */
typedef struct
{
    double                      OverLoad;                   /*!< Model: 15A -> 0.5-15 A - with the ability to adjust each step 0.1A
                                                                 Model: 60A -> 1-60 A  with the ability to adjust each step 1A  */

    double                      UnderLoad;                  /*!< from zero to one unit less than over load          */

    uint16_t                    UnbalanceCoefficient;       /*!< 7-100%                                             */

} CT_SettingTypeDef;


/**
 * @defgroup
 */
typedef struct
{
    ADC_InitTypeDef*            ADC_InitStruct;             /*!< ADC Init structure definition */

    CT_SettingTypeDef*          Setting;                    /*!< Contain ct load setting value */

    CT_Status_t                 Status;                     /*!< The status of each type of CT is one of the following */
    
} CT_TypeDef;



/* Auxiliary definitions CT */
#define ADC_CT                      ADC1
#define ADC_CT1_CH                  ADC_Channel_1
#define ADC_CT2_CH                  ADC_Channel_2
#define ADC_CT_SMPL                 ADC_SampleTime_239Cycles5

#define ADC_BITS                    12
#define ADC_COUNTS                  (1<<ADC_BITS)

#define LOOP_TIME                   45                          /*!< Micro second   */

/** Primaty Define */
#define CT_15A                      15
#define CT_60A                      60
#define CT_MODEL                    CT_15A

/** Phase type */
#define SINGEL_PHASE                1
#define THREE_PHASE                 3
#define PHASE_TYPE                  THREE_PHASE


// constants which must be set individually for each system
#if (CT_MODEL == CT_15A)
    #define ACCURACY                0.1
    #define SECONDARY_WINDINGS      280
    #define BURDEN_RESISTOR         2.8
    #define ICAL                    442.14      // CT Ratio / Burden resistor
#elif (CT_MODEL == CT_60A)
    #define ACCURACY                1
    #define SECONDARY_WINDINGS      400
    #define BURDEN_RESISTOR         2
    #define ICAL
#endif


#if (PHASE_TYPE == SINGEL_PHASE)
    #define VOLTAGE                 220
#elif (PHASE_TYPE == THREE_PHASE)
    #define VOLTAGE                 380
    #define PHASE_DIFFERENCE        120
#endif



/* Public function prototype --------------------------------------------------*/
void            CT_Init(void);

double          CT_CalcIrms(uint8_t Channel_Number, unsigned int Number_of_Samples);

CT_Status_t     CT_GetStatus (CT_TypeDef *CT);

uint8_t         CT_IsNormal (CT_TypeDef *CT);


#endif
