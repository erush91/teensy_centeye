#!/usr/bin/python
# # -*- coding: utf-8 -*-

# - * -coding: utf - 8 - * -
import numpy as np
from numpy import pi
from math import sin, cos
import rospy
import itertools
# from sensor_msgs.msg import LaserScan
#from teensy_centeye.msg import Float32MultiArray
# from geometry_msgs.msg import Twist
#from teensy_centeye.msg import FloatArray_of
from teensy_centeye.msg import *
import matplotlib
#matplotlib.use('TkAgg')
import matplotlib.pyplot as plt

import matplotlib.patches as mpatches

import message_filters
# import pandas as pd

global col_size
col_size = 0

global HA
HA = []

global HA_LK
HA_LK = [0,
        18,
        36,
        54,
        72,
        90,
        108,
        126,
        144,
        162,
        180,
        198,
        216,
        234,
        252,
        270,
        288,
        306,
        324,
        342,
        ]

global HA_HT
HA_HT = [0,
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
        356.12023,
        ]

global tanof_avg_NP, row1_NP, row2_NP, row3_NP, row4_NP
tanof_avg_NP = []
row1_NP = []
row2_NP = []
row3_NP = []
row4_NP = []

def callback(tan_avg_of, row1_of, row2_of, row3_of, row4_of):

    # PROCESS DATA
    global tanof_avg_NP
    global row1_NP
    global row2_NP
    global row3_NP
    global row4_NP

    tanof_avg_NP = np.asarray(tan_avg_of.data)
    row1_NP = np.asarray(row1_of.data)
    row2_NP = np.asarray(row2_of.data)
    row3_NP = np.asarray(row3_of.data)
    row4_NP = np.asarray(row4_of.data)

    global col_size
    col_size = np.size(tanof_avg_NP)
       
    # print('tan avgsize: '+ str(len(tanof_avg_NP)))
    # #print('npsize'+ str(tanof_avg_NP.size))
    # print('r1: '+ str(len(row1_NP)))
    # print('r2: '+ str(len(row2_NP)))
    # print('r3: '+ str(len(row3_NP)))
    # print('r4: '+ str(len(row4_NP)))

rospy.init_node('LK_HT_4X_vis', anonymous=True)
oflow_cb = message_filters.Subscriber('/tang_oflow_all_averaged', FloatArray_of)
oflow_row1 = message_filters.Subscriber('/row1', FloatArray_of)
oflow_row2 = message_filters.Subscriber('/row2', FloatArray_of)
oflow_row3 = message_filters.Subscriber('/row3', FloatArray_of)
oflow_row4 = message_filters.Subscriber('/row4', FloatArray_of)
    
sync_oflow = message_filters.ApproximateTimeSynchronizer([oflow_cb, oflow_row1, oflow_row2, oflow_row3, oflow_row4], queue_size=10, slop=0.1)
sync_oflow.registerCallback(callback)
    
try:
    while( not rospy.is_shutdown() ):
                    
        # PLOT DATA

        if col_size == 0:
            flag_plot_data = 0
        if col_size == 20:
            flag_plot_data = 1
            HA = HA_LK
        elif col_size == 96:
            flag_plot_data = 1
            HA = HA_HT
       
        for i in range(len(HA)):
            if (HA[i] > 180):  
                HA[i] -= 360;  

        if flag_plot_data == 1:
                
            plt.cla()
            plt.clf()
            #To run just the optic flow avg with gamma ( comment lines 256-264 and uncomment 254)
            plt.plot(HA, tanof_avg_NP, 'b.')
            
            #To run optic flow avg, row 1,2,3,4 with gamma [line(256-264)]
            #plt.plot(HA, tanof_avg_NP ,'b-', HA, row1_NP,'r-', HA , row2_NP,'g-', HA , row3_NP,'m-', HA , row4_NP ,'y-')

            #blue_patch = mpatches.Patch(color='blue', label='Average')
            #red_patch = mpatches.Patch(color='red', label='Row1')
            #green_patch = mpatches.Patch(color='green', label='Row2')
            #mediumvioletred_patch = mpatches.Patch(color='mediumvioletred',label='Row3')
            #yellow_patch = mpatches.Patch(color='olive', label='Row4')
            #plt.legend(handles=[blue_patch, red_patch, green_patch,mediumvioletred_patch, yellow_patch], fontsize='xx-small')
            
            #plt.hold(False)
            plt.grid()
            plt.xlim([-180, 180])

        if col_size == 20:
            plt.ylim([-0.25, 0.25])
        elif col_size == 96:
            plt.ylim([-150, 150])
            
        # plt.ylim([-100, 100])
        plt.draw()
        plt.xlabel('Viewing Angle [degrees]')
        plt.ylabel('Magnitude')
        plt.title('Tangential Optic Flow ')
        plt.pause(0.05)

    plt.show()

except KeyboardInterrupt:
	pass
