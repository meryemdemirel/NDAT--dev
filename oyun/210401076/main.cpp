#include "glew.h"
#include <GL/glut.h>
#include <cstdlib>
#include <memory>
#include <vector>
#include "freeglut.h"
#include "glut.h"
#include<iostream>
#include<math.h>
#include <stdlib.h>



using namespace std;

static void Timer(int value)
{
	glutPostRedisplay();

	glutTimerFunc(1, Timer, value);
}

class Cube
{
public:
	float x;
	float y;
	float width;
	float height;
	float speed;

	Cube(float xIn, float yIn, float wIn, float hIn, float sIn)
	{
		x = xIn;
		y = yIn;
		width = wIn;
		height = hIn;
		speed = sIn;
	}

	virtual void draw() = 0;
	virtual void move() {}
};

class Player :public Cube
{
public:
	int score;
	int life;
	int array[4];
	int level;

	Player(float xIn, float yIn, float wIn, float hIn, float sIn) : Cube(xIn, yIn, wIn, hIn, sIn)
	{
		level = 0;
		score = 0;
		life = 3;
		for (int i = 0; i < 4; i++)
		{
			array[i] = 0;
		}
	}

	void draw()
	{
		glColor3f(1.0, 0.0, 0.7);
		glBegin(GL_POLYGON);
		glVertex3f(x, y, 0.0);
		glVertex3f(x + width, y, 0.0);
		glVertex3f(x + width, y + height, 0.0);
		glVertex3f(x, y + height, 0.0);
		glEnd();
		glFlush();
	}

	float moveIn(float coordinate, float speedIn)
	{
		return coordinate + speedIn;
	}

	void move()
	{
		for (int i = 0; i < 4; i++)
		{
			if (array[i] == 1)
			{
				array[i] = 0;

				if (i == 0)
				{
					x = moveIn(x, -speed);
				}

				if (i == 1)
				{
					x = moveIn(x, +speed);
				}

				if (i == 2)
				{
					y = moveIn(y, -speed);
				}

				if (i == 3)
				{
					y = moveIn(y, +speed);
				}
				if (x <= 0)
				{
					x = 0;
				}
				if (y <= 0)
				{
					y = 0;
				}
				if (x + width >= 480)
				{
					x = 480 - width;
				}
				if (y + height >= 640)
				{
					x = 215;
					y = 95;
					score++;
					level = 1;
				}
			}
		}
	}

	void collision(float xIn, float wIn, float yIn, float hIn)
	{
		if ((xIn >= x && xIn <= x + width) || (xIn + wIn >= x && xIn + wIn <= x + width))
		{
			if ((yIn >= y && yIn <= y + height) || (yIn + hIn >= y && yIn + hIn <= y + height))
			{
				life--;
				x = 215;
				y = 95;
				if (life <= 0)
				{
					exit(0);
				}
			}
		}
	}
};

class Enemy :public Cube
{
public:

	float rColor;
	float gColor;
	float bColor;

	Enemy(float xIn, float yIn, float wIn, float hIn, float sIn, float rIn, float gIn, float bIn) : Cube(xIn, yIn, wIn, hIn, sIn)
	{

		rColor = rIn;
		gColor = gIn;
		bColor = bIn;
	}

	void levelUp()
	{
		speed *= 1.1;
	}

	void move()
	{
		if (speed <= 0)
		{
			if (x >= 0)
			{
				x += speed;
			}
			else {
				speed *= -1;
			}
		}
		else {
			if (x + width <= 480)
			{
				x += speed;
			}
			else {
				speed *= -1;
			}
		}

	}

	void draw()
	{
		glColor3f(rColor, gColor, bColor);
		glBegin(GL_POLYGON);
		glVertex3f(x, y, 0.0);
		glVertex3f(x + width, y, 0.0);
		glVertex3f(x + width, y + height, 0.0);
		glVertex3f(x, y + height, 0.0);
		glEnd();
		glFlush();


		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);

		glEnd();
		glFlush();

		glColor3f(0.0, 0.0, 0.0);
		glLineWidth(7.0);
		glBegin(GL_LINES);
		glVertex2f(0, 220);
		glVertex2f(480, 220);
		glEnd();
		glFlush();


	}
};

shared_ptr<Player> myCube;
vector<shared_ptr<Enemy>> arr;

void klavye(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:

		myCube->array[0] = 1;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:

		myCube->array[1] = 1;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:

		myCube->array[2] = 1;
		glutPostRedisplay();
		break;

	case GLUT_KEY_UP:

		myCube->array[3] = 1;
		glutPostRedisplay();
		break;

	}

}






