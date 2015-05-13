#include <cmath>

#include "light.h"

using namespace std;

double DirectionalLight::distanceAttenuation(const Vec3d& P) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}

double DirectionalLight::distanceAttenuation(const double d) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


Vec3d DirectionalLight::shadowAttenuation(const ray& r, const Vec3d& p) const
{
	Vec3d atten(1,1,1);

	isect i;
	ray shadowRay( p, getDirection(p), ray::SHADOW );


	if ( scene->intersect(shadowRay, i) ) {
		const Material& m = i.getMaterial();
		atten = Vec3d(0.0,0.0,0.0);

		if ( !m.kt(i).iszero() ){
			atten = ( distanceAttenuation(p) * getColor() ) % m.kt(i);
		}
	}

	return atten;
}

Vec3d DirectionalLight::getColor() const
{
	return color;
}


Vec3d DirectionalLight::getDirection(const Vec3d& P) const
{
	// for directional light, direction doesn't depend on P
	return -orientation;
}

Vec3d DirectionalLight::getPosition() const
{
	return orientation * 1000.0;
}

double PointLight::distanceAttenuation(const Vec3d& P) const
{
	double d = (position - P).length();
	return distanceAttenuation(d);
}

double PointLight::distanceAttenuation(const double d) const
{
	return min(1.0, 1.0/( constantTerm + linearTerm*d + quadraticTerm*d*d ));
}

Vec3d PointLight::getColor() const
{
	return color;
}

Vec3d PointLight::getDirection(const Vec3d& P) const
{
	Vec3d ret = position - P;
	ret.normalize();
	return ret;
}


Vec3d PointLight::shadowAttenuation(const ray& r, const Vec3d& p) const
{
	Vec3d atten(1,1,1);
	Vec3d d = position - p;
	d.normalize();
	double distanceToLight = (position - p).length();

	isect i;
	ray shadowRay( p, d, ray::SHADOW );

	if ( scene->intersect(shadowRay, i) ) {
		const Material& m = i.getMaterial();
		double distanceToIntersect = (shadowRay.at(i.t) - p).length();
		if (distanceToLight < distanceToIntersect) return atten; // for when intersect occurs behind the light

		atten = Vec3d(0.4,0.4,0.4);

		if ( !m.kt(i).iszero() ){
			atten = ( distanceAttenuation(p) * getColor() ) % m.kt(i);
		}
	}

	return atten;
}

Vec3d PointLight::getPosition() const
{
	return position;
}
