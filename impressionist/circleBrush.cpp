//
// circleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circleBrush.h"
#include "math.h"

extern float frand();

CircleBrush::CircleBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name)
{
}

void CircleBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize( (float)size );

	BrushMove( source, target );
}

void CircleBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "CircleBrush::BrushMove  document is NULL\n" );
		return;
	}
	float angle;
	float radius = pDoc->getSize() / 2.0;
	float alpha = pDoc->getAlpha();

	SetColor( source, alpha );
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(target.x,target.y);
		 
		for (angle=0.0f; angle <= 2.0f * 3.14159265 + 0.2 ;angle+=0.2){
		    glVertex2f(target.x+sin(angle)*radius, target.y+cos(angle)*radius);
		}
	 
	glEnd();
}

void CircleBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

void CircleBrush::AutoBrush(int width, int height){
	// do nothing so far		
}

