//
// lineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "lineBrush.h"
#include "math.h"

extern float frand();

LineBrush::LineBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name)
{
}

void LineBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize( (float)size );

	BrushMove( source, target );
}

void LineBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "LineBrush::BrushMove  document is NULL\n" );
		return;
	}
	float angle;
	int radius = pDoc->getSize();

	glBegin(GL_TRIANGLE_FAN);
		SetColor( source );
		glVertex2f(target.x,target.y);
		 
		for (angle=0.0f; angle <= 2.0f * 3.14159265 + 0.2 ;angle+=0.2){
			glVertex2f(target.x+sin(angle)*radius, target.y+cos(angle)*radius);
		}
	 
	glEnd();
}

void LineBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

