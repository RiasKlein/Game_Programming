#include "Entity.h"

class Player : public Entity {
public:
	Player();

	void Update(float elapsed);
	void Render();
};