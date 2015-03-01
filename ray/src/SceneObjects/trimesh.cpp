#include <cmath>
#include <float.h>
#include <algorithm>
#include <assert.h>
#include "trimesh.h"
#include "../ui/TraceUI.h"
extern TraceUI* traceUI;

using namespace std;

Trimesh::~Trimesh()
{
	for( Materials::iterator i = materials.begin(); i != materials.end(); ++i )
		delete *i;
}

// must add vertices, normals, and materials IN ORDER
void Trimesh::addVertex( const Vec3d &v )
{
    vertices.push_back( v );
}

void Trimesh::addMaterial( Material *m )
{
    materials.push_back( m );
}

void Trimesh::addNormal( const Vec3d &n )
{
    normals.push_back( n );
}

// Returns false if the vertices a,b,c don't all exist
bool Trimesh::addFace( int a, int b, int c )
{
    int vcnt = vertices.size();

    if( a >= vcnt || b >= vcnt || c >= vcnt ) return false;

    TrimeshFace *newFace = new TrimeshFace( scene, new Material(*this->material), this, a, b, c );
    newFace->setTransform(this->transform);
    if (!newFace->degen) faces.push_back( newFace );


    // Don't add faces to the scene's object list so we can cull by bounding box
    // scene->add(newFace);
    return true;
}

char* Trimesh::doubleCheck()
// Check to make sure that if we have per-vertex materials or normals
// they are the right number.
{
    if( !materials.empty() && materials.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of materials.";
    if( !normals.empty() && normals.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of normals.";

    return 0;
}

bool Trimesh::intersectLocal(ray& r, isect& i) const
{
	double tmin = 0.0;
	double tmax = 0.0;
	typedef Faces::const_iterator iter;
	bool have_one = false;
	for( iter j = faces.begin(); j != faces.end(); ++j )
	  {
	    isect cur;
	    if( (*j)->intersectLocal( r, cur ) )
	      {
		if( !have_one || (cur.t < i.t) )
		  {
		    i = cur;
		    have_one = true;
		  }
	      }
	  }
	if( !have_one ) i.setT(1000.0);
	return have_one;
}

bool TrimeshFace::intersect(ray& r, isect& i) const {
  return intersectLocal(r, i);
}

// Intersect ray r with the triangle abc.  If it hits returns true,
// and put the parameter in t and the barycentric coordinates of the
// intersection in u (alpha) and v (beta).
bool TrimeshFace::intersectLocal(ray& r, isect& i) const
{

    const Vec3d& a = parent->vertices[ids[0]];
    const Vec3d& b = parent->vertices[ids[1]];
    const Vec3d& c = parent->vertices[ids[2]];

    Vec3d n = (a - b) ^ (b - c);
    n.normalize();
    double d = -(n * a);

    double t = - ( (n * r.getPosition()) + d )/(n * r.getDirection());
    if( t <= RAY_EPSILON ) {
        return false;
    }

    Vec3d Q = r.at(t);

    // project to xy (default)
    double max = abs(n[2]);
    Mat3d mat(a[0], b[0], c[0],
              a[1], b[1], c[1],
                1,    1,    1);
    Vec3d Qprime(Q[0], Q[1], 1);
    
    // project to xz
    if ( max < abs(n[1]) ) {
        mat = Mat3d(a[0], b[0], c[0],
                      1,    1,    1,
                    a[2], b[2], c[2]);
        Qprime = Vec3d(Q[0], 1, Q[2]);
        max = abs(n[1]);
    }
    // project to yz
    if ( max < abs(n[0]) ) {
        mat = Mat3d(  1,    1,    1,
                    a[1], b[1], c[1],
                    a[2], b[2], c[2]);
        Qprime = Vec3d(1, Q[1], Q[2]);
        max = abs(n[0]);
    }

    Vec3d bary = mat.inverse() * Qprime;

    if( bary.iszero() || bary[0] < 0 || bary[1] < 0 || bary[2] < 0) return false;

    if( parent->vertNorms ){
        n = parent->normals[ids[0]] % bary + parent->normals[ids[1]] % bary + parent->normals[ids[2]] % bary;
        n.normalize();
    }

    i.setN(n);
    i.setT(t);
    i.setBary(bary);
    i.setMaterial(getMaterial());
    // i.setUVCoordinates(Vec2d(bary[0],bary[1]));

    return true;
}

void Trimesh::generateNormals()
// Once you've loaded all the verts and faces, we can generate per
// vertex normals by averaging the normals of the neighboring faces.
{
    int cnt = vertices.size();
    normals.resize( cnt );
    int *numFaces = new int[ cnt ]; // the number of faces assoc. with each vertex
    memset( numFaces, 0, sizeof(int)*cnt );
    
    for( Faces::iterator fi = faces.begin(); fi != faces.end(); ++fi )
    {
		Vec3d faceNormal = (**fi).getNormal();
        
        for( int i = 0; i < 3; ++i )
        {
            normals[(**fi)[i]] += faceNormal;
            ++numFaces[(**fi)[i]];
        }
    }

    for( int i = 0; i < cnt; ++i )
    {
        if( numFaces[i] )
            normals[i]  /= numFaces[i];
    }

    delete [] numFaces;
    vertNorms = true;
}
