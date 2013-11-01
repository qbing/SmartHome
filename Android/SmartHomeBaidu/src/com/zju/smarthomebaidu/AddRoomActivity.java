package com.zju.smarthomebaidu;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.Gallery;
import android.widget.ImageView;
import android.widget.AdapterView.OnItemClickListener;

public class AddRoomActivity extends Activity {
	
	private int[] m_imgIDs = { R.drawable.main_room, R.drawable.room_1,
			R.drawable.room_2 };
	private int m_nPicId;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.add_room);
		
		Gallery gl = (Gallery) this.findViewById(R.id.glyPics);
		gl.setSelection((m_imgIDs.length + 1) / 2);
		BaseAdapter ba = new BaseAdapter() {
			@Override
			public int getCount() {
				return m_imgIDs.length;
			}

			@Override
			public Object getItem(int arg0) {
				return null;
			}

			@Override
			public long getItemId(int arg0) {
				return 0;
			}

			@Override
			public View getView(int arg0, View arg1, ViewGroup arg2) {
				ImageView iv = new ImageView(AddRoomActivity.this);
				iv.setImageResource(m_imgIDs[arg0]);
				iv.setScaleType(ImageView.ScaleType.FIT_XY);
				iv.setLayoutParams(new Gallery.LayoutParams(300, 250));
				return iv;
			}
		};
		gl.setAdapter(ba);
		gl.setOnItemClickListener(new OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
					long arg3) {
				Gallery gl = (Gallery) findViewById(R.id.glyPics);
				gl.setSelection(arg2);
				m_nPicId = m_imgIDs[arg2];
			}
		});
	}


}
