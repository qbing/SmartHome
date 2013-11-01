package com.zju.smarthomebaidu;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.ImageView;
import android.widget.ToggleButton;

public class Device_NormalLight_Activity extends Activity{

	public Device_NormalLight_Activity() {
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
		
		
		setContentView(R.layout.device_normal_light_layout);
		final ImageView light_State_imgView = (ImageView)this.findViewById(R.id.light_State_imgView); 
		final ToggleButton tBtn = (ToggleButton)this.findViewById(R.id.light_toggleBtn);
		tBtn.setOnCheckedChangeListener(new OnCheckedChangeListener(){

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				// TODO Auto-generated method stub
				tBtn.setChecked(isChecked);
				light_State_imgView.setImageResource(isChecked?R.drawable.dev_icon_normal_light_on:R.drawable.dev_icon_normal_light_off);
			}
		});
	}
}
