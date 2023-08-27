//including all of the essential libraries

#include<DallasTemperature.h> //library for DS18B20 digital temperature sensor
#include<OneWire.h> //library for onewire device
#include <GravityTDS.h>
#include <SIM900.h>
#include<math.h>
#include <DFRobot_PH.h>



//temperature variables and instance
#define wirebus 3
OneWire onewire(wirebus); //creating onewire instance to communicate to onewire device
DallasTemperature sensor(&onewire); //passing onewire refrence
float temperature = 0;



//relay module variables
#define relaypin 9

//water detector sensor variables
#define water_lvl 10


//tds variables and instance
#define tds_pin A0
GravityTDS gravitytds;
float tds_avg, tds_val;



//turbidity variables
#define turb_pin A1
float turb_avg, turb_val, ntu;
int decimal_p = 2;



//ph variables
#define ph_pin A2
float ph_avg, b, phValue=0.0, voltage_ph=0.0;
DFRobot_PH ph;
int buf[10],temp;


//Sim800L and http variables 
Connection * SIM900 = NULL; //Pointer to connection class
unsigned int  httpCodeResponse;
char * bodyResponse = NULL;
String apiKeyValue="YourApiKey"; //14.11-Api Key value any number but it should be updated at Php file also
String product_id="YourProductId";






void setup() {
    Serial.begin(115200);
    Serial.println("Initializing all of the sensors, modules and I/O pins....");

    //defining I/O pins
    pinMode(relaypin, OUTPUT);
    digitalWrite(relaypin,LOW);

    //water level pin
    pinMode(water_lvl,INPUT);

    //Turbidity sensor initialization
    pinMode(turb_pin,INPUT);// turbidity pinmode setup

    //Ph sensor initialization
    pinMode(ph_pin,INPUT);

    //temperature sensor initialization
    sensor.begin();
    Serial.println("DS18B20 sensor initialization successfull!");
    delay(15);

    //tds sensor initialization
    pinMode(tds_pin,INPUT);
    gravitytds.setPin(tds_pin);
    gravitytds.setAref(5.0); //default refrence voltage ADC only for arduino uno
    gravitytds.setAdcRange(1024); // 1024 for arduino which have 10bit ADC
    gravitytds.begin();
    Serial.println("Gravity TDS sensor initialization successfull!");
    delay(15);

   

    //Sim800L initialization
    // Init SIM900 library ( PinCode, APN, APN_USER, APN_PASSWORD, ENABLE_PIN, SERIAL )
    SIM900 = new Connection( "0000", "internet", "", "", 2, Serial,115200);  
    delay(10000);
    
    if ( !SIM900->Configuration() )
    {
      Serial.println( "@@@@@@@@@@@@@@@@@@@@@@@@@@" );
      Serial.println( "SIM900 CONFIGURATION FAIL" );
      Serial.println( "@@@@@@@@@@@@@@@@@@@@@@@@@@" );
    }
   
      while (!Serial) {
      Serial.print("."); // wait for serial port to connect. Needed for Native USB only
    }
    Serial.println("Program started");


}

void loop() {

      digitalWrite(relaypin,HIGH); //high level trigger to turn the valve ON

      int val = digitalRead(water_lvl); //reading the water level sensor data high or low


      if(val){ // initiating the program if certain water level has reached


      digitalWrite(relaypin,LOW); //turning off the valve, to start the draining and begin sensor reading

      
  
      sensor.requestTemperatures();
      temperature = sensor.getTempCByIndex(0);

      tds_avg  = 0.0;
      turb_avg = 0.0;
      ph_avg   = 0.0;      

      
      
      //loop 10 times to calculate more accurate readings
      for(int i=0;i<10;i++){

            //TDS calculation 
            gravitytds.setTemperature(temperature);
            gravitytds.update();
            tds_val = gravitytds.getTdsValue();
            tds_avg += tds_val;
            delay(1000);


            //turbidity calculation
            
            turb_val = 0;
            for(int i=0; i<800; i++)
            {
                turb_val += ((float)analogRead(turb_pin)/1023)*5;
            }
            turb_val = turb_val/800;
            
            turb_val = round(turb_val * pow(10,decimal_p)) / pow(10,decimal_p);

            if(turb_val < 2.5){
              ntu = 3000;
            }else{
              ntu = -1120.4*pow(turb_val,2)+5742.3*turb_val-4353.8; 
            }

            turb_avg += ntu;
            
            

            //ph calculation

            delay(1000);
            static unsigned long timepoint1 = millis();
            if(millis()-timepoint1>1000U){                  //time interval: 1s
                timepoint1 = millis();
                voltage_ph = analogRead(ph_pin)/1024.0*5000;  // read the voltage
                phValue = ph.readPH(voltage_ph,temperature);  // convert voltage to pH with temperature compensation
                ph_avg=ph_avg+phValue;
                }

      }



      //averaging all of the readings
      tds_val = tds_avg/10;
      ntu = turb_avg/10;
      phValue = ph_avg/10;
      Serial.println(phValue);

      
  
    // Prepare your HTTP POST request data
    String url;
    url += F("api_key=");
    url += String(apiKeyValue); 
    url += F("&product_id=");
    url += String(product_id); 
    url += F("&tds_value=");
    url += String(tds_val,2);
    url += F("&turb_value=");
    url += String(ntu);
    url += F("&ph_value=");
    url += String(phValue);
//    url += F("&ec_value=");
//    url += String(ecValue);
//    Serial.println(url);

    int str_len = url.length() + 1; 
    char data1[str_len];
    url.toCharArray(data1, str_len);
    
    // GSM Module sending Data ___________________________________________________
    // Make a POST petition to the server
   
  
  if ( SIM900->Post( "Url","endpoint","name of the server file",data1,httpCodeResponse ) )
  {
    Serial.println("");
    Serial.println(data1);
    Serial.println( "########################" );
    Serial.println( "==POST OPERATION RESULTS== " );
    Serial.print( "HTTP CODE  REPLY: " );
    Serial.println( httpCodeResponse );
    Serial.println( "########################" );
    Serial.println("AT+HTTPREAD");
  }
  
  else 
  {
    Serial.println( httpCodeResponse );
    Serial.println( "FAIL POST METHOD"); 
    Serial.println( "@@@@@@@@@@@@@@@@@@@@@@@@@@" );
  } 
    
   Serial.print(url);
   
   delay(900000);//Send an HTTP POST request every 15 minutes

      
      } // end of water level check block
  

}
