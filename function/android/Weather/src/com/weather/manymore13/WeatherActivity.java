package com.weather.manymore13;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Map;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.Bundle;
import android.text.StaticLayout;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.SimpleAdapter.ViewBinder;
import android.widget.TextView;

public class WeatherActivity extends Activity {
	private AutoCompleteTextView autoText;
	private Button btnConfirm;
	private TextView tViewCurrent;
	private ListView lv;
	private ArrayList<Map<String,Object>> list;
	private ImageView currentIcon;
	private TextView currentInfo;
	private TextView futionTime;
	private SimpleAdapter simpAdapter = null;
	private boolean visit = true;
    public void init()
    {
    	autoText = (AutoCompleteTextView)findViewById(R.id.autoTextView);
    	btnConfirm = (Button)findViewById(R.id.btn_confirm);
    	tViewCurrent = (TextView)findViewById(R.id.current_time);
    	lv = (ListView)findViewById(R.id.myListView);
    	currentIcon = (ImageView)findViewById(R.id.current_icon);
    	currentInfo = (TextView)findViewById(R.id.current_info);
    	futionTime = (TextView)findViewById(R.id.future_time);
    }
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        init();
        
        
        ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>
        (this, android.R.layout.simple_dropdown_item_1line,
        		                Tools.city);
        autoText.setAdapter(arrayAdapter);
        btnConfirm.setOnClickListener(new OnClickListener() {
			
			public void onClick(View arg0) {
				
				System.out.println("按钮被按下");
				String city = autoText.getText().toString().trim();
				
				tViewCurrent.setVisibility(View.INVISIBLE);
				futionTime.setVisibility(View.INVISIBLE);
				if(!(city.equals("")))
				{
					try {
						
					
						obtainWeacherInfo(Tools.SEACH_URL+city);
						if(simpAdapter!=null)
						{
							simpAdapter.notifyDataSetChanged();
						}

						
					} catch (MalformedURLException e) {
						System.out.println("url出错");
						e.printStackTrace();
					} 
					 catch (Exception e) {
						e.printStackTrace();
					}
				}
				
			}
		});
        
        
    }
    
    public void obtainWeacherInfo(String path) throws ParserConfigurationException, SAXException, UnsupportedEncodingException, IOException
    {
    	
    	SAXParserFactory parserFactory = SAXParserFactory.newInstance();
    	
		SAXParser sp = parserFactory.newSAXParser();
		
		XMLReader xmlReader = sp.getXMLReader();
		
		WeacherHandler handler = new WeacherHandler();
		
		xmlReader.setContentHandler(handler);
		
		URL url = new URL(path);
		
		URLConnection conn = url.openConnection();
	
		InputStreamReader isr = new InputStreamReader(conn.getInputStream(),"GBK");
		
		BufferedReader br = new BufferedReader(isr);
	
		InputSource is = new InputSource(br);
	
		xmlReader.parse(is);
		
		br.close();
		
		displayWeatherInfo(handler);
		
		
		
		
    }
    
    
    public void displayWeatherInfo(WeacherHandler handler)
    {
    	list = handler.getForecastWeachers();
    	
    	//显示实时信息
    	Bitmap bmp = handler.getCurrentWeather().getBmp();
    	String currentInfoStr = handler.getCurrentWeather().getCurrentInfo().toString();
    	currentIcon.setImageBitmap(bmp);
    	currentInfo.setText(currentInfoStr);
    
    	if(list.size() < 1)
    	{

    		System.out.println("displayWeatherInfo: list.size() < 1");
 
        	currentInfo.setText(R.string.prompt);
        	
        	currentInfo.setTextColor(Color.YELLOW);
            
        	currentInfo.setVisibility(View.VISIBLE);
        	
			
    	}
    	
    	// 显示未来天气, 我们这里是在ListView中显示
    	String[] itemName = new String[]{"week","temperature","condition","icon"};
    	int[] itemId = new int[]{R.id.week, R.id.temperature, R.id.condition,R.id.icon};
    	simpAdapter = new SimpleAdapter(this,
    			list,R.layout.list_item,
    			itemName,itemId); 
    	lv.setAdapter(simpAdapter);
    	if(list.size() > 0)
    	{
    		tViewCurrent.setVisibility(View.VISIBLE);
    		futionTime.setVisibility(View.VISIBLE);
    	}
    	simpAdapter.setViewBinder(new ViewBinder() {
			
			public boolean setViewValue(View view, Object data,
					String textRepresentation) {
				if(view instanceof ImageView  && data instanceof Bitmap){  
                    ImageView iv = (ImageView) view;  
                    iv.setImageBitmap((Bitmap) data);  
                    return true;  
                }else {
                	return false; 
                }
                
            }  
				
		});
    }
}
