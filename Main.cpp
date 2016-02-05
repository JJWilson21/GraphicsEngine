/* Graphics Engine
   Uses OpenGL 3.3 technology
   by Jeremiah Wilson
*/

/******************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
// Use GLEW so we can access the latest OpenGL functionality
// Always include GLEW before GLFW!
#include <GL/glew.h>
// Use GLFW so we can create windows and handle events in a
//   platform-independent way
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>


/******************************************************************/
// Local includes
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Vector3.h"
#include "Camera.h"
#include "Matrix3.h"
#include "AiScene.h"
#include "KeyBuffer.h"
#include "Matrix4.h"
#include "Skybox.h"
#include "Scene.h"
#include "Light.h"
#include "Material.h"
#include "Model.h"


/******************************************************************/
// Type declarations/globals vars/prototypes
ShaderProgram g_shaderProgram;
ShaderProgram g_shaderSkybox;

KeyBuffer keys;

Camera* camera;

GLFWwindow* window;

Material natural;

// Projection (defines view volume)
Math::Matrix4 g_projection;
Math::Matrix4 g_skyboxProjection;

bool isPitchYaw = 0;
bool isRoll = 0;
bool wireframe = false;
bool kicked= false;
bool held = false;
bool bounce = false;

double xOrigin;
double yOrigin;

size_t matIndex = 0;

float colorWeight = 0.0f;
float speed = 0;
float MOVEMENT_DELTA = 0.01f;
float x = 9.8f;
float power = 1;

Skybox* skybox;

std::vector<Material> mats;

Scene* scene = new Scene();

Mesh* ball;
Mesh* rus;
Mesh* rus2;
Mesh* rus3;
Mesh* rus4;
Mesh* rus5;


/******************************************************************/

void
init (GLFWwindow*& window);

void
initGlfw ();

void
initGlew ();

void
initWindow (GLFWwindow*& window);

void
resetViewport (GLFWwindow* window, int width, int height);

void
initScene ();

void
initShaders ();

void
initCamera ();

void
initProjection();

void
initMesh ();

void
initSkybox();

void
drawSkybox();

void
updateScene (double time);

void
drawScene (GLFWwindow* window);

void
drawObject ();

void
processKey ();


void
bufferKeys (GLFWwindow* window, int key, int scanCode, int action, int modifiers);

void
processMousePosition (GLFWwindow* window, double x, double y);

void
processMouseButton (GLFWwindow* window, int button, int action, int modifiers);

void
processScrolling(GLFWwindow* window, double xOffSet, double yOffSet);

void
releaseGlResources ();

void
handleGlfwError (int error, const char* description);



/******************************************************************/

int
main (int argc, char* argv[])
{
  
  init (window);
  // Game/render loop
  double previousTime = glfwGetTime ();
  while (!glfwWindowShouldClose (window))
  {
    processKey ();
    double currentTime = glfwGetTime ();
    double deltaTime = currentTime - previousTime;
    previousTime = currentTime;
    updateScene (deltaTime);
    drawScene (window);

    glfwPollEvents ();
  }

  releaseGlResources ();
  glfwDestroyWindow (window);
  glfwTerminate ();

  return EXIT_SUCCESS;
}

/******************************************************************/

void
init (GLFWwindow*& window)
{
  // Always initialize GLFW before GLEW
  initGlfw ();
  initWindow (window);
  initGlew ();
  initScene ();
  initSkybox();
}

/******************************************************************/

void
initGlfw ()
{
  glfwSetErrorCallback (handleGlfwError);
  if (!glfwInit ())
  {
    fprintf (stderr, "Failed to initialize GLFW\n");
    exit (-1);
  }
}

/******************************************************************/

void
initWindow (GLFWwindow*& window)
{
#ifdef __APPLE__
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
  window = glfwCreateWindow (1200, 700, "OpenGL", nullptr, nullptr);
  glfwSetWindowPos (window, 100, 20);
  if (!window)
  {
    fprintf (stderr, "Failed to initialize the window\n");
    glfwTerminate ();
    exit (-1);
  }

  glfwMakeContextCurrent (window);
  // Swap buffers after 1 frame
  glfwSwapInterval (1);
  // glfwSetKeyCallback (window, processKey);
  glfwSetKeyCallback (window, bufferKeys);
  // GMZ: Don't track mouse movement yet
  glfwSetCursorPosCallback (window, processMousePosition);
  glfwSetMouseButtonCallback (window, processMouseButton);
  glfwSetScrollCallback(window, processScrolling);
  glfwSetFramebufferSizeCallback (window, resetViewport);

  // Specify background color
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
  // Enable depth testing so we don't draw occluded surfaces
  glEnable (GL_DEPTH_TEST);
  // Cull back-facing triangles
  glEnable (GL_CULL_FACE);
  // Set initial viewport size
  int width, height;
  glfwGetFramebufferSize (window, &width, &height);
  glViewport (0, 0, width, height);
}

/******************************************************************/

