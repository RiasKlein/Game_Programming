#include "SheetSprite.h"

SheetSprite::SheetSprite() {}

SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float width, float height) : textureID(textureID), u(u), v(v), width(width), height(height) {}

void SheetSprite::Draw(float scale_x, float scale_y) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLfloat quad[] = { -width * scale_x, height * scale_y, -width * scale_x, -height * scale_y, width * scale_x, -height * scale_y, width * scale_x, height * scale_y };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	float x = u;
	float y = v;
	float yheight = v + height;
	float xwidth = u + width;

	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };

	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

