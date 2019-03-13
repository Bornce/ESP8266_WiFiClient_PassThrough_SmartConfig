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

#define SSID_BEGIN_ADDR   100
#define STAPSK_BEGIN_ADDR 200

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
  EEPROM.begin(4096);
  
  for(i=SSID_BEGIN_ADDR;i<SSID_BEGIN_ADDR+200;i++)
  {
    EEPROM.write(SSID_BEGIN_ADDR, 0);
  }
  //
  EEPROM.end();
}
//

void EEPROM_Save(char * SSID_Name,char * STAPSK_Name)
{
  int i=0;
  EEPROM_Clear();
  
  EEPROM.begin(4096);
  
  for(i=SSID_BEGIN_ADDR;i<SSID_BEGIN_ADDR+strlen(SSID_Name);i++)
  {
    EEPROM.write(i, *(SSID_Name+i-SSID_BEGIN_ADDR));
  }
  //
  for(i=SSID_BEGIN_ADDR+strlen(SSID_Name);i<STAPSK_BEGIN_ADDR;i++)
  {
    EEPROM.write(i,0);
   }
   //

    

  for(i=STAPSK_BEGIN_ADDR;i<STAPSK_BEGIN_ADDR+strlen(STAPSK_Name);i++)
  {
    EEPROM.write(i, *(STAPSK_Name+i-STAPSK_BEGIN_ADDR));
  }
  //

  for(i=STAPSK_BEGIN_ADDR+strlen(STAPSK_Name);i<STAPSK_BEGIN_ADDR+100;i++)
  {
    EEPROM.write(i,0);
  }
  //

  EEPROM.end();

  EEPROM.begin(4096);

  Serial.printf("The Data in Flash:");
  for(i=SSID_BEGIN_ADDR;i<SSID_BEGIN_ADDR+200;i++)
  {
    Serial.printf("%c",EEPROM.read(i));
  }
  //
    

  
}
//




void EEPROM_Read(char * SSID_Name,char * STAPSK_Name)
{
  int i=0;

  EEPROM.begin(4096);
  for(i=SSID_BEGIN_ADDR;i<SSID_BEGIN_ADDR+100;i++)
  {
    *(SSID_Name+i-SSID_BEGIN_ADDR)  = EEPROM.read(i);
  }
  //

  for(i=STAPSK_BEGIN_ADDR;i<STAPSK_BEGIN_ADDR+100;i++)
  {
    *(STAPSK_Name+i-STAPSK_BEGIN_ADDR)  = EEPROM.read(i);
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
      char SSID_name[100]={0};
      char STAPSK_name[100]={0};
      waitRebootCount = 0;
      WiFi.setAutoConnect(true);  // 设置自动连接
      
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());



      sprintf(SSID_name,"%s",WiFi.SSID().c_str());
      sprintf(STAPSK_name,"%s",WiFi.psk().c_str());

      Serial.printf("SAVED SSID:%s\r\n",SSID_name);
      Serial.printf("SAVED PSW:%s\r\n",STAPSK_name);
            
      EEPROM_Save(SSID_name,STAPSK_name);

      
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
  
  delay(5000);

  pinMode(LED2, OUTPUT);
  staticTicker.attach_ms(2000, staticBlink);

  EEPROM.begin(4096);
  
  if(EEPROM.read(SSID_BEGIN_ADDR)==0)
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

  
//	Serial.println("UID&KEY INF:");
//	Serial.print("SSID_Name is"); Serial.println(SSID_Name);
//	Serial.print("STAPSK_Name is"); Serial.println(STAPSK_Name);
	
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

  Serial.flush();
  
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
        ESP.restart();
      }
      //
      if(!client.connected())
      {
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
  
//  char RestoreFactoryDefaults[]="RestoreFactoryDefaults_YueDian";
  char RestoreFactoryDefaults[]="ReFDYD";
  char  RebootCmd[]="Reboot";
  
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


	if(!strncmp((char *)UartGet,(char *)RestoreFactoryDefaults,sizeof(RestoreFactoryDefaults)-1))
	{
		EEPROM_Clear();
		ESP.restart();
	}
	//
  if(!strncmp((char *)UartGet,(char *)RebootCmd,sizeof(RebootCmd)-1))
  {
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

  char RestoreFactoryDefaults[]="ReFDYD";
  char  RebootCmd[]="Reboot";
  
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
		
		memset(WifiGet,0,sizeof(WifiGet));
		while(lenOfSeverRev--)
		{
			WifiGet[index]	=	client.read();
			index++;
		}
		//

  if(!strncmp((char *)WifiGet,(char *)RestoreFactoryDefaults,sizeof(RestoreFactoryDefaults)-1))
  {
    EEPROM_Clear();
    ESP.restart();
  }
  //
  if(!strncmp((char *)WifiGet,(char *)RebootCmd,sizeof(RebootCmd)-1))
  {
    ESP.restart();
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
