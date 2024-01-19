#pragma once

#include "pbd/Vec2.h"
#include "pbd/Particle.h"
#include "PlaneCollider.h"
// ------------------------------------------------



class Context
{
public:
  Context(int capacity);

  int num_particles() const { return m_num_particles; }
  int num_collissions_plan() const { return m_num_collissions_plan;}
  int num_collissions_sphere() const { return m_num_collissions_sphere;}

  void addParticle(Vec2 world_pos,
                   float radius, float mass,
                   Vec2 velocity, int draw_id);
  void addPlan(Vec2 point, Vec2 normale);
  void addSphere(Vec2 centre, int rayon);
  const Particle& particle(int id) const { return m_particles[id]; }

  void updatePhysicalSystem(float dt);

private:
  // Methods below are called by updatePhysicalSystem
  void applyExternalForce(float dt);
  void updateExpectedPosition(float dt);
  void addStaticContactConstraints();
  void updateVelocityAndPosition(float dt);
  void applyFriction();
  void checkContactWithPlane(int particle_id);
  void checkContactWithSphere(int particle_id);

private:
  int m_num_particles;
  Particle* m_particles;
  int m_num_collissions_plan;
  PlanCollider* m_collider_plan;
  int m_num_collissions_sphere;
  SphereCollider* m_collider_sphere;
};

// ------------------------------------------------