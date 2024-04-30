#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper/torus.h"
#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/teapot.h"
#include "helper/cube.h"
#include "helper/skybox.h"



using namespace std;
using namespace glm;

class SceneBasic_Uniform : public Scene
{
private:
   Torus torus;
    Cube cube;
    Plane plane;
    Plane plane2;
    std::unique_ptr<ObjMesh> phone;
    float lightAngle;
    float lightRotationSpeed;
    float tPrev;
    float angle;
    float rotSpeed;
    float time;


    GLSLProgram prog, skyboxProg;
    
    GLuint mossTex;
    GLuint planeTex;
    GLuint torusTex;
    GLuint torus2Tex;
    GLuint skyBoxTex;
    SkyBox skybox;
  

    vec4 lightPos;

    void compile();
    void setMatrices(GLSLProgram& prog);
    void drawScene();
    void drawFloor();
    void drawSpot(const vec3& pos, float rough, int metal, const vec3& color);
  

   

public:
    SceneBasic_Uniform();
    void initScene();
    void update(float t, glm::vec3 Orientation, glm::vec3 Position, glm::vec3 Up);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
