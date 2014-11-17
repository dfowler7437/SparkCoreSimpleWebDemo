const int AIN_BATTERY = A7; //Battery voltage is monitored though a divide by 2 voltage 
                            //divider on my hardware bread board. This will not work for
                            //you unless you add this cicuit.  Do not connect more then 3.3V 
                            //to any pin on the Spark Core.
const int DIO_LED = D7; //Built in LED on D7. It's blue and next to the usb connector
unsigned long currentMS=0;  //Used to keep the current time in milliseconds for paceing in the loop function
unsigned long oldMS=0;  //Used to keep last elappsed time in milliseconds, for paceing in the loop function

double battVoltage;  //The battery voltage, registered in setup for the Spark API.
int rssi;  //WiFi Singal quality value,  registered in setup for the Spark API.

int LED(String command); //this funciton will contol the LED when called from the Web.

//Configure the hardware and register this core's services to the Spark Server. 
void setup(void) { // Arduino style setup. This routine runs only once upon reset
   //Register our Spark functions here.  Note that we are telling the Spark module where to find 
   //the varaibles. We will keep the data fresh in our loop funciton.
  Spark.variable("BattV", &battVoltage, DOUBLE); //Register the BattV variable.
  Spark.variable("rssi", &rssi, INT); //Register the rssi variable. This is the WiFi signal level
  Spark.function("LED",LED);  //Here we register the "led" function. A function is called by the 
                              //Spark Core when someone makes the web request. It receives a string
                              //with any arguments that you must parse.
  
  //Setup hardware functions
  pinMode(DIO_LED,OUTPUT);    //Set the LED pin to output mode
  digitalWrite(DIO_LED,LOW);  //Set the LED pin LOW to start, off
}

//Do all our work in the loop funciton. Note that the Spark Core takes care of 
//all the network stuff when loop returns so dont get stuck in loop for long. Careful with "delay" functions
void loop(void) {
    //Use millisecond time to pace things, we want to get out of the loop function ASAP and will 
    //use this snip of code to check for when it's time to do something.
    currentMS=millis();  //Grab the current millisecond timer value
    if(currentMS>(oldMS+1000)){ //Has 1000 milliseconds ellapsed since last time (1 second)
        oldMS=currentMS;  //Yes, it's time.. Keep the current milliseconds to check later.
        int adcValue=analogRead(AIN_BATTERY);  //Read the battery input voltage (note this requires some wiring)
        battVoltage = ((double)adcValue/4095.0)*6.6; //Convert the raw ADC input to actual volts. (note divide by 2 resistor divider used)

        rssi=WiFi.RSSI();  //Grab the WiFi Signal Level value.
    }
}

//Funciton to control the LED. Takes a string parameter and parses it for the word "ON".
int LED(String command){  //Called by the web LED request automaticaly as registered in setup().
    if(command=="ON"){    //is the command string set to "ON"
        digitalWrite(DIO_LED,HIGH);  //Yes it is, turn on the LED
    }
    else {
        digitalWrite(DIO_LED,LOW);  //Nope, turn it off.
    }
    return(1);  //This value is passed all the way back to the web requester as an int. -1 usualy means fail.
}