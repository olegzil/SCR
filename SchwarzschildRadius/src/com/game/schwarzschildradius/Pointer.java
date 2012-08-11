package com.game.schwarzschildradius;

public class Pointer{
	float distance(float x, float y){
		float distX = java.lang.Math.abs(this.getX()-x);
		float distY = java.lang.Math.abs(this.getY()-y);
		return (float) java.lang.Math.sqrt(distX*distX+distY+distY);
	}
	
	public float x, y;
	public int index;
	public int action;
	public double time;
	
	public Pointer(){
		x=y=0;
		index=0;
		action=0;
		time=0;
	}
	//getters/setters
	public float getX(){return this.x;}
	public float getY(){return this.y;}
}