#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <string.h>
//#include <stdlib.h>
#include <random>
#include <ctime>
#include <iostream>
#include <glut.h>
using namespace std;

class Fish {
public:
	int x = 0;
	int y = 0;
	int dx = 0;
	int dy = 0;
	int scale = 0;
	bool notEaten = true;

	void createFish(int Size, int X, int Y) {
		x = X;
		y = Y;
		scale = 3 * Size;
	}

	void move() {
		if (!(x < 0 && dx < 0) && !(x > 1800 && dx > 0))
			x = x + dx;
		else
			if (x < 0) {
				dx = 20;
				x += dx;
			}
			else
				if (x > 1800) {
					dx = -20;
					x += dx;
				}

		if (!(y < 0 && dy < 0) && !(y > 1000 && dy > 0))
			y = y + dy;
		else
			if (y < 0) {
				dy = +20;
				y += dy;
			}
			else
				if (y > 1000) {
					dy = -20;
					y += dy;
				}
		dx = dx + 0.09 * 9 * (rand() % 10 - 4.5);
		dy = dy + 0.09 * 9 * (rand() % 10 - 4.5);
	}
};

class Bubble {
public:
	float x;
	float y;
	void createBubble(float X, float Y) {
		x = X;
		y = Y;
	}
	void move() {
		x = (x - 10);
		if (x < 0)
			x+= 1980;
	}
};

vector< Fish > fishes;
vector <Bubble> bubbles;

int passX = 700;
int passY = 100;
float mainScale = 4.50;
int eaten = 0;
bool stillAlive = true;
int score = 0;
int scoreFraction = 0;
int aliveFish = 20;
int collide = 0;
void drawFish(float scale, int x, int y);
//void Timer(int value);
void Display();
void drawRect(int x, int y, int w, int h);
bool collidedWithMain(Fish f);
void handleCollisions();
//void passM(int x, int y);


void anim() {
//void Timer(int val){
	if (stillAlive && aliveFish > 0) {

	for (int i = 0; i < fishes.size(); i++) {
		fishes.at(i).move();
		//cout << "fish: x: " << fishes.at(i).x;
		//cout << "      y: " << fishes.at(i).y << '\n';
	}

	for (int i = 0; i < bubbles.size(); i++) {
		bubbles.at(i).move();
		//cout << "bubble: x: " << bubbles.at(i).x;
		//cout << "      y: " << bubbles.at(i).y << '\n';
	}
	if (scoreFraction == 5) {
		score++;
		scoreFraction = 0;
	}
	scoreFraction++;


	handleCollisions();

	glutPostRedisplay();						// redraw 		
	if (stillAlive)
		glClear(GL_COLOR_BUFFER_BIT);
		//glutTimerFunc(50, Timer, 0);			//ms

	}
}

void Timer(int val) {
	glutIdleFunc(anim);
}


void special(int k, int x, int y)// keyboard special key function takes 3 parameters
							// int k: is the special key pressed such as the keyboard arrows the f1,2,3 and so on
{
	if (k == GLUT_KEY_RIGHT)//if the right arrow is pressed, then the object will be translated in the x axis by 10. (moving right)
		passX += 10 * mainScale;
	if (k == GLUT_KEY_LEFT)//if the left arrow is pressed, then the object will be translated in the x axis by -10. (moving left)
		passX -= 10 * mainScale;
	if (k == GLUT_KEY_UP)//if the up arrow is pressed, then the object will be translated in the y axis by 10. (moving upwords)
		passY += 10 * mainScale;
	if (k == GLUT_KEY_DOWN)//if the down arrow is pressed, then the object will be translated in the y axis by -10. (moving downwords)
		passY -= 10 * mainScale;

	if (passX < -100)
		passX = -100;
	if (passX > 1800)
		passX = 1800;

	if (passY < -50)
		passY = -50;
	if (passY > 1000)
		passY = 1000;
	glutPostRedisplay();//redisplay to update the screen with the changes
}

void print(int x, int y, char* string) {
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
}

