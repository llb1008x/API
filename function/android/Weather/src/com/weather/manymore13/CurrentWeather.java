package com.weather.manymore13;

import android.graphics.Bitmap;

public class CurrentWeather {
	private Bitmap bmp;
	private StringBuilder currentInfo = new StringBuilder();
	public Bitmap getBmp() {
		return bmp;
	}
	public void setBmp(String path) {
		this.bmp = Tools.setIcon(path);
	}
	public StringBuilder getCurrentInfo() {
		return currentInfo;
	}
	public void appendCurrentInfo(String currentInfo) {
		this.currentInfo.append(currentInfo);
	}
    
}
