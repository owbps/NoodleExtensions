#include "scotland2/shared/loader.hpp"

#include "NEConfig.h"
#include "NEHooks.h"
#include "NELogger.h"

// VNJS entry hook installer (defined in src/hooks/vnjs_entry_hook.cpp)
extern "C" void InstallVNJSEntryHook();

extern "C" void setup(CModInfo* info) {
  info->id = "NoodleExtensions";
  info->version = VERSION;
  info->version_long = 0;
  getNEConfig().Init(modloader::ModInfo(info->id, info->version, info->version_long));
}

extern "C" void late_load() {
  NELogger::Logger.info("Installing NoodleExtensions Hooks!");

  // Install VNJS entry hook (BeatmapObjectSpawnController::Start)
  InstallVNJSEntryHook();

  // Install all existing NoodleExtensions hooks
  InstallAndRegisterAll();

  NELogger::Logger.info("Installed NoodleExtensions Hooks!");
}
