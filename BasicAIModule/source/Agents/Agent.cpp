/*
 *  Agent.cpp
 */
#include "Agent.h"
#include "Manager.h"
#include "DebugFlags.h"

#include <string>

#include <BWAPI.h>

using namespace BWAPI;

using std::string;

Agent::Agent(Unit& u)
    : unit(u)
    , state(IdleState)
    , unitTarget(NULL)
    , positionTarget()
    , unitTypeTarget(UnitTypes::None)
    , buildingReserved(false)
    , buildingLocation()
    , parentManager(NULL)
{ }

bool Agent::operator==(const Agent& other)
{
	// Compare by pointer values
	if( &(this->unit) == &(other.unit) )
		return true;
	else
		return false;
}

void Agent::update()
{
    if( state == BuildState )
        positionTarget = Position(buildingLocation);
}

void Agent::setParentManager(Manager *manager)
{
    if( manager != NULL ) 
        parentManager = manager;
}

const std::string Agent::getParentManagerName() const 
{
    return (parentManager != NULL) ? 
        parentManager->getName() : "None";
}

void Agent::draw()
{
	if( DebugFlags::instance().getFlag(none) )
		return;

	const int px = unit.getPosition().x();
	const int py = unit.getPosition().y();
	const int radius = unit.getRight() - px;

	// Draw owner, state, type
	if (//state != GatherState &&
		DebugFlags::instance().getFlag(agent_details) )
		Broodwar->drawTextMap(px, py, "(%s, %s%s)", getParentManagerName().c_str(),
			StateStrings[state], unitTypeTargetValid() ? (string(", ") += string(UnitTypeStrings[unitTypeTarget.getID()])).c_str()
														:	"");
	// Targets
	if( DebugFlags::instance().getFlag(agent_targets) )
	{
        Broodwar->drawCircleMap(px, py, radius, Colors::Yellow);

        // Draw line to position target if its valid
        if( positionTarget.isValid()
         && positionTarget != Positions::None 
         && positionTarget != Positions::Unknown 
         && positionTarget.x() != 0 && positionTarget.y() != 0 )
        {
            const int tx = positionTarget.x();
            const int ty = positionTarget.y();
            Broodwar->drawLineMap(px, py, tx, ty, Colors::Green);
            Broodwar->drawTextMap(tx, ty, "(%d, %d)", tx, ty);
        }

        // Draw a line to the unit target if its valid
        if( unitTarget != NULL )
        {
            const Position targetPos = unitTarget->getPosition();
            if( targetPos.isValid() 
             && targetPos != Positions::None 
             && targetPos != Positions::Unknown 
             && targetPos.x() != 0 && targetPos.y() != 0 )
            {
                const int tx = unitTarget->getPosition().x();
                const int ty = unitTarget->getPosition().y();
                Broodwar->drawLineMap(px+1, py+1, tx+1, ty+1, Colors::Red);
                Broodwar->drawTextMap(tx, ty, "id: %x", unitTarget);
            }
        }

		// Draw in-game target
		if (unit.getTargetPosition().isValid()) {
			const int tx = unit.getTargetPosition().x();
			const int ty = unit.getTargetPosition().y();
			Broodwar->drawLineMap(px+2, py+2, tx+2, ty+2, Colors::Cyan);
		}
    }

	/*
	if (state == AttackState)
	{
		// Draw our attack range
		Position unitPosition = unit.getPosition();
		int x = unitPosition.x();
		int y = unitPosition.y();
		int attackRadius = unit.getType().seekRange();
		Broodwar->drawCircle(CoordinateType::Map, x, y, attackRadius, BWAPI::Colors::White);
	}
	*/
}

void Agent::build(UnitType type)
{
	if (type.isBuilding())
		setState(BuildState);
	else
		setState(TrainState);
	setUnitTypeTarget(type);
}

