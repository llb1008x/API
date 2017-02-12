package com.weather.manymore13;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;



public class WeacherHandler extends DefaultHandler{

	ArrayList<ForecastWeacher> ForecastWeachers;
	
	ForecastWeacher forecastweather;
	
	CurrentWeather currentWeather = new CurrentWeather();
	
	ArrayList<Map<String,Object>> list;

	boolean forecast_conditions = false;
	
	boolean current_conditions = false;
	
	public void clearWeather()
	{
		if(ForecastWeachers.size() > 0)
		{
			ForecastWeachers.clear();
		}
		if(list.size() > 0)
		{
			list.clear();
		}
		if(currentWeather != null)
		{
			currentWeather = null;
		}
		
	}
	
	public CurrentWeather getCurrentWeather()
	{
		return currentWeather;
	}
	
	// 组装天气信息 最后打包ArrayList返回
	public ArrayList<Map<String,Object>> getForecastWeachers()
	{
	
		Iterator<ForecastWeacher> ite = ForecastWeachers.iterator();
		String str;
		HashMap<String,Object> map;
		while(ite.hasNext())
		{
			forecastweather = null;
			map = new HashMap<String, Object>();
			forecastweather = ite.next();
			if(forecastweather!=null)
			{
				map.put("week", forecastweather.getDayOfWeek());
				map.put("temperature", forecastweather.getLow()+"°C~~"+forecastweather.getHigh()+"°C");
				map.put("condition", forecastweather.getCondition());
				map.put("icon", forecastweather.getIcon());
				list.add(map);
			}
			
		}
		return list;
	}
	@Override
	public void endElement(String uri, String localName, String qName)
			throws SAXException {
		super.endElement(uri, localName, qName);
		if(qName.equals("forecast_conditions"))
		{
			forecast_conditions = false;
			ForecastWeachers.add(forecastweather);
		}else if(qName.equals("current_conditions"))
		{
			current_conditions = false;
		}
		
	}

	@Override
	public void startDocument() throws SAXException {
		
		ForecastWeachers = new ArrayList<ForecastWeacher>(0);
		list = new ArrayList<Map<String,Object>>(0);
		super.startDocument();
	}

	@Override
	public void startElement(String uri, String localName, String qName,
			Attributes attributes) throws SAXException {
		if(qName.equals("current_conditions"))
		{
			current_conditions = true;
		}
		if(qName.equals("forecast_conditions"))
		{
			forecastweather = new ForecastWeacher();
			forecast_conditions = true;
		}else if(forecast_conditions)
		{
			if(qName.equals("day_of_week"))
			{
				forecastweather.setDayOfWeek((attributes.getValue(0)));
			}
			else if(qName.equals("low"))
			{
				forecastweather.setLow(attributes.getValue(0));
				
			}else if(qName.equals("high"))
			{
				forecastweather.setHigh(attributes.getValue(0));
				
			}
			else if(qName.equals("icon"))
			{
				forecastweather.setIcon(Tools.GOOGLE_URL+attributes.getValue(0));
				
			}
			else if(qName.equals("condition"))
			{
				forecastweather.setCondition(attributes.getValue(0));
			}
		}
		if(qName.equals("current_date_time")||qName.equals("city"))
		{
			currentWeather.appendCurrentInfo(attributes.getValue(0)+"\n");
		}
		if(current_conditions)
		{
			if(qName.equals("icon"))
			{
				currentWeather.setBmp(Tools.GOOGLE_URL+attributes.getValue(0));
			}
			if(qName.equals("humidity")||qName.equals("wind_condition")||qName.equals("condition"))
			{
				currentWeather.appendCurrentInfo(attributes.getValue(0)+"\n");
			}
			
		}
		
		super.startElement(uri, localName, qName, attributes);
	}
	

}

