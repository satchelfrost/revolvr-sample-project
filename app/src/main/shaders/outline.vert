#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

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
    int bandState;
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