void
initGlew ()
{
  // Necessary to use more recent OpenGL features
  glewExperimental = GL_TRUE;
  GLenum status = glewInit ();
  if (status != GLEW_OK)
  {
    fprintf (stderr, "Failed to initialize GLEW"
             " (%s).\n",
             glewGetErrorString (status));
    exit (-1);
  }
  auto version = glewGetString (GLEW_VERSION);
  fprintf (stderr, "Using GLEW version %s.\n", version);
  version = glGetString (GL_VERSION);
  fprintf (stderr, "Using OpenGL version %s\n", version);
}

/******************************************************************/

void
resetViewport (GLFWwindow* window, int width, int height)
{
  // Render into entire window
  // Origin for window coordinates is lower-left of window,
  //   NOT upper-left like screen coordinates
  camera->resetAspectRatio(width, height);
  g_projection = camera->getPrespectiveProjectionMatrix();
  initProjection();
  glViewport (0, 0, width, height);

}

/******************************************************************/

void
initScene ()
{
  // Compile and link all shader programs
  //Initialize Camera, and scene lighting
  initShaders ();
  initCamera ();
  Light light;
  light.l.ambient.set(0.2, 0.2, 0.2);
  light.l.diffuse.set(0.5, 0.5, 0.5);
  light.l.specular.set(1 ,1, 1);
  light.l.direction.set(0, 0, 1);
  scene->setLighting(light);
  scene->setShaderProgram(g_shaderProgram);
  GLint colorWeightLocation = g_shaderProgram.getUniformLocation("colorWeight");
  glUniform1f(colorWeightLocation, colorWeight);
  initMesh ();
}

