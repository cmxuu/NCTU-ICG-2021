#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Object.h"
#include "glut.h"
#include <math.h>
using namespace std;

class Vertex {
	public:
		float x;
		float y;
		float z;
		void normalize() {
			float magnitude;
			magnitude = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
			*this = *this / magnitude;
		}
		Vertex operator + (Vertex const& obj) {
			Vertex temp;
			temp.x = this->x + obj.x;
			temp.y = this->y + obj.y;
			temp.z = this->z + obj.z;
			return temp;
		}
		Vertex operator - (Vertex const& obj) {
			Vertex temp;
			temp.x = this->x - obj.x;
			temp.y = this->y - obj.y;
			temp.z = this->z - obj.z;
			return temp;
		}
		Vertex operator / (float const& num) {
			Vertex temp;
			temp.x = this->x / num;
			temp.y = this->y / num;
			temp.z = this->z / num;
			return temp;
		}
		friend ostream& operator<<(ostream& os, const Vertex& v) {
			os << v.x << "," << v.y << "," << v.z;
			return os;
		}
};

int windowSize[2];
unsigned int texture;
int width, height, nrChannels;
unsigned char* pikachuTexture;
Object* Clock = new Object("clock.obj");
Object* Pikachu = new Object("Pikachu.obj");
GLUquadricObj* Hour, * Mintue;
int frame = 0, hour_angle = 0, min_angle = 0;
int base_angle = 0, pika_angle = 0, pika_revolve = 0;
bool key_r = false, key_p = false, key_m = false;

void light();
void display();
void idle();
void reshape(GLsizei, GLsizei);
void InitTexture();
void LoadModel(Object*, bool is_Pikachu = false);
void keyboard(unsigned char key, int x, int y);
void DrawBase();

// default light color, can be use to reset the material color
GLfloat default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f};
GLfloat default_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f};


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("HW1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	InitTexture();
	glutMainLoop();
	return 0;
}

void light()
{
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_position[] = { 10.0, 10.0, 5.0, 0.0 };
	glShadeModel(GL_SMOOTH);
	// z buffer enable
	glEnable(GL_DEPTH_TEST);
	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, light_specular);
	GLfloat mat_shininess[] = { 100 };
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// viewport transformation
	glViewport(0, 0, windowSize[0], windowSize[1]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)windowSize[0] / (GLfloat)windowSize[1], 1.0, 1000.0);
	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 15.0, 40.0,	// eye
			  0.0, 0.0, 0.0,	// center
		      0.0, 1.0, 0.0);	// up

	light();

	////TO DO : add model, translate, rotate, scale ,material
	//

	/* 旋轉 */

	// 時針
	if (frame % 12 == 0) {
		hour_angle++;
		if (hour_angle == 360)
			hour_angle = 0;
	}

	// 分針
	min_angle++;
	if (min_angle == 360)
		min_angle = 0;

	// R：底座+時鐘自轉
	if (key_r == true) {
		base_angle++;
		if (base_angle == 360)
			base_angle = 0;
	}

	// P：皮卡丘自轉
	if (key_p == true) {
		pika_angle++;
		if (pika_angle == 360)
			pika_angle = 0;
	}

	// M：皮卡丘公轉
	if (key_m == true) {
		pika_revolve += 2;
		if (pika_revolve == 360)
			pika_revolve = 0;
	}


	/* 模型 */

	// 底座
	DrawBase();
	
	// 時鐘
	glPushMatrix();
	glRotatef(base_angle, 0, 1, 0);						// R自轉
	glTranslatef(0.0, 7.5, 0.0);
	glScalef(0.08, 0.08, 0.08);
	LoadModel(Clock, false);
	glPopMatrix();

	// 分針
	GLfloat Mintue_color[] = { 1, 0, 1, 1 };
	glPushMatrix();
	glRotatef(base_angle, 0, 1, 0);						// R自轉
	glTranslatef(0.0, 7.5, 0.0);
	glRotatef(min_angle, 0, 0, -1);
	glRotatef(90, 0, 1, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Mintue_color);
	Mintue = gluNewQuadric();
	gluCylinder(Mintue, 0.3, 0.3, 5, 30, 30);
	glPopMatrix();
	
	// 時針
	GLfloat Hour_color[] = { 0, 1, 1, 1 };
	glPushMatrix();
	glRotatef(base_angle, 0, 1, 0);						// R自轉
	glTranslatef(0.0, 7.5, 0.0);
	glRotatef(hour_angle, 0, 0, -1);
	glRotatef(90, -1, 0, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Hour_color);
	Hour = gluNewQuadric();
	gluCylinder(Hour, 0.3, 0.3, 3, 30, 30);
	glPopMatrix();

	// 皮卡丘
	glPushMatrix();
	glRotatef(pika_revolve, 0, 1, 0);					// M公轉
	glTranslatef(14.0, 0.0, 0.0);
	glRotatef(45 + pika_angle, 0, 1, 0);				// P自轉
	glScalef(10, 10, 10);
	LoadModel(Pikachu, true);
	glPopMatrix();
	glutSwapBuffers();

	frame += 1;
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

