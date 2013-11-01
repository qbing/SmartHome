package com.qbryan.smarthome;

import java.util.Timer;
import java.util.TimerTask;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.ActivityInfo;
import android.util.Log;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class MainActivity extends Activity {

	private static final String TAG = "QBryan SmartHome";
	MyIOServer ioServer = new MyIOServer(this);
	private int current_view = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        /////处理服务主线程
        ioServer.start();
		//restore
		ReadSharedPreferences();

        ///////////////full screen & landscape/////////begin/////////
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);//设置成全屏模式
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);//强制为横屏
		requestWindowFeature(Window.FEATURE_NO_TITLE);//无标题
        ///////////////full screen & landscape/////////end/////////

		change_to_login_view();
		
		//test
		startSendLoop();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    public void updateViewState()
    {
    	if(current_view == R.layout.activity_main){
			final ImageButton light1 = (ImageButton)findViewById(R.id.light1Btn);
			final ImageButton light2 = (ImageButton)findViewById(R.id.light2Btn);
			final ImageButton cdplayer = (ImageButton)findViewById(R.id.cdplayerBtn);
			final TextView    msgTV = (TextView)findViewById(R.id.msgTV);
			final TextView		TempTV  = (TextView)findViewById(R.id.TempTV);
			if(ioServer.isConnected){
				light1.setEnabled(true);
				light2.setEnabled(true);
				cdplayer.setEnabled(true);
				msgTV.setText("Connected!");
				TempTV.setText(String.format("%.1f\u2103",ioServer.current_temp/10.0));
				if (ioServer.isLamb1On) {
					light1.setImageResource(R.drawable.on_light1);
				} else {
					light1.setImageResource(R.drawable.off_light);
				}
				if (ioServer.isLamb2On) {
					light2.setImageResource(R.drawable.on_read);
				} else {
					light2.setImageResource(R.drawable.off_read);
				}
				if (ioServer.isCDPlayerOn) {
					cdplayer.setImageResource(R.drawable.on_player);
				} else {
					cdplayer.setImageResource(R.drawable.off_player);
				}
		    	//isConnected = false;
		    	//current_temp = 0;
			}else{
				msgTV.setText("Connecting... Please wait a moment!");

				light1.setImageResource(R.drawable.disable_light_);
				light2.setImageResource(R.drawable.disable_read);
				cdplayer.setImageResource(R.drawable.disable_player);
				
				light1.setEnabled(false);
				light2.setEnabled(false);
				cdplayer.setEnabled(false);
			}
    	}

    }

    public void change_to_login_view()
    {
        setContentView(R.layout.login_layout);
        current_view = R.layout.login_layout;
        Button login = (Button)this.findViewById(R.id.loginBtn);
        login.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				change_to_main_view();
			}
		});  	
        Button exit = (Button)this.findViewById(R.id.exitBtn);
        exit.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				onBackPressed();
			}
		});  	
    }
    
    public void change_to_main_view()
    {
    	setContentView(R.layout.activity_main);
        current_view = R.layout.activity_main;
		
		final ImageButton setting = (ImageButton)findViewById(R.id.settingBtn);
		final ImageButton light1 = (ImageButton)findViewById(R.id.light1Btn);	
		final ImageButton light2 = (ImageButton)findViewById(R.id.light2Btn);
		final ImageButton cdplayer = (ImageButton)findViewById(R.id.cdplayerBtn);
		final TextView    msgTV = (TextView)findViewById(R.id.msgTV);
		
		/////////////////////////////按钮效果///////////////////////////////
		OnTouchListener btnListner =  new OnTouchListener(){
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				
				if(event.getAction()==MotionEvent.ACTION_UP){
					if(v==setting){
						v.setBackgroundResource(R.drawable.off_setting);	
						updateViewState();
						change_to_setting_view();
						return true;
					}
				}
				
				if(event.getAction()==MotionEvent.ACTION_DOWN){
					if(v==setting){
						v.setBackgroundResource(R.drawable.on_setting);
					}else if(v==light1){
						ioServer.isLamb1On=!ioServer.isLamb1On;
					}else if(v==light2){
						ioServer.isLamb2On=!ioServer.isLamb2On;
					}else if(v==cdplayer){
						ioServer.isCDPlayerOn=!ioServer.isCDPlayerOn;
					}
				//}else if(event.getAction()==MotionEvent.ACTION_UP){
					if(v==light1){
						if(ioServer.isLamb1On){
							ioServer.operate(MyIOServer.DEVICE_ID.ID_LAMB1,MyIOServer.OP_TYPE.OP_OPEN, new byte[1]);						
						}else{
							ioServer.operate(MyIOServer.DEVICE_ID.ID_LAMB1,MyIOServer.OP_TYPE.OP_CLOSE, new byte[1]);													
						}
					}else if(v==light2){
						if(ioServer.isLamb2On){
							ioServer.operate(MyIOServer.DEVICE_ID.ID_LAMB2,MyIOServer.OP_TYPE.OP_OPEN, new byte[1]);						
						}else{
							ioServer.operate(MyIOServer.DEVICE_ID.ID_LAMB2,MyIOServer.OP_TYPE.OP_CLOSE, new byte[1]);													
						}
					}else if(v==cdplayer){
						if(ioServer.isCDPlayerOn){
							ioServer.operate(MyIOServer.DEVICE_ID.ID_CDPLAYER,MyIOServer.OP_TYPE.OP_OPEN, new byte[1]);						
						}else{
							ioServer.operate(MyIOServer.DEVICE_ID.ID_CDPLAYER,MyIOServer.OP_TYPE.OP_CLOSE, new byte[1]);													
						}
					}
					ioServer.delay_count = 3;
					startSendLoop();
				}
				updateViewState();
				return true;
			}
		};
		
		setting.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View arg0) {
				change_to_setting_view();
			}
		});
		//setting.setOnTouchListener(btnListner);
		light1.setOnTouchListener(btnListner);
		light2.setOnTouchListener(btnListner);
		cdplayer.setOnTouchListener(btnListner);
		/////////////////////////////按钮效果///////////////////////////////

		updateViewState();
    }
    
    public void change_to_setting_view()
    {
    	setContentView(R.layout.setting);
        current_view = R.layout.setting;
        
		final EditText ipEdit = (EditText)findViewById(R.id.editIP);
		final EditText portEdit = (EditText)findViewById(R.id.editPort);
		ipEdit.setText(MyIOServer.IP);
		portEdit.setText(String.valueOf(MyIOServer.Port));
		
		Button setting_ok = (Button)findViewById(R.id.setting_ok);
		setting_ok.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View arg0) {
				Log.v(TAG, "setServerAddress:"+ipEdit.getText().toString()+" " + portEdit.getText().toString());
				ioServer.setServerAddress(ipEdit.getText().toString(),Integer.valueOf(portEdit.getText().toString()));
				change_to_main_view();
			}
		}); 		
		
		Button setting_exit = (Button)findViewById(R.id.setting_exit);
		setting_exit.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View arg0) {
				change_to_main_view();
			}
		});    
		
		SeekBar tempBar = (SeekBar)findViewById(R.id.tempSBar);
		final TextView settedTempTv = (TextView)findViewById(R.id.settedTempTV);
		tempBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener(){

			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				// TODO Auto-generated method stub
				settedTempTv.setText(String.format("%.1f\u2103",(float)progress));
			}

			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub
				
			}
			
		});		
    }
    

	///////////////for loop /////////////////begin///////////////
    Timer timer = null;
    final int wait_ms = 500;

	Handler handler = null;
    TimerTask task = null;
    public void startSendLoop()
    {
    	stopSendLoop();
    	handler = new Handler(){
            @Override
			public void handleMessage(Message msg) {
                switch (msg.what) {
                case 1:
                	//ioServer.operate(MyIOServer.DEVICE_ID.ID_INDICATOR,MyIOServer.OP_TYPE.OP_NOT, new byte[1]);
                	if(current_view == R.layout.activity_main){updateViewState();}
                	break;
                }
            }
        };
    	task = new TimerTask(){
            @Override
			public void run() {
                Message message = new Message();
                message.what = 1;
                handler.sendMessage(message);
            }
        };
        timer = new Timer();
		timer.schedule(task, wait_ms,wait_ms);
	}
    public void stopSendLoop()
    {
    	Log.v(TAG, "stopViewLoop");
    	if(task != null){
    		task.cancel();task =null;
    	}
    	if(timer != null){
    		timer.cancel();timer =null;
    	}
    }
	///////////////for loop /////////////////end/////////////////
 
	@Override
	public void onBackPressed() {
    	if(current_view == R.layout.activity_main || current_view == R.layout.login_layout){
			ioServer.close();
			WriteSharedPreferences();
    		stopSendLoop();
    		super.onBackPressed();
    		this.finish();
    		android.os.Process.killProcess(android.os.Process.myPid());
    	}else
    	{
    		change_to_main_view();
    	}
	}
	
	//////////save & read system setting//////////begin//////////////////////////
	public void  ReadSharedPreferences(){
		SharedPreferences   user = getSharedPreferences("server_info",0);
		if(user!=null){
			MyIOServer.IP = user.getString("IP",MyIOServer.IP);
			MyIOServer.Port = user.getInt("PORT",MyIOServer.Port);
		}
	}
	public void  WriteSharedPreferences(){
		SharedPreferences   user = getSharedPreferences("server_info",0);
		Editor editor = user.edit();
        editor.putString("IP", MyIOServer.IP);
        editor.putInt("age", MyIOServer.Port);
        editor.commit();
	}
	//////////save & read system setting//////////begin//////////////////////////

}
