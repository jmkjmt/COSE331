#include "scene.h"
#include "binary/teapot.h"
#include "binary/rgb.h"
#include "binary/cloud.h"
#include "binary/tex_flower.h"
#include "checker.h"

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::teapot = nullptr;
Texture* Scene::diffuse = nullptr;
Texture* Scene::dissolve = nullptr;
Material* Scene::material = nullptr;
Light* Scene::light = nullptr;

int Scene::width = 0;
int Scene::height = 0;

// Arcball variables
float lastMouseX = 0, lastMouseY = 0;
float currentMouseX = 0, currentMouseY = 0;

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(20.0f, 30.0f, 20.0f);

    Scene::light = new Light(program);
    Scene::light->position = vec3(15.0f, 15.0f, 0.0f);

    //////////////////////////////
    /* TODO: Problem 2 : Change the texture of the teapot
     *  Modify and fill in the lines below.
     */
    Texel red={0xFF,0x00,0x00};
    Texel green={0x00,0xFF,0x00};
    Texel blue={0x00,0x00,0xFF};


    for(int i=0;i<rgbTexels.size();i++){
        if(rgbTexels[i].red==0xff){
            rgbTexels[i]=blue;
        }
        else if(rgbTexels[i].blue==0xff){
            rgbTexels[i]=green;
        }
        else{
            rgbTexels[i]=red;
        }
    }

    Scene::diffuse  = new Texture(Scene::program, 0, "textureDiff", rgbTexels, rgbSize);
    //////////////////////////////

    Scene::material = new Material(Scene::program, diffuse, dissolve);
    Scene::teapot = new Object(program, material, teapotVertices, teapotIndices);
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
    Scene::width = width;
    Scene::height = height;
}

void Scene::update(float deltaTime) {
    static float time = 0.0f;

    Scene::program->use();

    Scene::camera->update();
    Scene::light->update();

    Scene::teapot->draw();

    time += deltaTime;
}

void Scene::mouseDownEvents(float x, float y) {
    lastMouseX = currentMouseX = x;
    lastMouseY = currentMouseY = y;
}

void Scene::mouseMoveEvents(float x, float y) {
    //////////////////////////////
    /* TODO: Problem 3 : Implement Phong lighting
     *  Fill in the lines below.
     */

    float w = Scene::width;
    float h = Scene::height;

    lastMouseX = x;
    lastMouseY = y;

    vec3 v0=vec3(0.0f,0.0f,0.0f);
    v0.x=currentMouseX * 2 / w - 1;
    v0.y=currentMouseY * 2 / h - 1;
    v0.y = -v0.y;
    if(v0.x * v0.x + v0.y * v0.y > 1.0f){
        v0=glm::normalize(v0);
    }
    v0.z=sqrt(glm::max(1-(v0.x * v0.x)-(v0.y * v0.y),0.0f));
    vec3 v1=vec3(0.0f,0.0f,0.0f);
    v1.x=lastMouseX * 2 / w - 1;
    v1.y=lastMouseY * 2 / h - 1;
    v1.y = -v1.y;
    if(v1.x * v1.x + v1.y * v1.y > 1.0f){
        v1=glm::normalize(v1);
    }
    v1.z=sqrt(glm::max(1-(v1.x * v1.x)-(v1.y * v1.y),0.0f));

    vec3 axis=glm::cross(v0,v1);
    mat4 viewMat = lookAt(Scene::camera->eye, Scene::camera->at, Scene::camera->up);
    vec3 new_axis=inverse(viewMat)*vec4(axis,0.0f);

    float rad=acos(glm::min(dot(v0,v1),1.0f));

    vec4 new_pos = glm::rotate(rad,new_axis)*vec4(Scene::light->position,1.0f);
    Scene::light->position=vec3(new_pos.x, new_pos.y, new_pos.z);

    currentMouseX=x;
    currentMouseY=y;
    //////////////////////////////
}