/******************************************************************/
void
initMesh ()
{
  //Initialize Materials for meshes to utilize for cycling through materials for active mesh
  Material emerald;
  emerald.mat.ambientReflection.set(0.0215f, 0.1745f, 0.0215f);
  emerald.mat.diffuseReflection.set(0.07568f, 0.61424f, 0.07568);
  emerald.mat.specularReflection.set(0.633f, 0.727811f, 0.633f);
  emerald.mat.shininess = 0.6f;
  mats.push_back(emerald);
  
  Material gold;
  gold.mat.ambientReflection.set(0.24725, 0.1995, 0.0745);
  gold.mat.diffuseReflection.set(0.75164, 0.60648, 0.22648);
  gold.mat.specularReflection.set(0.628281, 0.555802, 0.366065);
  gold.mat.shininess = 0.4f;
  mats.push_back(gold);


  natural.mat.ambientReflection.set(0.5, 0.5, 0.5);
  natural.mat.diffuseReflection.set(0, 0, 0);
  natural.mat.specularReflection.set(0.95, 0.95, 0.95);
  natural.mat.shininess = 0.9f;

  Material ruby;
  ruby.mat.ambientReflection.set(0.1745,0.01175,0.01175);
  ruby.mat.diffuseReflection.set(0.61424,0.04136,0.0136);
  ruby.mat.specularReflection.set(0.727811,0.626959,0.626959);
  ruby.mat.shininess = 0.6f;
  mats.push_back(ruby);


  Material chrome;
  chrome.mat.ambientReflection.set(0.25,0.25,0.25);
  chrome.mat.diffuseReflection.set(0.4,0.4,0.4);
  chrome.mat.specularReflection.set(0.774597,0.774597,0.774597);
  chrome.mat.shininess = 0.6f;
  mats.push_back(chrome);

  Material turquoise;
  turquoise.mat.ambientReflection.set(0.1, 0.18725, 0.7145);
  turquoise.mat.diffuseReflection.set(0.396, 0.74151, 0.69102);
  turquoise.mat.specularReflection.set(0.297254, 0.30829, 0.306678);
  turquoise.mat.shininess = 0.1;
  mats.push_back(turquoise);

  
  //Create field and all objects in scene
  AiScene fieldScene("plane.obj");
  Mesh* field = new Mesh();
  field->setTexture("field.jpg");
  field->setMaterial(natural);
  field->addGeometry(fieldScene.readVertexData(0));
  field->prepareVao();
  field->setShaderProgram(g_shaderProgram);
  field->scaleLocal(0.5f,0.5f,0.5f);
  field->pitch(-90);


  Mesh* wallLeft1 = new Mesh();
  wallLeft1->setTexture("cobble.jpg");
  wallLeft1->setMaterial(natural);
  wallLeft1->addGeometry(fieldScene.readVertexData(0));
  wallLeft1->prepareVao();
  wallLeft1->setShaderProgram(g_shaderProgram);
  wallLeft1->scaleLocal(0.1f, 0.03f, 0.1f);
  wallLeft1->yaw(90);
  wallLeft1->moveBack(-90);
  wallLeft1->moveUp(20);
  wallLeft1->moveRight(80);

  Mesh* wallLeft2 = new Mesh();
  wallLeft2->setTexture("cobble.jpg");
  wallLeft2->setMaterial(natural);
  wallLeft2->addGeometry(fieldScene.readVertexData(0));
  wallLeft2->prepareVao();
  wallLeft2->setShaderProgram(g_shaderProgram);
  wallLeft2->scaleLocal(0.1f, 0.03f, 0.1f);
  wallLeft2->yaw(90);
  wallLeft2->moveBack(-90);
  wallLeft2->moveUp(20);
  wallLeft2->moveRight(40);

  Mesh* wallLeft3 = new Mesh();
  wallLeft3->setTexture("cobble.jpg");
  wallLeft3->setMaterial(natural);
  wallLeft3->addGeometry(fieldScene.readVertexData(0));
  wallLeft3->prepareVao();
  wallLeft3->setShaderProgram(g_shaderProgram);
  wallLeft3->scaleLocal(0.1f, 0.03f, 0.1f);
  wallLeft3->yaw(90);
  wallLeft3->moveBack(-90);
  wallLeft3->moveUp(20);

  Mesh* wallLeft4 = new Mesh();
  wallLeft4->setTexture("cobble.jpg");
  wallLeft4->setMaterial(natural);
  wallLeft4->addGeometry(fieldScene.readVertexData(0));
  wallLeft4->prepareVao();
  wallLeft4->setShaderProgram(g_shaderProgram);
  wallLeft4->scaleLocal(0.1f, 0.03f, 0.1f);
  wallLeft4->yaw(90);
  wallLeft4->moveBack(-90);
  wallLeft4->moveUp(20);
  wallLeft4->moveRight(-40);

  Mesh* wallLeft5 = new Mesh();
  wallLeft5->setTexture("cobble.jpg");
  wallLeft5->setMaterial(natural);
  wallLeft5->addGeometry(fieldScene.readVertexData(0));
  wallLeft5->prepareVao();
  wallLeft5->setShaderProgram(g_shaderProgram);
  wallLeft5->scaleLocal(0.1f, 0.03f, 0.1f);
  wallLeft5->yaw(90);
  wallLeft5->moveBack(-90);
  wallLeft5->moveUp(20);
  wallLeft5->moveRight(-80);



  Mesh* wallRight = new Mesh();
  wallRight->setTexture("cobble.jpg");
  wallRight->setMaterial(natural);
  wallRight->addGeometry(fieldScene.readVertexData(0));
  wallRight->prepareVao();
  wallRight->setShaderProgram(g_shaderProgram);
  wallRight->scaleLocal(0.1f, 0.03f, 0.1f);
  wallRight->yaw(-90);
  wallRight->moveBack(-90);
  wallRight->moveUp(20);
  wallRight->moveRight(80);

  Mesh* wallRight2 = new Mesh();
  wallRight2->setTexture("cobble.jpg");
  wallRight2->setMaterial(natural);
  wallRight2->addGeometry(fieldScene.readVertexData(0));
  wallRight2->prepareVao();
  wallRight2->setShaderProgram(g_shaderProgram);
  wallRight2->scaleLocal(0.1f, 0.03f, 0.1f);
  wallRight2->yaw(-90);
  wallRight2->moveBack(-90);
  wallRight2->moveUp(20);
  wallRight2->moveRight(40);

  Mesh* wallRight3 = new Mesh();
  wallRight3->setTexture("cobble.jpg");
  wallRight3->setMaterial(natural);
  wallRight3->addGeometry(fieldScene.readVertexData(0));
  wallRight3->prepareVao();
  wallRight3->setShaderProgram(g_shaderProgram);
  wallRight3->scaleLocal(0.1f, 0.03f, 0.1f);
  wallRight3->yaw(-90);
  wallRight3->moveBack(-90);
  wallRight3->moveUp(20);
  wallRight3->moveRight(0);

  Mesh* wallRight4 = new Mesh();
  wallRight4->setTexture("cobble.jpg");
  wallRight4->setMaterial(natural);
  wallRight4->addGeometry(fieldScene.readVertexData(0));
  wallRight4->prepareVao();
  wallRight4->setShaderProgram(g_shaderProgram);
  wallRight4->scaleLocal(0.1f, 0.03f, 0.1f);
  wallRight4->yaw(-90);
  wallRight4->moveBack(-90);
  wallRight4->moveUp(20);
  wallRight4->moveRight(-40);

  Mesh* wallRight5 = new Mesh();
  wallRight5->setTexture("cobble.jpg");
  wallRight5->setMaterial(natural);
  wallRight5->addGeometry(fieldScene.readVertexData(0));
  wallRight5->prepareVao();
  wallRight5->setShaderProgram(g_shaderProgram);
  wallRight5->scaleLocal(0.1f, 0.03f, 0.1f);
  wallRight5->yaw(-90);
  wallRight5->moveBack(-90);
  wallRight5->moveUp(20);
  wallRight5->moveRight(-80);

  Mesh* wallBack = new Mesh();
  wallBack->setTexture("cobble.jpg");
  wallBack->setMaterial(natural);
  wallBack->addGeometry(fieldScene.readVertexData(0));
  wallBack->prepareVao();
  wallBack->setShaderProgram(g_shaderProgram);
  wallBack->scaleLocal(0.1f, 0.03f, 0.1f);
  wallBack->yaw(180);
  wallBack->moveBack(-90);
  wallBack->moveUp(20);
  wallBack->moveRight(80);

  Mesh* wallBack2 = new Mesh();
  wallBack2->setTexture("cobble.jpg");
  wallBack2->setMaterial(natural);
  wallBack2->addGeometry(fieldScene.readVertexData(0));
  wallBack2->prepareVao();
  wallBack2->setShaderProgram(g_shaderProgram);
  wallBack2->scaleLocal(0.1f, 0.03f, 0.1f);
  wallBack2->yaw(180);
  wallBack2->moveBack(-90);
  wallBack2->moveUp(20);
  wallBack2->moveRight(40);

  Mesh* wallBack3 = new Mesh();
  wallBack3->setTexture("cobble.jpg");
  wallBack3->setMaterial(natural);
  wallBack3->addGeometry(fieldScene.readVertexData(0));
  wallBack3->prepareVao();
  wallBack3->setShaderProgram(g_shaderProgram);
  wallBack3->scaleLocal(0.1f, 0.03f, 0.1f);
  wallBack3->yaw(180);
  wallBack3->moveBack(-90);
  wallBack3->moveUp(20);
  wallBack3->moveRight(0);

  Mesh* wallBack4 = new Mesh();
  wallBack4->setTexture("cobble.jpg");
  wallBack4->setMaterial(natural);
  wallBack4->addGeometry(fieldScene.readVertexData(0));
  wallBack4->prepareVao();
  wallBack4->setShaderProgram(g_shaderProgram);
  wallBack4->scaleLocal(0.1f, 0.03f, 0.1f);
  wallBack4->yaw(180);
  wallBack4->moveBack(-90);
  wallBack4->moveUp(20);
  wallBack4->moveRight(-40);

  Mesh* wallBack5 = new Mesh();
  wallBack5->setTexture("cobble.jpg");
  wallBack5->setMaterial(natural);
  wallBack5->addGeometry(fieldScene.readVertexData(0));
  wallBack5->prepareVao();
  wallBack5->setShaderProgram(g_shaderProgram);
  wallBack5->scaleLocal(0.1f, 0.03f, 0.1f);
  wallBack5->yaw(180);
  wallBack5->moveBack(-90);
  wallBack5->moveUp(20);
  wallBack5->moveRight(-80);


  Mesh* wallFront1 = new Mesh();
  wallFront1->setTexture("cobble.jpg");
  wallFront1->setMaterial(natural);
  wallFront1->addGeometry(fieldScene.readVertexData(0));
  wallFront1->prepareVao();
  wallFront1->setShaderProgram(g_shaderProgram);
  wallFront1->scaleLocal(0.1f, 0.03f, 0.1f);
  wallFront1->moveUp(20);
  wallFront1->moveBack(-100);
  wallFront1->moveRight(80);

  Mesh* wallFront2 = new Mesh();
  wallFront2->setTexture("cobble.jpg");
  wallFront2->setMaterial(natural);
  wallFront2->addGeometry(fieldScene.readVertexData(0));
  wallFront2->prepareVao();
  wallFront2->setShaderProgram(g_shaderProgram);
  wallFront2->scaleLocal(0.1f, 0.03f, 0.1f);
  wallFront2->moveUp(20);
  wallFront2->moveBack(-100);
  wallFront2->moveRight(40);

  Mesh* wallFront3 = new Mesh();
  wallFront3->setTexture("cobble.jpg");
  wallFront3->setMaterial(natural);
  wallFront3->addGeometry(fieldScene.readVertexData(0));
  wallFront3->prepareVao();
  wallFront3->setShaderProgram(g_shaderProgram);
  wallFront3->scaleLocal(0.1f, 0.03f, 0.1f);
  wallFront3->moveUp(20);
  wallFront3->moveBack(-100);
  wallFront3->moveRight(-40);

  Mesh* wallFront4 = new Mesh();
  wallFront4->setTexture("cobble.jpg");
  wallFront4->setMaterial(natural);
  wallFront4->addGeometry(fieldScene.readVertexData(0));
  wallFront4->prepareVao();
  wallFront4->setShaderProgram(g_shaderProgram);
  wallFront4->scaleLocal(0.1f, 0.03f, 0.1f);
  wallFront4->moveUp(20);
  wallFront4->moveBack(-100);
  wallFront4->moveRight(-80);

  Mesh* goal1 = new Mesh();
  goal1->setTexture("net.jpg");
  goal1->setMaterial(natural);
  goal1->addGeometry(fieldScene.readVertexData(0));
  goal1->prepareVao();
  goal1->setShaderProgram(g_shaderProgram);
  goal1->scaleLocal(0.1f, 0.03f, 0.1f);
  goal1->moveUp(20);
  goal1->moveBack(-110);
  

  AiScene ballScene("Sphere.obj");
  ball = new Mesh();
  ball->setTexture("ballTexture.jpg");
  ball->setMaterial(natural);
  ball->addGeometry(ballScene.readVertexData(0));
  ball->prepareVao();
  ball->setShaderProgram(g_shaderProgram);
  ball->scaleLocal(0.1f, 0.1, 0.1f);
  ball->moveUp(2);

  Mesh* goal2 = new Mesh();
  goal2->setTexture("net.jpg");
  goal2->setMaterial(natural);
  goal2->addGeometry(fieldScene.readVertexData(0));
  goal2->prepareVao();
  goal2->setShaderProgram(g_shaderProgram);
  goal2->scaleLocal(0.1f, 0.03f, 0.1f);
  goal2->moveUp(20);
  goal2->moveBack(-110);
  goal2->pitch(90);
  goal2->moveBack(-20);
  goal2->moveUp(-10);

  Mesh* goal3 = new Mesh();
  goal3->setTexture("field.jpg");
  goal3->setMaterial(natural);
  goal3->addGeometry(fieldScene.readVertexData(0));
  goal3->prepareVao();
  goal3->setShaderProgram(g_shaderProgram);
  goal3->scaleLocal(0.1f, 0.03f, 0.1f);
  goal3->moveUp(20);
  goal3->moveBack(-110);
  goal3->pitch(-90);
  goal3->moveBack(-17);
  goal3->moveUp(-10);

  Mesh* goal4 = new Mesh();
  goal4->setTexture("net.jpg");
  goal4->setMaterial(natural);
  goal4->addGeometry(fieldScene.readVertexData(0));
  goal4->prepareVao();
  goal4->setShaderProgram(g_shaderProgram);
  goal4->scaleLocal(0.1f, 0.03f, 0.1f);
  goal4->moveUp(20);
  goal4->moveBack(-110);
  goal4->yaw(90);
  goal4->moveBack(-20);
  goal4->moveRight(9.35f);

  Mesh* goal5 = new Mesh();
  goal5->setTexture("net.jpg");
  goal5->setMaterial(natural);
  goal5->addGeometry(fieldScene.readVertexData(0));
  goal5->prepareVao();
  goal5->setShaderProgram(g_shaderProgram);
  goal5->scaleLocal(0.1f, 0.03f, 0.1f);
  goal5->moveUp(20);
  goal5->moveBack(-110);
  goal5->yaw(-90);
  goal5->moveBack(-21);
  goal5->moveRight(-10.4f);

  AiScene grassScene1("grass.obj");
  Mesh* grass1 = new Mesh();
  grass1->setTexture("field.jpg");
  grass1->setMaterial(natural);
  grass1->addGeometry(grassScene1.readVertexData(0));
  grass1->prepareVao();
  grass1->setShaderProgram(g_shaderProgram);
  scene->add(grass1, "grass1");
  grass1->scaleLocal(0.5f, 0.2f, 0.5f);
  grass1->pitch(-90);

  AiScene russ("Hands.obj");
  rus = new Mesh();
  rus->setTexture("white.jpg");
  rus->setMaterial(natural);
  rus->addGeometry(russ.readVertexData(0));
  rus->prepareVao();
  rus->setShaderProgram(g_shaderProgram);
  scene->add(rus, "rus");
  rus->scaleLocal(0.01f, 0.01f, 0.01f);
  rus->moveUp(3);
  rus->roll(90);
  rus->yaw(-25);
  rus->moveBack(-300);
  rus->moveUp(1000);

  rus2 = new Mesh();
  rus2->setTexture("white.jpg");
  rus2->setMaterial(natural);
  rus2->addGeometry(russ.readVertexData(0));
  rus2->prepareVao();
  rus2->setShaderProgram(g_shaderProgram);
  scene->add(rus2, "rus2");
  rus2->scaleLocal(0.01f, 0.01f, 0.01f);
  rus2->moveUp(3);
  rus2->roll(90);
  rus2->moveBack(-300);
  rus2->moveUp(1000);
  rus2->moveBack(600);
  rus2->moveRight(120);
  rus2->yaw(-25);

  rus3 = new Mesh();
  rus3->setTexture("brown.jpg");
  rus3->setMaterial(natural);
  rus3->addGeometry(russ.readVertexData(0));
  rus3->prepareVao();
  rus3->setShaderProgram(g_shaderProgram);
  scene->add(rus3, "rus3");
  rus3->scaleLocal(0.01f, 0.01f, 0.01f);
  rus3->moveUp(3);
  rus3->roll(90);
  rus3->moveBack(-300);
  rus3->moveUp(1000);
  rus3->moveBack(-600);
  rus3->moveRight(120);
  rus3->yaw(-25);

  rus4 = new Mesh();
  rus4->setTexture("white.jpg");
  rus4->setMaterial(natural);
  rus4->addGeometry(russ.readVertexData(0));
  rus4->prepareVao();
  rus4->setShaderProgram(g_shaderProgram);
  scene->add(rus4, "rus4");
  rus4->scaleLocal(0.01f, 0.01f, 0.01f);
  rus4->moveUp(3);
  rus4->roll(90);
  rus4->moveBack(-300);
  rus4->moveUp(1000);
  rus4->moveBack(-300);
  rus4->moveRight(120);
  rus4->yaw(-25);

  rus5 = new Mesh();
  rus5->setTexture("brown.jpg");
  rus5->setMaterial(natural);
  rus5->addGeometry(russ.readVertexData(0));
  rus5->prepareVao();
  rus5->setShaderProgram(g_shaderProgram);
  scene->add(rus5, "rus5");
  rus5->scaleLocal(0.01f, 0.01f, 0.01f);
  rus5->moveUp(3);
  rus5->roll(90);
  rus5->moveBack(-300);
  rus5->moveUp(1000);
  rus5->moveBack(300);
  rus5->moveRight(120);
  rus5->yaw(-25);

  
//Add all meshes to scene to draw as one
  scene->add(goal1, "goal1");
  scene->add(goal2, "goal2");
  scene->add(goal3, "goal3");
  scene->add(goal4, "goal4");
  scene->add(goal5, "goal5");
  scene->add(wallFront1, "wallFront1");
  scene->add(wallFront2, "wallFront2");
  scene->add(wallFront3, "wallFront3");
  scene->add(wallFront4, "wallFront4");
  scene->add(wallBack, "wallBack");
  scene->add(wallBack2, "wallBack2");
  scene->add(wallBack3, "wallBack3");
  scene->add(wallBack4, "wallBack4");
  scene->add(wallBack5, "wallBack5");
  scene->add(wallRight, "wallRight");
  scene->add(wallRight2, "wallRight2");
  scene->add(wallRight3, "wallRight3");
  scene->add(wallRight4, "wallRight4");
  scene->add(wallRight5, "wallRight5");
  scene->add(wallLeft1, "wallLeft");
  scene->add(wallLeft2, "wallLeft2");
  scene->add(wallLeft3, "wallLeft3");
  scene->add(wallLeft4, "wallLeft4");
  scene->add(wallLeft5, "wallLeft5");
  scene->add(ball, "ball");
  scene->add(field, "field");

  
  
}

