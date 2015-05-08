
#include "photonMap.h"
#include "ray.h"
#include "light.h"
#include "../ui/TraceUI.h"
extern TraceUI* traceUI;

#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>
#include <fstream>

PhotonMap::PhotonMap(Scene *scene, int size):
  scene(scene), size(size) {

}

PhotonMap::~PhotonMap() {}

void PhotonMap::build() {}

ray *PhotonMap::nearestPhoton(Vec3d p){
  ray *closest = NULL;
  double dist = DOUBLE_MAX;

  for (int i = 0; i < storedPhotons; i++) {
    if (std::abs((p - photons[i]->p).length()) < dist) {
      closest = photons[i];
    }
  }
  return closest;
}

int PhotonMap::emit(ray r, Light* light, int depth){
  return 0;
}

float PhotonMap::get_rand() { return ((2) * ((float) rand() / RAND_MAX)) - 1; }