#include "TestAcceptanceRadiusProvider.h"
#include "TestNodeAction_MoveToLocation.h"

// Add default functionality here for any IITestAcceptanceRadiusProvider functions that are not pure virtual.
float ITestAcceptanceRadiusProvider::GetAcceptanceRadius() const
{
	return UTestNodeAction_MoveToLocation::DefaultArrivalRadius;
}