/******************************************************************/

void
initShaders ()
{

  g_shaderProgram.createProgramId ();
  g_shaderProgram.createVertexShader("Vec3NormTexPhong.vert");
  g_shaderProgram.createFragmentShader("Vec3TexMix.frag");
  g_shaderProgram.link ();
  g_shaderSkybox.createProgramId();
  g_shaderSkybox.createVertexShader("Skybox.vert");
  g_shaderSkybox.createFragmentShader("Skybox.frag");
  g_shaderSkybox.link();
}

/******************************************************************/

void
initCamera ()
{
  camera = new Camera ();
  camera->setEyePoint(0.0f,0.7f,6.0f);
  camera->setBackVec (0, 0, 1.0f);
  camera->setUpVec (0, 1.0f, 0);
  camera->setRightVec (1.0f, 0, 0);
  g_projection = camera->getPrespectiveProjectionMatrix();
  initProjection();     
}

/******************************************************************/
void
initProjection()
{
  // Enable shader program so we can set uniforms
  g_shaderProgram.enable();
  GLint projectionLoc = g_shaderProgram.getUniformLocation("vProjection");
  float projectionMatrix[16];
  g_projection.getAsArray(projectionMatrix);
  g_shaderProgram.setUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMatrix);
  g_shaderProgram.disable();
  
}

