#pragma warning(disable : 4786)

#include "particleSystem.h"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>

using namespace std;

static float prevT;

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() 
{
    // TODO
}




/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
    // TODO
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    // These values are used by the UI ...
    // negative bake_end_time indicates that simulation
    // is still progressing, and allows the
    // indicator window above the time slider
    // to correctly show the "baked" region
    // in grey.
    bake_end_time = -1;
    simulate = true;
    dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    // TODO

    // These values are used by the UI
    simulate = false;
    dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    particles.clear();
    bakedParticles.clear();
    prevT = -1;

    // These values are used by the UI
    simulate = false;
    dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
    if (!simulate)
        return;
    if(t - prevT < 0.01)
        return;

    bakeParticles(t);

    
    int numParticles =  particles.size();
    
    std::vector<Particle> newParticles;
    for (int i = 0; i <  numParticles; i++){
        Particle p = particles[i];
        // remove 'dead' particles
        if (p.createdAt == -1)
            p.createdAt = t;
        if (p.createdAt + p.ttl < t)
            continue;
        // gravity and drag
        p.force = Vec3d(0.0, -9.8*p.mass, 0.0) - 0.47*p.velocity;

        p.position += (t - prevT) * (p.velocity);
        p.velocity += (t - prevT) * ((p.force)/(p.mass));

        newParticles.push_back(p);
    }
    particles = newParticles;
    
    // Debugging info
    if( t - prevT > .04 )
        printf("(!!) Dropped Frame %lf (!!)\n", t-prevT);
    prevT = t;
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
    if(bakedParticles.empty() || t > bake_end_time)
        return;
    int numParticles =  bakedParticles[t].size();

    for(int i = 0; i < numParticles; i++){
        glPushMatrix();
            Particle p = bakedParticles[t][i];
            glTranslatef(p.position[0], p.position[1], p.position[2]);
            setDiffuseColor(p.color[0], p.color[1], p.color[2]);
            drawSphere(0.1);
        glPopMatrix();
    }
}

/** Create particles */
void ParticleSystem::createParticles(int number, Vec3d origin, Vec3d direction) {
    if(!simulate)
        return;
    for(int i = 0; i < number; i++){
        // chose a semi random direction
        float vX = (rand() % 10 + 10) * direction[0];
        float vY = (rand() % 5 + 5) * direction[1];
        float vZ = (rand() % 10 + 10) * direction[2];
        Vec3d v(vX, vY, vZ);
        double r = ((double) rand() / (RAND_MAX));
        double g = ((double) rand() / (RAND_MAX));
        double b = ((double) rand() / (RAND_MAX));
        Vec3d color(r,g,b);
        particles.push_back(Particle(origin, v, Vec3d(0.0,0.0,0.0), color, 0.5, 2.0));
    }
}


/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
    bakedParticles[t] = particles;
    bake_end_time = t;
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
    bakedParticles.clear();
    bake_end_time = -1;
}




