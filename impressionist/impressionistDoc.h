// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas

	void applyFilter( const unsigned char* sourceBuffer, //The filter kernel
		int srcBufferWidth, int srcBufferHeight,
		unsigned char* destBuffer,
		const double *filterKernel, 
		int knlWidth, int knlHeight, 
		double divisor, double offset );

	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setStrokeType(int type);			// called by the UI to set the brushType
	int		getSize();						// get the UI size
	int		getWeight();					// get the UI weight
	int		getAngle();						// get the UI angle
	float	getAlpha();						// get the UI alpha
	void	setSize(int size);				// set the UI size
	char*	getImageName();					// get the current image name
	

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;

	// Used by the filtering code so that we can
	// preview filters before applying.
	unsigned char*	m_ucPreviewBackup;



	// The current active brush.
	ImpBrush*			m_pCurrentBrush;
	int 				m_nCurrentStroke;	
	// Size of the brush.
	int m_nSize;							
	int m_nWeight;
	int m_nAngle;

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );
	// Sets the angle and length base on line start and end
	void SetFromRightClick( const Point start, const Point end);



private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
