package com.game.schwarzschildradius;

import java.util.ArrayList;
import java.util.List;

import android.view.MotionEvent;
import android.view.View;


public class GameInputHandler{
	final String fTAG="GestureInputHandler =-=-=-=-= ";
	List<Pointer> pointers;
	int indexInMotion;
	View mView;
	public GameInputHandler(View view)
	{
		indexInMotion = 0;
		pointers =new ArrayList<Pointer>();		mView = view;
	}
	public boolean onTouchEvent(MotionEvent e){
		boolean retVal = true;
		int action = e.getAction() & MotionEvent.ACTION_MASK;
		switch(action)
		{
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
			case MotionEvent.ACTION_MOVE:
				for (int i=0; i<e.getPointerCount(); ++i)
					NativeEntry.touchInput(action, e.getX()/mView.getWidth(), e.getY()/mView.getHeight(), i, e.getEventTime());
				break;
			default:
				retVal = false;
		}
		return retVal;
	}

}
