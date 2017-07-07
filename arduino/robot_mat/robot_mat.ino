#include <ESP8266WiFi.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <ros/time.h>
#include <rosserial_arduino/MotorMove.h>
#include <rosserial_arduino/EngineMove.h>
#include <rosserial_arduino/EngineEncoder.h>
#include "Robot.h"
//Necessary for the timer function
extern "C"
{
  #include "user_interface.h"
}

#define DEBUG 1


//Network Configuration
const char* ssid = "***";
const char* password = "***";  
IPAddress server(161,72,123,196);      
const uint16_t serverPort = 11411;

//Timer configuration
uint32_t last = 0;
uint32_t now = 0;
os_timer_t a_timer;
int timer_period = 1000000;       

//Robot
Robot robot;
//Ros node handler
ros::NodeHandle nh;
/**
 * 
 */
void messageMotorMove( const rosserial_arduino::MotorMove& msg) {  
  nh.logdebug("Motor Move");  
  robot.move(msg.number,msg.power,msg.direction);
}
ros::Subscriber<rosserial_arduino::MotorMove> motor_move("motor_move", &messageMotorMove);
/**
 * 
 */
void messageEngineMove( const rosserial_arduino::EngineMove& msg) {  
  nh.logdebug("Engine Move");  
  robot.move(msg.power,msg.direction);
}
ros::Subscriber<rosserial_arduino::EngineMove> engine_move("engine_move", &messageEngineMove);

rosserial_arduino::EngineEncoder engine_encoder;
ros::Publisher motor_encoder("motor_encoder", &engine_encoder);

void timer_callback(void) {
  timer0_write(ESP.getCycleCount() + timer_period * 80); // 160 when running at 160mhz
  //Update velocity
  robot.updateVelocity();
}

void setup() {
  //Setup Serial line.
  Serial.begin(115200);

  //Setup Wifi
  setupWiFi();
  delay(2000);

  //Configure timers 
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(timer_callback);
  timer0_write(ESP.getCycleCount() + timer_period * 80); // 160 when running at 160mhz
  interrupts();
    
  //Setup Robot
  robot.setup();
  //Attact the Interruptions 
  attachInterrupt(digitalPinToInterrupt(14), update_wheel_1_position, RISING);        
  attachInterrupt(digitalPinToInterrupt(12), update_wheel_2_position, RISING);        
  
  //Init rose node & subscribe
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();
  nh.subscribe(motor_move);
  nh.subscribe(engine_move);
  nh.advertise(motor_encoder);
}

void loop() {
  //if (nh.connected()) {
    //Publish the telemetry information.
    engine_encoder.encoder_1=robot.getPosition(0);
    engine_encoder.encoder_2=robot.getPosition(1);
    engine_encoder.velocity_1=robot.getVelocity(0);
    engine_encoder.velocity_2=robot.getVelocity(1);      
    //Publish
    motor_encoder.publish(&engine_encoder);
    nh.spinOnce(); 
    //Delay
    delay(100);  
  //} 
 }

void update_wheel_1_position() { 
  robot.updatePosition(0);
}

void update_wheel_2_position() {
  robot.updatePosition(1);
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
