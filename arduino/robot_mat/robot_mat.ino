#include <ESP8266WiFi.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <rosserial_arduino/MotorMove.h>
#include <rosserial_arduino/EngineMove.h>
#include <rosserial_arduino/EngineEncoder.h>
#include "motor.h"

const char* ssid = "***";
const char* password = "***";  

Motor *engine[2];


long previous_time=0;

int previous_pos_1=0;
int previous_pos_2=0;

IPAddress server(192, 168, 1, 40); // ip of your ROS server
IPAddress ip_address;
int status = WL_IDLE_STATUS;

WiFiClient client;

class WiFiHardware {
public:
  WiFiHardware() {};

  void init() {
    client.connect(server, 11411);
  }
  // read a byte from the serial port. -1 = failure
  int read() {
    // implement this method so that it reads a byte from the TCP connection and returns it
    //  you may return -1 is there is an error; for example if the TCP connection is not open
    return client.read();         //will return -1 when it will works
  }
  // write data to the connection to ROS
  void write(uint8_t* data, int length) {
    // implement this so that it takes the arguments and writes or prints them to the TCP connection
    for(int i=0; i<length; i++)
      client.write(data[i]);
  }
  // returns milliseconds since start of program
  unsigned long time() {
     return millis(); // easy; did this one for you
  }
};

ros::NodeHandle_<WiFiHardware>  nh;
/**
 * 
 */
void messageMotorMove( const rosserial_arduino::MotorMove& msg) {  
  nh.logdebug("Motor Move");
  if ( (msg.number <0) || (msg.number >1) ) {
    nh.logdebug("Number motor out of range");
    return;
  }  
  int result = engine[msg.number]->move(msg.power,msg.direction);
  if (result == -1) {
    nh.logdebug("Out of range");
  }  
}
ros::Subscriber<rosserial_arduino::MotorMove> motor_move("motor_move", &messageMotorMove);
/**
 * 
 */
void messageEngineMove( const rosserial_arduino::EngineMove& msg) {  
  nh.logdebug("Engine Move");  
  for (int i=0;i<2;i++) {
    int result = engine[i]->move(msg.power,msg.direction);
      if (result == -1) {
        nh.logdebug("Out of range");
      }
  }
}
ros::Subscriber<rosserial_arduino::EngineMove> engine_move("engine_move", &messageEngineMove);

rosserial_arduino::EngineEncoder engine_encoder;
ros::Publisher motor_encoder("motor_encoder", &engine_encoder);

void setup() {

  Serial.begin(115200);
  setupWiFi();
  delay(2000);
  
  engine[0] = new Motor(5,0,14);
  engine[1] = new Motor(4,2,12);
  for (int i=0;i<2;i++) {
    engine[i]->setup();
  }  
  attachInterrupt(digitalPinToInterrupt(14), update_wheel_1_position, RISING);        
  attachInterrupt(digitalPinToInterrupt(12), update_wheel_2_position, RISING);        

  
  nh.initNode();
  nh.subscribe(motor_move);
  nh.subscribe(engine_move);
  nh.advertise(motor_encoder);

  engine_encoder.encoder_1=0;
  engine_encoder.encoder_2=0;  

  previous_time=millis();
}

void loop() {
  if (millis() > previous_time + 1000 ) {    
    engine_encoder.velocity_1=engine[0]->getPosition()-previous_pos_1;
    engine_encoder.velocity_2=engine[1]->getPosition()-previous_pos_2;  

    previous_pos_1=engine[0]->getPosition();
    previous_pos_2=engine[1]->getPosition();    
    previous_time=millis();
  }

  engine_encoder.encoder_1=engine[0]->getPosition();
  engine_encoder.encoder_2=engine[1]->getPosition();  

  motor_encoder.publish(&engine_encoder);
  delay(100);
  nh.spinOnce();
}

void update_wheel_1_position() { 
  engine[0]->updatePosition();
}

void update_wheel_2_position() {
  engine[1]->updatePosition();    
}

void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial.print("Could not connect to"); Serial.println(ssid);
    while(1) delay(500);
  }
  Serial.print("Ready! Use ");
  Serial.print(WiFi.localIP());
  Serial.println(" to access client");
}