/******************************************************************/

void
initSkybox()
{
  skybox = new Skybox();
  skybox->setFront("negz.jpg");
  skybox->setRight("negx.jpg");
  skybox->setLeft("posx.jpg");
  skybox->setBack("posz.jpg");
  skybox->setTop("posy.jpg");
  skybox->setBottom("bottom.jpg");
  skybox->setShaderProgram(g_shaderProgram);
}


/******************************************************************/

/******************************************************************/

void
updateScene (double time)
{
  // AI

  // Physics
        
		
  //Used for determining the fans arms movement
  if (x >= 5.0f)
  {
    rus->yaw(2);
    rus2->yaw(2.2f);
    rus3->yaw(1.5f);
    rus4->yaw(2.2f);
    rus5->yaw(2);
    x -= 0.2f;
  }
  else if(x >=0.0f && x < 5){
    rus->yaw(-2);
    rus2->yaw(-2.2f);
    rus3->yaw(-1.5f);
    rus4->yaw(-2.2f);
    rus5->yaw(-2);
    x -= 0.2f;
  }
  else{
    x = 9.8f;
  }


  //Makeshift physics, no going through walls
  if (camera->getEyePoint().x < -8.75f)
  {
    camera->setEyePoint(-8.75f, camera->getEyePoint().y, camera->getEyePoint().z);
  }
  if (camera->getEyePoint().x > 8.75f)
  {
    camera->setEyePoint(8.75f, camera->getEyePoint().y, camera->getEyePoint().z);
  }
  if (camera->getEyePoint().z > 8.75f)
  {
    camera->setEyePoint(camera->getEyePoint().x, camera->getEyePoint().y, 8.75f);
  }
  if (camera->getEyePoint().z < -9.75f)
  {
    camera->setEyePoint(camera->getEyePoint().x, camera->getEyePoint().y, -9.75f);
  }
  if (ball->getTransform().getPosition().x > 8.75f && !held)
  {
    Vector3 newPos;
    newPos.set(8.75f, ball->getTransform().getPosition().y, ball->getTransform().getPosition().z);
    ball->setTransform(ball->getTransform().getOrientation(), newPos);
  }
  if (ball->getTransform().getPosition().x < -8.75f && !held)
  {
    Vector3 newPos;
    newPos.set(-8.75f, ball->getTransform().getPosition().y, ball->getTransform().getPosition().z);
    ball->setTransform(ball->getTransform().getOrientation(), newPos);
  }
  if (ball->getTransform().getPosition().z > 8.75f)
  {
    Vector3 newPos;
    newPos.set(ball->getTransform().getPosition().x, ball->getTransform().getPosition().y, 8.75f);
    ball->setTransform(ball->getTransform().getOrientation(), newPos);
  }
        
  if (ball->getTransform().getPosition().z < -9.75f && (ball->getTransform().getPosition().x < -1.8f || ball->getTransform().getPosition().x > 1.8f))
  {
    Vector3 newPos;
    newPos.set(ball->getTransform().getPosition().x, ball->getTransform().getPosition().y, -9.75f);
    ball->setTransform(ball->getTransform().getOrientation(), newPos);
  }
  if (ball->getTransform().getPosition().z < -10.5f)
  {
    Vector3 newPos;
    newPos.set(ball->getTransform().getPosition().x, ball->getTransform().getPosition().y, -10.75f);
    ball->setTransform(ball->getTransform().getOrientation(), newPos);
    power = 1;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    newPos.set(0,0.2f,0);
    Math::Matrix3 scaler;
    scaler.setToScale(0.1f);
    Math::Matrix3 newOrient(camera->getViewMatrix().getOrientation());
    held = false;
    newOrient *= scaler;
    ball->setTransform(newOrient, newPos);
    camera->reset();
    speed = 0;
  }
  
  //Determine if we can pick up the ball and attach it to the camera
  if (ball->getTransform().getPosition().x < camera->getEyePoint().x + 1 && ball->getTransform().getPosition().x > camera->getEyePoint().x - 1)
  {
    if (ball->getTransform().getPosition().z > camera->getEyePoint().z - 1 && ball->getTransform().getPosition().z < camera->getEyePoint().z)
    {
      held = true;
    }
  }
  //Lock ball to camera
  if (held && !kicked && !(camera->getEyePoint().x > 9 || camera->getEyePoint().x < -9 || camera->getEyePoint().z > 9 || camera->getEyePoint().z < -9.5f))
  {
    Vector3 newPos;
    newPos.set(camera->getEyePoint().x, camera->getEyePoint().y - 0.5f, camera->getEyePoint().z - 1.5f);
    ball->setTransform(ball->getTransform().getOrientation(), newPos);
  }
  //If the ball has stopped moving after kicked, set it to be not kicked
  if (speed <= 0)
  {
    kicked = false;
  }
  //on Mouse1 kick the ball
  if (kicked)
  {
    Math::Matrix3 scaler(1,0,0,0,1,0,0,0,1);
    //scaler.setToIdentity();
    scaler.setToScale(0.1f);
    Vector3 b;
    Vector3 r;
    Vector3 u;
    b.set(camera->getBackVec().x, camera->getBackVec().y, camera->getBackVec().z);
    r.set(camera->getRightVec().x, camera->getRightVec().y, camera->getRightVec().z);
    u.set(camera->getUpVec().x, camera->getUpVec().y, camera->getUpVec().z);
    Math::Matrix3 other(r,u,b);
    held = false;
    other *= scaler;
    ball->setTransform(other, ball->getTransform().getPosition());
    ball->moveBack(speed);
    speed -=0.1f;
  
  }

        
        
}

