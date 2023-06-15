#include <hand_tracking_ritual.h>
#include <global_context.h>
#include <helper_macros.h>
#include <ecs/component/types/mesh.h>
#include <action/io.h>

HandTrackingRitual::HandTrackingRitual(rvr::type::EntityId id) : Ritual(id) {}

void HandTrackingRitual::Update(float delta) {
    if (IsPinched(rvr::Hand::Left) || IsPinched(rvr::Hand::Right)) {
        constexpr int DISAPPEARING_BOX_ID = 53;
        auto mesh = GetComponent<rvr::Mesh>(DISAPPEARING_BOX_ID);
        mesh->SetVisibilityRecursive(true);
    }
}

void HandTrackingRitual::OnTriggered(rvr::Collider* other) {
    rvr::Entity* entity = GetEntity(other->id);
    if (entity->GetName() == "disappearing_box") {
        auto mesh = GetComponent<rvr::Mesh>(other->id);
        mesh->SetVisibilityRecursive(false);
    }
}