void drawOtherFish(Fish f) {
	int x = f.x;
	int y = f.y;
	int scale = f.scale;

	glPushMatrix();

	glTranslatef(1920, 0, 0);

	if (scale == 9)
		glColor3f(0.949f, 0.522f, 0);
	if (scale == 6)
		glColor3f(1, 0.957f, 0.31f);
	if (scale == 3)
		glColor3f(0.1f, 0.6715f, 0.1f);


	//Tail
	glBegin(GL_TRIANGLES);
	glVertex2f(0 - x, y + 5 * scale);
	glVertex2f(0 - x, y + 15 * scale);
	glVertex2f(0 - (x + 8 * scale), y + 10 * scale);
	glEnd();

	//Body
	glBegin(GL_POLYGON);
	glVertex2f(0 - (x + 7 * scale), y + 10 * scale);
	glVertex2f(0 - (x + 30 * scale), y + 22.5 * scale);
	glVertex2f(0 - (x + 40 * scale), y + 12 * scale);
	glVertex2f(0 - (x + 36 * scale), y + 10 * scale);
	glVertex2f(0 - (x + 40 * scale), y + 7 * scale);
	glVertex2f(0 - (x + 30 * scale), y + 0 * scale);
	glEnd();

	//Fin
	glColor3f(0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(0 - (x + 18 * scale), y + 10 * scale);
	glVertex2f(0 - (x + 24 * scale), y + 7 * scale);
	glVertex2f(0 - (x + 24 * scale), y + 13 * scale);
	glEnd();

	//Eye
	glTranslatef(0 - (30 * scale + x), 16 * scale + y, 0);
	glColor3f(1, 0, 0);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, 2 * scale, 100, 100);

	glPopMatrix();
}

void drawFish(float scale, int x, int y) {
	glPushMatrix();
	glTranslatef(x , y , 0);

	glColor3f(1.0f, 0.03f, 0.23f);

	//Tail
	glBegin(GL_TRIANGLES);
	glVertex2f(0 , 0 + 5 * scale);
	glVertex2f(0 , 0 + 15 * scale);
	glVertex2f(0 + 8 * scale, 0 + 10 * scale);
	glEnd();

	//Body
	glBegin(GL_POLYGON);
	glVertex2f(0 + 7 * scale, 0 + 10 * scale);
	glVertex2f(0 + 30 * scale, 0 + 22.5 * scale);
	glVertex2f(0 + 40 * scale, 0 + 12 * scale);
	glVertex2f(0 + 36 * scale, 0 + 10 * scale);
	glVertex2f(0 + 40 * scale, 0 + 7 * scale);
	glVertex2f(0 + 30 * scale, 0 + 0 * scale);
	glEnd();

	//Fin
	glColor3f(0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(0 + 18 * scale, 0 + 10 * scale);
	glVertex2f(0 + 24 * scale, 0 + 7 * scale);
	glVertex2f(0 + 24 * scale, 0 + 13 * scale);
	glEnd();

	//Eye
	glTranslatef(30 * scale + 0 , 16 * scale + 0 , 0);
	glColor3f(0, 0, 1);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, 2 * scale, 100, 100);

	glPopMatrix();
}

void drawCircle(int x, int y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}


void makeBackground() {
	//background dots
	glColor3f(1, 1, 1);
	for (int i = 0; i < bubbles.size(); i++) {
		Bubble b = bubbles.at(i);
		drawCircle(b.x, b.y, 4);
	}
}

