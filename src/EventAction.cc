#include <G4Event.hh>

#include "EventAction.hh"

//__________________________________________________________________________________________________________

EventAction::EventAction(AnalysisManager *pAnalysisManager)
{
  m_pAnalysisManager = pAnalysisManager;
}

//__________________________________________________________________________________________________________

EventAction::~EventAction()
{
}

//__________________________________________________________________________________________________________

void
EventAction::BeginOfEventAction(const G4Event *pEvent)
{
  if(pEvent->GetEventID() % 10000 == 0)
    {
      G4cout << "EventAction::BeginOfEventAction:: Begin event " << pEvent->GetEventID()<< "------" << G4endl;
    }
  
  if(m_pAnalysisManager)
    m_pAnalysisManager->BeginOfEvent(pEvent);
}

//__________________________________________________________________________________________________________

void EventAction::EndOfEventAction(const G4Event *pEvent)
{
  if(m_pAnalysisManager)
    m_pAnalysisManager->EndOfEvent(pEvent);
}

//__________________________________________________________________________________________________________