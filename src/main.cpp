//This main is for test purposes only

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
	#include <OpenGL/OpenGL.h>
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "engine.h"
#include "renderer.h"
#include "openglrend.h"
#include "scene.h"
#include "camera.h"
#include "transform.h"
#include "entity.h"
#include "cube.h"
#include "sphere.h"
#include "light.h"
#include "material.h"

using namespace std;

#define WIN_W 400
#define WIN_H 400

void handleKeyPress(unsigned char k, int x, int y);
void initRender();
void handleResize(int w, int h);
void render();
void createScene();

int main(int argc, char ** argv)
{
	OpenGLRenderer* r = new OpenGLRenderer;	
	Engine::getInstance().setRenderer(r);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIN_W, WIN_H);

	glutCreateWindow("Demo - Scene Graph");
	initRender();
	createScene();
	
	glutDisplayFunc(render);
	glutKeyboardFunc(handleKeyPress);
	glutReshapeFunc(handleResize);
	
	glutMainLoop();

	return 0;
}

void handleKeyPress(unsigned char k, int x, int y)
{
	switch (k) {
		case 27: //ESC
			exit(0);
	}
}

void initRender()
{
	Engine::getInstance().initRender();
}

void handleResize(int w, int h)
{

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,                  //The camera angle
			(double)w / (double)h, //The width-to-height ratio
			1.0,                   //The near z clipping coordinate
			200.0);                //The far z clipping coordinate
}

void render()
{
	Engine::getInstance().renderCurrentScene();
}

void createScene()
{
	Camera* theCamera = new Camera;
	theCamera->setZPlanes(1.0f, 100.f);
	theCamera->setEye(10.0f, 5.0f, 20.0f);
	theCamera->setUp(0.0f, 1.0f, 0.0f);
	theCamera->setCenter(10.0f, 3.0f, 10.0f);

	Material* redPlastic = new Material(0.5f, 0.0f, 0.0f);
	redPlastic->setSpecular(0.5f, 0.5f, 0.5f);
	redPlastic->setShininess(128);
	
	Material* greenPlastic = new Material(0.0f, 0.5f, 0.0f);
	greenPlastic->setSpecular(0.5f, 0.5f, 0.5f);
	greenPlastic->setShininess(128);

	Entity* box = new Entity(new Cube(5.0f, 1.0f, 5.0f), redPlastic);
	Entity* ball = new Entity(new Sphere(1.0f), greenPlastic);
	
	Transform* transl = new Transform;
	transl->translate(10.0f, 2.5f, 10.0f);
	transl->addNode(box);

	Transform* tr2 = new Transform;
	tr2->translate(0.0f, 1.5f, 0.0f); 
	tr2->addNode(ball);
	transl->addNode(tr2);

	Light* l0 = new Light(10.0f, 20.0f, 10.0f, 1.0f);
	l0->setAmbient(0.2f, 0.2f, 0.2f, 1.0f);
	l0->setDiffuse(0.6f, 0.6f, 0.6f, 1.0f);
	l0->setSpecular(0.5f, 0.5f, 0.5f, 1.0f);

	Scene* theScene = new Scene;
	theScene->addNode(theCamera);
	theScene->addNode(l0);
	theScene->addNode(transl);

	Engine::getInstance().setScene(theScene);
}