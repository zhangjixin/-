/*#include <Adafruit_NeoPixel.h> //引用此文件

#define PIN_NUM 2 //允许接的led灯的个数
   
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIN_NUM, PIN, NEO_GRB + NEO_KHZ800);  //该函数第一个参数控制串联灯的个数，第二个是控制用哪个pin脚输出，第三个显示颜色和变化闪烁频率
//mcookie的LED
*/


#include <ESP8266.h>
#ifdef ESP32
#error "This code is not recommended to run on the ESP32 platform! Please check your Tools->Board setting."
#endif
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); 
#define EspSerial mySerial
#define UARTSPEED  9600
#endif
#define SSID        F("Honor 10")//WIFI名
#define PASSWORD    F("qwertyuiopa")//WIFI密码
#define HOST_NAME   F("api.heclouds.com")//域名
#define HOST_PORT   (80)
static const byte  GETDATA[]  PROGMEM = {
  "GET https://api.heclouds.com/devices/503230484/datapoints?datastream_id=ah,am,status,tem&limit=1 HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key:ezz1czmZvq8ByuuF4Fu9uHKvYL0= \r\nConnection: close\r\n\r\n"
};//关键头文件
/* "GET GET https://api.heclouds.com/devices/503230484/datapoints?datastream_id=ah,am,status,tem&limit=1 //url+  【 datapoints？datastream id=】 你的几个数据名称【&limit=】（几个数据1个表示最近）
 *  HTTP/1.1\r\n
 *  Host:api.heclouds.com//域名
 *  \r\n
 *  api-key:sCCE3BRbnvyyKsX1iWY7=dSFlME=///API
 *  \r\nConnection: close\r\n\r\n"
*/
ESP8266 wifi(&EspSerial);
//wifi

#include <U8glib.h>
//OLED
#include <Microduino_RTC.h>
//RTC时钟模块
int AL=1;
RTC rtc;
//short RECV_PIN = 10;//遥控//_________________________________________________________________________________________________int RECV_PIN = 10;//遥控
short pin3 = 2;//触摸      //_________________________________________________________________________________________________int pin3 = 2;//触摸
short pin2 = A0;//光敏     //_________________________________________________________________________________________________int pin2 = A0;//光敏
#define POUT 5   //led灯控制引脚   //led灯控制引脚
int sensorValue;
int up=23,down=21,rh=0,rm=0;
//#include <IRremote.h>
//IRrecv irrecv(RECV_PIN);
//decode_results results;
//遥控 内存不够
#include "audio.h"
//#include <SoftwareSerial.h>
int music_vol = 1; //初始音量0~30


DateTime dateTime = {2018,12,5,18,11,0, 0};//时间初值
short tem=-3;//______________________________________________________________short temp1,Rstatus=0,lx=0;
short ah=0,am=0,ch=0;
bool Rstatus=0,ws=0,tcp=0,ala=0,touch=1;
String rstatus,wstatus,astatus;

uint16_t tYear;
uint8_t tMonth, tWeekDay,tDay, tHour, tMinute, tSecond; 
#define INTERVAL_LCD             1000             //定义OLED刷新时间间隔  
 long lcd_time = millis();                 //OLED刷新时间计时器
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);     //设置OLED型号
//-------字体设置，大、中、小
#define setFont_H u8g.setFont(u8g_font_timB18)
#define setFont_M u8g.setFont(u8g_font_timB10)
//#define setFont_S u8g.setFont(u8g_font_chikitar)
long previousMillis = 0;        // 存储LED最后一次的更新
long interval = 1000;           // 闪烁的时间间隔（毫秒）
unsigned long currentMillis = 0;


