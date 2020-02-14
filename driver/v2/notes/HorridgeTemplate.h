/*******************************************************************************
HorridgeTemplate.h : Horridge template and related EMD code

Created by Geoffrey Barrows.

History:
January 23, 2020: Created.
===============================================================================
Copyright (c) 2020, Centeye, Inc.
All rights reserved.

* This software is licensed for use only on products by Centeye.
* All modifications to this software shall be the property of Centeye.
* Use of this software is solely at your own risk and Centeye shall not be 
responsible for any damages, direct or indirect, sustained or caused while 
using this software.
* Centeye makes no warranty against patent infringement.
* Use of this software implies consent to the above terms.
* Any deviation from the above terms shall be allowable only through written 
authorization.
*******************************************************************************/

#ifndef	HORRIDGETEMPLATE_H
#define	HORRIDGETEMPLATE_H

#include <stdio.h>
#include "ARMInterface.h"
#include "Images.h"
#include "CommInterface.h"

/*******************************************************************************
DESCRIPTION OF HORRIDGE TEMPLATE METHOD

The Horridge template (HT) algorihtm is a simple EMD that is effectively a 
binarization of the classic Hassenstein-Reichardt algorithm. Our implementation
here is loosely based on a variation invented by Barrows and described in 
a patent application (available on request). The original algorithm was
conceived by Prof. Adrian Horridge at the Australian National University.
I am told he invented the algorithm as a joke, but like Ravel's Bolero jokes
sometimes become something else. Below are concepts:

Templates:

A "template" is a 2x2 array of bits that define the value of two adjacent 
binary pixels over two sequehtial time steps. For example, the template

	[A1 B1]
	[A2 B2]

defines the values of two binary pixels A and B over two sequential time steps
1 and 2. Thus there are 2^4=16 templates available. Each template has an
associated "charge" that is +1, 0, or -1 and indicates direction. The following
four templates are associated with motion in the +1 or right direction:

	00	01	10	11
	10	00	11	01

The following four templates are associated with -1 or left motion:

	00	01	10	11
	01	11	00	10

The following four templates are associated with no motion e.g. zero charge

	00	01	10	11
	00	01	10	11

And the remaining four templates are ambiguous since both bits flip thus they
are also associated with zero motion e.g. zero charge

	00	01	10	11
	11	10	01	00

We can also associate each of the 16 templates with an integer in 0...15 using
the following, which is used to generate lookup table T2:

	Template Number = 8*A1 + 4*B1 + 2*A2 + B2

To compute motion from a longer string of binary values, we extract all 
sequential templates and sum up their respective charges, for example:

	00110 = 00 01 11 10 =	-1  -1  +0  +0   =  -2
	01110   01 11 11 10

As you can see above, templates are sentivite to motion in just one direction.
Thus to meausre two-dimensional motion, we need to construct one set of 
templates in the horizontal direction to compute X optical flow, and another
set of templates in the vertical direction to compute Y optical flow. Thus
for X-direction optical flow, the pixels A and B are horizontally adjacent
while for Y-direction optical flow, the pixels A and B are vertically adjacent.

Binarization:

Pixel values in acquired images are generally integers, while templates require
binary values. Binarization is thus a method for generating an image of binary
values from an image of integers (or whatever). In Horridge's original work,
binarization was achieved by simply thresholding pixel intensities or their
first derivative. I have found a better method is to convolve the images with
edge-detection kernels that enhance texture parallel to a template's orientation
and then threshold the resulting values. For example, suppose matrix R defines
a raw image of pixel intensities. One possible kernel K2 could be:

	K2 = [-1 -1 +1 +1]
	     [-1 -1 +1 +1]
	     [-1 -1 +1 +1]
	     [-1 -1 +1 +1]
	     [-1 -1 +1 +1]

We refer to this kernal as K2 since it corresponds to feature kernel #2 as
defined in the code below (e.g. HT_CHANNEL_FEAT_2_PPNN). We then create
binarized image BIN2 using the method as expressed below in MATLAB:

	BIN2 = (conv2(R,K2) > threshold);

where the threshold is normally zero but may be another value. Rows of BIN2 may
then be used as input rows to X-direction templates. Regarding K2 above, note
that the basic kernel is [-1 -1 1 1] which is repeated over five rows. We define
the "radius" of a kernel as the number of lines above and below that the 
basic kernel is expanded. In the above case, the radius is 2 pixelss e.g. 
with the center rwo, two rows up, and two rows down, we have five rows total.

The above kernel K2 will only be useful for binarizing images to compute 
X-direction optical flow. To produce binarized images for computing Y-direction
optical flow, we can use the tranpose of K2 e.g.:

	K2' = [-1 -1 -1 -1 -1]
	      [-1 -1 -1 -1 -1]
	      [+1 +1 +1 +1 +1]
	      [+1 +1 +1 +1 +1]

Note that, stricly speaking, in our code implementation we perform a
"correlation" rather than a "convolution" and we do not convolve the
entire image. The effect is qualitatively the same.

Capacitors:

A "capacitor" is a construct that is used to generate optical flow measurements
from the output of a number of template detectors. You can think of a 
capacitor as a mechanism for spatially and temporally pooling EMD outputs. 
In our implementation, each capacitor takes as input the template EMD outputs
over a rectangular region of the field of view. Each capacitor has actually
two charges, one for X-direction optical flow and one for Y-direction optical 
flow. Each capacitor is "leaky" and thus has an update rate "alpha" that is 
effectively the inverse of the time constant of the capacitor. The two "charges"
of the capacitor define it's two-dimensional optical flow output.

In early implementations of this algorithm a capacitor was updated in one
direction by first decaying the charges by the alpha rate and then adding new
charge, e.g.

	charge := charge * (1 - alpha) + new_total_input_charge;

but in later implementations, including the one used in this code, we moved to 
a running average implementation, e.g.

	charge := (1 - alpha) * charge + alpha * (average_input_charge);

where average_input_charge is the average charge produced by all input EMDs.
The value average_input_charge and thus charge will thus be limited to a range
between +1 and -1.

The charge across a capacitor very roughly approximates an optical flow rate
according to the equation

	charge = Eff * OF + noise

where OF is the actual optical flow in pixels per frame and Eff is an
"efficiency" value between 0 and 1. An efficiency of 1.0 is an "ideal" value
that occurs when the raw image and the binarization kernel together produce
a binarized image where lines (rows or columns) parallel to the direction being
sensed results in the a binary pattern like 001100110011. Real-world 
efficiencies will be much less and can range between 0.05 and 0.3 in practice,
or over an even wider range.

Capacitors can be constructed to produce optical flow values that respond to
different types of optical flow stimulus. For example, large area capacitors
are good for sensing global optical flow values such as what might be useful
for wide field integration (WFI), while smaller area capacitors will be noisier
(our friend the central limit theorem...) but more adept at detecting quickly
moving narrow objects. You can also vary the update rate accordingly- lower
values of alpha produce smoother measurements but introduce lag, while higher
alpha values are noisier but have less lag. You will need to balance these
items out depending on the dynamics of your control system.

Now we can dive further into code contructs:

Images: In our code we organize images as one-dimensional arrays in which the
image is stored row-wise. For example, to store a 10x10 image, the first ten
entries are the first row of the image, the second ten entries the second row, 
and so on.

Binarization Channel: A "binarization channel" is simply one method of
generating a binarized image from a raw image. This code supports a total of
8 binarization channels (to allow 8 binary images to be stored in char-sized
arrays), which may be split arbitrarily among X and Y direction optical flows.
Four parameters define how binarization occurs: 1) whether it is X or Y,
2) the feature detection kernel used, 3) the radius of the feature detection 
kernel e.g. how far the kernel is expanded in lateral directions, and
4) the threshold value used for binarization. The first two of these are
stored in BCType[] while the other two parameters are stored in separate arrays.

Capacitors: The concept of a "capacitor" has already been defined above. Each
capacitor is defined by the boundaries of the input rectangle and it's
"capacitor type" (below).

Capacitor Type: A "capacitor type" is where the "alpha" and "1-alpha" values
are stored. This contruct allows whole banks of capacitors to have their
alpha values changed at once.

Function call sequence:

In the sensor firmware, the binarization channels, capacitors, and capacitor
types are initialized using first a call to member function Initialize()
and then subsequent calls to member functions SetupBinChannel(), SetupCap(),
and SetupCapType() as appropriate. The pointers to the image arrays (such as 
pImgSource) are also set to external image array space in memory.

In the sensor firmware, the following sequence of steps is performed every
frame cycle:

1) Acquire imagery from vision chip - This occurs outside the HorridgeTemplate
object.
2) MakeIntegralImages() - This constructs row-wise and column-wise integral
images that speed up the computation of computations in the next function.
3) ComputeBinaryImages() - This generates the binary images from the acquired
image, according to settings set up during initialization.
4) ComputeStimulations() - This function implements the template detectors, 
e.g. computes the +1, 0, and -1 stimulatios for each generated binary image.
The charges are stored in column-wise integral images to speed up the
computations of the next step.
5) StimCaps() - This function implements the capacitors, including summing
(or really averaging) total input stimulations as well as implementing the 
running average. The resulting arrays CapQX[] and CapQY[] form the resulting
optical flow measurements.
6) Store result in output buffers- This occurs outside the HorridgeTemplate
object and readies the resulting data for transmission to the host.

*******************************************************************************/

