
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
  std::cout << "camera direction: " << scene->getCamera().getLook() << std::endl;
  std::cout << "building photon map with " << size << " photons" << std::endl;
  std::cout << "scene bbox min: " << scene->bounds().getMin() << std::endl;
  std::cout << "scene bbox max: " << scene->bounds().getMax() << std::endl;

  Vec3d minPoints = scene->bounds().getMin();
  Vec3d maxPoints = scene->bounds().getMax();

  Light* pLight;
  for ( vector<Light*>::const_iterator litr = scene->beginLights(); litr != scene->endLights(); ++litr ) {
    pLight = *litr;
  }
  
  int displayInterval = 0;
  int i = 0;
  while(storedPhotons < size){
  // for(int j = 0; j < 5; j++) {
    Vec3d tmp(get_rand(minPoints[0], maxPoints[0]),
              get_rand(minPoints[1], maxPoints[1]),
              get_rand(minPoints[2], maxPoints[2]));

    Vec3d direction = -1.0*pLight->getDirection(tmp);
    Photon r(pLight->getPosition(), direction, Vec3d(1.0,1.0,1.0));

    if (storedPhotons >= displayInterval) {
      std::cout << "mapped " << storedPhotons << " photons" << std::endl;
      displayInterval += size/10;
    }

    int ret = emit(scene, r, pLight, depth);
  }
}

Photon *PhotonMap::nearestPhoton(Vec3d p, double radius){
  Photon *closest = NULL;
  double dist = DOUBLE_MAX;

  for (int i = 0; i < storedPhotons; i++) {
    if (std::abs((p - photons[i]->p).length()) < dist) {
      closest = photons[i];
    }
  }
  return closest;
}

int PhotonMap::emit(Scene *scene, Photon r, Light* light, int depth){
  isect i;
  bool hit = false;
  if(traceUI->acceleration()){
    hit = scene->kdIntersect(r, i);
  }
  else {
    hit = scene->intersect(r,i);
  }
  if(hit){
    Vec3d newPos = r.at(i.t);
    double dist = light->distanceAttenuation(newPos);

    photons.push_back(new Photon(newPos, r.getDirection(), dist*r.getColor()));
    storedPhotons++;
  }

  return 0;
}

float PhotonMap::get_rand() { return ((2) * ((float) rand() / RAND_MAX)) - 1; }

float PhotonMap::get_rand(float min, float max) {
  float tmp = min;
  if (max < min){
    min = max;
    max = tmp;
  }
  float random = ((float) rand()) / (float) RAND_MAX;

  float range = max - min;  
  return (random*range) + min;
}