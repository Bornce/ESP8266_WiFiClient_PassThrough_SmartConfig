/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/
/*
 * LED灯状态表
 *    没连上wifi                   4秒闪烁一次
 *    连上wifi没连上server         2秒闪烁一次
 *    连上server                   200ms闪烁一次
*/



/*
 * *********include
*/
#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "Ticker.h"
#include <EEPROM.h>
#include "string.h"




/*
 * ***********define
*/
#ifndef STASSID
#define STASSID "DTU_AP_191A"
//#define STASSID "alex"
//#define STASSID "vivo NEX A"
//#define STASSID "Master08"
//#define STASSID "CMCC-1103-24G"


//#define STAPSK  "18818263137"
#define STAPSK  "12345678"
//#define STAPSK  "20080808"
#endif


#define LED2  2


/*
 * struct
*/
typedef enum 
{
   DISCONNECT,
   CONNECTED,
}Enum_Net_Status;



/*
 * global argument
*/
WiFiClient client;
Ticker staticTicker;

int addr = 0; //EEPROM数据地址


const char* ssid     = STASSID;
const char* password = STAPSK;


const char* host = "mqtt.17cc.cn";
const uint16_t port = 1883;

byte WifiGet[512]={0};
byte UartGet[512]={0};
byte WifiSend[512]={0};
byte UartSend[512]={0};


/*
 * Functions
*/
void staticBlink() 
{
  digitalWrite(LED2, !digitalRead(LED2));
}
//

void EEPROM_Clear(void)
{
  int i=0;
  for(i=0;i<200;i++)
  {
    EEPROM.write(i, 0);
  }
  //
}
//

void EEPROM_Save(char * SSID_Name,char * STAPSK_Name)
{
  int i=0;
  for(i=0;i<strlen(SSID_Name);i++)
  {
    EEPROM.write(i, *(SSID_Name+i));
  }
  //

  for(i=100;i<100+strlen(STAPSK_Name);i++)
  {
    EEPROM.write(i, *(STAPSK_Name+i));
  }
  //
  
}
//

void EEPROM_Read(char * SSID_Name,char * STAPSK_Name)
{
  int i=0;
  for(i=0;i<100;i++)
  {
    *(SSID_Name+i)	=	EEPROM.read(i);
  }
  //

  for(i=100;i<200;i++)
  {
    *(STAPSK_Name+i-100)	=	EEPROM.read(i);
  }
  //
  
}
//


void smartConfig()
{
  WiFi.mode(WIFI_STA);
  delay(2000);
  // 等待配网
  WiFi.beginSmartConfig();
 
 while (1)
  {
    static	short waitRebootCount	=	0;
	
  	if(waitRebootCount++<30)
  	{
  		delay(1000);
  	}
  	else
  	{
		waitRebootCount = 0;
		ESP.restart();
  	}
  	//
    

    if (WiFi.smartConfigDone())
    {
      char *SSID_Name;
      char *STAPSK_Name;
      waitRebootCount = 0;
      WiFi.setAutoConnect(true);  // 设置自动连接
      
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());


      SSID_Name = (char *)(WiFi.SSID().c_str());
      STAPSK_Name = (char *)(WiFi.psk().c_str());
      
      EEPROM_Save(SSID_Name,STAPSK_Name);

      
      break;
    }
  }
 

  Serial.print("Wifi Connected!\r\n"); 

}
//
/*********************** slipe line *********************************/
/*
 * main FUNCTION
*/

void setup() 
{
  char SSID_Name[100]={0};
  char STAPSK_Name[100]={0};  

  Serial.begin(115200);
  Serial.flush();
  
  delay(1000);

  pinMode(LED2, OUTPUT);
  staticTicker.attach_ms(2000, staticBlink);

  
  if(EEPROM.read(0)==0)
  {
	  Serial.print("Go to Smart Mode");
    smartConfig();
  }
  //
  else
  {
	EEPROM_Read(SSID_Name,STAPSK_Name);

	WiFi.mode(WIFI_STA);
	//WiFi.begin(ssid);
	WiFi.begin(SSID_Name, STAPSK_Name);
	
	Serial.print("保存的账号密码");
	Serial.print("SSID_Name is"); Serial.print(SSID_Name);
	Serial.print("STAPSK_Name is"); Serial.print(STAPSK_Name);
	
	while (WiFi.status() != WL_CONNECTED) 
	{
		#define de_WAITTICK     500
		#define de_SECPRE       1000/de_WAITTICK
		#define REBOOTTIME      30*de_SECPRE
	
	
		static  int waitRebootTime = 0;
		waitRebootTime ++;
	
	
		delay(de_WAITTICK);
		
		if(waitRebootTime>REBOOTTIME)
		{
		waitRebootTime = 0;
		ESP.restart();
		}
		//
	
	}
	//
	Serial.print("Wifi Connected!\r\n");  
	
  }
  //


  
  delay(5000);
}
//

