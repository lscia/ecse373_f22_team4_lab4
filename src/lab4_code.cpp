#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Float32.h>
#include <algorithm>
#include <vector>
#include <sstream>

geometry_msgs::Twist des_vel;
std::vector<float> ranges;
void desvelCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
  des_vel.linear = msg->linear;
  des_vel.angular = msg->angular;
  //ROS_INFO("I heard: [%s]", msg->data.c_str());
}
void lidarCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
  ranges.clear();
  int num_r = scan->ranges.size();
  for(int i = 0; i<num_r; i++){
  	ranges.push_back(float(scan->ranges[i]));
  	}
  //ROS_INFO("I heard: [%s]", msg->data.c_str());
  
}

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "lab4");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
  ros::Subscriber des_vel_sub = n.subscribe("des_vel", 1000, desvelCallback);
  ros::Subscriber lidar_sub = n.subscribe("laser_1", 1000, lidarCallback);
  ros::Publisher cmd_vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

  ros::Rate loop_rate(10);
  
  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  int count = 0;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
     
     /**
     *  
     	Helpful answers for writing Twist messages
     	https://stackoverflow.com/questions/43515772/subscribing-and-publishing-geometry-twist-messages-from-turtlesim
     	https://answers.ros.org/question/259708/subscribing-and-publishing-geometrytwist-messages-from-turtlesim/
     
     	[geometry_msgs/Twist]:
     
	geometry_msgs/Vector3 linear
  		float64 x
 	 	float64 y
 	 	float64 z
	geometry_msgs/Vector3 angular
  		float64 x
 		float64 y
 		float64 z
     */
     
    ros::spinOnce();
    ros::spinOnce();
    
    
     
    geometry_msgs::Twist msg;
    
    //std::vector <float64> lin (3);
    //std::vector <float64> ang (3);
    
    /*
    msg.linear.x=double(1.0);
    msg.linear.y=double(1.0);
    msg.linear.z=double(1.0);
    msg.angular.x=double(1.0);
    msg.angular.y=double(1.0);
    msg.angular.z=double(1.0);
    */
    
    msg.linear=des_vel.linear;
    msg.angular=des_vel.angular;

	
    for (int i = 111; i <= 157; ++i) 
    {
  	
  	if(ranges.size() > 0){
  		if(ranges.at(i) <= 0.5)
  		{
  			msg.linear.x = std::min(msg.linear.x, 0.0);
  			break;
  		}
  	}
  	
    }
    
    /**
    std::stringstream ss;
    ss << "hello world " << count;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());
    */

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
     
    cmd_vel_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }	


  return 0;
}
