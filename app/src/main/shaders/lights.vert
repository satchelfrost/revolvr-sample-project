#version 450

const vec2 OFFSETS[6] = vec2[](
vec2(-1.0, -1.0),
vec2(-1.0, 1.0),
vec2(1.0, -1.0),
vec2(1.0, -1.0),
vec2(-1.0, 1.0),
vec2(1.0, 1.0)
);

layout (location = 0) in vec3 inPos;
layout (location = 0) out vec2 fragOffset;

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
} uboScene;

layout (push_constant) uniform PushConsts {
    vec4 position;
    vec4 color;
    float radius;
} push;

void main() {
    fragOffset = OFFSETS[gl_VertexIndex];
    vec3 cameraRightWorld = {uboScene.view[0][0], uboScene.view[1][0], uboScene.view[2][0]};
    vec3 cameraUpWorld = {uboScene.view[0][1], uboScene.view[1][1], uboScene.view[2][1]};

    vec3 positionWorld = push.position.xyz
    + push.radius * fragOffset.x * cameraRightWorld
    + push.radius * fragOffset.y * cameraUpWorld;
    gl_Position = uboScene.projection * uboScene.view * vec4(positionWorld, 1.0);
}