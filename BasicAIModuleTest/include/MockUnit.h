#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "BWAPI.h"

using namespace testing;
using namespace std;


// googlemock\scripts\generator>python gmock_gen.py include\BWAPI\Unit.h Unit >> MockUnit.h


namespace BWAPI {
//
//class Unit;
//class UnitType;
//class UnitCommand;
//class UpgradeType;
//class TechType;
//class Order;
//class Player;
//class Position;
//class TilePosition;
//class Region;
//class WeaponType;

class MockUnit : public Unit {
 public:

	MockUnit();

  MOCK_CONST_METHOD0(getID,
      int());
  MOCK_CONST_METHOD0(getReplayID,
      int());
  MOCK_CONST_METHOD0(getPlayer,
      Player*());
  MOCK_CONST_METHOD0(getType,
      UnitType());
  MOCK_CONST_METHOD0(getPosition,
      Position());
  MOCK_CONST_METHOD0(getTilePosition,
      TilePosition());
  MOCK_CONST_METHOD0(getAngle,
      double());
  MOCK_CONST_METHOD0(getVelocityX,
      double());
  MOCK_CONST_METHOD0(getVelocityY,
      double());
  MOCK_CONST_METHOD0(getRegion,
      BWAPI::Region*());
  MOCK_CONST_METHOD0(getLeft,
      int());
  MOCK_CONST_METHOD0(getTop,
      int());
  MOCK_CONST_METHOD0(getRight,
      int());
  MOCK_CONST_METHOD0(getBottom,
      int());
  MOCK_CONST_METHOD0(getHitPoints,
      int());
  MOCK_CONST_METHOD0(getShields,
      int());
  MOCK_CONST_METHOD0(getEnergy,
      int());
  MOCK_CONST_METHOD0(getResources,
      int());
  MOCK_CONST_METHOD0(getResourceGroup,
      int());
  MOCK_CONST_METHOD1(getDistance,
      int(Unit* target));
  MOCK_CONST_METHOD1(getDistance,
      int(Position target));
  MOCK_CONST_METHOD1(hasPath,
      bool(Unit* target));
  MOCK_CONST_METHOD1(hasPath,
      bool(Position target));
  MOCK_CONST_METHOD0(getLastCommandFrame,
      int());
  MOCK_CONST_METHOD0(getLastCommand,
      UnitCommand());
  MOCK_CONST_METHOD0(getLastAttackingPlayer,
      BWAPI::Player*());
  MOCK_CONST_METHOD1(getUpgradeLevel,
      int(UpgradeType upgrade));
  MOCK_CONST_METHOD0(getInitialType,
      UnitType());
  MOCK_CONST_METHOD0(getInitialPosition,
      Position());
  MOCK_CONST_METHOD0(getInitialTilePosition,
      TilePosition());
  MOCK_CONST_METHOD0(getInitialHitPoints,
      int());
  MOCK_CONST_METHOD0(getInitialResources,
      int());
  MOCK_CONST_METHOD0(getKillCount,
      int());
  MOCK_CONST_METHOD0(getAcidSporeCount,
      int());
  MOCK_CONST_METHOD0(getInterceptorCount,
      int());
  MOCK_CONST_METHOD0(getScarabCount,
      int());
  MOCK_CONST_METHOD0(getSpiderMineCount,
      int());
  MOCK_CONST_METHOD0(getGroundWeaponCooldown,
      int());
  MOCK_CONST_METHOD0(getAirWeaponCooldown,
      int());
  MOCK_CONST_METHOD0(getSpellCooldown,
      int());
  MOCK_CONST_METHOD0(getDefenseMatrixPoints,
      int());
  MOCK_CONST_METHOD0(getDefenseMatrixTimer,
      int());
  MOCK_CONST_METHOD0(getEnsnareTimer,
      int());
  MOCK_CONST_METHOD0(getIrradiateTimer,
      int());
  MOCK_CONST_METHOD0(getLockdownTimer,
      int());
  MOCK_CONST_METHOD0(getMaelstromTimer,
      int());
  MOCK_CONST_METHOD0(getOrderTimer,
      int());
  MOCK_CONST_METHOD0(getPlagueTimer,
      int());
  MOCK_CONST_METHOD0(getRemoveTimer,
      int());
  MOCK_CONST_METHOD0(getStasisTimer,
      int());
  MOCK_CONST_METHOD0(getStimTimer,
      int());
  MOCK_CONST_METHOD0(getBuildType,
      UnitType());
  MOCK_CONST_METHOD0(getTrainingQueue,
      std::list<UnitType>());
  MOCK_CONST_METHOD0(getTech,
      TechType());
  MOCK_CONST_METHOD0(getUpgrade,
      UpgradeType());
  MOCK_CONST_METHOD0(getRemainingBuildTime,
      int());
  MOCK_CONST_METHOD0(getRemainingTrainTime,
      int());
  MOCK_CONST_METHOD0(getRemainingResearchTime,
      int());
  MOCK_CONST_METHOD0(getRemainingUpgradeTime,
      int());
  MOCK_CONST_METHOD0(getBuildUnit,
      Unit*());
  MOCK_CONST_METHOD0(getTarget,
      Unit*());
  MOCK_CONST_METHOD0(getTargetPosition,
      Position());
  MOCK_CONST_METHOD0(getOrder,
      Order());
  MOCK_CONST_METHOD0(getSecondaryOrder,
      Order());
  MOCK_CONST_METHOD0(getOrderTarget,
      Unit*());
  MOCK_CONST_METHOD0(getOrderTargetPosition,
      Position());
  MOCK_CONST_METHOD0(getRallyPosition,
      Position());
  MOCK_CONST_METHOD0(getRallyUnit,
      Unit*());
  MOCK_CONST_METHOD0(getAddon,
      Unit*());
  MOCK_CONST_METHOD0(getNydusExit,
      Unit*());
  MOCK_CONST_METHOD0(getPowerUp,
      Unit*());
  MOCK_CONST_METHOD0(getTransport,
      Unit*());


