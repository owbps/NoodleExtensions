#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

#include "GlobalNamespace/BeatmapObjectSpawnController.hpp"
#include "GlobalNamespace/BeatmapCallbacksController.hpp"
#include "GlobalNamespace/CallbacksInTime.hpp"
#include "GlobalNamespace/BeatmapLineData.hpp"
#include "GlobalNamespace/BeatmapObjectData.hpp"
#include "GlobalNamespace/SortedList_1.hpp"
#include "GlobalNamespace/SortedList_2.hpp"
#include "GlobalNamespace/BeatmapCallbacksUpdater.hpp"
#include "GlobalNamespace/VariableMovementDataProvider.hpp"
#include "System/Collections/Generic/HashSet_1.hpp"
#include "System/Collections/Generic/Dictionary_2.hpp"
#include "System/Action.hpp"

#include "custom-json-data/shared/CustomBeatmapData.h"
#include "Animation/Events.h"
#include "AssociatedData.h"
#include "tracks/shared/TimeSourceHelper.h"
#include "NEHooks.h"
#include "NELogger.h"
#include "SharedUpdate.h"
#include "NECaches.h"
#include "SpawnDataHelper.h"

using namespace GlobalNamespace;

// Initialize globals defensively
static BeatmapCallbacksController* controller = nullptr;
static GlobalNamespace::IReadonlyBeatmapData* beatmapData = nullptr;

static BeatmapObjectSpawnController::InitData* initData = nullptr;
static GlobalNamespace::BeatmapObjectSpawnMovementData* movementData = nullptr;

// Mark helpers as maybe_unused while LateUpdate sorting is disabled.
[[maybe_unused]] inline float GetSpawnAheadTime(BeatmapObjectSpawnController::InitData* initData_,
                                                BeatmapObjectSpawnMovementData* movementData_,
                                                std::optional<float> inputNjs,
                                                std::optional<float> inputOffset) {
  return 0.5f + (SpawnDataHelper::GetJumpDuration(inputNjs, inputOffset) * 0.5f);
}

[[maybe_unused]] inline float ObjectSortGetTime(BeatmapDataItem* n) {
  static auto* customObstacleDataClass = classof(CustomJSONData::CustomObstacleData*);
  static auto* customNoteDataClass = classof(CustomJSONData::CustomNoteData*);

  float* aheadTime;
  CustomJSONData::JSONWrapper* customDataWrapper;

  if (n->klass == customObstacleDataClass) {
    auto* obstacle = reinterpret_cast<CustomJSONData::CustomObstacleData*>(n);
    aheadTime = &obstacle->aheadTimeNoodle;
    customDataWrapper = obstacle->customData;
  } else if (n->klass == customNoteDataClass) {
    auto* note = reinterpret_cast<CustomJSONData::CustomNoteData*>(n);
    aheadTime = &note->aheadTimeNoodle;
    customDataWrapper = note->customData;
  } else {
    return n->time;
  }

  auto const& ad = getAD(customDataWrapper);
  auto const njs = ad.objectData.noteJumpMovementSpeed;
  auto const spawnOffset = ad.objectData.noteJumpStartBeatOffset;

  *aheadTime = GetSpawnAheadTime(initData, movementData, njs, spawnOffset);
  return n->time - *aheadTime;
}

[[maybe_unused]] constexpr bool ObjectTimeCompare(BeatmapDataItem* a, BeatmapDataItem* b) {
  return ObjectSortGetTime(a) < ObjectSortGetTime(b);
}

[[maybe_unused]] System::Collections::Generic::LinkedList_1<BeatmapDataItem*>*
SortAndOrderList(CustomJSONData::CustomBeatmapData* beatmapData_) {
  initData = NECaches::GameplayCoreContainer->Resolve<BeatmapObjectSpawnController::InitData*>();
  movementData = GlobalNamespace::BeatmapObjectSpawnMovementData::New_ctor();
  movementData->Init(initData->noteLinesCount, NECaches::JumpOffsetYProvider.ptr(), NEVector::Vector3::right());

  auto items = beatmapData_->GetAllBeatmapItemsCpp();
  std::stable_sort(items.begin(), items.end(), ObjectTimeCompare);

  initData = nullptr;
  movementData = nullptr;

  auto newList = SafePtr(System::Collections::Generic::LinkedList_1<BeatmapDataItem*>::New_ctor());
  auto newListPtr = static_cast<System::Collections::Generic::LinkedList_1<BeatmapDataItem*>*>(newList);
  if (items.empty()) return newListPtr;

  for (auto const& o : items) {
    newList->AddLast(o);
  }
  return newListPtr;
}

// --- TEMP 1.40.8 shim ---
// Bypass the LateUpdate sorting path that triggers safeAbort during animated-note maps.
// We'll re-enable a fixed version once the VNJS provider integration is in place.
MAKE_HOOK_MATCH(BeatmapCallbacksUpdater_LateUpdate, &BeatmapCallbacksUpdater::LateUpdate, void,
                BeatmapCallbacksUpdater* self) {
  // Keep normal behavior for non-NE maps or when hooks disabled
  if (!Hooks::isNoodleHookEnabled()) {
    controller = nullptr;
    beatmapData = nullptr;
    return BeatmapCallbacksUpdater_LateUpdate(self);
  }

  static bool s_warned = false;
  if (!s_warned) {
    NELogger::Logger.warn("Bypassing NE LateUpdate sort (temporary 1.40.8 shim)");
    s_warned = true;
  }

  // Call the game’s original LateUpdate without any NE sorting/relinking for now.
  return BeatmapCallbacksUpdater_LateUpdate(self);
}

void InstallBeatmapObjectCallbackControllerHooks() {
  INSTALL_HOOK(NELogger::Logger, BeatmapCallbacksUpdater_LateUpdate);
}

NEInstallHooks(InstallBeatmapObjectCallbackControllerHooks);
