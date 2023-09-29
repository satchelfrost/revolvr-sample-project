#version 450

layout (set = 1, binding = 0) uniform sampler2D samplerColorMap;

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inViewVec;
layout (location = 4) in vec3 inLightVec;

layout (location = 0) out vec4 outFragColor;

void main()
{
    // Texture color
    vec4 color = texture(samplerColorMap, inUV) * vec4(inColor, 1.0);

    // Diffuse calculation
    vec3 diffuse = max(dot(inNormal, inLightVec), 0.15) * inColor;

    // Specular calculation
    vec3 halfAngle = normalize(inViewVec + inLightVec);
    float blinnTerm = dot(inNormal, halfAngle);
    blinnTerm = clamp(blinnTerm, 0, 1);
    blinnTerm = pow(blinnTerm, 64.0);
    vec3 specular = vec3(0.0);
    specular += blinnTerm;

    // Final color
    outFragColor = vec4(diffuse * color.rgb + specular, 1.0);
}


