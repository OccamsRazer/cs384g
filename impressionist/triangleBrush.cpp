//
// triangleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "triangleBrush.h"
#include "time.h"
#include "math.h"

extern float frand();

TriangleBrush::TriangleBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name)
{
}

void TriangleBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	BrushMove( source, target );
}

void TriangleBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "TriangleBrush::BrushMove  document is NULL\n" );
		return;
	}

	int size = pDoc->getSize();
	float alpha = pDoc->getAlpha();

	int Ax,Ay,Bx,By,Cx,Cy;
		
	Ax = target.x - (.5*size);
	Bx = target.x + (.5*size);
	Cx = target.x;
	Ay = target.y - (.5*size);
	By = target.y - (.5*size);
	Cy = target.y + (.5*size);

	SetColor( source, alpha );
	glBegin( GL_POLYGON );
		glVertex2i( Ax, Ay );
		glVertex2i( Bx, By );
		glVertex2i( Cx, Cy );
	glEnd();
}

void TriangleBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

void TriangleBrush::AutoBrush(int width, int height){
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	srand(time(NULL));
	float alpha = pDoc->getAlpha();
	int i, x_pos, y_pos;
	int Ax,Ay,Bx,By,Cx,Cy;
	int size = pDoc->getSize();

	for(i = 0; i <= (width * height); i++){
		x_pos = rand() % width;
		y_pos = rand() % height;

		Ax = x_pos - (.5*size);
		Bx = x_pos + (.5*size);
		Cx = x_pos;
		Ay = y_pos - (.5*size);
		By = y_pos - (.5*size);
		Cy = y_pos + (.5*size);

		SetColor( Point(x_pos, y_pos), alpha );

		glBegin( GL_POLYGON );
			glVertex2i( Ax, Ay );
			glVertex2i( Bx, By );
			glVertex2i( Cx, Cy );
		glEnd();
	}
}
