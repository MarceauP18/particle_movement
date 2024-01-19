#include "pbd/Context.h"
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

// ------------------------------------------------

Context::Context(int capacity) //Initialiser le Context vide
{
  this->m_num_particles = 0;
  this->m_particles = new Particle[capacity];
  this->m_num_collissions_plan = 0;
  this->m_collider_plan = new PlanCollider[capacity];
  this->m_num_collissions_sphere = 0;
  this->m_collider_sphere = new SphereCollider[capacity];
 
}

// ------------------------------------------------

void Context::addParticle(Vec2 world_pos,
                          float radius, float mass,
                          Vec2 velocity, int draw_id) // Ajout d'une particule au context en fonction de ses caractéristiques
{
    this->m_particles[m_num_particles] = Particle{
        world_pos, {}, velocity,
        radius, mass, draw_id}; // Ajout dans m_particles
    this->m_num_particles++; // Augmentation du nombre de particules
}

void Context::addPlan(Vec2 point, Vec2 normale) // Ajout d'un plan au context
{
  this->m_collider_plan[m_num_collissions_plan] = PlanCollider(point,normale); //Update du PlanCollider
  this->m_num_collissions_plan++; // Augmentation du nombre de collissions de plan
}
// -----------------------------------------------
void Context::addSphere(Vec2 centre, int rayon) // Ajout d'une Sphère fixe au contexte
{
  this->m_collider_sphere[m_num_collissions_sphere] = SphereCollider(centre,rayon); // Update de SphereCollider
  this->m_num_collissions_sphere++; // Augmentation du nombre de collissions sphériques 
}

// Fonction globale permettant d'update tous le système d'un pas dt
void Context::updatePhysicalSystem(float dt) 
{

  applyExternalForce(dt); // Modification de la vitesse suite au poids et aux frottements
  updateExpectedPosition(dt); // Update de la position next_pos suite à ces modifications de vitesse

  addStaticContactConstraints(); //On ajoute les modifications de position suite aux contacts avec des structures fixes (plan ou sphère) 
  applyFriction(); // Modification des positions des sphères quand elles entrent en collision entre elles

  updateVelocityAndPosition(dt); // Update de la vitesse et de la position pour plot les sphères et recommencer la boucle 

}

// ------------------------------------------------

void Context::applyExternalForce(float dt) // Ici on ajoute le poids et les frottements
{
  Vec2 gravite = Vec2(0.0f, -9.8);
  float rho = 1.225; // masse volumique de l'air au niveau de la mer à 15°C
  for (int i=0;i<num_particles();i++){
    // Impact du poids
    m_particles[i].velocity =  m_particles[i].velocity + gravite*dt;


    float A = 3.1415*m_particles[i].radius*m_particles[i].radius; // section transversale de la sphere
    float Cd = 0.47; // Coefficient de trainée
    float k = 0.5*rho*A*Cd; // Coefficient de frottement

    // Impact des frottements
    m_particles[i].velocity =  m_particles[i].velocity - m_particles[i].velocity*(k*dt/m_particles[i].mass); 
  }
}


void Context::updateExpectedPosition(float dt) // Modifications de la next_pos suite au changement de vitesse
{
  for (int i=0;i<num_particles();i++){
    m_particles[i].next_pos = m_particles[i].position + m_particles[i].velocity*dt;
  }
}


void Context::addStaticContactConstraints() // Utilisation des 2 fonctions pour faire les modifications par particules
{
  for (int i=0;i<num_particles();i++){
    checkContactWithPlane(i); // Sur les plans
    checkContactWithSphere(i); // Sur les sphères fixes
  }
}

void Context::updateVelocityAndPosition(float dt) // Modification de la vitesse et de la position suite aux modifications de next_pos
{
  for (int i=0;i<num_particles();i++){
    m_particles[i].velocity = (m_particles[i].next_pos - m_particles[i].position)*(1/dt);
    m_particles[i].position = m_particles[i].next_pos;
  }
}

void Context::applyFriction() // Nous allons vérifier pour chaque particule si elle est en contact avec d'autres
// Si oui, on effectuera les modifications nécessaires
// Ces modifications suivent les formules données dans le sujet
{
  for (int i = 0; i<num_particles();i++){
    for (int j = i+1; j<num_particles();j++){ // On va check pour i < j pour n'effectuer qu'une fois le check
      Vec2 xji = m_particles[i].next_pos - m_particles[j].next_pos;
      Vec2 xij = m_particles[j].next_pos - m_particles[i].next_pos;
      float norme = sqrt(xij.x*xij.x + xij.y*xij.y); // on aurait pu faire avec xji aussi, c'est pareil
      float C = norme - (m_particles[i].radius + m_particles[j].radius);
      if (C<0){ // Ici, on a donc un contact entre les particules i et j
        float mi = m_particles[i].mass;
        float mj = m_particles[j].mass;

        float sigmai = C * (1/mi)/(1/mi + 1/mj);
        float sigmaj = C * (1/mj)/(1/mi + 1/mj);

        Vec2 deltai = xji*(-sigmai / norme);
        Vec2 deltaj = xji*(sigmaj / norme);

        m_particles[i].next_pos = m_particles[i].next_pos + deltai;
        m_particles[j].next_pos = m_particles[j].next_pos + deltaj;

      }
    }
  }
}


// On va modifier la next_pos d'une certaine particule en cas de contact avec un plan fixe
// Ces modifications suivent les formules données dans le sujet
void Context::checkContactWithPlane(int particle_id){
  for (int i=0;i<m_num_collissions_plan;i++){
    if (((m_particles[particle_id].next_pos - m_collider_plan[i].point).dot(m_collider_plan[i].normale) - m_particles[particle_id].radius) < 0){
      Vec2 qc = m_particles[particle_id].next_pos - m_collider_plan[i].normale*((m_particles[particle_id].next_pos - m_collider_plan[i].point).dot(m_collider_plan[i].normale));
      float C = ((m_particles[particle_id].next_pos - qc).dot(m_collider_plan[i].normale)) - m_particles[particle_id].radius;
      Vec2 deltai = m_collider_plan[i].normale*(-C);
      m_particles[particle_id].next_pos =  m_particles[particle_id].position +  deltai;
    }
  } 
}


// On va modifier la next_pos d'une certaine particule en cas de contact avec une sphère fixe
// Ces modifications suivent les formules données dans le sujet
void Context::checkContactWithSphere(int particle_id){
  for (int i=0;i<m_num_collissions_sphere;i++){
    Vec2 part = m_particles[particle_id].next_pos;
    Vec2 centre = m_collider_sphere[i].centre;
    Vec2 xji = part - centre;
    float norme = sqrt(xji.x*xji.x + xji.y*xji.y);
    float sdf = norme - (m_collider_sphere[i].rayon + m_particles[particle_id].radius);
    if (sdf<0){
      Vec2 deltai = xji * (-sdf/norme); 
      m_particles[particle_id].next_pos =  m_particles[particle_id].next_pos +  deltai;
    }
  }
}