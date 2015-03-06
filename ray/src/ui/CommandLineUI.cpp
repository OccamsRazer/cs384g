#include <iostream>
#include <time.h>
#include <stdarg.h>

#include <assert.h>

#include "CommandLineUI.h"
#include "../fileio/bitmap.h"

#include "../RayTracer.h"

using namespace std;

// The command line UI simply parses out all the arguments off
// the command line and stores them locally.
CommandLineUI::CommandLineUI( int argc, char* const* argv )
	: TraceUI()
{
	int i;

	progName=argv[0];
	m_nAASamples = 1;
	m_enableAcceleration = true;
	m_nThreads = std::thread::hardware_concurrency();
	m_nLeafMax = 10;
	m_enableStereogram = false;
	while( (i = getopt( argc, argv, "t:r:w:h:s:a:l:m" )) != EOF )
	{
		switch( i )
		{
			case 'r':
				m_nDepth = atoi( optarg );
				break;

			case 'w':
				m_nSize = atoi( optarg );
				break;

			case 's':
				m_nAASamples = atoi( optarg );
				break;

			case 'a':
				m_enableAcceleration = atoi( optarg ) == 1;
				break;

			case 'l':
				m_nLeafMax = atoi( optarg );
				break;

			case 't':
				m_nThreads = atoi( optarg );
				break;

			case 'm':
				m_enableStereogram = true;
				break;

			default:
			// Oops; unknown argument
			std::cerr << "Invalid argument: '" << i << "'." << std::endl;
			usage();
			exit(1);
		}
	}

	if ( m_enableStereogram ) {
		m_nSize = maxX;
	}

	if( optind >= argc-1 )
	{
		std::cerr << "no input and/or output name." << std::endl;
		exit(1);
	}

	rayName = argv[optind];
	imgName = argv[optind+1];
}

int CommandLineUI::run()
{
	assert( raytracer != 0 );
	raytracer->loadScene( rayName );

	if( raytracer->sceneLoaded() )
	{
		int width = m_nSize;
		int height = (int)(width / raytracer->aspectRatio() + 0.5);

		raytracer->traceSetup( width, height );

		clock_t start, end;
		int numThreads = m_nThreads;
		int squareSize = height/numThreads/100;
		int chunkSize = 4;
		std::vector<thread> threads;
		start = clock();

		if (numThreads > 1){
			for (int i = 0; i < numThreads; i++){
				threads.push_back(thread(&CommandLineUI::threadedRender, this, i, width, height, chunkSize));
				// threadedRender(i, width, height, chunkSize);
			}
			for (auto& th : threads) { th.join(); }
			threads.clear();
		}
		else {
			for( int j = 0; j < height; ++j )
				for( int i = 0; i < width; ++i )
					raytracer->tracePixel(i,j);
		}

		end=clock();

		// save image
		unsigned char* buf;

		raytracer->getBuffer(buf, width, height);

		if (buf)
			writeBMP(imgName, width, height, buf);

		double t=(double)(end-start)/CLOCKS_PER_SEC;
		// int totalRays = TraceUI::resetCount();
		std::cout << "total time = " << t << std::endl;
        return 0;
	}
	else
	{
		std::cerr << "Unable to load ray file '" << rayName << "'" << std::endl;
		return( 1 );
	}
}

void CommandLineUI::renderStereogram(int width, int height){
	int x, y;
	for( y = 0; y < maxY; y++){
		int pix[maxX];
		int same[maxX];

		int s;
		int left, right;

		for ( x = 0; x < maxX; x++ )
			same[x] = x;

		for ( x = 0; x < maxX; x++){
			s = separation()
		}
	}
	
}

void CommandLineUI::threadedRender(int index, int width, int height, int size){
	int numThreads = m_nThreads;
	for (int y = 0; y < height; y+=size) {
		for (int x = index*size; x < width; x+=(size*numThreads)){
			threadedRenderSquare(index, x, y, width, height, size);
		}
	}
}

void CommandLineUI::threadedRenderSquare( int index, int xStart, int yStart, int xMax, int yMax, int size){
	for (int y = 0; y < size && yStart + y < yMax; y++) {
		for (int x = 0; x < size && xStart + x < xMax; x++){
			raytracer->tracePixel(xStart + x, yStart + y);
		}
	}
}

void CommandLineUI::alert( const string& msg )
{
	std::cerr << msg << std::endl;
}

void CommandLineUI::usage()
{
	std::cerr << "usage: " << progName << " [options] [input.ray output.bmp]" << std::endl;
	std::cerr << "  -r <#>      set recursion level (default " << m_nDepth << ")" << std::endl; 
	std::cerr << "  -w <#>      set output image width (default " << m_nSize << ")" << std::endl;
	std::cerr << "  -s <#>      set number of samples for anti-aliasing (default " << m_nAASamples << ")" << std::endl;
	std::cerr << "              Note: this value will be squared, ie -aa 3 = 9 samples" << std::endl;
	std::cerr << "  -a [0,1]    enable or disable kdtree acceleration (default " << m_enableAcceleration << ")" << std::endl;
	std::cerr << "  -a <#>      set maximum number of object in a leaf of the kdtree (default " << m_nLeafMax << ")" << std::endl;
	std::cerr << "  -t <#>      set the number of threads to use for rendering (default: number of hardware threads)" << std::endl;
	std::cerr << "  -m          render a stereogram instead of the image (default: false)" << std::endl;
	std::cerr << "              Note: this value will override several other flags" << std::endl;

}
