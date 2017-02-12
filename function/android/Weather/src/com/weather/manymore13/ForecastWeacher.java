package com.weather.manymore13;

import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

public class ForecastWeacher {
	private Bitmap icon ;
	private String dayOfWeek;
	private String low;
	private String high;
	private String condition;
	
	public Bitmap getIcon() {
		return icon;
	}
	public String getDayOfWeek() {
		return dayOfWeek;
	}
	public String getLow() {
		return low;
	}
	public String getHigh() {
		return high;
	}
	public String getCondition() {
		return condition;
	}
	
	public void setIcon(String path)
	{
		this.icon = Tools.setIcon(path);
	}
	public void setDayOfWeek(String dayOfWeek) {
		this.dayOfWeek = dayOfWeek;
	}
	public void setLow(String low) {
		this.low = low;
	}
	public void setHigh(String high) {
		this.high = high;
	}
	public void setCondition(String condition) {
		this.condition = condition;
	}
	

}
