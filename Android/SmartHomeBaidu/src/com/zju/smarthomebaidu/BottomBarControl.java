package com.zju.smarthomebaidu;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.ImageView;

public class BottomBarControl {

	enum ActType
	{
		ActType_UNKOWN,
		ActType_HOME,
		ActType_MONITOR,
		ActType_ME
	}
	static private Activity current_act = null;
	static ActType current_type = ActType.ActType_HOME;
	private Activity act = null;
	public BottomBarControl(Activity act) {
		this.act = act;
	}
	public void BindToActivity(final Activity act)
	{
		final ImageView buttom_home_btn = (ImageView)act.findViewById(R.id.buttom_home_btn);
		final ImageView buttom_monitor_btn = (ImageView)act.findViewById(R.id.buttom_monitor_btn);
		final ImageView buttom_me_btn = (ImageView)act.findViewById(R.id.buttom_me_btn);
		buttom_home_btn.setOnTouchListener(new OnTouchListener(){
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				if(event.ACTION_DOWN==event.getAction()){
					buttom_home_btn.setBackgroundResource(R.drawable.pressed_home_btn);
					buttom_monitor_btn.setBackgroundResource(R.drawable.normal_monitor_btn);
					buttom_me_btn.setBackgroundResource(R.drawable.normal_me_btn);
					return true;
				}else if(event.ACTION_UP == event.getAction()){
					GotoActivityByBottomBar(ActType.ActType_HOME,act);
					return false;
				}
				return false;
			}
		});		
		buttom_monitor_btn.setOnTouchListener(new OnTouchListener(){
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				if(event.ACTION_DOWN==event.getAction()){
					buttom_home_btn.setBackgroundResource(R.drawable.normal_home_btn);
					buttom_monitor_btn.setBackgroundResource(R.drawable.pressed_monitor_btn);
					buttom_me_btn.setBackgroundResource(R.drawable.normal_me_btn);
					return true;
				}else if(event.ACTION_UP == event.getAction()){
					GotoActivityByBottomBar(ActType.ActType_MONITOR,act);
				}
				return false;
			}
		});		
		buttom_me_btn.setOnTouchListener(new OnTouchListener(){
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				if(event.ACTION_DOWN==event.getAction()){
					buttom_home_btn.setBackgroundResource(R.drawable.normal_home_btn);
					buttom_monitor_btn.setBackgroundResource(R.drawable.normal_monitor_btn);
					buttom_me_btn.setBackgroundResource(R.drawable.pressed_me_btn);
					return true;
				}else if(event.ACTION_UP == event.getAction()){
					GotoActivityByBottomBar(ActType.ActType_ME,act);
				}
				return false;
			}
		});
	}
	public void GotoActivityByBottomBar(ActType type,Activity act)
	{
		Log.v("QBing", "GotoActivityByBottomBar");
		if(current_type != type){
			switch(type){
			case ActType_HOME:
				Intent homt_it = new  Intent(act,HomeActivity.class);
				act.startActivity(homt_it);
				act.finish();
				break;
			case ActType_MONITOR:
				Intent monitor_it = new  Intent(act,Monitor_Activity.class);
				act.startActivity(monitor_it);
				act.finish();
				break;
			case ActType_ME:
				Intent me_it = new  Intent(act,Me_Activity.class);
				act.startActivity(me_it);
				act.finish();
				break;
			}
			current_type = type;
		}
	}
}