/******************************************************************/

void
drawScene (GLFWwindow* window)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  drawObject ();

  // Swap the front and back buffers
  glfwSwapBuffers (window);
}

/******************************************************************/

void
drawObject ()
{
  // The shader program is already enabled, but we do not want to
  //   make that assumption in general
  g_shaderProgram.enable ();

  scene->draw(*camera);
  
  skybox->draw(*camera);
  
  g_shaderProgram.disable ();
}

/******************************************************************/

void
processKey ()
{
  
  const float PITCH_YAW_DELTA = 1;

  
 //Movement / Sprint
  if (keys.isDown (GLFW_KEY_W))
  {
    camera->moveLocalBack (MOVEMENT_DELTA);
    skybox->moveBack(MOVEMENT_DELTA);
    if (held){
      ball->pitch(-1);
    }
  }
  if (keys.isDown(GLFW_KEY_LEFT_SHIFT))
  {
    MOVEMENT_DELTA = 0.05f;
  }else
    MOVEMENT_DELTA = 0.01f;
  if (keys.isDown (GLFW_KEY_S))
  {
    camera->moveLocalBack (-MOVEMENT_DELTA);
    skybox->moveBack(-MOVEMENT_DELTA);
    if (held)
    {
      ball->pitch(1);
    }
  }
  if (keys.isDown (GLFW_KEY_D))
  {
    camera->moveLocalRight (MOVEMENT_DELTA);
    skybox->moveRight(MOVEMENT_DELTA);
    if (held)
    {
      ball->roll(-1);
    }
  }
  if (keys.isDown (GLFW_KEY_A))
  {
    camera->moveLocalRight (-MOVEMENT_DELTA);
    skybox->moveRight(-MOVEMENT_DELTA);
    if (held)
    {
      ball->roll(1);
    }
  }
  //Turning
  if (keys.isDown (GLFW_KEY_Q))
  {

    camera->yaw (-PITCH_YAW_DELTA/2);
  }
  if (keys.isDown (GLFW_KEY_E))
  {
    camera->yaw (PITCH_YAW_DELTA/2);
  }
  
  if (keys.isDown (GLFW_KEY_R))
  {
    camera->reset ();
  }
 
  if (keys.isDown (GLFW_KEY_ESCAPE))
  {
    glfwSetWindowShouldClose (window, GL_TRUE);
    return;
  }
        
        
}

