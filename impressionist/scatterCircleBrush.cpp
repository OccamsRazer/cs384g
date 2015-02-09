//
// scatterCircleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatterCircleBrush.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"

extern float frand();

ScatterCircleBrush::ScatterCircleBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name)
{
}

void ScatterCircleBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	srand(time(NULL));
	BrushMove( source, target );
}

void ScatterCircleBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "ScatterCircleBrush::BrushMove  document is NULL\n" );
		return;
	}
	float angle;
	int size = pDoc->getSize();
	float alpha = pDoc->getAlpha();
	float radius = 0.5 * size;

	float x_val, y_val, min_x, min_y;
	min_x = (float)target.x - 0.5 * size;
	min_y = (float)target.y - 0.5 * size;

	int i;

	int numCircles =  4;

	for(i = 0; i <= numCircles; i++){
		x_val = min_x + rand() % size;
		y_val = min_y + rand() % size;
		glBegin( GL_TRIANGLE_FAN );
			SetColor( Point(x_val, y_val), alpha );
			glVertex2f(x_val, y_val);
			for (angle=0.0f; angle <= 2.0f * 3.14159265 + 0.2 ;angle+=0.2){
				glVertex2f(x_val+sin(angle)*radius, y_val+cos(angle)*radius);
			}
		glEnd();
	}


}

void ScatterCircleBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

