#version 300 es

uniform mat4 worldMat, viewMat, projMat;
uniform vec3 lightPos;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 v_normal;
out vec2 v_texCoord;
out vec3 v_lightDir;

void main() {
    float scale = 1.0;

    //////////////////////////////
    /* TODO: Problem 1.
    *  Fill in the lines below.
    *  Scale the part of the teapot below XZ plane.
    */
    vec3 scaled_pos = position;
    if((worldMat * vec4(position, 1.0)).y <0.0){
        scaled_pos *= 1.5;
    }

    gl_Position = projMat * viewMat * worldMat *vec4(scaled_pos, 1.0);
    v_normal = normalize(transpose(inverse(mat3(worldMat)))*normal);
    v_texCoord = texCoord;

    //////////////////////////////

    vec3 posWS = (worldMat * vec4(position, 1.0)).xyz;
    v_lightDir = normalize(lightPos - posWS);
}
