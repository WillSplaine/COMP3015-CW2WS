#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>
#include <string>
using std::string;
#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include "helper/texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "helper/scenerunner.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


SceneBasic_Uniform::SceneBasic_Uniform() :
	tPrev(0), angle(0.0f), rotSpeed(glm::pi<float>()/9.0f), torus(7.3f * .75, .4f, 50, 50),
	plane(50.0f, 50.0f, 1, 1), lightPos(5.0f,5.0f,5.0f,1.0f), skybox(110.0f){
	phone = ObjMesh::load("media/iphone_13.obj", true); 
	
}

void SceneBasic_Uniform::initScene()
{
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);

	glDebugMessageControl(
		GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, NULL, GL_TRUE);

    compile();
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	model = mat4(1.0f);
	projection = mat4(1.0f);
	angle = 0.0f;

	view = lookAt(

		vec3(0.0f, 4.0f, 7.0f),
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f)
	);

	projection = perspective(radians(50.0f), (float)width / height, 0.5f, 100.0f);

	lightAngle = 0.0f;
	lightRotationSpeed = 1.5f;

	prog.setUniform("PBRLight[0].L", vec3(45.0f));
	prog.setUniform("PBRLight[0].Position", view * lightPos);
	prog.setUniform("PBRLight[1].L", vec3(0.3f));
	prog.setUniform("PBRLight[1].Position", vec4(0.0f, 0.15f, -1.0f, 0.0f));
	prog.setUniform("PBRLight[2].L", vec3(45.0f));
	prog.setUniform("PBRLight[2].Position", view * vec4(-7.0f, 3.0f, 7.0f, 1.0f));
	
	
	prog.setUniform("Light.L", vec3(1.0f)); 
	prog.setUniform("Light.La", vec3(0.05f));

	
	prog.setUniform("Fog.MaxDist", 100.0f );
	prog.setUniform("Fog.MinDist", 1.0f);
	prog.setUniform("Fog.Color", vec3(1.0f,1.0f,1.0f));
	

	 planeTex = Texture::loadTexture("media/texture/rock_floor.jpg");
	 mossTex = Texture::loadTexture("media/texture/moss.png");
	 torusTex = Texture::loadTexture("media/texture/minecraftore.png");
	 torus2Tex = Texture::loadTexture("media/texture/l_h.png");
	 skyBoxTex = Texture::loadHdrCubeMap("media/texture/cubemap");
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
		
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update(float t, glm::vec3 Orientation, glm::vec3 Position, glm::vec3 Up)
{
	float deltaT = t - tPrev;

	if (tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;
	angle += 0.1f * deltaT;
	if (this->m_animate) {
		angle += rotSpeed * deltaT;
		if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
	
	}
	prog.setUniform("ViewMatrix", view);
	view = glm::lookAt(Position, Position + Orientation, Up);

	if (animating()) {
		
		lightAngle = mod(lightAngle + deltaT * lightRotationSpeed, two_pi<float>());
		lightPos.x = cos(lightAngle) * 7.0f; 
		lightPos.x = 3.0f;
		lightPos.x = sin(lightAngle) * 7.0f;

	}
}

void SceneBasic_Uniform::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.setUniform("Light[0].Position", view * lightPos);
	drawScene();

	prog.setUniform("Light.Position", view * glm::vec4(10.0f * cos(angle), 0.0f, 10.0f * sin(angle), 0.0f));
	prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
	prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
	prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
	prog.setUniform("Material.Shininess", 100.0f);
	prog.setUniform("Material.texChoice", 1);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, torusTex);
	model = mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	setMatrices(prog);
	torus.render();

	prog.setUniform("Material.texChoice", 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, torus2Tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mossTex);
	model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 3.0, 0.0));
	model = glm::rotate(model, glm::radians(35.0f), vec3(9.0f * cos(angle), 1.0f, 10.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	setMatrices(prog);
	phone->render();

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTex);
	model = mat4(1.0f);
	setMatrices(prog);
	skybox.render();
	}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);
    width = w;
    height = h;
    
	projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}


void SceneBasic_Uniform::setMatrices(GLSLProgram& prog) 
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix",mv);
	prog.setUniform("NormalMatrix",glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);

}





void SceneBasic_Uniform::drawScene()
{
	drawFloor();

	int numPhones = 9;
	vec3 phoneBasColor(0.1f, 0.33f, 0.97f);
	for (int i = 0; i < numPhones; i++)
	{
		float phoneX =  i * (10.0f / (numPhones - 1)) - 5.0f;
		float rough = (i + 1) * (1.0f / numPhones);
		drawSpot(vec3(phoneX, 0.0f, 0.0f), rough, 0, phoneBasColor);
	}

	float metalRough = 0.43f;
	drawSpot(vec3(-3.0f, 0.0f, 3.0f), metalRough, 1, vec3(1.0f, 0.71f, 0.29f));
	drawSpot(vec3(-1.5f, 0.0f, 3.0f), metalRough, 1, vec3(0.95f, 0.71f, 0.29f));
	drawSpot(vec3(-0.0f, 0.0f, 3.0f), metalRough, 1, vec3(0.91f, 0.71f, 0.29f));
	drawSpot(vec3(1.5f, 0.0f, 3.0f), metalRough, 1, vec3(0.542f, 0.71f, 0.29f));
	drawSpot(vec3(3.0f, 0.0f, 3.0f), metalRough, 1, vec3(0.95f, 0.71f, 0.29f));

	
}


void SceneBasic_Uniform::drawFloor()
{
	prog.setUniform("Material.texChoice", 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, planeTex);
	model = glm::rotate(model, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(glm::mat4(1.0), glm::vec3(0, -1.5, 0));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	setMatrices(prog);
	plane.render();
}

void SceneBasic_Uniform::drawSpot(const vec3& pos, float rough, int metal, const vec3& color)
{
	prog.setUniform("PBRMaterial.Rough", rough);
	prog.setUniform("PBRMaterial.Metal", metal );
	prog.setUniform("PBRMaterial.Color", color);

	model = mat4(1.0f);
	model = translate(model, pos);
	model = rotate(model, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
	setMatrices(prog);
	phone->render();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, torusTex);
	model = mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	setMatrices(prog);
	torus.render();
}


