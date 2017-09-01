#include <ESP8266WiFi.h>
#include <ros.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>

#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <rosserial_arduino/Pid.h>
#include "Robot.h"

//Necessary for the timer function
extern "C"
{
  #include "user_interface.h"
}

#define DEBUG 1


//Network Configuration
const char* ssid = "MOVISTAR_D659";
const char* password = "gQK9NJ6amoPbTMfcqz67";
IPAddress server(192, 168, 1, 40); 
const uint16_t serverPort = 11411;


//Timer configuration
os_timer_t a_timer;
int timer_period = 250;       

//Robot
Robot robot;
//Ros node handler
ros::NodeHandle nh;

nav_msgs::Odometry odom_nav_msg;              
ros::Publisher odom_pub("/car/odom", &odom_nav_msg); 
tf::TransformBroadcaster odom_broadcaster;
geometry_msgs::TransformStamped odom_trans;



geometry_msgs::Twist odom_geometry_msg;       

//Initial values, for testing.
double x = 1.0;
double y = 0.0;
double theta = 0.0;
double DistancePerCount = (TWO_PI * 0.035) / 20; 
double WheelSeparation= 0.135;   
double wheel_distance_last_1=0;
double wheel_distance_last_2=0;


ros::Time current_time = nh.now();
ros::Time last_time = current_time;

/*
void messagePidCondigure( const rosserial_arduino::Pid& msg) {  
  nh.logdebug("Pid Configure");  
  robot.configurePid(msg.Kp,msg.Ki,msg.Kd);
  
}
ros::Subscriber<rosserial_arduino::Pid> configure_pid("configure_pid", &messagePidCondigure);
*/

void cmd_velCallback( const geometry_msgs::Twist& CVel) {
  nh.logdebug("cmd_velCallback");   

  double velocity_x = CVel.linear.x;
  double velocity_theta = CVel.angular.z;

  double velocity_1  = 0;
  double velocity_2  = 0;
   
  if(velocity_x == 0){  
    velocity_1 = velocity_theta * WheelSeparation / 2.0;
    velocity_2 = (-1) * velocity_1;
  } else if(velocity_theta == 0) { 
    velocity_1 = velocity_x;
    velocity_2 = velocity_x;
  } else{ 
    velocity_1  = velocity_x - velocity_theta * WheelSeparation / 2.0;
    velocity_2  = velocity_x + velocity_theta * WheelSeparation / 2.0;
  }

  Serial.print("velocity1:");
  Serial.println(velocity_1);
  Serial.print("velocity2:");
  Serial.println(velocity_2);
  
  robot.move(velocity_1,velocity_2);  
}
ros::Subscriber<geometry_msgs::Twist> cmd_vel_sub("/car/cmd_vel", &cmd_velCallback );


void timer_callback(void *pArg) {
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
  os_timer_setfn(&a_timer, timer_callback, NULL); 
  os_timer_arm(&a_timer, timer_period, true);   // timer in ms
     
  //Setup Robot
  robot.setup();
  //Attact the Interruptions 
  attachInterrupt(digitalPinToInterrupt(14), update_wheel_1_position, RISING);        
  attachInterrupt(digitalPinToInterrupt(12), update_wheel_2_position, RISING);        
  
  //Init rose node & subscribe
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();
  //Init TF broad caster.
  odom_broadcaster.init(nh);
  nh.subscribe(cmd_vel_sub);
  //nh.subscribe(configure_pid);  
  nh.advertise(odom_pub);
}

void loop() {
  //if (nh.connected()) {

    current_time = nh.now();

//---------------------- BEGIN Testing Broad Caster

    double dt = current_time.toSec() - last_time.toSec();
    double wheel_distance_1  = (robot.getPosition(0) - wheel_distance_last_1)*DistancePerCount;
    double wheel_distance_2  = (robot.getPosition(1) - wheel_distance_last_2)*DistancePerCount;
    double wheel_linear_velocity_1 = wheel_distance_1 / dt;
    double wheel_linear_velocity_2 = wheel_distance_2 / dt;

    double theta = (robot.getPosition(0) - robot.getPosition(1)) / 45. * PI;

    double vx = (wheel_distance_1 + wheel_distance_2) / 2.;                        
    double vy = 0;    
    double vtheta = theta / dt; 

    
    double delta_x = (vx * cos(theta)) * dt;
    double delta_y = (vx * sin(theta)) * dt;

    x += delta_x;
    y += delta_y;

    geometry_msgs::Quaternion odom_quat = tf::createQuaternionFromYaw(theta);
    
    // tf odom->base_link
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "/odom";
    odom_trans.child_frame_id = "/base_link";
  
    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;
    
  
    odom_broadcaster.sendTransform(odom_trans);


//---------------------- END Testing Broad Caster

//-----------------------BEGIN odometry

    odom_nav_msg.header.stamp = current_time;
    odom_nav_msg.header.frame_id = "/odom";

    //set the position
    odom_nav_msg.pose.pose.position.x = x;
    odom_nav_msg.pose.pose.position.y = y;
    odom_nav_msg.pose.pose.position.z = 0.0;
    odom_nav_msg.pose.pose.orientation = odom_quat;

    //set the velocity
    odom_nav_msg.child_frame_id = "/base_link";
    odom_nav_msg.twist.twist.linear.x = vx;
    odom_nav_msg.twist.twist.linear.y = vy;
    odom_nav_msg.twist.twist.angular.z = vtheta;

    odom_pub.publish(&odom_nav_msg);
//-----------------------END odometry  

    wheel_distance_last_1 = robot.getPosition(0) ;
    wheel_distance_last_2 = robot.getPosition(1) ;
    last_time = current_time;


    nh.spinOnce(); 
    //Delay
    delay(250);  
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

