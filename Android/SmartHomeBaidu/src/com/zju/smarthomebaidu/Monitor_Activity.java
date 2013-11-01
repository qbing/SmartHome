package com.zju.smarthomebaidu;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageButton;

public class Monitor_Activity extends Activity {

	public Monitor_Activity() {
		// TODO Auto-generated constructor stub
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
        ///////////////full screen & landscape/////////begin/////////
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);//设置成全屏模式
		//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);//强制为横屏
		requestWindowFeature(Window.FEATURE_NO_TITLE);//无标题
        ///////////////full screen & landscape/////////end/////////
		setContentView(R.layout.monitor_main);
		
		ImageButton ibtn_camera = (ImageButton)this.findViewById(R.id.ibtn_camera);
		ibtn_camera.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent it = new Intent(Monitor_Activity.this,VideoManagerActivity.class);
				startActivity(it);
			}
		});		
		
		ImageButton ibtn_Water = (ImageButton)this.findViewById(R.id.ibtn_Water);
		ibtn_Water.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent it = new Intent(Monitor_Activity.this,WaterAnaliyzeAcitvity.class);
				startActivity(it);
			} 
		});		
		
		ImageButton ibtn_power = (ImageButton)this.findViewById(R.id.ibtn_power);
		ibtn_power.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent it = new Intent(Monitor_Activity.this,PowerAnaliyzeAcitvity.class);
				startActivity(it);
			}
		});
		
		BottomBarControl bottombar_btns = new BottomBarControl(this);
		bottombar_btns.BindToActivity(this);
	}
}