/*******************************************************************************
COMMUNICATING WITH THE SENSOR, HORRIDGE TEMPLATE SPECIFICS

NOTE: The information in this comment block is particular to the implementation
in the sensor code that uses this objet, not to this object itself. This is
for code provided January 2020. This commentary is included here for 
your convenience

This section assumes you are familiar with how to send commands to the sensor
as well as receive data from it.

COMMAND TO UPDATE HORRIDGE TEMPLATE ALGORITHM PARAMETERS

Command CMD = 46, Generic Horridge template parameter update

# Arguments: 1 unsigned integer, 4 bytes total, MSB sent first
For example, to send argument = decimal 10003 = 0x2713, send the four bytes
as follows for arguments: 0x00, 0x00, 0x27, 0x13

See the comments of fuction QuickUpdateParams() in HorridgeTemplate.cpp to
see how the single integer argument is constructed.

GETTING DATA FROM THE SENSOR

Dataset DSID = 11, Horridge Template

Number of bytes: 246

Format: 

Bytes 0...3: Cycle number as 32-bit integer
Byte 4: CapQX[0] e.g. capacitor 0, X-direction output, rescaled into a char
Byte 5: CapQY[0] e.g. capacitor 0, Y-direction output, rescaled into a char
Byte 6: CapQX[1] rescaled
Byte 7: CapQY[1] rescaled
Byte 8: CapQX[2] rescaled
...

Note that capacitor charges are floats and we rescale them into signed char
variables. The rescaling is performed using FFScalar as follows:

	int ival = (int)( Cap * 128.0 * FFScalar + 0.5);
	if (ival < -127) ival = -127;
	if (ival > 127) ival = 127;
	ByteVal = (unsigned char)(ival & 0x000000FF);

where "Cap" is the capacitor charge (e.g. CapQX[c] etc.) and ByteVal is the 
resulting byte. If the capacitor values are overall too small or clipped more
than tolerable, send a command to the sensor to change FFScalar.

*******************************************************************************/


