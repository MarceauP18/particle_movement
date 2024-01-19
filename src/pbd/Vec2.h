#pragma once
#include <cmath>
#include <iostream>

// ------------------------------------------------

struct Vec2 {
  float x;
  float y;

  Vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

  // Multiplication par un scalaire
  Vec2 operator*(float scalar) const {
    return Vec2(x * scalar, y * scalar);
  }

  // Sommation de vecteur
  Vec2 operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
  }

  // Soustraction de vecteur
  Vec2 operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
  }

  // Produit scalaire
  float dot(const Vec2& other) const {
    return x * other.x + y * other.y;
  }

  // Normalisation
  Vec2 normalize() const {
    float length = std::sqrt(x * x + y * y);
    if (length != 0.0f) {
      return Vec2(x / length, y / length);
    } else {
      return Vec2(0.0f, 0.0f);
    }
  }
};


// ------------------------------------------------