  MOCK_CONST_METHOD0(getLoadedUnits,
      std::set<Unit*>());
  MOCK_CONST_METHOD0(getCarrier,
      Unit*());
  MOCK_CONST_METHOD0(getInterceptors,
      std::set<Unit*>());
  MOCK_CONST_METHOD0(getHatchery,
      Unit*());
  MOCK_CONST_METHOD0(getLarva,
      std::set<Unit*>());
  MOCK_CONST_METHOD1(getUnitsInRadius,
      std::set<Unit*>&(int radius));
  MOCK_CONST_METHOD1(getUnitsInWeaponRange,
      std::set<Unit*>&(WeaponType weapon));
  MOCK_CONST_METHOD0(getClientInfo,
      void*());
  MOCK_METHOD1(setClientInfo,
      void(void* clientinfo));
  MOCK_CONST_METHOD0(exists,
      bool());
  MOCK_CONST_METHOD0(hasNuke,
      bool());
  MOCK_CONST_METHOD0(isAccelerating,
      bool());
  MOCK_CONST_METHOD0(isAttacking,
      bool());
  MOCK_CONST_METHOD0(isAttackFrame,
      bool());
  MOCK_CONST_METHOD0(isBeingConstructed,
      bool());
  MOCK_CONST_METHOD0(isBeingGathered,
      bool());
  MOCK_CONST_METHOD0(isBeingHealed,
      bool());
  MOCK_CONST_METHOD0(isBlind,
      bool());
  MOCK_CONST_METHOD0(isBraking,
      bool());
  MOCK_CONST_METHOD0(isBurrowed,
      bool());
  MOCK_CONST_METHOD0(isCarryingGas,
      bool());
  MOCK_CONST_METHOD0(isCarryingMinerals,
      bool());
  MOCK_CONST_METHOD0(isCloaked,
      bool());
  MOCK_CONST_METHOD0(isCompleted,
      bool());
  MOCK_CONST_METHOD0(isConstructing,
      bool());
  MOCK_CONST_METHOD0(isDefenseMatrixed,
      bool());
  MOCK_CONST_METHOD0(isDetected,
      bool());
  MOCK_CONST_METHOD0(isEnsnared,
      bool());
  MOCK_CONST_METHOD0(isFollowing,
      bool());
  MOCK_CONST_METHOD0(isGatheringGas,
      bool());
  MOCK_CONST_METHOD0(isGatheringMinerals,
      bool());
  MOCK_CONST_METHOD0(isHallucination,
      bool());
  MOCK_CONST_METHOD0(isHoldingPosition,
      bool());
  MOCK_CONST_METHOD0(isIdle,
      bool());
  MOCK_CONST_METHOD0(isInterruptible,
      bool());
  MOCK_CONST_METHOD0(isInvincible,
      bool());
  MOCK_CONST_METHOD1(isInWeaponRange,
      bool(Unit *target));
  MOCK_CONST_METHOD0(isIrradiated,
      bool());
  MOCK_CONST_METHOD0(isLifted,
      bool());
  MOCK_CONST_METHOD0(isLoaded,
      bool());
  MOCK_CONST_METHOD0(isLockedDown,
      bool());
  MOCK_CONST_METHOD0(isMaelstrommed,
      bool());
  MOCK_CONST_METHOD0(isMorphing,
      bool());
  MOCK_CONST_METHOD0(isMoving,
      bool());
  MOCK_CONST_METHOD0(isParasited,
      bool());
  MOCK_CONST_METHOD0(isPatrolling,
      bool());
  MOCK_CONST_METHOD0(isPlagued,
      bool());
  MOCK_CONST_METHOD0(isRepairing,
      bool());
  MOCK_CONST_METHOD0(isResearching,
      bool());
  MOCK_CONST_METHOD0(isSelected,
      bool());
  MOCK_CONST_METHOD0(isSieged,
      bool());
  MOCK_CONST_METHOD0(isStartingAttack,
      bool());
  MOCK_CONST_METHOD0(isStasised,
      bool());
  MOCK_CONST_METHOD0(isStimmed,
      bool());
  MOCK_CONST_METHOD0(isStuck,
      bool());
  MOCK_CONST_METHOD0(isTraining,
      bool());
  MOCK_CONST_METHOD0(isUnderAttack,
      bool());
  MOCK_CONST_METHOD0(isUnderDarkSwarm,
      bool());
  MOCK_CONST_METHOD0(isUnderDisruptionWeb,
      bool());
  MOCK_CONST_METHOD0(isUnderStorm,
      bool());
  MOCK_CONST_METHOD0(isUnpowered,
      bool());
  MOCK_CONST_METHOD0(isUpgrading,
      bool());
  MOCK_CONST_METHOD0(isVisible,
      bool());
  MOCK_CONST_METHOD1(isVisible,
      bool(Player* player));
  MOCK_CONST_METHOD1(canIssueCommand,
      bool(UnitCommand command));
  MOCK_METHOD1(issueCommand,
      bool(UnitCommand command));
  MOCK_METHOD2(attack,
      bool(Position, bool));
  MOCK_METHOD2(attack,
      bool(MockUnit, bool));
  MOCK_METHOD2(build,
      bool(TilePosition target, UnitType type));
  MOCK_METHOD1(buildAddon,
      bool(UnitType type));
  MOCK_METHOD1(train,
      bool(UnitType type));
  MOCK_METHOD1(morph,
      bool(UnitType type));
  MOCK_METHOD1(research,
      bool(TechType tech));
  MOCK_METHOD1(upgrade,
      bool(UpgradeType upgrade));
  MOCK_METHOD1(setRallyPoint,
      bool(Position target));
  MOCK_METHOD1(setRallyPoint,
      bool(Unit* target));
  MOCK_METHOD2(move,
      bool(Position, bool));
  MOCK_METHOD2(patrol,
      bool(Position, bool));
  MOCK_METHOD1(holdPosition,
      bool(bool));
  MOCK_METHOD1(stop,
      bool(bool));
  MOCK_METHOD2(follow,
      bool(MockUnit, bool));
  MOCK_METHOD2(gather,
      bool(MockUnit, bool));
  MOCK_METHOD1(returnCargo,
      bool(bool));
  MOCK_METHOD2(repair,
      bool(MockUnit, bool));
  MOCK_METHOD0(burrow,
      bool());
  MOCK_METHOD0(unburrow,
      bool());
  MOCK_METHOD0(cloak,
      bool());
  MOCK_METHOD0(decloak,
      bool());
  MOCK_METHOD0(siege,
      bool());
  MOCK_METHOD0(unsiege,
      bool());
  MOCK_METHOD0(lift,
      bool());
  MOCK_METHOD1(land,
      bool(TilePosition target));
  MOCK_METHOD2(load,
      bool(MockUnit, bool));
  MOCK_METHOD1(unload,
      bool(Unit* target));
  MOCK_METHOD1(unloadAll,
      bool(bool));
  MOCK_METHOD2(unloadAll,
      bool(Position, bool));
  MOCK_METHOD2(rightClick,
      bool(Position, bool));
  MOCK_METHOD2(rightClick,
      bool(MockUnit, bool));
  MOCK_METHOD0(haltConstruction,
      bool());
  MOCK_METHOD0(cancelConstruction,
      bool());
  MOCK_METHOD0(cancelAddon,
      bool());
  MOCK_METHOD1(cancelTrain,
      bool(int));
  MOCK_METHOD0(cancelMorph,
      bool());
  MOCK_METHOD0(cancelResearch,
      bool());
  MOCK_METHOD0(cancelUpgrade,
      bool());
  MOCK_METHOD1(useTech,
      bool(TechType tech));
  MOCK_METHOD2(useTech,
      bool(TechType tech, Position target));
  MOCK_METHOD2(useTech,
      bool(TechType tech, Unit* target));
  MOCK_METHOD1(placeCOP,
      bool(TilePosition target));
};

}  // namespace BWAPI
