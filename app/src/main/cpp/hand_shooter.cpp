#include <hand_shooter.h>
#include <global_context.h>
#include <helper_macros.h>
#include "io/io.h"
#include <ecs/component/types/timer.h>
#include <ecs/component/types/mesh.h>

HandShooter::HandShooter(rvr::type::EntityId id) : Ritual(id) {
    shooterBoxSpatial_ = GetComponent<rvr::Spatial>(id);
    originalTransform_ = shooterBoxSpatial_->GetWorld();
    isDetached_ = false;
    projectileSpeed_ = 7.0f;
}

void HandShooter::Begin() {}

void HandShooter::Update(float delta) {
    if (IsPinched(rvr::Hand::Right) && !isDetached_) {
        // Play shoot sound...

        // Reparent from hand to the origin
        rvr::Entity* shooterBox = GetEntity(id);
        rvr::Entity* origin = GetEntity(0);

        shooterBoxSpatial_->SetLocal(shooterBoxSpatial_->GetWorld());
        shooterBox->SetParent(origin);
        isDetached_ = true;

        // Start positional and visual reset timer
        auto timer = GetComponent<rvr::Timer>(id);
        timer->Start();
    }

    if (isDetached_) {
        auto position = shooterBoxSpatial_->GetLocal().GetPosition();
        auto forward = -shooterBoxSpatial_->GetLocal().GetYAxis();
        position += forward * projectileSpeed_ * delta;
        shooterBoxSpatial_->SetLocalPosition(position);
    }
}

void HandShooter::OnTriggered(rvr::Collider* other) {
    // Play boom boom noise...

    SetVisibilityOfBoxes(false);
    Log::Write(Log::Level::Info,"Collision has occurred");
}

void HandShooter::OnTimeout() {
    if (isDetached_)
        Reset();
    Log::Write(Log::Level::Info,"Timer has reset");
}

void HandShooter::Reset() {
    //Reset projectile by attaching to hand
    rvr::Entity* shooterBox = GetEntity(id);
    int RIGHT_CENTER_HAND_JOINT = 34;
    rvr::Entity* rightHand = GetEntity(RIGHT_CENTER_HAND_JOINT);

    shooterBoxSpatial_->SetLocal(originalTransform_);
    shooterBox->SetParent(rightHand);
    isDetached_ = false;
    SetVisibilityOfBoxes(true);
}

void HandShooter::SetVisibilityOfBoxes(bool visibility) {
    constexpr int collidedWithBox = 55;
    auto otherMesh = GetComponent<rvr::Mesh>(collidedWithBox);
    otherMesh->SetVisibilityRecursive(visibility);
    auto thisMesh = GetComponent<rvr::Mesh>(id);
    thisMesh->SetVisibilityRecursive(visibility);
}

