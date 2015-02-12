//
// pointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "pointBrush.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"

extern float frand();

PointBrush::PointBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name)
{
}

void PointBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize( (float)size );

	BrushMove( source, target );
}

void PointBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}
	float alpha = pDoc->getAlpha();

	SetColor( source, alpha );
	glBegin( GL_POINTS );

		glVertex2d( target.x, target.y );

	glEnd();
}

void PointBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

void PointBrush::AutoBrush(int width, int height ){
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	srand(time(NULL));
	int i, x_pos, y_pos;
	int size = pDoc->getSize();
	glPointSize( (float)size );
	glBegin( GL_POINTS );
		for(i = 0; i <= (width * height); i++){
			x_pos = rand() % width;
			y_pos = rand() % height;
			SetColor( Point(x_pos, y_pos), 1.0f );
			glVertex2d( x_pos, y_pos );
		}
	glEnd();
}

