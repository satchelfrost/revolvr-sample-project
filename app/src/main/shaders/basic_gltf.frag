#version 450

layout (set = 1, binding = 0) uniform sampler2D samplerColorMap;

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec3 inWorldPos;
layout (location = 2) in vec3 inWorldNormal;
layout (location = 3) in vec2 inUV;

struct PointLightData {
    vec4 position; // ignore w
    vec4 color;    // color + intensity
};

//struct CelShading {
//    int numBands;
//    float h;
//    float b0;
//    float b1;
//    float b2;
//};

layout (set = 0, binding = 0) uniform UBOScene
{
    mat4 projection;
    mat4 view;
    vec4 viewPos;
    vec4 ambientColor; // color + intensity
    PointLightData pointLights[10];
    int numLights;
    float outlineWidth;
    int numBands;
    float h;
    float b0;
    float b1;
    float b2;
} ubo;

layout (location = 0) out vec4 outFragColor;

void main()
{
    vec3 diffuse = ubo.ambientColor.xyz * ubo.ambientColor.w;
    vec3 specular = vec3(0.0);
    vec3 normal = normalize(inWorldNormal);
    vec3 viewDirection = normalize(ubo.viewPos.xyz - inWorldPos.xyz);
    vec4 fragColor = texture(samplerColorMap, inUV) * vec4(inColor, 1.0);

    for (int i = 0; i < ubo.numLights; i++) {
        PointLightData light = ubo.pointLights[i];
        vec3 directionToLight = light.position.xyz - inWorldPos;
        float attenuation = 1.0 / dot(directionToLight, directionToLight);
        directionToLight = normalize(directionToLight);

        float cosAngleIncedence = max(dot(normal, directionToLight), 0);
        vec3 intensity = light.color.xyz * light.color.w * attenuation;

        // Calculate band effects for cel shading
        if (ubo.numBands == 1) {
             cosAngleIncedence = smoothstep(ubo.b0 - ubo.h, ubo.b0 + ubo.h, cosAngleIncedence);
        } else if (ubo.numBands == 2) {
            float y1 = smoothstep(ubo.b0 - ubo.h, ubo.b0 + ubo.h, cosAngleIncedence);
            float y2 = smoothstep(ubo.b1 - ubo.h, ubo.b1 + ubo.h, cosAngleIncedence);
            cosAngleIncedence = mix(mix(0.0, 0.5, y1),mix(0.5,1.0, y2),y1);
        } else if (ubo.numBands == 3) {
            float y1 = smoothstep(ubo.b0 - ubo.h, ubo.b0 + ubo.h, cosAngleIncedence);
            float y2 = smoothstep(ubo.b1 - ubo.h, ubo.b1 + ubo.h, cosAngleIncedence);
            float y3 = smoothstep(ubo.b2 - ubo.h, ubo.b2 + ubo.h, cosAngleIncedence);
            cosAngleIncedence = mix(0.0, 0.33, y1);
            cosAngleIncedence = mix(cosAngleIncedence, mix(0.33, 0.66, y2), y1);
            cosAngleIncedence = mix(cosAngleIncedence, mix(0.66, 0.83, y3), y2);
            cosAngleIncedence = mix(cosAngleIncedence, 1.0, y3);
        }
        diffuse += intensity * cosAngleIncedence;

        // Specular calculation
        vec3 halfAngle = normalize(directionToLight + viewDirection);
        float blinnTerm = dot(normal, halfAngle);
        blinnTerm = clamp(blinnTerm, 0, 1);
        blinnTerm = pow(blinnTerm, 64.0);
        specular += intensity * blinnTerm;
    }

    // Final color
    outFragColor = vec4(diffuse * fragColor.rgb + specular * fragColor.rgb, 1.0);
}