/*******************************************************************************
INITIALIZATION OF BINARIZATION CHANNELS, CAPACITORS, AND CAPACITOR TYPES IN
SENSOR CODE

NOTE: The information in this comment block is particular to the implementation
in the sensor code that uses this objet, not to this object itself. This is
for code provided January 2020. This commentary is included here for your 
convenience

Below are the commands used in the overall sensor code to initialize the
Horridge template object.
	
	// HT Cap Types
	pHT->SetupCapType(0,0.2); // Cap type 0 (global window) alpha is 0.2
	pHT->SetupCapType(1,0.1); // Cap type 1 (big windows) alpha is 0.1
	pHT->SetupCapType(2,0.25); // Cap type 2 (tiny windows) alpha is 0.25
	// HT Binarization Channels and global capacitor	
	pHT->SetupBinChannel(0, HT_CHANNEL_IS_ROW|HT_CHANNEL_FEAT_2_PPNN, 4, 1, 55, 2, 4, 74, 0);
	pHT->SetupBinChannel(1, HT_CHANNEL_IS_COL|HT_CHANNEL_FEAT_2_PPNN, 4, 1, 71, 2, 4, 58, 0); 
	pHT->SetupBinChannel(2, HT_CHANNEL_IS_ROW|HT_CHANNEL_FEAT_5_PNNP, 4, 1, 55, 2, 4, 74, 0);
	pHT->SetupBinChannel(3, HT_CHANNEL_IS_COL|HT_CHANNEL_FEAT_5_PNNP, 4, 1, 71, 2, 4, 58, 0);
	pHT->SetupCap(0, 4, 59, 4, 74, 0);
	pHT->FFScalar = 2.5;

	// Windows 0 is the global window of the entire sensor // Gene
	// Windows 1 through 24 are Cap Type 1 and are set up to cover approximately the same region as the corresponding regular sensor measurement windows
	// Windows 1 through 20 are main 4x5 array
	w=1;
	for (r=0; r<4; ++r)
		for (c=0; c<5; ++c) {
			switch(r) {
			// 63x79 for RC30. center is row 32
			case 0: ar=10; break; 
			case 1: ar=20; break;  
			case 2: ar=30; break;  
			case 3: ar=40; break;  
			}
			switch(c) {
			// 63x79 for RC30. center is column 40
			case 0: ac=5;  break; 
			case 1: ac=19; break;  
			case 2: ac=33; break;  
			case 3: ac=47; break;  
			case 4: ac=61; break;  
			}
			pHT->SetupCap(w,ar+1,ar+12,ac,ac+14,1);
			w++;
		}
	// Windows 21 through 24 are top and bottom windows
	w=21;
	for (r=0; r<2; ++r)
		for (c=0; c<2; ++c) {			
			switch(r) {
			// 63x79: 
			case 0: ar=5; break; 
			case 1: ar=45; break;
			}
			switch(c) {
			// 63x79: 
			case 0: ac=24; break; 
			case 1: ac=42; break; 
			}
			pHT->SetupCap(w,ar+1,ar+12,ac,ac+14,1);
			w++;
		}	
	// Add smaller HT capacitors (type 2)
	w = 25; // start at window 25
	for (r=0; r<4; ++r)
		for (c=0; c<24; ++c) {
			ar = 10 + 10 * r;
			ac = 4 + 3 * c;
			pHT->SetupCap(w,ar,ar+9,ac,ac+2,2);
			w++;
		}

*******************************************************************************/


