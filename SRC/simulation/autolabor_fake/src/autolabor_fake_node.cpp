#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/TwistStamped.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>

#include <cmath>


float vx = 0.;
float vy = 0.;
float vth = 0.;
double max_x_accel;
double max_theta_accel;
double cmd_timeout;
ros::Time last_callback;

static float joint2_stat_last = 0.;

inline void getValidVelocity( float& v, const float tmp_v, const double max_delta_v )
{
  double sign = tmp_v > v ? 1.0 : -1.0;
  v = v + sign * max_delta_v;
}

void cmdCallback(geometry_msgs::Twist::ConstPtr cmd)
{
  static ros::Time last_time = ros::Time::now();
  ros::Time current_time = ros::Time::now();

  double dt = ( current_time - last_time ).toSec();

  float max_delta_vx = max_x_accel * dt;
  float max_delta_vth = max_theta_accel * dt;

  ROS_INFO("I heard twist message, vx: %lf, vy: %lf, vtheta: %lf", cmd->linear.x, cmd->linear.y, cmd->angular.z);

  float tmp_vx = cmd->linear.x;
  float tmp_vth = cmd->angular.z;

  float d_vx  = vx - tmp_vx;
  float d_vth = vth - tmp_vth;

  vy = 0.;

  //if ( fabs(d_vx) > max_delta_vx )
  //{
    //ROS_WARN("accelerat ax too large, reset velocity ");
    //getValidVelocity( vx, tmp_vx, max_delta_vx );
  //} else {
    vx = tmp_vx;
  //}

  //if ( fabs(d_vth) > max_delta_vth )
  //{
    //ROS_WARN("accelerat atheta too large, reset velocity ");
    //getValidVelocity( vth, tmp_vth, max_delta_vth );
  //} else {
    vth = tmp_vth;
  //}

  last_callback = last_time = current_time;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "autolabor_fake");

  ros::NodeHandle nh("autolabor");
  //ros::NodeHandle nh;
  ros::Publisher odom_pub = nh.advertise< nav_msgs::Odometry >("odom", 10);
  tf::TransformBroadcaster odom_broadcaster;
  ros::Subscriber cmd_sub = nh.subscribe< geometry_msgs::Twist >("cmd_vel", 10, cmdCallback);

  nh.param( "max_x_accel", max_x_accel, 1.0 );
  nh.param( "max_theta_accel", max_theta_accel, 1.0 );
  nh.param( "cmd_timeout", cmd_timeout, 0.2 );

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time    = ros::Time::now();

  float x = 0.;
  float y = 0.;
  float th = 0.;
  ros::Rate rate(10.0);
  while(nh.ok())
  {
    ros::spinOnce();
    current_time = ros::Time::now();

    if ( (current_time - last_callback).toSec() > cmd_timeout )
    {
      ROS_WARN( "subscriber timeout, set all velocity to Zero" );
      vx = 0.; vy = 0.; vth = 0.;
    }

    float dt       = (current_time - last_time).toSec();
    float delta_x  = vx*cos(th)*dt - vy*sin(th)*dt;
    float delta_y  = vx*sin(th)*dt + vy*cos(th)*dt;
    float delta_th = vth*dt;

    x += delta_x;
    y += delta_y;
    th += delta_th;

    // setuo tf frame
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;

    geometry_msgs::Quaternion quat = tf::createQuaternionMsgFromYaw(th);
    odom_trans.transform.rotation      = quat;

    // broadcast tf frame
    odom_broadcaster.sendTransform(odom_trans);

    //set up odom frame
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "base_link";

    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = quat;

    odom.child_frame_id = "odom";
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    // publish odom frame
    odom_pub.publish(odom);

    last_time = current_time;
    rate.sleep();
  }

}
