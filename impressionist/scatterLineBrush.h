//
// scatterLineBrush.h
//
// The header file for Point Brush. 
//

#ifndef SCATTERLINEBRUSH_H
#define SCATTERLINEBRUSH_H

#include "impBrush.h"

class ScatterLineBrush : public ImpBrush
{
public:
	ScatterLineBrush( ImpressionistDoc* pDoc = NULL, const char* name = NULL );

	void BrushBegin( const Point source, const Point target );
	void BrushMove( const Point source, const Point target );
	void BrushEnd( const Point source, const Point target );
	char* BrushName( void );
};

#endif