void Display() {

	if(collide==0)
		glClearColor(0, 0.467f, 0.745f, 0.0f);//the background color is changed in the timer function after 1 sec by changing these parameters.
	else {
		glClearColor(1, 0, 0, 0.0f);
		collide--;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	if (!stillAlive||aliveFish==0) {
		glColor3f(0, 0, 0);
		drawRect(0, 0, 1920, 1080);
		glFlush();
		if (!stillAlive) {
			glColor3f(1, 0, 0);
			char* scoreChar[40];
			sprintf((char*)scoreChar, "You Lose,Score:%d", score);
			print(800, 500, (char*)scoreChar);
		}
		else {
			glColor3f(0.831f, 0.686f, 0.216f);
			char* scoreChar[40];
			sprintf((char*)scoreChar, "You Win,Score:%d", score);
			print(800, 500, (char*)scoreChar);
		}
			
		
	}
	else
	{

	makeBackground();

	//other Fish
	aliveFish = 0;
	for (int i = 0; i < fishes.size(); i++)
		if (fishes.at(i).notEaten) {
			drawOtherFish(fishes.at(i));
			aliveFish++;
		}

	//main fish
	drawFish(mainScale, passX, passY);

	//Score draw
	glColor3f(1, 0, 0);
	char* scoreChar[20];
	sprintf((char*)scoreChar, "%d", score);
	print(10, 1040, (char *) scoreChar);

	}
	glFlush();
}

void passM(int x, int y){
	passX = x - 30 * mainScale;
	passY = 1080 - (y + 17.5 * mainScale);
	glutPostRedisplay();
}

void increaseEaten() {
	eaten++;
	if(eaten >= 6)
		mainScale = 10.5;
	else
	if (eaten >= 3)
		mainScale = 7.5;
}

bool collidedWithMain(Fish f) {

	//other mouth
//	drawRect(1920-f.x- 30*f.scale  , f.y, -10*f.scale,22.5*f.scale);
	//main mouth
//	drawRect(passX + 30 * mainScale, passY, 10 * mainScale, 22.5 * mainScale);



	bool collisionX = passX + 30 * mainScale <= 1920 - f.x - 40 * f.scale && passX + 40 * mainScale >= 1920 - f.x - 40 * f.scale;
	bool collisionY = (passY + 5*mainScale <= f.y + 17.5*f.scale && passY + 5*mainScale >= f.y ) || (passY + 17.5*mainScale >= f.y && passY + 17.5 * mainScale <= f.y +17.5*f.scale)
		|| ( f.y >= passY + 5*mainScale && f.y <= passY + 17.5*mainScale);
	return collisionX&&collisionY;
}


void handleCollisions() {
	for (int i = 0; i < fishes.size(); i++)
		if (fishes.at(i).notEaten && collidedWithMain(fishes.at(i))) {
			collide = 3;
			if (fishes.at(i).scale > mainScale)
				stillAlive = false;
			else {
				fishes.at(i).notEaten = false;
				increaseEaten();
			}
		}
}
void drawRect(int x, int y, int w, int h) {
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(x, y, 0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(w, 0);
	glVertex2f(w, h);
	glVertex2f(0, h);
	glEnd();
	glPopMatrix();
}

void main(int argc, char** argr) {

	glutInit(&argc, argr);
	srand(time(NULL));

	//3 large fish
	for (int i = 0; i < 3; i++) {
		Fish f;
		f.createFish(3, rand() % (1920 / 2), rand() % 500);
		fishes.push_back(f);
	}
	//7 medium fish
	for (int i = 0; i < 7; i++) {
		Fish f;
		f.createFish(2, rand() % (1920 / 2), rand() % 500);
		fishes.push_back(f);
	}
	//10 small fish
	for (int i = 0; i < 10; i++) {
		Fish f;
		f.createFish(1, rand() % (1920 / 2), rand() % 500);
		fishes.push_back(f);
	}
	//bubbles
	for (int i = 1; i <= 1000; i++) {
		Bubble b;
		b.createBubble(rand() % 1980, rand() % 1080);
		bubbles.push_back(b);
	}

	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Feeding Frenzy");
	glutDisplayFunc(Display);
	glutSpecialFunc(special);			//call the keyboard special keys function
	//glutPassiveMotionFunc(passM);		//call the passive motion function
	glutTimerFunc(3000, Timer, 0);			// sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(1, 1, 1, 0);
	gluOrtho2D(0, 1920, 0, 1080);

	glutMainLoop();
}
