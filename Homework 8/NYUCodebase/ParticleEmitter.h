#pragma once
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Particle.h"
#include "Color.h"

class ParticleEmitter {
public:
	ParticleEmitter(unsigned int particleCount);

	ParticleEmitter();
	~ParticleEmitter();

	void Update(float elapsed);
	void Render();

	void effect();

	Vector position;
	Vector velocity;
	Vector velocityDeviation;

	float emitterSize;
	float maxLifetime;
	float lifetime;

	Color startColor;
	Color endColor;

	std::vector<Particle> particles;
	std::vector<float> particleColors;
};

float lerp(float v0, float v1, float t);