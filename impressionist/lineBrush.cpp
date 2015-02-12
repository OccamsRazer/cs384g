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
#include "time.h"

extern float frand();

LineBrush::LineBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name)
{
}

void LineBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

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
	float alpha = pDoc->getAlpha();
	float angle = pDoc->getAngle() * 3.14159265 / 180.0;
	float width = pDoc->getWeight()*1.0;
	float length = pDoc->getSize()*1.0;
	float start_x, start_y, end_x, end_y;

	start_x = target.x - (0.5*length) * cos(angle);
	start_y = target.y - (0.5*length) * sin(angle);

	end_x = target.x + (0.5*length) * cos(angle);
	end_y = target.y + (0.5*length) * sin(angle);

	glLineWidth(width);
	SetColor( source, alpha );
	glBegin(GL_LINES);
		glVertex2f(start_x, start_y);
		glVertex2f(end_x, end_y);
	glEnd();
}

void LineBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

void LineBrush::AutoBrush(int width, int height){
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	srand(time(NULL));
	int i, x_pos, y_pos;

	float alpha = pDoc->getAlpha();
	float angle = pDoc->getAngle() * 3.14159265 / 180.0;
	float line_width = pDoc->getWeight()*1.0;
	float length = pDoc->getSize()*1.0;
	float start_x, start_y, end_x, end_y;

	float cos_angle = cos(angle);
	float sin_angle = sin(angle);

	glLineWidth(line_width);
	glBegin( GL_LINES );
		for(i = 0; i <= (width * height); i++){
			x_pos = rand() % width;
			y_pos = rand() % height;
			SetColor( Point(x_pos, y_pos), alpha );

			start_x = x_pos - (0.5*length) * cos_angle;
			start_y = y_pos - (0.5*length) * sin_angle;

			end_x = x_pos + (0.5*length) * cos_angle;
			end_y = y_pos + (0.5*length) * sin_angle;

			glVertex2f(start_x, start_y);
			glVertex2f(end_x, end_y);
		}
	glEnd();
}

