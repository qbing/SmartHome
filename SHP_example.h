#ifndef SHP_EXAMPLE_H__
#define SHP_EXAMPLE_H__
#include "SHP.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SHP_MAC_TEMP__ 2

#if SHP_MAC_TEMP__==1
#define SHP_MAC_TEMP		"AAAAAAAAAAAA"
#elif SHP_MAC_TEMP__==2
#define SHP_MAC_TEMP		"AAAAAAAAAAAB"
#elif SHP_MAC_TEMP__==3
#define SHP_MAC_TEMP		"AAAAAAAAAAAC"
#elif SHP_MAC_TEMP__==4
#define SHP_MAC_TEMP		"AAAAAAAAAAAD"
#elif SHP_MAC_TEMP__==5
#define SHP_MAC_TEMP		"AAAAAAAAAAAE"
#elif SHP_MAC_TEMP__==6
#define SHP_MAC_TEMP		"AAAAAAAAAAAF"
#else
#define SHP_MAC_TEMP		"FFFFFFFFFFFF"
#endif

void shp_example_init_called_by_system(void);
void shp_example_tick_called_by_system(void);
void shp_example_data_arrived_called_by_system(U8* bin,U16 len);


#ifdef __cplusplus
}
#endif
#endif