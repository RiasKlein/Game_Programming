class Entity {
	// This class will be used for each of the game elements of PONG
	public:
		void Draw();
		float x = 0.0;
		float y = 0.0;
		float rotation = 0.0f;
		float scale = 1.0f;

		GLuint textureID;

		float width = 1.0f;
		float height = 1.0f;
		float speed = 0.9f;
		float velocity_x;
		float velocity_y;
};

/*	Draws the entity based on its settings such as location, scale, width, height, etc.
	For more details, please look at the Entity class.
*/
void Entity::Draw() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);

	GLfloat quad[] = {
		//top left point
		(x - (width / 2.0)), (y + (height / 2.0)),
		//top right point
		(x + (width / 2.0)), (y + (height / 2.0)),
		//bottom right point
		(x + (width / 2.0)), (y - (height / 2.0)),
		//bottom left point
		(x - (width / 2.0)), (y - (height / 2.0)),
	};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}
