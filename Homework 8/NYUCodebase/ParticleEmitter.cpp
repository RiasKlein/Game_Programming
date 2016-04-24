#include "ParticleEmitter.h"

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

ParticleEmitter::ParticleEmitter(unsigned int particleCount){
	maxLifetime = 1.5f;

	for (int i = 0; i < particleCount; i++){
		Particle particle;
		particle.lifetime = ((float)rand() / (float)RAND_MAX) * maxLifetime;
		particle.position = position;
		particles.push_back(particle);
	}
}

ParticleEmitter::ParticleEmitter() {}
ParticleEmitter::~ParticleEmitter() {}

void ParticleEmitter::Update(float elapsed) {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].lifetime += elapsed;
		particles[i].position = (particles[i].position + (particles[i].velocity * elapsed));
	}

	lifetime += elapsed;
}

void ParticleEmitter::Render() {
	
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::vector<float> particleVertices;
	for (int i = 0; i < particles.size(); i++) {
		particleVertices.push_back(particles[i].position.x);
		particleVertices.push_back(particles[i].position.y);
	}

	glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	/*
	std::vector<float> particleColors;
	for (unsigned int i = 0; i < particles.size(); i++) {
		float m = (particles[i].lifetime / maxLifetime);
		particleColors.push_back(lerp(1.0f, 1.0f, m));
		particleColors.push_back(lerp(1.0f, 1.0f, m));
		particleColors.push_back(lerp(1.0f, 1.0f, m));
		particleColors.push_back(lerp(1.0f, 0.0f, m));
	}

	glColorPointer(4, GL_FLOAT, 0, particleColors.data());
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	*/

	glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
}

void ParticleEmitter::effect() {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].position = position;
		particles[i].velocity.x = velocity.x + (velocityDeviation.x * (((float)rand() / (float)RAND_MAX)));
		particles[i].velocity.y = velocity.y + (velocityDeviation.y * (((float)rand() / (float)RAND_MAX)));
	}
}