/******************************************************************/
void
bufferKeys (GLFWwindow* window, int key, int scanCode, int action,
            int modifiers)
{
  if (keys.isDown(GLFW_KEY_TAB))
  {
    camera->getEyePoint();
    ball->getTransform();
  }
	//Set kick power
  if (keys.isDown(GLFW_KEY_2))
  {
    power = 1.25f;
  }
  if (keys.isDown(GLFW_KEY_3))
  {
    power = 1.5f;
  }
  if (keys.isDown(GLFW_KEY_1))
  {
    power = 1.0f;
  }
  //Switch Maerial on active mesh
  if (keys.isDown(GLFW_KEY_M))
  {
    if (matIndex == mats.size()-1)
    {
      matIndex = 0;
    }
    else
    {
      ++matIndex;
    }
    scene->getActiveMesh()->setMaterial(mats[matIndex]);
  }

  if (keys.isDown(GLFW_KEY_DELETE))
  {
    scene->clear();
  }
  //Change color weight in fragment shader
  if (keys.isDown(GLFW_KEY_B))
  {
    if (colorWeight >= 1.0f)
    {
      colorWeight = 0.0f;
      g_shaderProgram.setUniform1f(g_shaderProgram.getUniformLocation("colorWeight"), colorWeight);
    }
    else
    {
      colorWeight = colorWeight + 0.1f;
      g_shaderProgram.setUniform1f(g_shaderProgram.getUniformLocation("colorWeight"), colorWeight);
    }
  }

   //Toggle Wireframe mode
  if (key == GLFW_KEY_I && action == GLFW_PRESS)
  {
    if (wireframe)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      wireframe = false;
    }
    else
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      wireframe = true;
    }
  }
  
  if (keys.isDown(GLFW_KEY_P))
  {
    if (camera->isPrespective())
    {
      g_projection = camera->getOrthographicProjectionMatrix();
      initProjection();
    }
    else
    {
      g_projection = camera->getPrespectiveProjectionMatrix();
      initProjection();
    }
  }
  if (action == GLFW_PRESS)
  {
    keys.setKeyDown (key);
  }
  if (action == GLFW_RELEASE)
  {
    keys.setKeyUp (key);
  }
}