/*******************************************************************************
Constants
*******************************************************************************/
// DIMENSIONS 
// The number of pixels used is the same as the raw array size. There will be 
// pixel waste here and there at the edges but this simplifies indexing.
#define	HT_NUMPIXELS	(IM_CANVAS0_PIX)	// # pixels is same as main image array, nominally 63*79=4977
#define	HT_ROWS		(IM_CANVAS0_H)		// # rows is same as main image array, nominally 63
#define	HT_COLS		(IM_CANVAS0_W)		// # columns is same as main array, nominally 79
#define HT_SCRATCHSIZE	HT_COLS			// scratch size should be the max of #rows and #cols, here set to 79

// This code supports up to 8 binarization channels, which may be split
// any way between X (row) and Y (column) directions
#define	HT_MAX_BINCHANNELS	8

// These constants are used to define a binarization "type", which is 
// 1) whether the channel is in the X or Y direction (upper nibble), and
// 2) the feature type used to binarize a line image (in X or Y directions)
#define	HT_CHANNEL_IS_ROW		0x10	// binarization channel is a row-wise e.g. X direction
#define	HT_CHANNEL_IS_COL		0x20	// binarization channel is column-wise or Y direction
#define	HT_CHANNEL_FEAT_EXTRACTMASK	0x0F	// mask to extract features

