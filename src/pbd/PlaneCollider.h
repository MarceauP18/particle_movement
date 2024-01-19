#pragma once

#include "pbd/Vec2.h"
#include <stdint.h>

struct PlanCollider { // Structure d'un Plan , donné par un point de ce plan et un vecteur normal
  Vec2 point;
  Vec2 normale;
  PlanCollider() : point(Vec2(0,0)), normale(Vec2(0,0)){}
  PlanCollider(const Vec2& p, const Vec2& n) : point(p), normale(n) {}
};

struct SphereCollider { // Structure d'une sphère fixe, donnée par un centre et un rayon
  Vec2 centre;
  int rayon;
  SphereCollider() : centre(Vec2(0,0)),rayon(0){}
  SphereCollider(const Vec2& c, const int r) : centre(c), rayon(r){}
};