void idle() {
	glutPostRedisplay();
}

void InitTexture() {
	stbi_set_flip_vertically_on_load(true);
	pikachuTexture = stbi_load("Pikachu.png", &width, &height, &nrChannels, 0);
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pikachuTexture);

	glDisable(GL_TEXTURE_2D);
}

void LoadModel(Object* Model,bool is_Pikachu) {
	if (is_Pikachu) {
		GLfloat pikachu_diffuse[] = { 1, 1, 0, 1 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pikachu_diffuse);
		glEnable(GL_TEXTURE_2D);
	}
	for (size_t i = 0; i < Model->fNum; ++i)
	{
		glBegin(GL_TRIANGLES);
		for (size_t j = 0; j < 3; ++j)
		{
			if (is_Pikachu) {
				int idx = i * 3 * 2 + j * 2;
				glTexCoord2f(Model->texcoords[idx],Model->texcoords[idx+1]);
			}
			glNormal3fv(Model->nList[Model->faceList[i][j].nIndex].ptr);
			glVertex3fv(Model->vList[Model->faceList[i][j].vIndex].ptr);
		}
		glEnd();
	}
	if (is_Pikachu) {
		// set amibent back to default
		glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
		glDisable(GL_TEXTURE_2D);
	}
}

void keyboard(unsigned char key, int x, int y) {
	//// TO DO : implement the keyboard function
	
	if (key == 'r' || key == 'R')
		key_r = !key_r;

	else if (key == 'p' || key == 'P')
		key_p = !key_p;

	else if (key == 'm' || key == 'M')
		key_m = !key_m;
}

