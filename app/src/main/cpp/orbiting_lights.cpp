#include <orbiting_lights.h>
#include <global_context.h>
#include <helper_macros.h>

#define LIGHT_1_ID 7
#define LIGHT_2_ID 8
#define LIGHT_3_ID 9
#define TWO_PI (2.0f * 3.14159f)

OrbitingLights::OrbitingLights(rvr::type::EntityId id) : Ritual(id) {
    lightSpatials_.push_back(GetComponent<rvr::Spatial>(LIGHT_1_ID));
    lightSpatials_.push_back(GetComponent<rvr::Spatial>(LIGHT_2_ID));
    lightSpatials_.push_back(GetComponent<rvr::Spatial>(LIGHT_3_ID));
}

void OrbitingLights::Update(float delta) {
    currentTime_ += delta;
    float angularSpeed = TWO_PI / period_;
    float angle = angularSpeed * currentTime_;
    for (int i = 0; i < lightSpatials_.size(); i++) {
        float angleOffset = (TWO_PI / (float)lightSpatials_.size()) * (float)i;
        float radius = a_ * (1 - (e_ * e_)) / (1.0f + e_ * (float)cos(angle + angleOffset));
        float x =  radius * (float)cos(angle + angleOffset);
        float z = -radius * (float)sin(angle + angleOffset);
        lightSpatials_[i]->SetLocalPosition({x, 0.0f, z});
    }
}