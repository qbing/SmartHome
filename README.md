/******************************************************************************
*  Copyright Statement:
*    Copyright (c)  Q.Bryan <qbing2010@163.com>       All rights reserved.
*
*-----------------------------------------------------------------------------
---------------------------------------
Smart Home Protocl
-----------------                                                                           
                                                                                      
            MMMMMMMMMM    MMMMMMMMMMMM      MMMMMMMMMMMM  MMMMMMMMMMMMMMMM            
          MMMM    MMMM        MMMMMM          MMMMMM        MMMMMM      MMMMMM        
        MMMM        MM        MMMM              MMMM          MMMM        MMMM        
        MMMM        MM        MMMM              MMMM          MMMM        MMMM        
        MMMMMM                MMMM              MMMM          MMMM        MMMM        
          MMMMMMMM            MMMM              MMMM          MMMM      MMMMMM        
            MMMMMMMM          MMMMMMMMMMMMMMMMMMMMMM          MMMMMMMMMMMM            
                MMMMMM        MMMM              MMMM          MMMM                    
                  MMMMMM      MMMM              MMMM          MMMM                    
        MM          MMMM      MMMM              MMMM          MMMM                    
        MM          MMMM      MMMM              MMMM          MMMM                    
        MMMM      MMMM        MMMMMM          MMMMMM        MMMMMM                    
            MMMMMMMM      MMMMMMMMMMMM      MMMMMMMMMMMM  MMMMMMMMMMMM                
                                                                             
--------------------------------------------------------------------------------
* Author:        Q.Bryan	2013/09/17 17:36:41        
--------------------------------------------------------------------------------



简要说明

1、 系统包含Sever、Webserver、Web页、模拟器、终端执行器、手机端控制器。
目前有些只完成了一部分。

2、 第三方自己的设备兼容我们的协议可以参考SHP_example进行。API部分的接口有待进一步完善。

3、 大体结构图可以参看《智能家居-草稿1》

4、 进一步抽象出模块后，大家合作编程。当前只是初步打通流程。







调试方法简介：

1、到SmartHomeServer目录启动sh_server.exe程序。<如果无此程序，可通过make命令编译>。
2、到SmartHomeBoa目录下，执行make，make install ，以SmartHomeWeb为http根目录配置好WEB Server。之后可通过boa -d调试运行WEB Server。
3、到MODIS目录下，启动模拟器SmartHomeModis.exe
4、WEB server配置正确的情况下，通过127.0.0.1可以看到根据设备的连接情况生成的控制界面。右键可操作设备。



