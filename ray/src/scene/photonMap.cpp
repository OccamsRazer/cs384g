
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
  if (depth <= 0) return;

  std::cout << "camera direction: " << scene->getCamera().getLook() << std::endl;
  std::cout << "building photon map with " << size << " photons" << std::endl;
  std::cout << "scene bbox min: " << scene->bounds().getMin() << std::endl;
  std::cout << "scene bbox max: " << scene->bounds().getMax() << std::endl;

  Vec3d minPoints = scene->bounds().getMin();
  Vec3d maxPoints = scene->bounds().getMax();

  Light* pLight;
  for ( vector<Light*>::const_iterator litr = scene->beginLights(); litr != scene->endLights(); ++litr ) {
    pLight = *litr;
  
    int displayInterval = 0;
    int i = 0;
    storedPhotons = 0;
    int photonsEmitted = 0;
    while(storedPhotons < size){
      // if( photonsEmitted > size*size*size && storedPhotons < size/2 ) break;

      Vec3d tmp(get_rand(minPoints[0], maxPoints[0]),
                get_rand(minPoints[1], maxPoints[1]),
                get_rand(minPoints[2], maxPoints[2]));

      Vec3d direction = -1.0*pLight->getDirection(tmp);
      Photon r(pLight->getPosition(), direction, 1.0/size * pLight->getColor(), 0.0, 1.0);

      if (storedPhotons >= displayInterval) {
        std::cout << "mapped " << storedPhotons << " photons" << std::endl;
        displayInterval += size/10;
      }

      int ret = emit(scene, r, pLight, depth, false);
      photonsEmitted++;
    }
    std::cout << "mapped " << storedPhotons << " photons" << std::endl;
    std::cout << "emitted " << photonsEmitted << " photons" << std::endl;
  }
}

Photon *PhotonMap::nearestPhoton(Vec3d p, double radius){
  Photon *closest = NULL;
  double dist = radius;

  for (int i = 0; i < storedPhotons; i++) {
    if (std::abs((p - photons[i]->p).length()) < dist) {
      closest = photons[i];
      dist = std::abs((p - photons[i]->p).length());
    }
  }
  return closest;
}

// returns radius of circle centered at p that contains k photons
int PhotonMap::kNearestPhotons(Vec3d p, double radius, std::vector<Photon*> &ret){
  int stored = 0;
  if (radius <= 0) return 0;

  for(vector<Photon*>::const_iterator pitr = photons.begin(); pitr != photons.end(); ++pitr){
    Photon* ptn = *pitr;
    // if array not filled add the current if within the radius
    if ( std::abs((p - ptn->p).length()) < radius ) {
      ret.push_back(ptn);
      stored++;
    }
  }

  // return the number of found photons;
  return stored;
}

int PhotonMap::emit(Scene *scene, Photon r, Light* light, int depth, bool caustic){
  int stored = 0;
  isect i;
  bool hit = false;
  if(traceUI->acceleration()){
    hit = scene->kdIntersect(r, i);
  }
  else {
    hit = scene->intersect(r,i);
  }
  if(hit){
    // store photon on initial intersect
    const Material& m = i.getMaterial();
    Vec3d newPos = r.at(i.t);

    double dist = (r.getPosition() - newPos).length() + r.getDistance();
    double distAtten = light->distanceAttenuation(dist);
    if(caustic && !m.kd(i).iszero()){
      photons.push_back(new Photon(newPos, r.getDirection(), r.getColor(), dist, distAtten));
      storedPhotons++;
      stored = 1;
    }

    if( depth < 1) return stored;

    Vec3d Ci = ( (-1 * r.getDirection()) * i.N ) * i.N;
    Vec3d Si = Ci + r.getDirection();

    if (!m.kr(i).iszero()){
      Vec3d reflectedDir = Ci + Si;
      reflectedDir.normalize();
      // TODO change reflected color
      Photon reflected(r.at(i.t), reflectedDir, m.kr(i) % r.getColor(), dist, distAtten);

      emit(scene, reflected, light, depth - 1, true);
    }

    double Thetai = i.N * r.getDirection();
    bool tir = 1.0 - m.index(i) * m.index(i) * ( 1.0 - Thetai * Thetai) < 0;
    bool entering = Thetai < 0.0;

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

      Photon refracted(r.at(i.t), refractedDir, m.kt(i) % r.getColor(), dist, distAtten);
      emit(scene, refracted, light, depth - 1, true);
    }
  }

  return stored;
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
