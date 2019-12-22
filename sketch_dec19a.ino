//GSM GPS ARDUINO CODE
#include<SoftwareSerial.h>
SoftwareSerial Serial1(2,3); //make RX arduino line is pin 2, make TX arduino line is pin 3.
SoftwareSerial gps(10,11);
#include<LiquidCrystal.h>
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);
const int buttonPin = 12;
#define echoPin 4 // Echo Pin
#define trigPin 5 // Trigger Pin
#define echoPin1 6 // Echo Pin
#define trigPin1 7 // Trigger Pin
#define buzzer 12
#define water 8
#define  button 9
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

int maximumRange1 = 200; // Maximum range needed
int minimumRange1 = 0; // Minimum range needed
long duration1, distance1; // Duration used to calculate distance
void beep()
{
 digitalWrite(buzzer,LOW);delay(100); digitalWrite(buzzer,HIGH);delay(100);  
//digitalWrite(buzzer,LOW);delay(1000); digitalWrite(buzzer,HIGH);delay(1000);
}




int buttonState = 0;  
int i=0,k=0;
int  gps_status=0;
float latitude=0; 
float logitude=0;                       
String Speed="";
String gpsString="";
char *test="$GPRMC";
void initModule(String cmd, char *res, int t)
{
  while(1)
  {
    Serial.println(cmd);
    Serial1.println(cmd);
    delay(100);
    while(Serial1.available()>0)
    {
       if(Serial1.find(res))
       {
        Serial.println(res);
        delay(t);
        return;
       }
       else
       {
        Serial.println("Error");
       }
    }
    delay(t);
  }
}

void setup() 
{
  pinMode(buzzer, OUTPUT);digitalWrite(buzzer,HIGH);delay(1000); 
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);

 pinMode(trigPin1, OUTPUT);
 pinMode(echoPin1, INPUT);

 pinMode(water, INPUT);pinMode(button, INPUT);digitalWrite(button,HIGH);delay(1000);
 
 
  Serial1.begin(9600);
  Serial.begin(9600);
  lcd.begin(16,2);  
  lcd.print("Blind Stick ");
  lcd.setCursor(0,1);
  lcd.print("     System     ");
  delay(2000);
  lcd.clear();
  lcd.print("Initializing");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  delay(1000);
  
  Serial.println("Initializing....");
  initModule("AT","OK",1000);
  initModule("ATE1","OK",1000);
  initModule("AT+CPIN?","READY",1000);  
  initModule("AT+CMGF=1","OK",1000);     
  initModule("AT+CNMI=2,2,0,0,0","OK",1000);  
  Serial.println("Initialized Successfully");
  
  
  lcd.clear();
  lcd.print("Waiting For GPS");
  lcd.setCursor(0,1);
  lcd.print("     Signal    ");
  delay(2000);
  gps.begin(9600);
  get_gps();
  show_coordinate();
  delay(2000);
  lcd.clear();
  lcd.print("GPS is Ready");
  delay(1000);
  lcd.clear();
  lcd.print("System Ready");
  Serial.println("System Ready..");
  
}
void loop() 
{
    
    buttonState = digitalRead(water);
    if (buttonState == LOW)
    {
      delay(100);lcd.clear();delay(100);lcd.print("water detected");delay(1000);
      beep();
      get_gps();
      show_coordinate();
      lcd.clear();
      lcd.print("Sending SMS ");
      Serial.println("Sending SMS");
      Send();
      Serial.println("SMS Sent");
      delay(2000);
      lcd.clear();
      lcd.print("System Ready");
    }
    else
    {
      delay(100);lcd.clear();delay(100);lcd.print("N0 WATER");delay(1000);
          
    }


 buttonState = digitalRead(button);
    if (buttonState == LOW)
    {
      delay(100);lcd.clear();delay(100);lcd.print("button pressed");delay(1000);
      beep();beep();
      get_gps();
      show_coordinate();
      lcd.clear();
      lcd.print("Sending SMS ");
      Serial.println("Sending SMS");
      Send();
      Serial.println("SMS Sent");
      delay(2000);
      lcd.clear();
      lcd.print("System Ready");
    }
    else
    {
      delay(100);lcd.clear();delay(100);lcd.print("no problem");delay(1000);
            
    }



digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 distance = duration/58.2;
  delay(100);lcd.clear();lcd.print("Block1:");lcd.print(distance);lcd.print(" cm");delay(1000);

if(distance<30)
{
 delay(100);lcd.setCursor(0,1);lcd.print("object detected");delay(1000);
      beep();beep();beep();
      get_gps();
      show_coordinate();
      lcd.clear();
      lcd.print("Sending SMS ");
      Serial.println("Sending SMS");
      Send();
      Serial.println("SMS Sent");
      delay(2000);
      lcd.clear();
      lcd.print("System Ready");  
}
else
{
 delay(100);lcd.setCursor(0,1);delay(100);lcd.print("no object ");delay(1000); 
}

digitalWrite(trigPin1, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin1, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin1, LOW);
 duration1 = pulseIn(echoPin1, HIGH);
 distance1 = duration1/58.2;
 lcd.setCursor(0,1); lcd.print("Block2:");lcd.print(distance1);lcd.print(" cm");delay(1000);lcd.clear();

if(distance1<30)
{
 delay(100);lcd.clear();delay(100);lcd.print("manhole here");delay(100);
      beep();beep();beep();beep();
      get_gps();
      show_coordinate();
      lcd.clear();
      lcd.print("Sending SMS ");
      Serial.println("Sending SMS");
      Send();
      Serial.println("SMS Sent");
      delay(2000);
      lcd.clear();
      lcd.print("System Ready");  
}
else
{
 delay(100);lcd.clear();delay(100);lcd.print("safe road");delay(1000); 
}

    
}

