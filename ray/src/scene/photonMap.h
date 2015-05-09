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


class PhotonMap {
public:
  PhotonMap();
  virtual ~PhotonMap();

  void build(Scene *scene, int size, int depth);
  ray *nearestPhoton(Vec3d p);
  int get_size() { return storedPhotons; }
private:
  Scene * scene;
  std::vector<ray*> photons;
  int size;
  int storedPhotons;
  float get_rand();
  float get_rand(float min, float max);
  int emit(ray r, Light* light, int depth);
};

#endif /* __PHOTON_MAP_H_ */