// feature definitions. Note that lateral offset is int((size-1)/2) and is
// used to place the feature's input range approximately over the resulting
// binary value
#define	HT_CHANNEL_FEAT_1_PN		0x01	// binarization feature [+ -], offset = 0
#define	HT_CHANNEL_FEAT_2_PPNN   	0x02	// binarization feature is [+ + - -], offset = 1
#define	HT_CHANNEL_FEAT_3_PPPNNN	0x03	// binarization feature is [+ + + - - -], offset = 2
#define	HT_CHANNEL_FEAT_4_P2NP   	0x04	// binarization feature is [- 2+ -], offset = 1
#define	HT_CHANNEL_FEAT_5_PNNP		0x05	// binarization feature is [+ - - +], offset = 1
#define	HT_CHANNEL_FEAT_6_PP2N2NPP	0x06	// binarization feature is [+ + 2- 2- + +], offset = 2
#define	HT_CHANNEL_FEAT_7_P2N2PN	0x07	// binarization feature is [+ 2- 2+ -], offset = 1
#define	HT_CHANNEL_FEAT_8_PNNPPN	0x08	// binarization feature is [+ - - + + -], offset = 2
#define	HT_CHANNEL_FEAT_9_PP2N2N2P2PNN	0x09	// binarization feature is [+ + 2- 2- 2+ 2+ - -], offset = 3

#define	HT_MAX_CAP			130	// maximum number of capacitors. Default code uses 121 of the allotted 200

#define	HT_MAX_CAPTYPE			10	// maximum number of capacitor types

// dump flags- these are used for debugging in the IAR environment with
// text printing to UART. These flags below define bytes that indicate which
// arrays are dumped. These will not normally be used in regular operation
#define	HT_DUMP_SOURCE			0x01	// dump source image
#define	HT_DUMP_INT			0x02	// dump integral images
#define	HT_DUMP_BINNEW			0x04	// dump new binarizized image array
#define	HT_DUMP_BINOLD			0x08	// dump old binarizized image array
#define	HT_DUMP_STIMX			0x10	// dump X stimulation image
#define	HT_DUMP_STIMY			0x20	// dump Y stimulation image
#define	HT_DUMP_CAP			0x40	// dump capacitor charges
#define	HT_DUMP_BC			0x80	// dump binarization channel info


/*******************************************************************************
Macros
*******************************************************************************/


/******************************************************************************/
class HorridgeTemplate
{
public:	
	// constructor / destructor
	HorridgeTemplate(void) {Initialize();};
	~HorridgeTemplate(void) {};

public:
	// Basic initialization - Call once at beginning to initialize values
	void Initialize(void); // initialization
	
	// Setup functions - These are called during setup to define binarization channels, capacitors, and capaicgtor types.
	void SetupBinChannel(unsigned char channel, 
			     unsigned char channeltype,
			     unsigned char linestart,
			     unsigned char lineskip,
			     unsigned char linenum,
			     unsigned char radius,
			     unsigned char pixelstart,
			     unsigned char pixelstop,
			     int threshold);
	void SetupCap(short capnum,
		      unsigned char capr1,
		      unsigned char capr2,
		      unsigned char capc1,
		      unsigned char capc2,
		      char captype);
	void SetupCapType(short captypenum,
			  float alpha);
	
	// This is the main fuction that is used to update parameters, for example based on a command sent to the sensor.
	void QuickUpdateParams(unsigned int command);
	
	// Processing functions - these perform the computations for the algorithm
	void MakeIntegralImages(void); // Generate integral images from source image
	void ComputeBinaryImages(void); // Compute binary images from integral images
	void ComputeStimulations(void); // Compute stimulation integral images from binary features
	void StimCaps(void); // Compute capacitor charges e.g. outputs
	
	// Debugging functions - these were used in development
	void MakeSimulatedImage(short method, float dispx, float dispy); // makes a simulated raw image
	void DumpArrays(unsigned char flags); // dump array contents
	
	// class member variables
public:
	// Pointers to other classes and particular to this implementation. These here are only used to facilitate debugging.
	Images *pIMG;
	CommInterface *pComm;
	
