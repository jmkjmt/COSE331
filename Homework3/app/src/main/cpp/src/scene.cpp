#include "scene.h"
#include "binary/animation.h"
#include "binary/skeleton.h"
#include "binary/player.h"

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::player = nullptr;
Texture* Scene::diffuse = nullptr;
Material* Scene::material = nullptr;
Object* Scene::lineDraw = nullptr;
Texture* Scene::lineColor = nullptr;
Material* Scene::lineMaterial = nullptr;

bool Scene::upperFlag = true;
bool Scene::lowerFlag = true;
float upper_time=0.0f;
float lower_time=0.0f;
void printmat4(mat4 mat){
    LOG_PRINT_DEBUG("start");
    for(int i=0;i<4;i++){
        LOG_PRINT_DEBUG("%f %f %f %f",mat[i][0],mat[i][1],mat[i][2],mat[i][3]);
    }
}

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(0.0f, 0.0f, 80.0f);
    Scene::diffuse = new Texture(Scene::program, 0, "textureDiff", playerTexels, playerSize);
    Scene::material = new Material(Scene::program, diffuse);
    Scene::player = new Object(program, material, playerVertices, playerIndices);
    player->worldMat = scale(vec3(1.0f / 3.0f));

    Scene::lineColor = new Texture(Scene::program, 0, "textureDiff", {{0xFF, 0x00, 0x00}}, 1);
    Scene::lineMaterial = new Material(Scene::program, lineColor);
    Scene::lineDraw = new Object(program, lineMaterial, {{}}, {{}}, GL_LINES);
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
}

