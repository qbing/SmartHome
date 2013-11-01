###################################################
###################################################
###This file is writed to build SmartHome Server###
####Creat By   QBryant <qbing2010@163.com> ########
###################################################


#####setting###################
VPATH   	= ./src/ ./export_lib/jsoncpp-src-0.5.0/src/lib_json/ ./export_lib/MD5/
OBJ   		= main.o network.o device.o SmartHomeInfr.o CDevice.o \
				CControlor.o json_reader.o json_value.o json_writer.o CDeviceTest.o CAtom.o MD5.o \
				SmartHomeComServer.o
TARGET_DIR	= ./build
TARGET 		= sh_server
INC			=. ./inc/ ../../ ./export_lib/jsoncpp-src-0.5.0/include/
LIBS		=




#####tools#####################
CC   		= g++ -g
COMPILE.C  	= $(CC) -c 
CXXFLAGS  	=  -lgcc_s  
MAKEEXE   	= $(CC) $(CXXFLAGS) $(LDFLAGS) -pthread


INC_DIRS = $(foreach dir,$(INC),-I $(dir))

all:clean $(TARGET)

$(TARGET):${OBJ}
	$(MAKEEXE) $^ -o $@
%.o:%.cpp
	$(COMPILE.C) $(INC_DIRS) $< -o $@
clean:     
	rm -f *.o $(TARGET)* core 

