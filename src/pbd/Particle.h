#pragma once

#include "pbd/Vec2.h"
#include <stdint.h>

// ------------------------------------------------

struct Particle {
  Vec2 position = {};   // Position actuelle
  Vec2 next_pos = {};   // Position attendue, utilisée dans toute les modifications 
  Vec2 velocity = {};   // Vitesse de la particule
  float radius  = 1.0F; // Rayon de la particule
  float mass = 1.0F;    // Masse de la particule
  int draw_id   = 0;    // Id de la particule
};

// ------------------------------------------------