void setup() {
  Serial.begin(9600);
  tem=2;
 audio_init(DEVICE_Flash,4,music_vol); 
 // pinMode(pin2, INPUT);//光敏
  pinMode(pin3, INPUT);//触摸
  pinMode(POUT, OUTPUT);
  //                 pinMode(9,OUTPUT);
  //               pinMode(11,OUTPUT);
 // irrecv.enableIRIn(); // 启动红外解码
  rtc.begin();
  rtc.clearAll();//设置启动时间
  rtc.setDateTime(dateTime);//第一次是校准，之后注释掉的话能实现每次不初始化时间，也就是RTC模块独立，顺便一提RTC断电一段时间也螚继续走至少三天（别被core 格式化就可以）
 //  Serial.begin(115200);
 // while (!Serial);
 // delay(100);
  WifiInit(EspSerial, UARTSPEED);
  if (wifi.setOprToStationSoftAP()) {} else {}
if (wifi.joinAP(SSID, PASSWORD)) {ws=1;} else {ws=0;}
  if (wifi.disableMUX()) {} else {}
}
//---------------------------------------------------------------------------------------------------------------------------------------遥控
/*
void remotecontrol( long a)
{
  if(a==33441975)
  {if(Rstatus)
  Rstatus=0;
  else
  Rstatus=1;
  delay(500);
  }//第一输入激活遥控，第二次撤销
          if(Rstatus)//如果激活则控制
          {
                  switch(a){
                        //case 33441975://电源
                        case 33456255://A
                        case 33439935://B
                        case 33472575://C
                        case 33431775:break;//D
                        case 33448095:lx+=5;
                        break;//E
                        case 33464415:(lx+50)>val_max?255:lx+=15;
                        break;//上
                        case 33478695:((lx-50)<0)||((lx-50)>2000)?0:lx-=15;
                        break;//下
                        case 33480735://左
                        case 33460335:
                        break;//右
                        case 33427695:lx=lx>0?0:255;
                        break;//OK
                        case 33486855://音大
                        case 33452175://音效
                        case 33444015://重置
                        case 33468495://禁言
                        case 33435855://暂停？
                        default:
                        break;}
  
  }
}    */ 

//_______________________________________________________________________________________________________________________   /online
void online()
{

 //if(!tcp)
wifi.createTCP(HOST_NAME, HOST_PORT);

 
 wifi.sendFromFlash(GETDATA, sizeof(GETDATA));
//从Flash读取发送内容，节约内存
  uint8_t buffer[512] = {0};
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 20000);
  if (len > 0)
  {
     short k=0,temp=0,tax[3];
 tax[0]=ah;
 tax[1]=am;
 tax[2]=tem;
    for (uint32_t i = 0; i < len-12; i++) {
      if((char)buffer[i]=='v'&&(char)buffer[i+1]=='a'&&(char)buffer[i+2]=='l'&&(char)buffer[i+3]=='u'&&(char)buffer[i+4]=='e')
      {
          for (uint32_t j = 0; j<4;j++)
              if((char)buffer[i+j+7]>='0'&&(char)buffer[i+j+7]<='9')
              {temp=((short)buffer[i+j+7]-48)+temp*10;
              //Serial.print(temp);
              }
              // Serial.print('\n');
              //Serial.println(temp);

       
           if(k==0)
           {//Serial.println(temp);
           ah=temp;}
           else if(k==1) 
           {//Serial.println(temp);
           am=temp;}
           else if(k==2)
           {//Serial.println(temp);
           tem=temp;}
            else if(k==3)
           {//Serial.println(temp);
          AL=temp;
          }
           k++;
           temp=0;     
    } }
  if(ah!=tax[0]||am!=tax[1]||tem!=tax[2]){
  Rstatus=1;
  previousMillis = currentMillis;
  }

  //Serial.println(currentMillis - previousMillis);
  }
  else
 {if(!tSecond%2) ws=0;}
 //wifi.releaseTCP();
}
//__________________________________________________
void alarm()
{
    if(touch=1);
    {
      ala=1;
      audio_play();
       audio_choose(1);}

       
}
//*******************************************************************************************************************************************

