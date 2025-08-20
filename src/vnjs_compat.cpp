#include "vnjs_compat.hpp"

// bs-hook utils available if/when we need reflection/field access
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

namespace NE::VNJS {

bool IsNoodleLevel(GlobalNamespace::BeatmapData* /*beatmapData*/) {
    // TODO: Inspect CustomJSONData for "_requirements": ["Noodle Extensions"] or NE fields.
    return false;
}

void ForceStaticNJSIfNoodle(GlobalNamespace::BeatmapObjectSpawnController* /*spawnController*/,
                            float /*baseNJS*/,
                            float /*spawnOffsetSec*/) {
    // TODO: Hook provider setup and replace with a static provider when IsNoodleLevel()==true.
}

} // namespace NE::VNJS
