#ifndef __PHOTON_MAP_H_
#define __PHOTON_MAP_H_

#include "scene.h"
#include "ray.h"
#include "light.h"
#include "../vecmath/vec.h"
#include "../vecmath/mat.h"
#include <string>

#define PI 3.14159265359

#define DOUBLE_MIN std::numeric_limits<double>::min()
#define DOUBLE_MAX std::numeric_limits<double>::max()

class Photon : public ray {
public:
  Photon(): ray(Vec3d(0,0,0), Vec3d(0,0,0), ray::PHOTON), c(Vec3d(0,0,0)), dist(0), distAtten(0) {}
  Photon(const Vec3d &pp, const Vec3d &dd, const Vec3d &cc, const double dist, const double distAtten)
    : ray(pp, dd, ray::PHOTON), c(cc), dist(dist), distAtten(distAtten) {}
  Photon(const Photon& other) : ray(other), c(other.c), dist(other.dist), distAtten(other.distAtten) {}
  ~Photon() {}

  Photon& operator =( const Photon& other ) 
  { p = other.p; d = other.d; c = other.c; dist = other.dist; distAtten = other.distAtten; return *this; }

  Vec3d getColor() const { return c; }
  double getDistance() const { return dist; }
  double getDistanceAttenuation() const { return distAtten; }

public:
  Vec3d c; // color
  double dist; // distance traveled
  double distAtten;
};

class PhotonMap {
public:
  PhotonMap();
  virtual ~PhotonMap();

  void build(Scene *scene, int size, int depth);
  Photon *nearestPhoton(Vec3d p, double radius);
  double kNearestPhotons(Vec3d p, int k, Photon* ret[]);
  int get_size() { return storedPhotons; }
private:
  Scene * scene;
  std::vector<Photon*> photons;
  int size;
  int storedPhotons;
  float get_rand();
  float get_rand(float min, float max);
  int emit(Scene *scene, Photon r, Light* light, int depth, bool caustic);
};

#endif /* __PHOTON_MAP_H_ */