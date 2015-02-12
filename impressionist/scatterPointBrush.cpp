//
// scatterPointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatterPointBrush.h"
#include "stdlib.h"
#include "time.h"

extern float frand();

ScatterPointBrush::ScatterPointBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name)
{
}

void ScatterPointBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	srand(time(NULL));
	BrushMove( source, target );
}

void ScatterPointBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "ScatterPointBrush::BrushMove  document is NULL\n" );
		return;
	}
	int size = pDoc->getSize();
	float alpha = pDoc->getAlpha();

	float x_val, y_val, min_x, min_y;
	// max_x = 0.5 * size + target.x;
	// max_y = 0.5 * size + target.y;
	min_x = (float)target.x - 0.5 * size;
	min_y = (float)target.y - 0.5 * size;

	int i;

	glPointSize( 1.0);

	for(i = 0; i <= (size * size)*0.5; i++){
		x_val = min_x + rand() % size;
		y_val = min_y + rand() % size;
		SetColor( Point(x_val, y_val), alpha );
		glBegin( GL_POINTS );
			glVertex2d(x_val, y_val);
		glEnd();
	}


}

void ScatterPointBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

void ScatterPointBrush::AutoBrush(int width, int height){
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	srand(time(NULL));
	int i, j, x_pos, y_pos;
	int size = pDoc->getSize();
	float x_val, y_val, min_x, min_y;
	float alpha = pDoc->getAlpha();

	glPointSize( 1.0f );
	glBegin( GL_POINTS );
		for(i = 0; i <= (width * height)*0.9; i++){
			x_pos = rand() % width;
			y_pos = rand() % height;

			SetColor( Point(x_pos, y_pos), alpha );
			glVertex2d(x_pos, y_pos);
		}
	glEnd();
}
