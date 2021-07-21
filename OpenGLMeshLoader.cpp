#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <string.h>

using namespace std;

int WIDTH = 1280;
int HEIGHT = 720;
double angle = 45;
double x = 0;
GLuint tex;
GLuint tex1;
char title[] = "3D Model Loader Sample";
int laserOn = 0;
bool scene1 = true;
bool win = false;

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 500;
double rotationAngle = 180 + 50.2;
double xz = 30;
double length = 500;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	} void setX(float value) {
		x = value;
	}
	void setY(float value) {
		y = value;
	}
	void setZ(float value) {
		z = value;
	}
};

Vector Eye(30 * cos(angle), 7.5, 30 * sin(angle));
Vector At(30 * cos(rotationAngle), 0, 30 * sin(rotationAngle));
Vector Up(0, 1, 0);

bool bottles[7] = { true };
bool bottles2[7] = { true };

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_bottle;
Model_3DS model_table;
Model_3DS model_GUN;
void laser(int val);

// Textures
GLTexture tex_ground;
GLTexture tex1_ground;

void collide() {
	Vector direct(Eye.x - At.x, Eye.y - At.y, Eye.z - At.z);
	double mag = sqrt(direct.x * direct.x + direct.y * direct.y + direct.z * direct.z);
	direct.setX(1.4 * 1.5 * direct.x / mag);
	direct.setY(1.4 * 1.5 * direct.y / mag);
	direct.setZ(1.4 * 1.5 * direct.z / mag);
	cout << "x: " << Eye.x - direct.x - 5;
	cout << ", y:" << 5 - direct.y * 7.5;
	cout << ", z:" << Eye.z - direct.z - 5 << endl;
}

void print(int x, int y, char* string) {
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
}

void Timer(int val) {
	if (laserOn == 0)
		glutTimerFunc(50, Timer, 0);
	else {
		glutTimerFunc(300, laser, 0);
	}

}

void laser(int val) {
	laserOn = 0;
	length = 500;
	glutPostRedisplay();//redisplay to update the screen with the changes
	glutTimerFunc(10, Timer, 0);
}

void myKeyboard(unsigned char k, int x, int y)
{
	if (k == 'd')//if the right arrow is pressed, then the object will be translated in the x axis by 10. (moving right)
	{
		rotationAngle += 0.01;
		At.setX(xz * cos(rotationAngle));
		At.setZ(xz *sin(rotationAngle));

	}
	if (k == 'a')//if the left arrow is pressed, then the object will be translated in the x axis by -10. (moving left)
	{
		rotationAngle -= 0.01;
		At.setX(xz * cos(rotationAngle));
		At.setZ(xz *sin(rotationAngle));
	}
	if (k == 'w')//if the left arrow is pressed, then the object will be translated in the x axis by -10. (moving left)
	{
		At.y += 1;
	}
	if (k == 's')//if the left arrow is pressed, then the object will be translated in the x axis by -10. (moving left)
	{
		At.y -= 1;
	}
	if (k == ' ') {
		laserOn = 1;
	}
	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//collide();

	glutPostRedisplay();//redisplay to update the screen with the changes
}

void spe(int k, int x, int y)// keyboard special key function takes 3 parameters
							// int k: is the special key pressed such as the keyboard arrows the f1,2,3 and so on
{
	if (k == GLUT_KEY_UP) {
		if (xz < 0)
			xz += 1;
		else
			xz -= 1;
		Eye.setX(xz * cos(angle));
		Eye.setZ(xz * sin(angle));
	}
	if (k == GLUT_KEY_DOWN) {
		if (xz < 0)
			xz -= 1;
		else
			xz += 1;
		Eye.setX(xz * cos(angle));
		Eye.setZ(xz * sin(angle));
	}
	if (k == GLUT_KEY_RIGHT)//if the right arrow is pressed, then the object will be translated in the x axis by 10. (moving right)
	{
		angle -= 0.01;
		Eye.setX(xz * cos(angle));
		Eye.setZ(xz * sin(angle));
		cout << angle;
	}
	if (k == GLUT_KEY_LEFT)//if the left arrow is pressed, then the object will be translated in the x axis by -10. (moving left)
	{
		angle += 0.01;
		Eye.setX(xz * cos(angle));
		Eye.setZ(xz * sin(angle));
		cout << angle;
	}
	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();//redisplay to update the screen with the changes
}

