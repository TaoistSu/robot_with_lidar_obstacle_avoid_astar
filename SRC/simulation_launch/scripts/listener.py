#!/usr/bin/env python
import rospy
from sensor_msgs.msg import LaserScan
##your/workspace/src/packagename/scripts/listener.py
def callback(scan):
    #LaserScan
    #std_msgs/Header header
    #float32 angle_min
    #float32 angle_max
    #float32 angle_increment
    #float32 time_increment
    #float32 scan_time
    #float32 range_min
    #float32 range_max
    #float32[] ranges
    #float32[] intensities
    #rospy.loginfo('header: {0}'.format(scan))
    data = scan.ranges
    print(min(data))
 
 
 
def listener():
 
    rospy.init_node('lasr_listener', anonymous=False)
    rospy.Subscriber('scan', LaserScan,callback)
    rospy.spin()
 
if __name__ == '__main__':
    listener()
