#!/usr/bin/env python
import numpy as np
from numpy import pi
from math import sin, cos
import rospy
import itertools
#from sensor_msgs.msg import LaserScan
#from std_msgs.msg import Float32MultiArray
#from geometry_msgs.msg import Twist
from my_msgs.msg import FloatArray_of

import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
#import pandas as pd

# ~~ Setting and Constants ~~
numReadings   = 192
threshold     =   9.0
preturn_thresh = 5.0
plotCarts     =   1

                            



# ~~ Variables ~~
"""
HA = [
357.6273,
353.8636,
350.1116,
346.3579,
342.5927,
338.8106,
335.0084,
331.1857,
327.3439,
323.48541,
319.61492,
315.73702,
311.85725,
307.98117,
304.1142,
300.261,
296.425,
292.6092,
288.814,
285.0379,
281.2774,
277.5254,
273.7716,
270,
267.6273,
263.8636,
260.1116,
256.3579,
252.5927,
248.8106,
245.0084,
241.1857,
237.3439,
233.48541,
229.61492,
225.73702,
221.85725,
217.98117,
214.1142,
210.261,
206.425,
202.6092,
198.814,
195.0379,
191.2774,
187.5254,
183.7716,
180,
177.6273,
173.8636,
170.1116,
166.3579,
162.5927,
158.8106,
155.0084,
151.1857,
147.3439,
143.48541,
139.61492,
135.73702,
131.85725,
127.98117,
124.1142,
120.261,
116.425,
112.6092,
108.814,
105.0379,
101.2774,
97.5254,
93.7716,
90,
87.6273,
83.8636,
80.1116,
76.3579,
72.5927,
68.8106,
65.0084,
61.1857,
57.3439,
53.48541,
49.61492,
45.73702,
41.85725,
37.98117,
34.1142,
30.261,
26.425,
22.6092,
18.814,
15.0379,
11.2774,
7.5254,
3.7716,
0,
]

314.26298,
318.03458,
321.78838,
325.54038,
329.30088,
333.07698,
336.87218,
340.68798,
344.52398,
348.37718,
352.24415,
356.12023,
"""

HA = [
0,
3.8779,
7.74839,
11.60688,
15.44868,
19.27138,
23.07358,
26.85568,
30.62088,
34.37458,
38.12658,
41.89028,
44.26298,
48.03458,
51.78838,
55.54038,
59.30088,
63.07698,
66.87218,
70.68798,
74.52398,
78.37718,
82.24415,
86.12023,
90,
93.8779,
97.74839,
101.60688,
105.44868,
109.27138,
113.07358,
116.85568,
120.62088,
124.37458,
128.12658,
131.89028,
134.26298,
138.03458,
141.78838,
145.54038,
149.30088,
153.07698,
156.87218,
160.68798,
164.52398,
168.37718,
172.24415,
176.12023,
180,
183.8779,
187.74839,
191.60688,
195.44868,
199.27138,
203.07358,
206.85568,
210.62088,
214.37458,
218.12658,
221.89028,
224.26298,
228.03458,
231.78838,
235.54038,
239.30088,
243.07698,
246.87218,
250.68798,
254.52398,
258.37718,
262.24415,
266.12023,
270,
273.8779,
277.74839,
281.60688,
285.44868,
289.27138,
293.07358,
296.85568,
300.62088,
304.37458,
308.12658,
311.89028, 
314.26298, 
318.03458,
321.78838,
325.54038,
329.30088,
333.07698,
336.87218,
340.68798,
344.52398,
348.37718,
352.24415,
356.12023
]


global lastOflow
global a_0, a_1, a_2, b_1, b_2
global forward_speed, yaw_rate
lastOflow_x = [ 0 for i in range(96)]
row1 = [ 0 for i in range(96)]
row2 = [ 0 for i in range(96)]
row3 = [ 0 for i in range(96)]
row4 = [ 0 for i in range(96)]
#lastOflow_y = [ 0 for i in range( 96) ]

car_state = ''
len_hist = 500

def oflow_cb( msg ):
    global lastOflow
    """ Process the laser scan message """
     	
    for g in range(0,96):
      lastOflow_x[g] = msg.data[g]