void DrawBase() {

	GLfloat pi = M_PI;
	GLfloat blue_diffuse[] = { 114.0f / 255.0f , 201.0f / 255.0f , 240.0f / 255.0f , 1.0f };
	GLfloat red_diffuse[] = { 247.0f / 255.0f , 37.0f / 255.0f , 133.0f / 255.0f , 1.0f };
	

	/* 六邊形 */

	glPushMatrix();
	glRotatef(base_angle, 0, 1, 0);		// R自轉
	glBegin(GL_POLYGON);	
	for (int i = 0; i < 6; i++) {
		glNormal3d(0, 1, 0);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diffuse);
		glVertex3d(10 * cos((pi / 3.0f) * i ), 0.0f, 10 * sin((pi / 3.0f) * i ));
	}
	glEnd();
	glPopMatrix();
	

	/* 三角形 */
	glPushMatrix();
	glRotatef(base_angle, 0, 1, 0);		// R自轉
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 6; i++) {
		glNormal3d(0, 1, 0);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);
		glVertex3d(10 * cos((pi / 3.0f) * i ), 0.0f, 10 * sin((pi / 3.0f) * i));
		glVertex3d(10 * sqrt(3) * cos((pi / 3.0f) * i + (pi / 6.0f)), 0.0f, 10 * sqrt(3) * sin((pi / 3.0f) * i + (pi / 6.0f)));
		glVertex3d(10 * cos((pi / 3.0f) * i + (pi / 3.0f)), 0.0f, 10 * sin((pi / 3.0f) * i + (pi / 3.0f)));
	}
	glEnd();
	glPopMatrix();
	

	/* 矩形 */
	
	Vertex v1, v2, c;

	glPushMatrix();
	glRotatef(base_angle, 0, 1, 0);		// R自轉
	for (int i = 0; i < 6; i++) {
		
		// 法向量
		glBegin(GL_POLYGON);
		v1.x = 10 * sqrt(3) * cos((pi / 3.0f) * i + (pi / 6.0f)) - 10 * cos((pi / 3.0f) * i);
		v1.y = 0.0f;
		v1.z = 10 * sqrt(3) * sin((pi / 3.0f) * i + (pi / 6.0f)) - 10 * sin((pi / 3.0f) * i);
		v2.x = 0.0f;
		v2.y = -5.0f;
		v2.z = 0.0f;
		c.x = v1.y * v2.z - v2.y * v1.z;
		c.y = -(v1.x * v2.z - v2.x * v1.z);
		c.z = v1.x * v2.y - v2.x * v1.y;
		c.normalize();
		glNormal3d(c.x, c.y, c.z);
		
		// 材質
		glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);

		// 算點
		glVertex3d(10 * cos((pi / 3.0f) * i), 0.0f, 10 * sin((pi / 3.0f) * i));
		glVertex3d(10 * sqrt(3) * cos((pi / 3.0f) * i + (pi / 6.0f)), 0.0f, 10 * sqrt(3) * sin((pi / 3.0f) * i + (pi / 6.0f)));
		glVertex3d(10 * sqrt(3) * cos((pi / 3.0f) * i + (pi / 6.0f)), -5.0f, 10 * sqrt(3) * sin((pi / 3.0f) * i + (pi / 6.0f)));
		glVertex3d(10 * cos((pi / 3.0f) * i), -5.0f, 10 * sin((pi / 3.0f) * i));
	
		glEnd();
		

		glBegin(GL_POLYGON);
		v1.x = 10 * sqrt(3) * cos((pi / 3.0f) * i + (pi / 6.0f)) - 10 * cos((pi / 3.0f) * i + (pi / 3.0f));
		v1.y = 0.0f;
		v1.z = 10 * sqrt(3) * sin((pi / 3.0f) * i + (pi / 6.0f)) - 10 * sin((pi / 3.0f) * i + (pi / 3.0f));
		v2.x = 0.0f;
		v2.y = -5.0f;
		v2.z = 0.0f;
		c.x = v1.y * v2.z - v2.y * v1.z;
		c.y = -(v1.x * v2.z - v2.x * v1.z);
		c.z = v1.x * v2.y - v2.x * v1.y;
		c.normalize();
		glNormal3d(-c.x, -c.y, -c.z);

		glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);

		glVertex3d(10 * sqrt(3) * cos((pi / 3.0f) * i + (pi / 6.0f)), 0.0f, 10 * sqrt(3) * sin((pi / 3.0f) * i + (pi / 6.0f)));
		glVertex3d(10 * cos((pi / 3.0f) * i + (pi / 3.0f)), 0.0f, 10 * sin((pi / 3.0f) * i + (pi / 3.0f)));
		glVertex3d(10 * cos((pi / 3.0f) * i + (pi / 3.0f)), -5.0f, 10 * sin((pi / 3.0f) * i + (pi / 3.0f)));
		glVertex3d(10 * sqrt(3) * cos((pi / 3.0f) * i + (pi / 6.0f)), -5.0f, 10 * sqrt(3) * sin((pi / 3.0f) * i + (pi / 6.0f)));
		
		
		glEnd();

	}
	glPopMatrix();
}