void loop() {
    rtc.getTime(&tHour, &tMinute, &tSecond);
  rtc.getDate(&tYear, &tMonth, &tWeekDay,&tDay);
    
  sensorValue = analogRead(pin2);
 //_______________________________________________________________________________________________________________________   
  if(digitalRead(pin3))
  {
  if(tMinute==ch)
   touch=1;
   }
   else
   {
    tMinute+2<60?ch=tMinute+2:ch=tMinute-58;
   touch=0;
    }
  
    //_______________________________________________________________________________________________________________________   
    Serial.println(digitalRead(pin3));
  currentMillis = millis();

//_______________________________________________________________________________________________________________________   //红外控制 
//  if (irrecv.decode(&results)) {
 //   remotecontrol(results.value);
 //   irrecv.resume(); // 接收下一个值
 //   previousMillis = currentMillis; 
//  } 
//_______________________________________________________________________________________________________________________   //光强控制 
/*if((tHour==ah&&((tMinute==am)||(tMinute==am+1)))&&touch&&AL==1)
alarm( );
else{
  audio_pause();
  ala=0;
 
}

  if(Rstatus&&ala==0)
  {
    if(rh==0&&rm==0)
  {rh=tHour;
  rm=tMinute;}
  else
  {
    if(tHour==rh+1&&tMinute==((rm+15)<60?rm+15:rm-45)&&touch)
    {if(tSecond%5)
    {
      lx=125;
     }
     
    }
    if(tHour==rh+1&&tMinute==((rm+30)<60?(rm+30):(rm-30)))
    {
     alarm( );
     }
     else{
  audio_pause();
  ala=0;
  }
  }
  }
  else
  {
    if(tHour==21&&tMinute==0)//熄灯
    lx=255;
    else if(tHour>=down&&tHour<=up)
    {
          if(tSecond==0)lx-=2;
     }
    else if(tHour>=18&&tHour<=20);//反休眠
    else
    lx=0;//休眠
  }
   
   if(tSecond==0&&ws==0&&ala==0)
     {
      if (wifi.joinAP(SSID, PASSWORD)) {ws=1;} else {ws=0;}
     }*/
  //Rstatus?rstatus="ON":rstatus="OFF";
  //ws?wstatus="ON":wstatus="OFF";
  AL==1?astatus="ON":astatus="OFF";
 //_______________________________________________________________________________________________________________________   //OLED  

  /*    u8g.firstPage();
      do {
               setFont_H;
        u8g.setPrintPos(0, 20);
        u8g.print(tHour);                
        u8g.setPrintPos(30, 20);
        u8g.print(":");
        u8g.setPrintPos(45, 20);
        u8g.print(tMinute);
         setFont_M;
        u8g.setPrintPos(75, 12);
        u8g.print(tMonth);
        u8g.setPrintPos(88, 12);
        u8g.print("/");
        u8g.setPrintPos(93, 12);
        u8g.print(tDay);
        u8g.setPrintPos(105, 12);
        u8g.print("/");
        u8g.setPrintPos(111, 12);
        u8g.print(tYear%100);

        u8g.setPrintPos(95, 24);
        u8g.print(astatus);
        u8g.setPrintPos(0, 36);
    
        u8g.setPrintPos(0, 64);
        u8g.print(tem);
        u8g.setPrintPos(50, 64);
        u8g.print((currentMillis - previousMillis)/1000);
        u8g.setPrintPos(80, 64);
        u8g.print(touch);


      } while ( u8g.nextPage() );
}
 */


 //_______________________________________________________________________________________________________________________   //抹除      

    /*      if(sensorValue>900)
     digitalWrite(POUT,LOW);
    else 
     analogWrite(POUT,lx);
     if(ws&&ala==0)
     {online();}
     if(((currentMillis - previousMillis)/1000>3600)&&sensorValue>800||((currentMillis - previousMillis)/1000>10800))//一小时
      Rstatus=0;

     //Serial.println(Rstatus);
     //Serial.println(tcp);
     //Serial.println(ala);


 // audio_play();
   //  audio_choose(1);


}*/
//_______________________________________________________________________________________________________________________  倒计时  alarm




  



  pos:

  for( int second=59;second>=0;second--)
{ u8g.firstPage();
 do {
 setFont_M;
 u8g.setPrintPos(0, 36);
 u8g.print(ah);        
 u8g.setPrintPos(25, 36);
 u8g.print(':');   
 u8g.setPrintPos(40,36);
 u8g.print(am);
 u8g.setPrintPos(50,36);
 u8g.print(':');
 u8g.setPrintPos(70, 36);
 u8g.print(second);
              
 setFont_H;
        u8g.setPrintPos(0, 20);
        u8g.print(tHour);                
        u8g.setPrintPos(30, 20);
        u8g.print(":");
        u8g.setPrintPos(45, 20);
        u8g.print(tMinute);
         setFont_M;
        u8g.setPrintPos(75, 12);
        u8g.print(tMonth);
        u8g.setPrintPos(88, 12);
        u8g.print("/");
        u8g.setPrintPos(93, 12);
        u8g.print(tDay);
        u8g.setPrintPos(105, 12);
        u8g.print("/");
        u8g.setPrintPos(111, 12);
        u8g.print(tYear%100);

        u8g.setPrintPos(95, 24);
        u8g.print(astatus);
        u8g.setPrintPos(0, 36);
    
        u8g.setPrintPos(0, 64);
        u8g.print(tem);
        u8g.setPrintPos(80, 64);
        u8g.print(touch);
       

 }while( u8g.nextPage() );
 delay(1000);
 if(second==0,am>=1)
 {am=am-1;
 goto pos;
 }
     if(second==0);
    {
      ala=1;
      audio_play();
       audio_choose(1);}





}

}




