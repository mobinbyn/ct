#ifndef _CT_H
#define _CT_H

/**
 * 
 * TODO write libary document.
 * 
 **/


/**
 * @brief The status of each type of CT is one of the following
 */ 
typedef enum
{
    CT_STATUS_NORMAL = 0,                           /*!< It is normal                                       */
    CT_STATUS_UNDER_LOAD,                           /*!< The current flows slightly beyond the set limit    */
    CT_STATUS_OVER_LOAD,                            /*!< Alot of current passes through                     */
    CT_STATUS_UNBALANCE                             /*!<                                                    */
} ct_status_t;

/**
 * @brief Contain ct setting
 */
typedef struct
{
    uint16_t            over_load;                  /*!< 0.5-15 A  / 1-60 A                                 */

    uint16_t            under_load;                 /*!< from zero to one unit less than over load          */

    uint16_t            unbalance_coefficient;      /*!< 7-100%                                             */

    // On time
    // Off time
    // Off time(u)
    // Delay
    uint32_t            loop_time;                  /*!<                                                    */
} ct_setting_t;

/**
 * @brief
 */
typedef struct
{
    bool                inited;                     /*!<                                                    */

    ct_setting_t        setting;                    /*!<                                                    */

    ct_status_t         status;                     /*!<                                                    */

    bool                balance;                    /*!<                                                    */
    
    // ADC_HandleTypeDef   hadc;                    /*!<                                                    */
    // Time
} ct_t;


/**
 * @brief Initialization function
 * 
 * @param ct: 
 * 
 * @return
 * 
 */
void        ct_init (ct_t* ct);


/**
 * @brief 
 * 
 * @param ct: 
 * 
 * @return
 * 
 */
ct_status   ct_get_status (ct_t *ct);


/**
 * @brief 
 * 
 * @param ct: 
 * 
 * @return
 * 
 */
uint32_t    ct_get_current (ct_t *ct);


/**
 * @brief 
 * 
 * @param ct: 
 * 
 * @return
 * 
 */
bool        ct_is_normal (ct_t* ct);


#endif