void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}
void RenderGround2()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex1_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-40, 0, -40);
	glTexCoord2f(5, 0);
	glVertex3f(40, 0, -40);
	glTexCoord2f(5, 5);
	glVertex3f(40, 0, 40);
	glTexCoord2f(0, 5);
	glVertex3f(-40, 0, 40);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glScalef(3, 1, 3);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(1, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(1, 1);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 1);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void Scene1() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	//// Draw Ground
	RenderGround();
	glPushMatrix();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);


	glPopMatrix();

	// Draw bottles Model

	//bottle1
	if (bottles[0]) {
		glPushMatrix();
		glTranslatef(-0.7, 3, -13);
		glScalef(0.2, 0.2, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//bottle2
	if (bottles[1]) {
		glPushMatrix();
		glTranslatef(-1.8, 3, -12);
		glScalef(0.2, 0.2, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//bottle3
	if (bottles[2]) {
		glPushMatrix();
		glTranslatef(-2.8, 3, -10);
		glScalef(0.2, 0.2, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//bottle4
	if (bottles[3]) {
		glPushMatrix();
		glTranslatef(-7.5, 3, 2);
		glScalef(0.2, 0.2, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//bottle5
	if (bottles[4]) {
		glPushMatrix();
		glTranslatef(-8, 3, 3);
		glScalef(0.2, 0.2, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//bottle6
	if (bottles[5]) {
		glPushMatrix();
		glTranslatef(-8, 3, 4);
		glScalef(0.2, 0.2, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//bottle7
	if (bottles[6]) {
		glPushMatrix();
		glTranslatef(-8, 3, 5);
		glScalef(0.2, 0.2, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//// Draw tables Model
	glColor3d(1, 1, 1);
	glPushMatrix();
	glTranslated(-2.5, -1, -10);
	glRotated(45, 0, 1, 0);
	glScalef(0.1, 0.1, 0.1);
	model_table.Draw();
	glPopMatrix();

	glColor3d(1, 1, 1);
	glPushMatrix();
	glTranslated(-8, -0.5, 5);
	glRotated(45, 0, 1, 0);
	glScalef(0.1, 0.1, 0.1);
	model_table.Draw();
	glPopMatrix();


	Vector direct(Eye.x - At.x, Eye.y - At.y, Eye.z - At.z);
	double mag = sqrt(direct.x * direct.x + direct.y * direct.y + direct.z * direct.z);
	direct.setX(1.4 * 1.5 * direct.x / mag);
	direct.setY(1.4 * 1.5 * direct.y / mag);
	direct.setZ(1.4 * 1.5 * direct.z / mag);

	//gun
	glPushMatrix();
	//glTranslated(x, 0, 0);
	glTranslated(Eye.x - direct.x - 5, 5 - direct.y * 7.5, Eye.z - direct.z - 5);
	glRotated(-120 + 180 + 480 - rotationAngle * 100, 0, 1, 0);
	//glRotated(-10, 1, 0, 0);
	//Laser
	if (laserOn == 1) {
		//if(true){
		glPushMatrix();
		//glTranslatef(5, 0, 5);
		glColor3f(1, 0, 0);
		//glTranslatef(0, 5, 0);
		glRotatef(-90, 0, 1, 0);
		glScalef(length, 0.05, 0.05);
		//glLineWidth(5);
		/*glBegin(GL_LINES);
		glVertex3f(Eye.x - direct.x - 5, 5 - direct.y * 7.5, Eye.z - direct.z - 5);
		glVertex3f(At.x, -At.y, At.z);
		glEnd();*/
		glutSolidCube(1);
		glPopMatrix();
	}

	//gun
	glColor3f(1, 1, 1);
	model_GUN.Draw();

	glPopMatrix();

	//glPushMatrix();
	//glColor3d(1, 0, 0);

	//glTranslated(x, 0, 0);
	//glTranslated(Eye.x - direct.x - 5, 5 - direct.y * 7.5, Eye.z - direct.z - 5);
	////glTranslated(-50, 0, -50);
	//glRotated(-120 + 180 + 480 - rotationAngle * 100, 0, 1, 0);

	//glScalef(2, 2, 2);
	////glutSolidCube(1);
	//glPopMatrix();

	//



	glutSwapBuffers();
}

void Scene2() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	//// Draw Ground
	RenderGround2();
	glPushMatrix();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);


	glPopMatrix();

	// Draw bottles Model

	//bottle1
	if (bottles2[0]) {
		glPushMatrix();
		glTranslatef(-0.7, 2.3, -13);
		glScalef(0.2, 0.1, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//bottle2
	if (bottles2[1]) {
		glPushMatrix();
		glTranslatef(-1.8, 2.3, -12);
		glScalef(0.2, 0.1, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//bottle3
	if (bottles2[2]) {
		glPushMatrix();
		glTranslatef(-2.8, 2.3, -10);
		glScalef(0.2, 0.1, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//bottle4
	if (bottles2[3]) {
		glPushMatrix();
		glTranslatef(-7.5, 2.5, 2);
		glScalef(0.2, 0.1, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//bottle5
	if (bottles2[4]) {
		glPushMatrix();
		glTranslatef(-8, 2.5, 3);
		glScalef(0.2, 0.1, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//bottle6
	if (bottles2[5]) {
		glPushMatrix();
		glTranslatef(-8, 2.5, 4);
		glScalef(0.2, 0.1, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//bottle7
	if (bottles2[6]) {
		glPushMatrix();
		glTranslatef(-8, 2.5, 5);
		glScalef(0.2, 0.1, 0.2);
		model_bottle.Draw();
		glPopMatrix();
	}

	//// Draw tables Model
	glColor3d(1, 1, 1);
	glPushMatrix();
	glTranslated(-2.5, -1, -10);
	glRotated(45, 0, 1, 0);
	glScalef(0.1, 0.1, 0.1);
	model_table.Draw();
	glPopMatrix();

	glColor3d(1, 1, 1);
	glPushMatrix();
	glTranslated(-8, -0.5, 5);
	glRotated(45, 0, 1, 0);
	glScalef(0.1, 0.1, 0.1);
	model_table.Draw();
	glPopMatrix();


	Vector direct(Eye.x - At.x, Eye.y - At.y, Eye.z - At.z);
	double mag = sqrt(direct.x * direct.x + direct.y * direct.y + direct.z * direct.z);
	direct.setX(1.4 * 1.5 * direct.x / mag);
	direct.setY(1.4 * 1.5 * direct.y / mag);
	direct.setZ(1.4 * 1.5 * direct.z / mag);

	//gun
	glPushMatrix();
	//glTranslated(x, 0, 0);
	glTranslated(Eye.x - direct.x - 5, 5 - direct.y * 7.5, Eye.z - direct.z - 5);
	glRotated(-120 + 180 + 480 - rotationAngle * 100, 0, 1, 0);
	//glRotated(-10, 1, 0, 0);
	//Laser
	if (laserOn == 1) {
		//	if(true){
		glPushMatrix();
		//glTranslatef(5, 0, 5);
		glColor3f(1, 0, 0);
		//glTranslatef(0, 5, 0);
		glRotatef(-90, 0, 1, 0);
		glScalef(length, 0.05, 0.05);
		//glLineWidth(5);
		/*glBegin(GL_LINES);
		glVertex3f(Eye.x - direct.x - 5, 5 - direct.y * 7.5, Eye.z - direct.z - 5);
		glVertex3f(At.x, -At.y, At.z);
		glEnd();*/
		glutSolidCube(1);
		glPopMatrix();
	}

	//gun
	glColor3f(1, 1, 1);
	model_GUN.Draw();

	glPopMatrix();

	//glPushMatrix();
	//glColor3d(1, 0, 0);

	//glTranslated(x, 0, 0);
	//glTranslated(Eye.x - direct.x - 5, 5 - direct.y * 7.5, Eye.z - direct.z - 5);
	////glTranslated(-50, 0, -50);
	//glRotated(-120 + 180 + 480 - rotationAngle * 100, 0, 1, 0);

	//glScalef(2, 2, 2);
	////glutSolidCube(1);
	//glPopMatrix();

	//



	glutSwapBuffers();
}

int argc;
char** argr;

void endDisplay() {
	glColor3f(0.831f, 0.686f, 0.216f);
	char* scoreChar[40];
	sprintf((char*)scoreChar, "3ash Yastaaaaa3");
	print(800, 500, (char*)scoreChar);
}

void endGame() {

	glutDisplayFunc(endDisplay);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, 1920, 0, 1080);

	glutMainLoop();

}


void myDisplay(void)
{
	if (win) {
		Scene2();
		cout << endl << endl << endl << "3ash Yastaaa333" << endl << endl << endl;
		exit(0);
	}
	else {

		if (scene1)
			Scene1();
		else
			Scene2();
	}
}


void scene1Anim(double x, double y, double z) {

	//bottle0
	if (bottles[0])
		if (9.54647 <= x && x <= 9.56512) {
			//	cout << "collide: " << x << endl;
			if (2.00545 <= y && y <= 3.30382) {
				//cout << "collide: " << z<< endl;
				if (18.8283 <= z && z <= 18.8493) {
					//cout << "collide: " << y  << endl;
					length = 70;
					bottles[0] = false;
				}

			}
		}
	//bottle1
	if (bottles[1])
		if (9.52752 <= x && x <= 9.54457) {
			//	cout << "collide: " << x << endl;
			if (2.02672 <= y && y <= 3.55804) {
				//cout << "collide: " << z<< endl;
				if (18.8375 <= z && z <= 18.8609) {
					//cout << "collide: " << y  << endl;
					length = 70;
					bottles[1] = false;
				}

			}
		}
	//bottle2
	if (bottles[2])
		if (9.50442 <= x && x <= 9.51978) {
			//	cout << "collide: " << x << endl;
			if (2.02307 <= y && y <= 3.29656) {
				//cout << "collide: " << z<< endl;
				if (18.8589 <= z && z <= 18.8794) {
					//cout << "collide: " << y  << endl; : 9.51977, y:2.02308, z:18.8793
					length = 60;
					bottles[2] = false;
				}

			}
		}
	//bottle3
	if (bottles[3])
		if (9.32915 <= x && x <= 9.33888) {
			//	cout << "collide: " << x << endl;
			if (2.48710 <= y && y <= 3.2690) {
				//cout << "collide: " << z<< endl;
				if (19.0070 <= z && z <= 19.0175) {
					//cout << "collide: " << y  << endl; : 9.51977, y:2.02308, z:18.8793
					length = 55;
					bottles[3] = false;
				}

			}
		}
	//bottle4  
	if (bottles[4])
		if (9.31156 <= x && x <= 9.32798) {
			//	cout << "collide: " << x << endl;
			if (2.22474 <= y && y <= 3.52965) {
				//cout << "collide: " << z<< endl;
				if (19.0188 <= z && z <= 19.037) {
					//cout << "collide: " << y  << endl; :
					length = 55;
					bottles[4] = false;
				}

			}
		}
	//bottle5   
	if (bottles[5])
		if (9.29276 <= x && x <= 9.30913) {
			//	cout << "collide: " << x << endl;
			if (2.47654 <= y && y <= 4.05982) {
				//cout << "collide: " << z<< endl;
				if (19.0296 <= z && z <= 19.0464) {
					//cout << "collide: " << y  << endl; :
					length = 47;
					bottles[5] = false;
				}

			}
		}

	//collision for Bottle 6
	if (bottles[6])
		if (9.27786f <= x && x <= 9.29448f) {
			//cout << "entered x";
			if (2.47085f <= y && y <= 4.05766f) {
				//cout << "entered y ";
				if (19.0443f <= z && z <= 19.061f) {
					//cout << "entered z ";
					length = 45;
					bottles[6] = false;
				}
			}
		}
	bool finished = false;
	for (int i = 0; i < 7; i++)
		finished = finished || bottles[i];
	scene1 = finished;
}


void scene2Anim(double x, double y, double z) {

	//bottle0
	if (bottles2[0])
		if (9.5576 <= x && x <= 9.56129) {
			//cout << "collide: " << x << endl;
			if (2.02878 <= y && y <= 2.27920) {
				//cout << "collide: " << z<< endl;
				if (18.8437 <= z && z <= 18.8489) {
					//cout << "collide: " << y  << endl;
					length = 70;
					bottles2[0] = false;
				}

			}
		}

	//bottle1
	if (bottles2[1])
		if (9.53763 <= x && x <= 9.54849) {
			//	cout << "collide: " << x << endl;
			if (1.77857 <= y && y <= 2.5299) {
				//cout << "collide: " << z<< endl;
				if (18.8512 <= z && z <= 18.8663) {
					//cout << "collide: " << y  << endl;
					length = 60;
					bottles2[1] = false;
				}

			}
		}

	//bottle2
	if (bottles2[2])
		if (9.51267 <= x && x <= 9.52378) {
			//	cout << "collide: " << x << endl;
			if (1.77464 <= y && y <= 2.52674) {
				//cout << "collide: " << z<< endl;
				if (18.8698 <= z && z <= 18.8848) {
					//cout << "collide: " << y  << endl; : 9.51977, y:2.02308, z:18.8793
					length = 60;
					bottles2[2] = false;
				}

			}
		}

	//bottle3
	if (bottles2[3])
		if (9.33525 <= x && x <= 9.34288) {
			//	cout << "collide: " << x << endl;
			if (2.4871 <= y && y <= 2.7459) {
				//cout << "collide: " << z<< endl;
				if (19.0136 <= z && z <= 19.0175) {
					//cout << "collide: " << y  << endl; : 9.51977, y:2.02308, z:18.8793
					length = 55;
					bottles2[3] = false;
				}

			}
		}
	//bottle4  
	if (bottles2[4])
		if (9.31695 <= x && x <= 9.32393) {
			//	cout << "collide: " << x << endl;
			if (2.48196 <= y && y <= 3.00244) {
				//cout << "collide: " << z<< endl;
				if (19.0245 <= z && z <= 19.0319) {
					//cout << "collide: " << y  << endl; :
					length = 55;
					bottles2[4] = false;
				}

			}
		}
	//bottle5   
	if (bottles2[5])
		if (9.30205 <= x && x <= 9.30913) {
			//	cout << "collide: " << x << endl;
			if (2.47654 <= y && y <= 2.9982) {
				//cout << "collide: " << z<< endl;
				if (19.0391 <= z && z <= 19.0464) {
					//cout << "collide: " << y  << endl; :
					length = 47;
					bottles2[5] = false;
				}

			}
		}

	//collision for Bottle 6
	if (bottles2[6])
		if (9.2873f <= x && x <= 9.29449f) {
			//cout << "entered x";
			if (2.47085f <= y && y <= 2.99354f) {
				//cout << "entered y ";
				if (19.0537f <= z && z <= 19.062f) {
					//cout << "entered z ";
					length = 45;
					bottles2[6] = false;
				}
			}
		}
	bool finished = false;
	for (int i = 0; i < 7; i++)
		finished = finished || bottles2[i];
	win = !finished;
}


void anim()
{
	Vector direct(Eye.x - At.x, Eye.y - At.y, Eye.z - At.z);
	double mag = sqrt(direct.x * direct.x + direct.y * direct.y + direct.z * direct.z);
	direct.setX(1.4 * 1.5 * direct.x / mag);
	direct.setY(1.4 * 1.5 * direct.y / mag);
	direct.setZ(1.4 * 1.5 * direct.z / mag);
	double x = Eye.x - direct.x - 5;
	double y = 5 - direct.y * 7.5;
	double z = Eye.z - direct.z - 5;

	bool modified = false;
	if (laserOn == 1) {

		//table 1
		if (9.47177f <= x && x <= 9.64635f) {
			//cout << "entered x";
			if (0.557042f <= y && y <= 1.78562f) {
				//cout << "entered y ";
				if (18.8134f <= z && z <= 18.96007f) {
					//cout << "entered z ";
					length = 64;
					modified = true;
				}

			}

		}
		//table 2
		if (9.2211f <= x && x <= 9.53483f) {
			if (1.02036f <= y && y <= 2.26454f) {
				//cout << "z: " << y << endl;
				if (18.9388f <= z && z <= 19.249f) {

					if ((18.9405f <= z && z <= 18.97f && 1.02568f <= y && y <= 1.75864f && 9.45141f <= x && x <= 9.48957f)
						|| (19.1652f <= z && z <= 19.1914f && 1.13522f <= y && y <= 1.65205f && 9.22340f <= x && x <= 9.23657f))
					{
					}
					else {
						length = 40;
						modified = true;
					}
				}
			}
		}

		if (scene1)
			scene1Anim(x, y, z);
		else
			scene2Anim(x, y, z);

	}
	glutPostRedisplay();
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");
	model_bottle.Load("Models/bottle/Bottle N210418.3ds");
	model_table.Load("Models/Pool Table/PoolTable.3DS");
	model_GUN.Load("Models/GUN/gun.3ds");
	//model_bottle.Load("Models/object/Stick_Figure_by_Swp.3DS");
	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
	tex1_ground.Load("Textures/Textura Terra.bmp");
	loadBMP(&tex1, "Textures/nightsky.bmp", true);
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	for (int i = 0; i < 7; i++)
		bottles[i] = true;
	for (int i = 0; i < 7; i++)
		bottles2[i] = true;
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);
	glutIdleFunc(anim);
	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	//glutSpecialFunc(spe);

	glutTimerFunc(500, Timer, 0);

	glutKeyboardFunc(myKeyboard);

	/*glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);*/
	glLineWidth(10);
	glutReshapeFunc(myReshape);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}