void render()
{
	glClear(GL_COLOR_BUFFER_BIT);


	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0.0);
	glVertex3f(480, 0, 0.0);
	glVertex3f(480, 240, 0.0);
	glVertex3f(0, 240, 0.0);
	glEnd();
	glFlush();

	myCube->move();

	if (myCube->level == 1)
	{
		myCube->level = 0;
		for (int i = 0; i < arr.size(); i++)
		{
			arr[i]->levelUp();
		}
	}



	for (int i = 0; i < arr.size(); i++)
	{
		arr[i]->move();
		arr[i]->draw();
		myCube->collision(arr[i]->x, arr[i]->width, arr[i]->y, arr[i]->height);
	}

	myCube->draw();


	glColor3f(0, 0, 0);
	glRasterPos2f(300, 630);
	char puantext[4] = { 'P','u','a','n', };
	for (int i = 0; i < 4; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, puantext[i]);
	}

	glColor3f(0, 0, 0);
	glRasterPos2f(0, 630);
	char metintext[26] = { 'C','i','k','m','a','k',' ','i','c','i','n',' ','s','a','g','a',' ','t','i','k','l','a','y','i','n' };
	for (int i = 0; i < 26; i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, metintext[i]);
	}



	glColor3f(0, 0, 0);
	glRasterPos2f(350, 630);
	char puan = myCube->score + '0';
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, puan);

	glColor3f(0, 0, 0);
	glRasterPos2f(400, 630);
	char lifetext[3] = { 'C','a','n', };
	for (int i = 0; i < 3; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, lifetext[i]);
	}

	char life = myCube->life + '0';
	glColor3f(0, 0, 0);
	glRasterPos2f(450, 630);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, life);

	glutSwapBuffers();

}


void myInit()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glColor3f(1.0f, 1.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);



	glOrtho(0.0, 480.0, 0.0, 640.0, -1.0, 1.0);

}

double rotate_y = 0;
double rotate_x = 0;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);

	glBegin(GL_POLYGON); //FRONT
	glColor3f(1.0, 0.5, 0.0);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glEnd();
	// BACK
	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glEnd();
	// RIGHT
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glEnd();
	// LEFT
	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glEnd();
	// TOP
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glEnd();
	// BOTTOM
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glEnd();
	glFlush();
	glutSwapBuffers();

}


void fare(int button, int state, int x, int y) {
	int secim = 0;
	if (GLUT_RIGHT_BUTTON) {

		std::cout << "Menu\n";
		std::cout << "Cikmak icin 1'e basiniz\n";
		std::cin >> secim;

	}
	switch (secim) {
	case 1:
		exit(0);
		break;

	}
}

void specialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)
		rotate_y += 5;
	else if (key == GLUT_KEY_LEFT)
		rotate_y -= 5;
	else if (key == GLUT_KEY_UP)
		rotate_x += 5;
	else if (key == GLUT_KEY_DOWN)
		rotate_x -= 5;
	glutPostRedisplay();
}


int main(int argc, char** argv)
{

	int secim;
	std::cout << "*Menu*\n";
	std::cout << "Hazirlayanlar: Hale Erdol 210401093, Meryem Demirel 210401076, Munire Aksoy 210401044\n";
	std::cout << "1-OYUN (Oyun yon tuslariyla oynanir)\n";
	std::cout << "2-3D Modelleme ( Yon tuslariyla bakilir)\n";
	std::cout << "Seciminizi yapin:";
	std::cin >> secim;

	switch (secim) {
	case 1:
		myCube = make_shared<Player>(215, 95, 50, 50, 10);

		arr.push_back(make_shared<Enemy>(430, 320, 50, 50, 0.3, 1, 1, 0));
		arr.push_back(make_shared<Enemy>(430, 400, 50, 50, 0.4, 0, 1, 0));
		arr.push_back(make_shared<Enemy>(0, 470, 50, 50, 0.5, 1, 0, 0));

		glutInit(&argc, argv);

		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

		glutInitWindowSize(480, 640);

		glutInitWindowPosition(0, 0);

		glutCreateWindow("Kutu Kutu Pense");

		myInit();

		glutSpecialFunc(klavye);
		glutMouseFunc(fare);

		glutDisplayFunc(render);

		Timer(0);

		glutMainLoop();

		return 0;

	case 2:
		glutInit(&argc, argv);

		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

		glutCreateWindow("Kup Cizimi");

		glEnable(GL_DEPTH_TEST);

		glutDisplayFunc(display);

		glutSpecialFunc(specialKeys);

		glutMainLoop();

		return 0;

	}
}