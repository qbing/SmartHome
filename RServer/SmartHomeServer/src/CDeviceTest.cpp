#include "SmartHomeInfr.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************
|--head--|-----ID----------|--device type--|--device name--|---p version--|--surported p--|
|-6Bytes-|----48bits MAC---|-----2Bytes----|---32Bytes-----|---1Bytes-----|---3Bytes------|
*****************************************************************************************/
const static struct
{
	SHP_HeadForSend head;
	U16				pid;
	U8  mac[6];
	U16 dev_type;
	S8  dev_name[32];
	U8  version;
	U8  surpoted_protocl[3];
}
SHP_EXAMPLE_NO1_MAC_PKG = {/*Please view the document!!!!!!!!!!!*/
	{SHP_MAGIC1,sizeof(SHP_EXAMPLE_NO1_MAC_PKG),SHP_MAGIC2,'C',0},	/*head*/
	0x0001,				/*PID*/
	"MAC01",			/*MAX : invalid mac address*/
	0,					/*device type : Demo*/
	"Modis",			/*device name : Modis*/
	0x01,				/*version : 0.1*/
	{'B','-','-'}
};

#define EXAMPLE_NO1_DEVICE_INFO \
"{\
    \"NAME\": \"SmartSauna\",\
    \"MAC\": \"ZJU001\",\
    \"TYPE\": \"DEV\",\
    \"CHILD\": [\
        {\
            \"NAME\": \"Heater\",\
            \"TYPE\": \"TERM\",\
            \"DEVTYPE\": \"SWITCH\"\
        },\
        {\
            \"NAME\": \"CDPlayer\",\
            \"TYPE\": \"TERM\",\
            \"DEVTYPE\": \"SWITCH\"\
        }\
    ]\
}"

/*****************************************************************************************
|--head--|----JSON----|
|-6Bytes-|----XXXX----|
*****************************************************************************************/
const static struct
{
	SHP_HeadForSend head;
	U16				pid;
	U8  topology[sizeof(EXAMPLE_NO1_DEVICE_INFO)];
}
SHP_EXAMPLE_NO1_TOPOLOGY_PKG = {/*Please view the document!!!!!!!!!!!*/
	{SHP_MAGIC1,8+sizeof(EXAMPLE_NO1_DEVICE_INFO),SHP_MAGIC2,'C',0},	/*head*/
	0x0003,
	EXAMPLE_NO1_DEVICE_INFO
};






/*****************************************************************************************
|--head--|-----ID----------|--device type--|--device name--|---p version--|--surported p--|
|-6Bytes-|----48bits MAC---|-----2Bytes----|---32Bytes-----|---1Bytes-----|---3Bytes------|
*****************************************************************************************/
const static struct
{
	SHP_HeadForSend head;
	U16				pid;
	U8  mac[6];
	U16 dev_type;
	S8  dev_name[32];
	U8  version;
	U8  surpoted_protocl[3];
}
SHP_EXAMPLE_NO2_MAC_PKG = {/*Please view the document!!!!!!!!!!!*/
	{SHP_MAGIC1,sizeof(SHP_EXAMPLE_NO2_MAC_PKG),SHP_MAGIC2,'C',0},	/*head*/
	0x0001,				/*PID*/
	"MAC02",			/*MAX : invalid mac address*/
	0,					/*device type : Demo*/
	"Modis",			/*device name : Modis*/
	0x01,				/*version : 0.1*/
	{'B','-','-'}
};

#define EXAMPLE_NO2_DEVICE_INFO \
"{\
    \"NAME\": \"SmartHeater\",\
    \"MAC\": \"ZJU002\",\
    \"TYPE\": \"DEV\",\
    \"CHILD\": [\
        {\
            \"NAME\": \"Light\",\
            \"TYPE\": \"SET\",\
            \"CHILD\": [\
                {\
                    \"NAME\": \"ReadLight\",\
                    \"TYPE\": \"TERM\",\
                    \"DEVTYPE\": \"SWITCH\"\
                },\
                {\
                    \"NAME\": \"DecorateLight\",\
                    \"TYPE\": \"TERM\",\
                    \"DEVTYPE\": \"SWITCH\"\
                }\
            ]\
        },\
        {\
            \"NAME\": \"CDPlayer\",\
            \"TYPE\": \"TERM\",\
            \"DEVTYPE\": \"SWITCH\"\
        }\
    ]\
}"


/*****************************************************************************************
|--head--|----JSON----|
|-6Bytes-|----XXXX----|
*****************************************************************************************/
const static struct
{
	SHP_HeadForSend head;
	U16				pid;
	U8  topology[sizeof(EXAMPLE_NO2_DEVICE_INFO)];
}
SHP_EXAMPLE_NO2_TOPOLOGY_PKG = {/*Please view the document!!!!!!!!!!!*/
	{SHP_MAGIC1,8+sizeof(EXAMPLE_NO2_DEVICE_INFO),SHP_MAGIC2,'C',0},	/*head*/
	0x0003,
	EXAMPLE_NO2_DEVICE_INFO
};



CDeviceTest_TTTTTT testtttttttt[] = 
{
	{
		(char*)&SHP_EXAMPLE_NO1_MAC_PKG,
		sizeof(SHP_EXAMPLE_NO1_MAC_PKG),
		(char*)&SHP_EXAMPLE_NO1_TOPOLOGY_PKG,
		sizeof(SHP_EXAMPLE_NO1_TOPOLOGY_PKG)
	},
	{
		(char*)&SHP_EXAMPLE_NO2_MAC_PKG,
		sizeof(SHP_EXAMPLE_NO2_MAC_PKG),
		(char*)&SHP_EXAMPLE_NO2_TOPOLOGY_PKG,
		sizeof(SHP_EXAMPLE_NO2_TOPOLOGY_PKG)
	}
};
	
CDeviceTest_TTTTTT* getTest(int index)
{
	return &testtttttttt[index];
}

#ifdef __cplusplus
}
#endif