	// Pointers to image arrays - Rather than store images in this object, we make use of memory space already allocated external to the HorridgeTemplate class. These need to be set elsewhere first before use.
	short		*pImgSource; // source image - the input image acquired with the vision chips shoud be placed here
	int		*pImgIntRow; // row-wise integral image
	int		*pImgIntCol; // column-wise integral image
	unsigned char	*pImgBinNew; // new binary image (8 images packed)
	unsigned char	*pImgBinOld; // old binary image (8 images packed), e.g. from previous frame
	short		*pImgStimIntX; // X-direction stimulation from template EMDs, encoded as column-wise integral images
	short		*pImgStimIntY; // Y-direction stimulation from template EMDs, encoded as column-wise integral images
	
	// Local scratch space
	int	Scratch[HT_SCRATCHSIZE];	// scratch space for lines

	// Parameters defining binarization channels (8 total)
	// BCActive defines, bit by bit, whether a particular binarization channel is active or in use
	unsigned char	BCActive;
	// BCType is channel binarization type. Upper nibble defines direction (row/col), lower nibble defines binarization feature type
	unsigned char	BCType[HT_MAX_BINCHANNELS];
	// Lines: A "line" is a horizontal row of computed feature signals when 
	// performing an X-direction binarization, or a vertical column of
	// coputed feature signals when peforming a Y-direction binarization.
	// BCLineStart is the starting line used to computed the binarziation channel
	unsigned char	BCLineStart[HT_MAX_BINCHANNELS];
	// BCLineSkip is the skip between lines use to compute the binarization channel. 1 means do every line
	unsigned char	BCLineSkip[HT_MAX_BINCHANNELS];
	// BCLineNum is the number of lines used in a binarization channel
	unsigned char	BCLineNum[HT_MAX_BINCHANNELS];
	// BCRadius is the "radius" of the feature kernel used
	unsigned char	BCRadius[HT_MAX_BINCHANNELS];
	// BCPixelStart is the first pixel within a line to compute. 
	unsigned char	BCPixelStart[HT_MAX_BINCHANNELS];
	// BCPixelStop is the last pixel within a line to compute
	unsigned char	BCPixelStop[HT_MAX_BINCHANNELS];
	// BCThreshold is the threshold used for binarization
	int		BCThreshold[HT_MAX_BINCHANNELS];
	
	// Parameters defining capacitors
	// CapNum is the number of defined capacitors and obviously should
	// never exceed HT_MAX_CAP
	short		CapNum; 
	// CapR1 and CapR2 define the first and last rows of a capacitor
	unsigned char	CapR1[HT_MAX_CAP];
	unsigned char	CapR2[HT_MAX_CAP];
	// CapC1 and CapC2 define the first and last columns of a capacitor
	unsigned char	CapC1[HT_MAX_CAP];
	unsigned char	CapC2[HT_MAX_CAP];
	// CapType defines the capacitor type. Negative means this capacitor is inactive
	char		CapType[HT_MAX_CAP];
	// CapInvArea is the inverse of the area. This is computed once during initialization to avoid computing divisions every cycle
	float		CapInvArea[HT_MAX_CAP];
	// CapQX, CapQY defines the capacitor charges in X and Y directions. These are the outputs.
	float		CapQX[HT_MAX_CAP];
	float		CapQY[HT_MAX_CAP];
	// FFScalar is fudge factor scalar for CapQX and CapQY. This "fudge factor" accounts for template efficiencies being much less than 1.0. Basically all resulting charges CapQX[] and CqpQY[] are multiplied by this (and clipped) before output.
	float		FFScalar;
	
	// Parameters defining capacitor types. For now this just stores "alpha" and "1-alpha" values
	// CTNum is number of capacitor types
	short	CTNum;
	// CTAlpha is the "alpha" leakage rate of the capacitor type
	float	CTAlpha[HT_MAX_CAPTYPE];
	// CTAlphaOM is the corresonding "one minus alpha"
	float	CTAlphaOM[HT_MAX_CAPTYPE];
	
	// Template stimulation arrays - For now only T2 is used.
	char T4[256]; // Array for four bits / three templates. This is not used here. I'm not sure why I left it in.
	char T2[16]; // Array for two bits / one template
};

#endif	// HORRIDGETEMPLATE_H

