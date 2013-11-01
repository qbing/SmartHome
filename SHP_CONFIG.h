
#ifndef __SHP_CONFIG_H__
#define __SHP_CONFIG_H__

#define SHP_SEND_TICK_BY_BUFFER				1			//send tick data by buffer or not
#define SHP_SEND_TICK_BY_BUFFER_SIZE		48			//if send tick data by buffer,then this is the buffer size


#define SHP_MAX_DEVICE_DATA_SIZE			16			//The max size of the device data

#define SHP_CALL_shp_system_tick_PER_100MS	0			//call shp_system_tick per 100ms or per second
#endif