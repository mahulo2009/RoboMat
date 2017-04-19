#include <ESP8266WiFi.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <rosserial_arduino/MotorMove.h>
#include <rosserial_arduino/EngineMove.h>
#include <rosserial_arduino/EngineEncoder.h>

const char* ssid = "***";
const char* password = "***";

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

int wheel_1_power = 5;        
int wheel_1_direction = 0;           
int wheel_1_position = 14;
   
int wheel_2_power = 4;           
int wheel_2_direction = 2;          
int wheel_2_position = 12;

void messageMotorMove( const rosserial_arduino::MotorMove& msg) {  
  nh.logdebug("Motor Move");

  int duty = floor((1023/100) * msg.power);
  
  if ( msg.number == 1 ) {
    analogWrite(wheel_1_power,duty);
    if( msg.direction== 0 ) {
      digitalWrite(wheel_1_direction,LOW);
    } else if( msg.direction== 1 ) {
      digitalWrite(wheel_1_direction,HIGH);
    }    
  } else if (msg.number == 2 ) {
    analogWrite(wheel_2_power,duty);
    if( msg.direction== 0 ) {
      digitalWrite(wheel_2_direction,LOW);
    } else if( msg.direction== 1 ) {
      digitalWrite(wheel_2_direction,HIGH);
    }
  }
}
ros::Subscriber<rosserial_arduino::MotorMove> motor_move("motor_move", &messageMotorMove);

void messageEngineMove( const rosserial_arduino::EngineMove& msg) {  
  nh.logdebug("Engine Move");

  int duty = floor((1023/100) * msg.power);
  
  analogWrite(wheel_1_power,duty);
  analogWrite(wheel_2_power,duty);
  if( msg.direction== 0 ) {
    digitalWrite(wheel_1_direction,LOW);
    digitalWrite(wheel_2_direction,LOW);
  } else if( msg.direction== 1 ) {
    digitalWrite(wheel_1_direction,HIGH);
    digitalWrite(wheel_2_direction,HIGH);
  }
}
ros::Subscriber<rosserial_arduino::EngineMove> engine_move("engine_move", &messageEngineMove);

rosserial_arduino::EngineEncoder engine_encoder;
ros::Publisher motor_encoder("motor_encoder", &engine_encoder);

void setup() {
  pinMode(wheel_1_power, OUTPUT);
  pinMode(wheel_1_direction, OUTPUT);
  pinMode(wheel_1_position, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(wheel_1_position), update_wheel_1_position, RISING);  
  pinMode(wheel_2_power, OUTPUT);
  pinMode(wheel_2_direction, OUTPUT);  
  pinMode(wheel_2_position, INPUT_PULLUP);  
  attachInterrupt(digitalPinToInterrupt(wheel_2_position), update_wheel_2_position, RISING);

  Serial.begin(115200);
  setupWiFi();
  delay(2000);
  
  nh.initNode();
  nh.subscribe(motor_move);
  nh.subscribe(engine_move);
  nh.advertise(motor_encoder);

  engine_encoder.encoder_1=0;
  engine_encoder.encoder_2=0;  
}

void loop() {
  nh.spinOnce();
  motor_encoder.publish(&engine_encoder);
  delay(30); 
}

void update_wheel_1_position() {
     engine_encoder.encoder_1=engine_encoder.encoder_1+1;
}

void update_wheel_2_position() {
    engine_encoder.encoder_2=engine_encoder.encoder_2+1;
}

void setupWiFi()
{
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