/******************************************************************/

void
processMousePosition (GLFWwindow* window, double x, double y)
{
  // "x" and "y" are screen (NOT window) coordinates, with
  //   (0.0, 0.0) at the upper left of the window
//click and drag camera
  if (isPitchYaw)
  {
    camera->yaw((x-xOrigin) * 0.005f);
    camera->pitch((y-yOrigin) * 0.005f);
  }
  if (isRoll)
  {
    camera->roll((x - xOrigin) * 0.005f);
  }
}

/******************************************************************/
void
processScrolling(GLFWwindow* window, double xOffSet, double yOffSet)
{
  camera->zoom(-yOffSet/10);
  if (camera->isPrespective())
  {
    g_projection = camera->getPrespectiveProjectionMatrix();
    initProjection();
  }
  else
  {
    g_projection = camera->getOrthographicProjectionMatrix();
    initProjection();
  }
}
/******************************************************************/
void
processMouseButton (GLFWwindow* window, int button, int action,
                    int modifiers)
{
  // Other relevant constants are GLFW_MOUSE_BUTTON_RIGHT and GLFW_RELEASE
  if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
    if (action == GLFW_PRESS)
    { 
      isPitchYaw = 1;
      glfwGetCursorPos(window, &xOrigin, &yOrigin);
      if (held)
      {
        kicked = true;
        speed = 3 * power;
      }
    }
    else
    { 
      isPitchYaw = 0;
    }
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT)
  {
    if (action == GLFW_PRESS)
    {
      isRoll = 1;
      glfwGetCursorPos(window, &xOrigin, &yOrigin);
    }
    else
    {
      isRoll = 0;
    }
  }


}

/******************************************************************/

void
releaseGlResources ()
{
  // Delete OpenGL resources if program will continue running
  //glDeleteVertexArrays (g_vaos.size (), &g_vaos[0]);
}

/******************************************************************/

void
handleGlfwError (int error, const char* description)
{
  fprintf (stderr, "GLFW error: %s, code %d\n", description, error);
  exit (-1);
}

/******************************************************************/

