#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inColor;

struct PointLightData {
    vec4 position; // ignore w
    vec4 color;    // color + intensity
};

layout (set = 0, binding = 0) uniform UBOScene
{
    mat4 projection;
    mat4 view;
    vec4 viewPos;
    vec4 ambientColor; // color + intensity
    PointLightData pointLights[10];
    int numLights;
    float outlineWidth;
} uboScene;

layout (push_constant) uniform PushConsts {
    mat4 model;
    mat4 normal;
} primitive;

void main()
{
    vec4 positionWorld = primitive.model * vec4(inPos + inNormal * uboScene.outlineWidth, 1.0);
    gl_Position        = uboScene.projection * uboScene.view * positionWorld;
}
