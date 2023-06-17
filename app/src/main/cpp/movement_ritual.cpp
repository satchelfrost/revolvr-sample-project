#include <movement_ritual.h>
#include <global_context.h>
#include <helper_macros.h>
#include <action/io.h>
#include <cmath>

MovementRitual::MovementRitual(rvr::type::EntityId id) : Ritual(id) {
    playerSpatial_ = GetComponent<rvr::Spatial>(1);
    joyStickSpatial_ = GetComponent<rvr::Spatial>(4);
}

void MovementRitual::Begin() {}

void MovementRitual::Update(float delta) {
    auto rightJoy = GetJoystickXY(rvr::Hand::Right);

    // forward/backward movement with right joystick y-axis
    auto position = playerSpatial_->GetLocal().GetPosition();
    auto forward = playerSpatial_->GetLocal().GetZAxis();
    forward.z *= -1; // -z is forward
    position += forward * rightJoy.y * delta;
    playerSpatial_->SetLocalPosition(position);

    // Rotate left/right movement with right joystick x-axis
    float angle = rightJoy.x * delta;
    auto transform = playerSpatial_->GetLocal().Rotated({0, 1, 0}, angle);
    playerSpatial_->SetLocal(transform);

    // Visualize the joystick
    auto xAxis = joyStickSpatial_->GetLocal().GetXAxis();
    auto yAxis = joyStickSpatial_->GetLocal().GetYAxis();
    float forwardBackwardTilt = -rightJoy.y * 45;
    float leftRightTilt = -rightJoy.x * 45;
    auto orientation = rvr::math::quaternion::FromAxisAngle(xAxis, forwardBackwardTilt);
    orientation *= rvr::math::quaternion::FromAxisAngle(yAxis, leftRightTilt);
    joyStickSpatial_->SetLocalOrientation(orientation);

}