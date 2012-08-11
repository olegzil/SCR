package com.game.schwarzschildradius;

import java.util.ArrayList;
import java.util.List;

public class MotionObject
{
	public MotionObject()
	{
		this.actionCode = -1;
		this.pointerIndex = -1;
		this.pointers = new ArrayList<Pointer>();
		this.startTime = eventTime = -1;
	}
	public int actionCode;
	public int pointerIndex;
	public long startTime, eventTime;
	List<Pointer> pointers;
}

