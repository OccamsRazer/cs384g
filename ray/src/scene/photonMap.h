#ifndef __PHOTON_MAP_H_
#define __PHOTON_MAP_H_

#include "scene.h"
#include "ray.h"
#include "light.h"

#define PI 3.14159265359

#define DOUBLE_MIN std::numeric_limits<double>::min()
#define DOUBLE_MAX std::numeric_limits<double>::max()


class PhotonMap {
public:
  PhotonMap(Scene *scene, int size);
  virtual ~PhotonMap();

  void build();
  ray *nearestPhoton(Vec3d p);
  int get_size() { return storedPhotons; }
private:
  Scene * scene;
  std::vector<ray*> photons;
  int size;
  int storedPhotons;
  float get_rand();
  int emit(ray r, Light* light, int depth);
};

#endif /* __PHOTON_MAP_H_ */