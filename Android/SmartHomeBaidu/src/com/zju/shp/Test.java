package com.zju.shp;

import java.nio.ByteBuffer;
import com.zju.shpEX.*;

public class Test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println( "Hello world!" );
		Shp.init();
		Shp shp = Shp.getShp();
		ShpOperatorSwitch switch_1 = new ShpOperatorSwitch();
		
	    ShpRoom.updata_ShpRoomList();
	    ShpRoom shp_room = ShpRoom.getShpRoom();
	    for(ShpRoom room:shp_room.room_list){
	    	System.out.println(room.getName());
	    }
	    
	    ShpRoomDevice.updata_ShpDeviceList();
	    ShpRoomDevice dev_list = ShpRoomDevice.getShpRoomDevices(); 
	    for(ShpRoomDevice dev:dev_list.getDevice_list()){
	    	System.out.println(dev.getName());
	    }
	    
		while(true){
			try{
				switch_1.operator("RServer/SmartHeaterAAAAAAAAAAAB", (short)0, (short)0, null);
			    Thread.sleep(5000);
			    

			    
			}catch(InterruptedException e){
			    System.out.println("got interrupted!");
			}finally{
			}
			//break;
		}
	}

}
