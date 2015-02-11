// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//


#include <FL/fl_ask.H>

#include <algorithm>
#include <math.h>
#include "time.h"
#include "stdlib.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "impBrush.h"

// Include individual brush headers here.
#include "pointBrush.h"
#include "lineBrush.h"
#include "circleBrush.h"
#include "triangleBrush.h"
#include "scatterPointBrush.h"
#include "scatterLineBrush.h"
#include "scatterCircleBrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_ucPreviewBackup = NULL;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS] = new PointBrush( this, "Points" );
	ImpBrush::c_pBrushes[BRUSH_LINES] = new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES] = new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_TRIANGLES] = new TriangleBrush( this, "Triangles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS] = new ScatterPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES] = new ScatterLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES] = new ScatterCircleBrush( this, "Scattered Lines" );

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];
	m_nCurrentStroke = STROKE_SLIDERS;

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Called by the UI when the user changes the stroke type.
// type: one of the defined stroke types.
//---------------------------------------------------------
void ImpressionistDoc::setStrokeType(int type)
{
	m_nCurrentStroke = type;
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Returns the weight of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getWeight()
{
	return m_pUI->getWeight();
}

//---------------------------------------------------------
// Returns the angle of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getAngle()
{
	return m_pUI->getAngle();
}

//---------------------------------------------------------
// Returns the alpha of the brush.
//---------------------------------------------------------
float ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}


//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	delete [] m_ucBitmap;
	delete [] m_ucPainting;
	delete [] m_ucPreviewBackup;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting		= new unsigned char [width*height*3];
	m_ucPreviewBackup	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}


//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::autoDraw() 
{
	srand(time(NULL));
	int x = 0, y = 0;
	Point source, target;
	int max_spacing = getSize()/2;

	m_pCurrentBrush->BrushBegin( source, target );
	for(x = 0; x < m_nPaintWidth; x = 1 + rand() % max_spacing){
		for(y = 0; y < m_nPaintHeight; y = 1 + rand() % max_spacing){
			source = Point(x,y);
			target = Point(x,y);
			m_pCurrentBrush->BrushMove( source, target );
		}
	}
	m_pUI->m_paintView->SaveCurrentContent();
	m_pUI->m_paintView->RestoreContent();
	m_pCurrentBrush->BrushEnd( source, target );
	m_pUI->m_paintView->flush();
}

// Apply the filter specified by filter_kernel to the 
// each pixel in the source buffer and place the resulting
// pixel in the destination buffer.  

// This is called from the UI when the
// "preview" or "apply" button is pressed in the filter dialog.



/*
 *	INPUT: 
 *		sourceBuffer:		the original image buffer, 
 *		srcBufferWidth:		the width of the image buffer
 *		srcBufferHeight:	the height of the image buffer
 *							the buffer is arranged such that 
 *							origImg[3*(row*srcBufferWidth+column)+0], 
 *							origImg[3*(row*srcBufferWidth+column)+1], 
 *							origImg[3*(row*srcBufferWidth+column)+2]
 *							are R, G, B values for pixel at (column, row).
 *		destBuffer:			the image buffer to put the resulting
 *							image in.  It is always the same size
 *							as the source buffer.
 *
 *      filterKernel:		the 2D filter kernel,
 *		knlWidth:			the width of the kernel
 *		knlHeight:			the height of the kernel
 *
 *		divisor, offset:	each pixel after filtering should be
 *							divided by divisor and then added by offset
 */
void ImpressionistDoc::applyFilter( const unsigned char* sourceBuffer,
		int srcBufferWidth, int srcBufferHeight,
		unsigned char* destBuffer,
		const double *filterKernel, 
		int knlWidth, int knlHeight, 
		double divisor, double offset )
{
	int r, c, rgb, flt_r, flt_c, row_offset, col_offset;
	double new_r_value, new_g_value, new_b_value;

	memcpy ( m_ucPreviewBackup, sourceBuffer,  srcBufferWidth*srcBufferHeight*3 );

	for(r = 0; r < srcBufferHeight; r++){
		for(c = 0; c < srcBufferWidth; c++){
			new_r_value = 0;
			new_g_value = 0;
			new_b_value = 0;
			for(flt_r = 0, row_offset = knlHeight/(-2); flt_r < knlHeight; flt_r++, row_offset++ ){
				for(flt_c = 0, col_offset = knlWidth/(-2); flt_c < knlWidth; flt_c++, col_offset++ ){
					new_r_value += getPixel(m_ucPreviewBackup, srcBufferWidth, srcBufferHeight,r + row_offset, c + col_offset, 0) * filterKernel[(flt_r)*knlWidth+(flt_c)];
					new_g_value += getPixel(m_ucPreviewBackup, srcBufferWidth, srcBufferHeight,r + row_offset, c + col_offset, 1) * filterKernel[(flt_r)*knlWidth+(flt_c)];
					new_b_value += getPixel(m_ucPreviewBackup, srcBufferWidth, srcBufferHeight,r + row_offset, c + col_offset, 2) * filterKernel[(flt_r)*knlWidth+(flt_c)];
				}
			}
			new_r_value = boundedColor(new_r_value / divisor + offset);
			new_g_value = boundedColor(new_g_value / divisor + offset);
			new_b_value = boundedColor(new_b_value / divisor + offset);
			destBuffer[3*(r*srcBufferWidth+c)+0] = new_r_value;
			destBuffer[3*(r*srcBufferWidth+c)+1] = new_g_value;
			destBuffer[3*(r*srcBufferWidth+c)+2] = new_b_value;
		}
	}

	m_pUI->m_paintView->flush();

}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

void ImpressionistDoc::SetFromMousePoints(const Point start, const Point end){
	int length = (int) sqrt(pow(start.x - end.x, 2)+pow(start.y - end.y, 2));
	int delta_y = (start.y - end.y);
	int delta_x = (start.x - end.x);
	int angle = (int) (atan2(delta_y, delta_x) * 180/ 3.14159265);
	if( angle < 0 )
		angle += 180;

	if(m_nCurrentStroke == STROKE_SLIDERS){
		m_pUI->setSize(length);
	}
	m_pUI->setAngle(angle);
}

unsigned char ImpressionistDoc::getPixel(const unsigned char* buffer, int bufferWidth, int bufferHeight, int row, int column, int rgbOffset){
	// if out of bounds return nothing
	if ( column < 0 ||  column >= bufferWidth || row < 0 || row >= bufferHeight) 
		return 0;

	return (buffer[3*(row*bufferWidth+column) + rgbOffset]);
}

int ImpressionistDoc::boundedColor(double color){
	if(color < 0)
		return 0;
	else if (color > 255)
		return 255;
	else
		return (int)color;
}
