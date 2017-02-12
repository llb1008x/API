package com.weather.manymore13;

import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

public class Tools {
	public static String SEACH_URL = "http://www.google.com/ig/api?hl=zh_CN&weather=";
	public static String GOOGLE_URL = "http://www.google.com";
	public static String[] city =
				{
					"beijing",
					"shanghai",
					"tianjin",
					"shenzhen",
					"wuhan",
					"huanggang"
					
				};
	public static Bitmap setIcon(String path)
	{
		URL url;
		try {
			url = new URL(path);
			URLConnection conn = url.openConnection();
			InputStream is = conn.getInputStream();
			Bitmap bm = BitmapFactory.decodeStream(is);
			is.close();
			return bm;
			
		} catch (MalformedURLException e) {
			
			e.printStackTrace();
		} catch (IOException e) {
	
			e.printStackTrace();
		}
		return null;
		
	}

}
