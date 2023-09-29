#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inColor;

layout (set = 0, binding = 0) uniform UBOScene
{
    mat4 projection;
    mat4 view;
    vec4 lightPos;
    vec4 viewPos;
} uboScene;

layout (push_constant) uniform PushConsts {
    mat4 model;
    mat4 normal;
} primitive;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec2 outUV;
layout (location = 3) out vec3 outViewVec;
layout (location = 4) out vec3 outLightVec;

void main()
{
    vec4 positionWorld = primitive.model * vec4(inPos, 1.0);
    gl_Position = uboScene.projection * uboScene.view * positionWorld;

    outNormal    = normalize(mat3(primitive.normal) * inNormal);
    outColor     = inColor;
    outUV        = inUV;
    outViewVec   = normalize(uboScene.viewPos.xyz - positionWorld.xyz);
    outLightVec  = normalize(uboScene.lightPos.xyz - positionWorld.xyz);
}
