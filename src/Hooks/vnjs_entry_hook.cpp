// src/hooks/vnjs_entry_hook.cpp
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "NELogger.h"

#include "GlobalNamespace/BeatmapObjectSpawnController.hpp"
#include "vnjs_compat.hpp"

// Hook BeatmapObjectSpawnController::Start so we have a stable entry point per level.
MAKE_HOOK_MATCH(BOSC_Start,
    &GlobalNamespace::BeatmapObjectSpawnController::Start,
    void, GlobalNamespace::BeatmapObjectSpawnController* self)
{
    // Non-invasive: just log for now so we can confirm the hook fires in logcat.
    NELogger::Logger.info("VNJS entry: BeatmapObjectSpawnController::Start()");
    // (Later we will call NE::VNJS::ForceStaticNJSIfNoodle(self, baseNJS, spawnOffsetSec); here.)

    BOSC_Start(self); // call original
}

// Small helper so main.cpp can install this without passing a logger around.
extern "C" void InstallVNJSEntryHook() {
    INSTALL_HOOK(NELogger::Logger, BOSC_Start);
}
