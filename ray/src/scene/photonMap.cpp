
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

using namespace std;

PhotonMap::PhotonMap() {

}

PhotonMap::~PhotonMap() {}

void PhotonMap::build(Scene *scene, int size, int depth) {
  std::cout << "building photon map with " << size << " photons" << std::endl;


  Light* pLight;
  for ( vector<Light*>::const_iterator litr = scene->beginLights(); litr != scene->endLights(); ++litr ) {
    pLight = *litr;
  }
  while(storedPhotons < size){

  }
}

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