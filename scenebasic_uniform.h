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





class SceneBasic_Uniform : public Scene
{
private:
   Torus torus;
    Cube cube;
    Plane plane;
    std::unique_ptr<ObjMesh> phone;
    float tPrev;
    float angle;
    float rotSpeed;
    GLSLProgram prog;
    void setMatrices();
    void compile();
    GLuint mossTex;
    GLuint planeTex;
    GLuint torusTex;
    GLuint torus2Tex;

public:
    SceneBasic_Uniform();
    void initScene();
    void update(float t, glm::vec3 Orientation, glm::vec3 Position, glm::vec3 Up);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