void loop() 
{
  static  Enum_Net_Status  ConnectServerStatus = DISCONNECT;

  /*
   * 检查wifi是否正常，不正常需要重新连接wifi
   */  
  if ( WiFi.status() != WL_CONNECTED )
  {
    ESP.restart();
  }
  //


  
  if (!client.connect(host, port)) 
  {
    staticTicker.attach_ms(1000, staticBlink);
    ConnectServerStatus = DISCONNECT;
    //Serial.println("NET DISCONNECTED!");
    delay(5000);
    return;
  }
  //

  if (client.connected()) 
  {
    if(ConnectServerStatus  !=CONNECTED)
    {
      ConnectServerStatus = CONNECTED;
      staticTicker.attach_ms(100, staticBlink);
      Serial.print("CONNECT OK\r\n");
    }

    delay(500);

  	while(client.connected())
  	{
  		serial_read();
      delay(1);
  		server_read();
      
      if(WiFi.status() != WL_CONNECTED)
      {
        Serial.print("WIFI DISCONNECTED....");
        ESP.restart();
      }
      //
      if(!client.connected())
      {
        Serial.print("NET DISCONNECTED....");
        client.connect(host, port);
      }
      //
  	}
  	//
    
  }
  //
}
//


void serial_read()
{
  int lenOfUartRev = 0;
  int lenOfUartRevTemp = 0;
  int index = 0;
  
  char RestoreFactoryDefaults[]="RestoreFactoryDefaults_YueDian";

  
  if (Serial.available()>0) 
  {
    lenOfUartRev = Serial.available();
    lenOfUartRevTemp  = lenOfUartRev;
    
	  
    
    if (lenOfUartRev > 512) 
    {
      lenOfUartRev = 512;
	    lenOfUartRevTemp	=	512;
    }
    //

    memset(UartGet,0,sizeof(UartGet));
    
    while(lenOfUartRev--) 
    {
      UartGet[index] = Serial.read();
      index ++;
    }
    //


	if(!strncmp((char *)UartGet,(char *)RestoreFactoryDefaults,sizeof(RestoreFactoryDefaults)))
	{
    Serial.print("Get Reboot CMD\r\n");
		EEPROM_Clear();
		ESP.restart();
	}
	//

    
//	Serial.write(&UartGet[0],lenOfUartRevTemp);
	client.write(&UartGet[0],lenOfUartRevTemp);
    
  }    
  //
}
//

void server_read()
{
	int	lenOfSeverRev	=	0;
	int	lenOfSeverRevTemp	=	0;
	int index = 0;
	static  int countOfNotGetDataFromServer = 0;

  
	if(client.available()>0)
	{
		lenOfSeverRev	=	client.available();
		lenOfSeverRevTemp	=	lenOfSeverRev;
		countOfNotGetDataFromServer = 0;

    
		if(lenOfSeverRev>512)
		{
			lenOfSeverRev		=	512;
			lenOfSeverRevTemp	=	512;
		}
		//
		
		
		while(lenOfSeverRev--)
		{
			WifiGet[index]	=	client.read();
			index++;
		}
		//
		
		Serial.write(&WifiGet[0],lenOfSeverRevTemp);
	}
	//
  else/* not get response from server in 120 seconds */
  {
    countOfNotGetDataFromServer ++;
    
    if(countOfNotGetDataFromServer>1200*1000)
    {
      countOfNotGetDataFromServer = 0;
      ESP.restart();
    }
    //
  }
  //
	
	
}
//
