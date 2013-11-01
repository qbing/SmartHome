################################################################################
#/******************************************************************************
#*  Copyright Statement:
#*    Copyright (c)  Q.Bryan <qbing2010@163.com>       All rights reserved.
#*
#*-----------------------------------------------------------------------------
#---------------------------------------
#Smart Home Protocl
#-----------------                                                                           
#                                                                                      
#            MMMMMMMMMM    MMMMMMMMMMMM      MMMMMMMMMMMM  MMMMMMMMMMMMMMMM            
#          MMMM    MMMM        MMMMMM          MMMMMM        MMMMMM      MMMMMM        
#        MMMM        MM        MMMM              MMMM          MMMM        MMMM        
#        MMMM        MM        MMMM              MMMM          MMMM        MMMM        
#        MMMMMM                MMMM              MMMM          MMMM        MMMM        
#          MMMMMMMM            MMMM              MMMM          MMMM      MMMMMM        
#            MMMMMMMM          MMMMMMMMMMMMMMMMMMMMMM          MMMMMMMMMMMM            
#                MMMMMM        MMMM              MMMM          MMMM                    
#                  MMMMMM      MMMM              MMMM          MMMM                    
#        MM          MMMM      MMMM              MMMM          MMMM                    
#        MM          MMMM      MMMM              MMMM          MMMM                    
#        MMMM      MMMM        MMMMMM          MMMMMM        MMMMMM                    
#            MMMMMMMM      MMMMMMMMMMMM      MMMMMMMMMMMM  MMMMMMMMMMMM                
#                                                                             
#--------------------------------------------------------------------------------
#* Author:        Q.Bryan	2013/10/31 10:41:00        
#--------------------------------------------------------------------------------
#################################################################################

MAKE:=make
ROOT_DIR=$(shell pwd)
WIN_ROOT_DIR=$(shell pwd | sed "s/\/cygdrive\/\(.\)/\1:/g")

RUN_WEBSERVER="$(ROOT_DIR)/SmartHomeGoahead/linux-x86-default/bin/goahead.exe" \
	-v $(ROOT_DIR)/SmartHomeWeb/ 
	
RUN_WEBSERVER_ARGS=\
	-auth $(ROOT_DIR)/SmartHomeGoahead/auth.txt \
	-route $(ROOT_DIR)/SmartHomeGoahead/route.txt \
	

run:run_sh_server run_webserver run_sh_modis
	$(shell echo "run sh_server and webserver...")
	
run_sh_modis:
	$(ROOT_DIR)/../Modis/Debug/SmartHomeModis.exe 0,1,2>modis.log &
	
run_sh_server:
	/bin/mintty.exe -e $(ROOT_DIR)/SmartHomeServer/sh_server.exe &

run_webserver:
	/bin/mintty.exe -e $(RUN_WEBSERVER)&
		
	
all:
	$(MAKE) -C ./SmartHomeGoahead/	
	$(MAKE) -C ./SmartHomeServer/