def oflow_row1(msg1):
	
	for m in range(0,96):
	   row1[m] = msg1.data[m]

def oflow_row2(msg2):
	
	for n in range(0,96):
	   row2[n] = msg2.data[n]

def oflow_row3(msg3):
	
	for o in range(0,96):
	   row3[o] = msg3.data[o]

def oflow_row4(msg4):
	
	for p in range(0,96):
	   row4[p] = msg4.data[p]
   
 	

minAng = 0
maxAng = 2*pi
rospy.init_node( 'scan_plot' , anonymous = True )

# rospy.Subscriber( "wfi/horiz/image_scan" , Float32MultiArray , image_scan_cb )
#pub = rospy.Publisher('tanoflow_plotted',FloatArray_of, queue_size=10)
rospy.Subscriber( "/tang_oflow_all_averaged", FloatArray_of , oflow_cb )
rospy.Subscriber( "/row1", FloatArray_of , oflow_row1 )
rospy.Subscriber( "/row2", FloatArray_of , oflow_row2 )
rospy.Subscriber( "/row3", FloatArray_of , oflow_row3 )
rospy.Subscriber( "/row4", FloatArray_of , oflow_row4 )

try:
    while ( not rospy.is_shutdown() ):

        lastOflowNP_x = np.array( lastOflow_x )
	row1_x = np.array( row1 )
	row2_x = np.array( row2 )
	row3_x = np.array( row3 )
	row4_x = np.array( row4 )

	'''
        indices = list(range(0,12))
        #print(indices)
        indices1 = list(range(12,96))
	#print indices1
	last = np.take(lastOflowNP_x, indices)
        
        first = np.take(lastOflowNP_x, indices1)
        overall = np.concatenate((first, last), axis=None)
	plotoverall = FloatArray_of()
	plotoverall.data = overall
	pub.publish(plotoverall)
	'''
	#lastOflowNP_y = np.asarray( lastOflow_y )
	#print(lastOflow_x)
	plt.cla()
        plt.clf() # Clear all figures
	
	
        plt.figure(1)
        plt.plot(HA ,lastOflowNP_x ,'b-',HA, row1 ,'r-', HA, row2 ,'g-', HA, row3 ,'m-',HA, row4 ,'y-')
	
	blue_patch = mpatches.Patch(color='blue', label='Average')
	red_patch = mpatches.Patch(color='red', label='Row1')
	green_patch = mpatches.Patch(color='green', label='Row2')
	mediumvioletred_patch = mpatches.Patch(color='mediumvioletred', label='Row3')
	yellow_patch = mpatches.Patch(color='olive', label='Row4')

	plt.legend(handles=[blue_patch, red_patch, green_patch, mediumvioletred_patch, yellow_patch], fontsize='xx-small')
	
	#plt.scatter(  HA ,lastOflowNP_x , color = 'blue', marker = ".")
        plt.hold( False )
	plt.grid()
        plt.xlim( [ 0 , 360] )
        plt.ylim( [ -200 , 200] )
        plt.xlabel("Viewing Angle [degrees]")
	plt.ylabel("Magnitude")
        plt.title("Tangential Optic Flow ")

	'''
	fig, ax = plt.subplots()
        ax.plot(HA ,lastOflowNP_x ,'b-', label = "Average")
	ax.plot(HA, row1 ,'r-', label = "Row1")
	ax.plot(HA, row2 ,'g-', label = "Row2")
	ax.plot(HA, row3 ,'m-', label = "Row3")
	ax.plot(HA, row4 ,'y-', label = "Row4")
	#plt.scatter(  HA ,lastOflowNP_x , color = 'blue', marker = ".")
        plt.hold( False )
	plt.grid()
        plt.xlim( [ 0 , 360] )
        plt.ylim( [ -200 , 200] )
        plt.xlabel("Viewing Angle [degrees]")
	plt.ylabel("Magnitude")
        plt.title("Tangential Optic Flow ")
	ax.legend()
	'''
        plt.pause( 0.01 )
    plt.show()
except KeyboardInterrupt:
	pass
