// The main ray tracer.

#pragma warning (disable: 4786)

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"

#include "parser/Tokenizer.h"
#include "parser/Parser.h"

#include "ui/TraceUI.h"
#include <cmath>
#include <algorithm>

extern TraceUI* traceUI;

#include <iostream>
#include <fstream>

using namespace std;

// Use this variable to decide if you want to print out
// debugging messages.  Gets set in the "trace single ray" mode
// in TraceGLWindow, for example.
bool debugMode = false;

// Trace a top-level ray through pixel(i,j), i.e. normalized window coordinates (x,y),
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.

Vec3d RayTracer::trace(double x, double y)
{
  // Clear out the ray cache in the scene for debugging purposes,
  if (TraceUI::m_debug) scene->intersectCache.clear();
  ray r(Vec3d(0,0,0), Vec3d(0,0,0), ray::VISIBILITY);
  scene->getCamera().rayThrough(x,y,r);
  Vec3d ret = traceRay(r, traceUI->getDepth());
  ret.clamp();
  return ret;
}

Vec3d RayTracer::tracePixel(int i, int j)
{
	Vec3d col(0,0,0);

	if( ! sceneLoaded() ) return col;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	int aaSamples = traceUI->getAASamples();

	if (aaSamples > 1) {
		double minX = (double)i - 0.5;
		double minY = (double)j - 0.5;
		double maxX = (double)i + 0.5;
		double maxY = (double)j + 0.5;

		double inc = 1.0/(aaSamples - 1);

		int r, c;
		for( r = 0; r < aaSamples; r++ ){
			x = (minX + r * inc)/double(buffer_width);
			for( c = 0; c < aaSamples; c++ ){
				y = (minY + c * inc)/double(buffer_height);
				col += trace (x, y);
			}
		}
		col /= (aaSamples*aaSamples);
	}
	else {
		col = trace( x, y);
	}

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;
	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
	return col;
}


// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
Vec3d RayTracer::traceRay(ray& r, int depth)
{
	isect i;
	Vec3d colorC;

	if(scene->intersect(r, i) ) {

		const Material& m = i.getMaterial();
		colorC = m.shade(scene, r, i);

		if ( depth < 1) return colorC;

		Vec3d Ci = ( (-1 * r.getDirection()) * i.N ) * i.N;
		Vec3d Si = Ci + r.getDirection();
		Vec3d reflectedDir = Ci + Si;
		reflectedDir.normalize();

		ray reflected(r.at(i.t), reflectedDir, ray::REFLECTION);

		Vec3d Ireflect = m.kr(i) % traceRay( reflected, depth - 1);

		colorC += Ireflect;

		double Thetai = i.N * r.getDirection();
		bool tir = 1.0 - m.index(i) * m.index(i) * ( 1.0 - Thetai * Thetai) < 0;
		bool entering = Thetai < 0.0;

		// refract iff kt is nonzero, ray is entering object, ray is exiting and not have tir
		if ( !m.kt(i).iszero() && ( entering || ( !entering && !tir ) ) ) {
			Vec3d refractedDir;
			if ( entering ) {
				Vec3d St = ( 1.0/m.index(i) ) * Si;
				double StdotSt = St * St;
				Vec3d Ct =  -1.0 * i.N * sqrt(1.0 - StdotSt);
				refractedDir = Ct + St;
			}
			else {
				Vec3d St = m.index(i) * Si;
				double StdotSt = St * St;
				Vec3d Ct = i.N * sqrt(1.0 - StdotSt);
				refractedDir = Ct + St;
			}
			refractedDir.normalize();
	
			ray refracted(r.at(i.t), refractedDir, ray::REFRACTION);
			Vec3d Irefract = m.kt(i) % traceRay( refracted, depth - 1);
	
			colorC += Irefract;
		}
	} else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.
		colorC = Vec3d(0.0, 0.0, 0.0);
	}
	return colorC;
}

RayTracer::RayTracer()
	: scene(0), buffer(0), buffer_width(256), buffer_height(256), m_bBufferReady(false)
{}

RayTracer::~RayTracer()
{
	delete scene;
	delete [] buffer;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return sceneLoaded() ? scene->getCamera().getAspectRatio() : 1;
}

bool RayTracer::loadScene( char* fn ) {
	ifstream ifs( fn );
	if( !ifs ) {
		string msg( "Error: couldn't read scene file " );
		msg.append( fn );
		traceUI->alert( msg );
		return false;
	}
	
	// Strip off filename, leaving only the path:
	string path( fn );
	if( path.find_last_of( "\\/" ) == string::npos ) path = ".";
	else path = path.substr(0, path.find_last_of( "\\/" ));

	// Call this with 'true' for debug output from the tokenizer
	Tokenizer tokenizer( ifs, false );
	Parser parser( tokenizer, path );
	try {
		delete scene;
		scene = 0;
		scene = parser.parseScene();
	} 
	catch( SyntaxErrorException& pe ) {
		traceUI->alert( pe.formattedMessage() );
		return false;
	}
	catch( ParserException& pe ) {
		string msg( "Parser: fatal exception " );
		msg.append( pe.message() );
		traceUI->alert( msg );
		return false;
	}
	catch( TextureMapException e ) {
		string msg( "Texture mapping exception: " );
		msg.append( e.message() );
		traceUI->alert( msg );
		return false;
	}

	if( !sceneLoaded() ) return false;

	return true;
}

void RayTracer::traceSetup(int w, int h)
{
	if (buffer_width != w || buffer_height != h)
	{
		buffer_width = w;
		buffer_height = h;
		bufferSize = buffer_width * buffer_height * 3;
		delete[] buffer;
		buffer = new unsigned char[bufferSize];
	}
	memset(buffer, 0, w*h*3);
	m_bBufferReady = true;
}

