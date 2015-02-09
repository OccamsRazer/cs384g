//
// scatterLineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatterLineBrush.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"

extern float frand();

ScatterLineBrush::ScatterLineBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name)
{
}

void ScatterLineBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	srand(time(NULL));
	BrushMove( source, target );
}

void ScatterLineBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "ScatterLineBrush::BrushMove  document is NULL\n" );
		return;
	}
	float angle = pDoc->getAngle() * 3.14159265 / 180.0;
	float width = pDoc->getWeight()*1.0;
	int length = pDoc->getSize();

	float x_val, y_val, min_x, min_y;
	min_x = (float)target.x - 0.5 * length;
	min_y = (float)target.y - 0.5 * length;
	float x_offset = (0.5*length) * cos(angle);
	float y_offset = (0.5*length) * sin(angle);

	int i;

	int numLines =  4;
	glLineWidth(width);
	glBegin( GL_LINES );

		for(i = 0; i <= numLines; i++){
			x_val = min_x + rand() % length;
			y_val = min_y + rand() % length;
			SetColor( Point(x_val, y_val) );

			glVertex2f(x_val - x_offset, y_val - y_offset);
			glVertex2f(x_val + x_offset, y_val + y_offset);
		}


	glEnd();
}

void ScatterLineBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