void Scene::update(float deltaTime) {
    Scene::program->use();
    Scene::camera->update();

    /*
     *
     * Write your code.
     *
     */
    if(upperFlag){
        upper_time+=deltaTime;
    }
    if(lowerFlag){
        lower_time+=deltaTime;
    }
    LOG_PRINT_DEBUG("%f %f",upper_time, lower_time);
    float upper_inter=upper_time - glm::floor(upper_time);
    int upper_motion1=int(glm::floor(upper_time)) % 4;
    int upper_motion2= int(glm::ceil(upper_time))%4;
    float lower_inter=lower_time - glm::floor(lower_time);
    int lower_motion1=int(glm::floor(lower_time)) % 4;
    int lower_motion2= int(glm::ceil(lower_time))%4;
    vector<mat4> anime(jNames.size()); // M(i,l)
    for (int i = 0; i < 12; i++) {
        mat4 tmp1(1.0f);
        mat4 tmp2(1.0f);
        mat4 tmp(1.0f);
        if (i == 0) {
            float x = radians(motions[lower_motion1][3]);
            float y = radians(motions[lower_motion1][4]);
            float z = radians(motions[lower_motion1][5]);
            tmp1 = glm::rotate(y, vec3(0.0f, 1.0f, 0.0f)) * tmp1;
            tmp1 = glm::rotate(x, vec3(1.0f, 0.0f, 0.0f)) * tmp1;
            tmp1 = glm::rotate(z, vec3(0.0f, 0.0f, 1.0f)) * tmp1;
            tmp1 = glm::translate(
                    vec3(motions[lower_motion1][0], motions[lower_motion1][1], motions[lower_motion1][2])) * tmp1;
            float x1 = radians(motions[lower_motion2][3]);
            float y1 = radians(motions[lower_motion2][4]);
            float z1 = radians(motions[lower_motion2][5]);
            tmp2 = glm::rotate(y1, vec3(0.0f, 1.0f, 0.0f)) * tmp2;
            tmp2 = glm::rotate(x1, vec3(1.0f, 0.0f, 0.0f)) * tmp2;
            tmp2 = glm::rotate(z1, vec3(0.0f, 0.0f, 1.0f)) * tmp2;
            tmp2 = glm::translate(
                    vec3(motions[lower_motion2][0], motions[lower_motion2][1], motions[lower_motion2][2])) * tmp2;
            glm::quat q1 = glm::quat_cast(tmp1);
            glm::quat q2 = glm::quat_cast(tmp2);
            glm::quat q = glm::mix(q1, q2, lower_inter);
            tmp = glm::mat4_cast(q);
        } else {
            float x = radians(motions[lower_motion1][3 * i + 3]);
            float y = radians(motions[lower_motion1][3 * i + 4]);
            float z = radians(motions[lower_motion1][3 * i + 5]);
            tmp1 = glm::rotate(y, vec3(0.0f, 1.0f, 0.0f)) * tmp1;
            tmp1 = glm::rotate(x, vec3(1.0f, 0.0f, 0.0f)) * tmp1;
            tmp1 = glm::rotate(z, vec3(0.0f, 0.0f, 1.0f)) * tmp1;
            float x1 = radians(motions[lower_motion2][3 * i + 3]);
            float y1 = radians(motions[lower_motion2][3 * i + 4]);
            float z1 = radians(motions[lower_motion2][3 * i + 5]);
            tmp2 = glm::rotate(y1, vec3(0.0f, 1.0f, 0.0f)) * tmp2;
            tmp2 = glm::rotate(x1, vec3(1.0f, 0.0f, 0.0f)) * tmp2;
            tmp2 = glm::rotate(z1, vec3(0.0f, 0.0f, 1.0f)) * tmp2;
            glm::quat q1 = glm::quat_cast(tmp1);
            glm::quat q2 = glm::quat_cast(tmp2);
            glm::quat q = glm::mix(q1, q2, lower_inter);
            tmp = glm::mat4_cast(q);
        }
        anime[i] = tmp;
    }
    for (int i = 12; i < jNames.size(); i++) {
            mat4 tmp1(1.0f);
            mat4 tmp2(1.0f);
            mat4 tmp(1.0f);
            if (i == 0) {
                float x = radians(motions[upper_motion1][3]);
                float y = radians(motions[upper_motion1][4]);
                float z = radians(motions[upper_motion1][5]);
                tmp1 = glm::rotate(y, vec3(0.0f, 1.0f, 0.0f)) * tmp1;
                tmp1 = glm::rotate(x, vec3(1.0f, 0.0f, 0.0f)) * tmp1;
                tmp1 = glm::rotate(z, vec3(0.0f, 0.0f, 1.0f)) * tmp1;
                tmp1 = glm::translate(
                        vec3(motions[upper_motion1][0], motions[upper_motion1][1], motions[upper_motion1][2])) * tmp1;
                float x1 = radians(motions[upper_motion2][3]);
                float y1 = radians(motions[upper_motion2][4]);
                float z1 = radians(motions[upper_motion2][5]);
                tmp2 = glm::rotate(y1, vec3(0.0f, 1.0f, 0.0f)) * tmp2;
                tmp2 = glm::rotate(x1, vec3(1.0f, 0.0f, 0.0f)) * tmp2;
                tmp2 = glm::rotate(z1, vec3(0.0f, 0.0f, 1.0f)) * tmp2;
                tmp2 = glm::translate(
                        vec3(motions[upper_motion2][0], motions[upper_motion2][1], motions[upper_motion2][2])) * tmp2;
                glm::quat q1 = glm::quat_cast(tmp1);
                glm::quat q2 = glm::quat_cast(tmp2);
                glm::quat q = glm::mix(q1, q2, upper_inter);
                tmp = glm::mat4_cast(q);
            } else {
                float x = radians(motions[upper_motion1][3 * i + 3]);
                float y = radians(motions[upper_motion1][3 * i + 4]);
                float z = radians(motions[upper_motion1][3 * i + 5]);
                tmp1 = glm::rotate(y, vec3(0.0f, 1.0f, 0.0f)) * tmp1;
                tmp1 = glm::rotate(x, vec3(1.0f, 0.0f, 0.0f)) * tmp1;
                tmp1 = glm::rotate(z, vec3(0.0f, 0.0f, 1.0f)) * tmp1;
                float x1 = radians(motions[upper_motion2][3 * i + 3]);
                float y1 = radians(motions[upper_motion2][3 * i + 4]);
                float z1 = radians(motions[upper_motion2][3 * i + 5]);
                tmp2 = glm::rotate(y1, vec3(0.0f, 1.0f, 0.0f)) * tmp2;
                tmp2 = glm::rotate(x1, vec3(1.0f, 0.0f, 0.0f)) * tmp2;
                tmp2 = glm::rotate(z1, vec3(0.0f, 0.0f, 1.0f)) * tmp2;
                glm::quat q1 = glm::quat_cast(tmp1);
                glm::quat q2 = glm::quat_cast(tmp2);
                glm::quat q = glm::mix(q1, q2, upper_inter);
                tmp = glm::mat4_cast(q);
            }
            anime[i] = tmp;
        }

    vector<mat4> final_anime(jNames.size());
    vector<mat4> go_ancestor(jNames.size());

    for(int i=0;i<jNames.size();i++){
        if(i==0){
            go_ancestor[0]=glm::translate(jOffsets[i]);
            final_anime[0]=anime[0];
            continue;
        }
        mat4 go_parent=mat4(1.0f);
        go_parent=glm::translate(jOffsets[i]);
        go_ancestor[i] = go_ancestor[jParents[i]] * go_parent;
        final_anime[i] = final_anime[jParents[i]] * go_parent * anime[i];
    }
    vector<Vertex> newVertices(playerVertices.size());
    for(int i=0;i<playerVertices.size();i++){
        vec3 pos=playerVertices[i].pos;
        vec3 nor=playerVertices[i].nor;
        ivec4 bone = playerVertices[i].bone;
        vec4 weight = playerVertices[i].weight;
        vec3 new_pos = {0.0f,0.0f,0.0f};
        vec3 new_nor = {0.0f,0.0f,0.0f};

        for(int j=0;j<4;j++){
            if(bone[j]==-1) continue;
            new_nor += weight[j] * vec3( glm::normalize(glm::transpose(glm::inverse( final_anime[bone[j]])) * vec4(nor,0.0f)));
            new_pos += weight[j] * vec3( final_anime[bone[j]] * inverse(go_ancestor[bone[j]]) * vec4(pos,1.0f));

        }
        newVertices[i].pos=new_pos;
        newVertices[i].nor=normalize(new_nor);
        newVertices[i].tex=playerVertices[i].tex;
    }

    //Line Drawer
    //glLineWidth(20);
    // Scene::lineDraw->load({{vec3(-20.0f, 0.0f, 0.0f)}, {vec3(20.0f, 0.0f, 0.0f)}}, {0, 1});
    // Scene::lineDraw->draw();
    Scene::player->load(newVertices, playerIndices);
    Scene::player->draw();
}

void Scene::setUpperFlag(bool flag)
{
    Scene::upperFlag = flag;
}

void Scene::setLowerFlag(bool flag)
{
    Scene::lowerFlag = flag;
}