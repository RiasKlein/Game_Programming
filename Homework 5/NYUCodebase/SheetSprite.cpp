#include "SheetSprite.h"

SheetSprite::SheetSprite() {}
SheetSprite::SheetSprite(GLuint textureID, unsigned int Xsprite, unsigned int Ysprite, unsigned int index) : textureID(textureID), Xsprite(Xsprite), Ysprite(Ysprite), index(index) {}

/*	The Draw function of the SheetSprite class will draw the object on the screen.
The drawn object's x location, y location, scale, and rotation are configurable.
*/
void SheetSprite::Draw(float x, float y, float scale, float rotation) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	float u = (float)(((int)index) % Xsprite) / (float)Xsprite;
	float v = (float)(((int)index) / Xsprite) / (float)Ysprite;

	width = 0.15f;
	height = 0.15f;
	GLfloat quad[] = { -width * scale, height * scale, -width * scale, -height * scale, width * scale, -height * scale, width * scale, height * scale };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	width = 1.0f / (float)Xsprite;
	height = 1.0f / (float)Ysprite;
	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
