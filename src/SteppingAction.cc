#include "SteppingAction.hh"
#include "AnalysisManager.hh"

#include "G4SteppingManager.hh"

#include <string.h>
#include <cmath>

//__________________________________________________________________________________________________________

SteppingAction::SteppingAction(AnalysisManager *myAM):myAnalysisManager(myAM)
{
}

//__________________________________________________________________________________________________________

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
}

//__________________________________________________________________________________________________________