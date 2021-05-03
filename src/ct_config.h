#ifndef _CT_CONFIG_H
    #define _CT_CONFIG_H


    #define CT_15A                      15
    #define CT_60A                      60
    #define CT_MODEL                    CT_15A

    #define SINGEL_PHASE                1
    #define THREE_PHASE                 3
    #define PHASE_MODE                  THREE_PHASE

    #if (CT_MODEL == CT_15A)
        #define ACCURACY                0.1
        #define SECONDARY_WINDINGS      300
        #define BURDEN                  2.8
    #elif (CT_MODEL == CT_60A)
        #define ACCURACY                1
        #define SECONDARY_WINDINGS      400
        #define BURDEN                  2
    #endif

    #if (PHASE_MODE == SINGEL_PHASE)
        #define VOLTAGE                 220
    #elif (PHASE_MODE == THREE_PHASE)
        #define VOLTAGE                 380
        #define PHASE_DIFFERENCE        120
    #endif

#endif