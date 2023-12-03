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
    float bands[5];
} uboScene;


layout (location = 0) out vec4 outFragColor;

void main()
{
    vec3 diffuse = uboScene.ambientColor.xyz * uboScene.ambientColor.w;
    vec3 specular = vec3(0.0);
    vec3 normal = normalize(inWorldNormal);
    vec3 viewDirection = normalize(uboScene.viewPos.xyz - inWorldPos.xyz);
    vec4 fragColor = texture(samplerColorMap, inUV) * vec4(inColor, 1.0);

    for (int i = 0; i < uboScene.numLights; i++) {
        PointLightData light = uboScene.pointLights[i];
        vec3 directionToLight = light.position.xyz - inWorldPos;
        float attenuation = 1.0 / dot(directionToLight, directionToLight);
        directionToLight = normalize(directionToLight);

        float cosAngleIncedence = max(dot(normal, directionToLight), 0);
        vec3 intensity = light.color.xyz * light.color.w * attenuation;
//        diffuse += intensity * smoothstep(0.45, 0.55, cosAngleIncedence);
//        if (uboScene.bandState == 0)
//            diffuse += intensity * smoothstep(0.45, 0.55, cosAngleIncedence);
//        else
//            diffuse += intensity * cosAngleIncedence;

        //        /* No bands */
//        diffuse += intensity * cosAngleIncedence;

        /* Single band */
        diffuse += intensity * smoothstep(0.45, 0.55, cosAngleIncedence);
//        diffuse += intensity * ((cosAngleIncedence < 0.5) ? 0.0 : 1.0);

//        /* Two bands */
//        float h  = 0.03; // half gradient length
//        float b0 = 0.33;
//        float b1 = 0.66;
//        float y1 = smoothstep(b0 - h, b0 + h, cosAngleIncedence);
//        float y2 = smoothstep(b1 - h, b1 + h, cosAngleIncedence);
//        float y = mix(mix(0.0, 0.5, y1),mix(0.5,1.0, y2),y1);
//        diffuse += intensity * y;

//        Three bands
//        float h  = 0.05; // half gradient length
//        float b0 = 0.25;
//        float b1 = 0.5;
//        float b2 = 0.75;
//        float b0 = 0.50;
//        float b1 = 0.65;
//        float b2 = 0.85;
//        float y1 = smoothstep(b0 - h, b0 + h, cosAngleIncedence);
//        float y2 = smoothstep(b1 - h, b1 + h, cosAngleIncedence);
//        float y3 = smoothstep(b2 - h, b2 + h, cosAngleIncedence);
//        float y = mix(0.0, 0.33, y1);
//        y = mix(y, mix(0.33, 0.66, y2), y1);
//        y = mix(y, mix(0.66, 0.83, y3), y2);
//        y = mix(y, 1.0, y3);
//        diffuse += intensity * y;




//        /* N bands */
//        float h  = 0.05; // half gradient length
//        float y = 0.0;
//        for (int i = 0; i < uboScene.numBands; i++) {
//            float p = smoothstep(uboScene.bands[i]-h, uboScene.bands[i]+h, cosAngleIncedence);
//            y = mix(y, mix(float(i) / float(uboScene.numBands), float(i+1) / float(uboScene.numBands), p), p);
//        }
//        diffuse += intensity * y;

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


