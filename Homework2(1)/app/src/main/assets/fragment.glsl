#version 300 es

precision mediump float;

uniform sampler2D textureDiff;

uniform vec3 matSpec, matAmbi, matEmit;
uniform float matSh;
uniform vec3 srcDiff, srcSpec, srcAmbi;

in vec3 v_normal, v_view, v_lightDir;
in vec2 v_texCoord;

layout(location = 0) out vec4 fragColor;

void main() {

    float alpha = 1.0;
    vec3 color = texture(textureDiff, v_texCoord).rgb;

    //////////////////////////////
    /* TODO: Problem 3 : Implement Phong lighting
     *  Fill in the lines below.
     */
    vec3 normal=normalize(v_normal);
    vec3 view=normalize(v_view);
    vec3 lightDir=normalize(v_lightDir);
    vec3 diff = max(dot(normal, lightDir),0.0) * srcDiff * color;
    vec3 refl = 2.0 * normal*dot(normal, lightDir) - lightDir;
    vec3 spec = pow(max(dot(refl, view),0.0),matSh) * srcSpec * matSpec;

    vec3 ambi=srcAmbi * matAmbi;

    color = diff + spec+ambi+matEmit;
    //////////////////////////////

    //////////////////////////////
    /* TODO: Problem 4 : Implement alpha blending
     *  Fill in the lines below.
     */
    alpha=length(color);
    //////////////////////////////

    fragColor = vec4(color, alpha);
}