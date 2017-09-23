#include <ESP8266WiFi.h>
#include <ros.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Range.h>

#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include "Robot.h"
#include "Ultrasonic.h"

//Necessary for the timer function
extern "C"
{
  #include "user_interface.h"
}

#define DEBUG 1


//Network Configuration
const char* ssid = "***";
const char* password = "***";

IPAddress server(192, 168, 1, 40); // ip of your ROS server

const uint16_t serverPort = 11411;

#define STOP_TIME_OUT 4 
int stop_time_out_count = 0;

//Timer configuration
os_timer_t a_timer;
int timer_period = 250;       

//Robot
Robot robot;
//Ros node handler
ros::NodeHandle nh;

nav_msgs::Odometry odom_nav_msg;              
ros::Publisher odom_pub("/car/odom", &odom_nav_msg); 

sensor_msgs::Range ultrasonic_msg;   
ros::Publisher pub_ultrasonic("/car/ultrasound", &ultrasonic_msg);      

tf::TransformBroadcaster broadcaster;
geometry_msgs::TransformStamped odom_trans;
geometry_msgs::TransformStamped ultrasonic_trans;   
geometry_msgs::Twist odom_geometry_msg;

ros::Time current_time = nh.now();
ros::Time last_time = current_time;

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

//TODO THIS CAN BE MOVE TO ENCODER CLASS
/**
 *
 */
void timer_callback(void *pArg) {
  robot.updateControlLoopLowLevel();
}

/**
 *
 */
void cmd_velCallback( const geometry_msgs::Twist& CVel) {
  nh.logdebug("cmd_velCallback");
  stop_time_out_count=0;   
  robot.move(CVel.linear.x,CVel.angular.z);  
}
ros::Subscriber<geometry_msgs::Twist> cmd_vel_sub("/car/cmd_vel", &cmd_velCallback );

/**
 *
 */
void setup() {
  //Setup Serial line.
  Serial.begin(115200);

  //Setup Wifi
  setupWiFi();
  delay(2000);

  //Configure timers 
  os_timer_setfn(&a_timer, timer_callback, NULL); 
  os_timer_arm(&a_timer, timer_period, true);   // timer in ms

  //Attact the Interruptions 
  attachInterrupt(digitalPinToInterrupt(14), update_wheel_1_position, RISING);        
  attachInterrupt(digitalPinToInterrupt(12), update_wheel_2_position, RISING);        
  
  //Init rose node & subscribe
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();

  //Init TF broad caster.
  broadcaster.init(nh);
  nh.subscribe(cmd_vel_sub);
  //nh.subscribe(configure_pid);  
  nh.advertise(odom_pub);
  nh.advertise(pub_ultrasonic);

  //Configure ultrasonic
  ultrasonic_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  ultrasonic_msg.header.frame_id = "/ultrasound";   
  ultrasonic_msg.field_of_view = 0.1;
  ultrasonic_msg.min_range = 0.0;
  ultrasonic_msg.max_range = 20;
}

void loop() {
  //if (nh.connected()) {

    robot.updateDistance();

    current_time = nh.now();

	  //BEGIN Testing Broad Caster
    double dt = current_time.toSec() - last_time.toSec();
    
    robot.updateControlLoopHighLevel(dt);

    //TODO REVIEW THIS THE ANGLE IS NOT CORRECT
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionFromYaw(robot.getTheta());

    
    // tf odom->base_link
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "/odom";
    odom_trans.child_frame_id = "/base_link";
  
    odom_trans.transform.translation.x = robot.getX();
    odom_trans.transform.translation.y = robot.getY();
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;
  
    broadcaster.sendTransform(odom_trans);
    
	  //END Testing Broad Caster

	  //BEGIN odometry
    
    odom_nav_msg.header.stamp = current_time;
    odom_nav_msg.header.frame_id = "/odom";

    //set the position
    odom_nav_msg.pose.pose.position.x = robot.getX();
    odom_nav_msg.pose.pose.position.y = robot.getY();
    odom_nav_msg.pose.pose.position.z = 0.0;
    odom_nav_msg.pose.pose.orientation = odom_quat;

    //set the velocity
    odom_nav_msg.child_frame_id = "/base_link";
    odom_nav_msg.twist.twist.linear.x = robot.getVx();
    odom_nav_msg.twist.twist.linear.y = robot.getVy();
    odom_nav_msg.twist.twist.angular.z = robot.getVtheta();

    odom_pub.publish(&odom_nav_msg);
    //END odometry  


    //BEGIN Ultrasonic
      ultrasonic_trans.header.frame_id = "/base_link";
    ultrasonic_trans.child_frame_id = "/ultrasound";
    ultrasonic_trans.transform.translation.x = 0.0; 
    ultrasonic_trans.transform.translation.y = 0.0; 
    ultrasonic_trans.transform.translation.z = 0.0;
    ultrasonic_trans.transform.rotation = tf::createQuaternionFromYaw(0.0); //TODO INCLUDE ORIENTATION + RELATIVE POSITION SENSOR.
    ultrasonic_trans.header.stamp = current_time;
    broadcaster.sendTransform(ultrasonic_trans);

    ultrasonic_msg.range = robot.getDistance();
    ultrasonic_msg.header.stamp = current_time;
    pub_ultrasonic.publish(&ultrasonic_msg);    
	  //END Ultrasonic
    
    last_time = current_time;

    nh.spinOnce(); 
    //Delay
    delay(250);  

    if (stop_time_out_count++ == STOP_TIME_OUT ) 
    {
      stop_time_out_count=0;
      robot.stop();           
    }
    
  //} 
 }

void update_wheel_1_position() { 
  robot.updateEncoder(0);
}

void update_wheel_2_position() {
  robot.updateEncoder(1);
}
