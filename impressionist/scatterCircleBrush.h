//
// scatterCircleBrush.h
//
// The header file for Point Brush. 
//

#ifndef SCATTERCIRCLEBRUSH_H
#define SCATTERCIRCLEBRUSH_H

#include "impBrush.h"

class ScatterCircleBrush : public ImpBrush
{
public:
	ScatterCircleBrush( ImpressionistDoc* pDoc = NULL, const char* name = NULL );

	void BrushBegin( const Point source, const Point target );
	void BrushMove( const Point source, const Point target );
	void BrushEnd( const Point source, const Point target );
    void AutoBrush(int width, int height);
	char* BrushName( void );
};

#endif
