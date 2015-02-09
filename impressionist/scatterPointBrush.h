//
// scatterPointBrush.h
//
// The header file for Point Brush. 
//

#ifndef SCATTERPOINTBRUSH_H
#define SCATTERPOINTBRUSH_H

#include "impBrush.h"

class ScatterPointBrush : public ImpBrush
{
public:
	ScatterPointBrush( ImpressionistDoc* pDoc = NULL, const char* name = NULL );

	void BrushBegin( const Point source, const Point target );
	void BrushMove( const Point source, const Point target );
	void BrushEnd( const Point source, const Point target );
	char* BrushName( void );
};

#endif