void gpsEvent()
{
  gpsString="";
  while(1)
  {
   while (gps.available()>0)            //Serial incoming data from GPS
   {
    char inChar = (char)gps.read();
     gpsString+= inChar;                    //store incoming data from GPS to temparary string str[]
     i++;
    // Serial.print(inChar);
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])         //check for right string
      {
        i=0;
        gpsString="";
      }
     }
    if(inChar=='\r')
    {
     if(i>60)
     {
       gps_status=1;
       break;
     }
     else
     {
       i=0;
     }
    }
  }
   if(gps_status)
    break;
  }
}
void get_gps()
{
  lcd.clear();
  lcd.print("Getting GPS Data");
  lcd.setCursor(0,1);
  lcd.print("Please Wait.....");
   gps_status=0;
   int x=0;
   while(gps_status==0)
   {
    gpsEvent();
    int str_lenth=i;
    coordinate2dec();
    i=0;x=0;
    str_lenth=0;
   }
}
void show_coordinate()
{
    lcd.clear();
    lcd.print("Lat:");
    lcd.print(latitude);
    lcd.setCursor(0,1);
    lcd.print("Log:");
    lcd.print(logitude);
    Serial.print("Latitude:");
    Serial.println(latitude);
    Serial.print("Longitude:");
    Serial.println(logitude);
    Serial.print("Speed(in knots)=");
    Serial.println(Speed);
    delay(2000);
    lcd.clear();
    lcd.print("Speed(Knots):");
    lcd.setCursor(0,1);
    lcd.print(Speed);
}
void coordinate2dec()
{
  String lat_degree="";
    for(i=19;i<=20;i++)         
      lat_degree+=gpsString[i];
      Serial.println(lat_degree);
  String lat_minut="";
     for(i=21;i<=30;i++)         
      lat_minut+=gpsString[i];
      Serial.println(lat_minut);
  String log_degree="";
    for(i=32;i<=34;i++)
      log_degree+=gpsString[i];
      Serial.println(log_degree);
  String log_minut="";
    for(i=35;i<=44;i++)
      log_minut+=gpsString[i];
      Serial.println(log_minut);
    
    Speed="";
    for(i=45;i<48;i++)          //extract longitude from string
      Speed+=gpsString[i];
      
     float minut= lat_minut.toFloat();
     minut=minut/60;
     float degree=lat_degree.toFloat();
     latitude=degree+minut;
     Serial.println(latitude);
     
     minut= log_minut.toFloat();
     minut=minut/60;
     degree=log_degree.toFloat();
     logitude=degree+minut;
     Serial.println(logitude);
}
void Send()
{ 
   Serial1.println("AT");
   delay(500);
   serialPrint();
   Serial1.println("AT+CMGF=1");
   delay(500);
   serialPrint();
   Serial1.print("AT+CMGS=");
   Serial1.print('"');
   Serial1.print("8886719545");    //mobile no. for SMS alert
   Serial1.println('"');
   delay(500);
   serialPrint();
   Serial1.print("Latitude:");
   Serial1.println(latitude,6);
   delay(500);
   serialPrint();
   Serial1.print(" longitude:");
   Serial1.println(logitude,6);
   delay(500);
   serialPrint();
   Serial1.print(" Speed:");
   Serial1.print(Speed);
   Serial1.println("Knots");
   delay(500);
   serialPrint();
   Serial1.print("http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=");
   Serial1.print(latitude,6);
   Serial1.print("+");              //28.612953, 77.231545   //28.612953,77.2293563
   Serial1.print(logitude,6);
   Serial1.write(26);
   delay(2000);
   serialPrint();
}
void serialPrint()
{
  while(Serial1.available()>0)
  {
    Serial.print(Serial1.read());
  }
}


