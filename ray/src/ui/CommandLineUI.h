//
// CommandLineUI.h
//
// The header file for the command line UI
//

#ifndef __CommandLineUI_h__
#define __CommandLineUI_h__

#define DPI 72
#define E round(2.5*DPI)
#define mu (1/3.0)
#define separation(Z) round((1-mu*Z)*E/(2-mu*Z))
#define far separation(0)
#define maxX 512
#define maxY 512

#include "../vecmath/vec.h"
#include "../vecmath/mat.h"
#include "TraceUI.h"

// ***********************************************************
// from getopt.cpp
//#ifdef _WIN32
extern int getopt(int argc, char **argv, char *optstring);
//#else
extern int getopt(int argc, char* const* argv, char *optstring);
//#endif
extern char* optarg;
extern int optind, opterr, optopt;
// ***********************************************************

class CommandLineUI : public TraceUI {

public:
	CommandLineUI( int argc, char* const* argv );
	int		run();

	void		alert( const string& msg );
	void threadedRender(int index, int width, int height, int size);
	void threadedRenderSquare( int index, int xStart, int yStart, int xMax, int yMax, int size);
	void renderStereogram(int width, int height);

	bool m_enableSterogram;

private:
	void		usage();

	char*	rayName;
	char*	imgName;
	char*	progName;
};

#endif
