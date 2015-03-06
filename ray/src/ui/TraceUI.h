//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

// who the hell cares if my identifiers are longer than 255 characters:
#pragma warning(disable : 4786)

#include <string>
#include <thread>
#include <mutex>

using std::string;

class RayTracer;

class TraceUI {
public:
	TraceUI() : m_nDepth(0), m_nSize(512), m_displayDebuggingInfo(false), m_usingCubeMap(false),
                    m_shadows(true), m_smoothshade(true), raytracer(0), m_enableAcceleration(true), 
                    m_nFilterWidth(1), m_nAASamples(1), m_nThreads(std::thread::hardware_concurrency()),
                    m_nTreeDepth(15), m_nLeafMax(10), m_enableStereogram(false)
                    {}

	virtual int	run() = 0;

	// Send an alert to the user in some manner
	virtual void alert(const string& msg) = 0;

	// setters
	virtual void setRayTracer( RayTracer* r ) { raytracer = r; }
	void setCubeMap(bool b) { m_gotCubeMap = b; }
	void useCubeMap(bool b) { m_usingCubeMap = b; }

	// accessors:
	int	getSize() const { return m_nSize; }
	int	getDepth() const { return m_nDepth; }
	int	getAASamples() const { return m_nAASamples; }
	int	getNumThreads() const { return m_nThreads; }
	int	getFilterWidth() const { return m_nFilterWidth; }
	int	getCubemapsEnabled() const { return m_usingCubeMap; }

	bool	shadowSw() const { return m_shadows; }
	bool	smShadSw() const { return m_smoothshade; }
	bool	acceleration() const { return m_enableAcceleration; }
	int		getTreeDepth() const { return m_nTreeDepth; }
	int		getLeafMax() const { return m_nLeafMax; }

	static bool m_debug;

protected:
	RayTracer*	raytracer;

	int	m_nSize;	// Size of the traced image
	int	m_nDepth;	// Max depth of recursion
	int m_nAASamples; // Number of samples for aa
	int m_nThreads; // Number of threads
	int m_nTreeDepth; // Depth of kdtree
	int m_nLeafMax; // max number of objects in leaf of kdtree

	// Determines whether or not to show debugging information
	// for individual rays.  Disabled by default for efficiency
	// reasons.
	bool m_displayDebuggingInfo;
	bool m_shadows;  // compute shadows?
	bool m_smoothshade;  // turn on/off smoothshading?
	bool m_usingCubeMap;  // render with cubemap
	bool m_gotCubeMap;  // cubemap defined
	bool m_enableAcceleration; // enable kd tree preprocessing
	int m_nFilterWidth;  // width of cubemap filter
	bool m_enableStereogram;
};

#endif
