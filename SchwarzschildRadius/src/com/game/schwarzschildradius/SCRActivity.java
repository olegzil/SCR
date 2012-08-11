/*
                * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.game.schwarzschildradius;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class SCRActivity extends Activity {

    SCRView mView;
    private byte[] readSceneGraph(){
        InputStream is = getResources().openRawResource(R.raw.scenegraph);
		try {
			ByteArrayOutputStream byteStream = new ByteArrayOutputStream();
			int data;
			while ( (data = is.read()) > -1)
			{
				byteStream.write(data); //offset to next byte to read
			}
	        return byteStream.toByteArray();
		} catch (IOException e) {
			e.printStackTrace();
		}
    	return null;
    }
    @Override protected void onCreate(Bundle bundle) {
    	Log.d("=-=-=-=", "from on onCreate\n");
        super.onCreate(bundle);
        mView = new SCRView(getApplication());
        byte[] value=readSceneGraph();
        NativeEntry.SceneGraphData(value, value.length);
	setContentView(mView);
    }

    @Override protected void onStart() {
    	Log.d("=-=-=-=", "from on onStart\n");
        super.onStart();
    }

    @Override protected void onRestart() {
    	Log.d("=-=-=-=", "from on onRestart\n");
        super.onRestart();
    }

    @Override protected void onResume() {
    	Log.d("=-=-=-=", "from on onResume\n");
        super.onResume();
    }
   
    @Override protected void onPause() {
    	Log.d("=-=-=-=", "from on onPause\n");
        super.onPause();
    }

    @Override protected void onStop() {
    	Log.d("=-=-=-=", "from on onStop\n");
        super.onStop();
    }
    
    @Override protected void onDestroy() {
    	Log.d("=-=-=-=", "from on onDestroy\n");
    	NativeEntry.destroy();
    	android.os.Process.killProcess(android.os.Process.myPid());
    	super.onDestroy();
    }


}
