#pragma once

// Use bs-cordl headers (qpm restored) for IL2CPP types.
#include "GlobalNamespace/BeatmapData.hpp"
#include "GlobalNamespace/BeatmapObjectSpawnController.hpp"

namespace NE::VNJS {

// Returns true if the loaded beatmap declares NoodleExtensions requirements
// (implementation will be filled in after we bind CustomJSONData access).
bool IsNoodleLevel(GlobalNamespace::BeatmapData* beatmapData);

// If IsNoodleLevel()==true, we will swap the jump-speed provider on this controller
// to a static provider using baseNJS/spawnOffsetSec (implementation to follow).
void ForceStaticNJSIfNoodle(GlobalNamespace::BeatmapObjectSpawnController* spawnController,
                            float baseNJS,
                            float spawnOffsetSec);

} // namespace NE::VNJS
