#include <stddef.h>
#include <stdint.h>

#include "Debounce.h"
#include "Logging.h"
#include "Osal.h"
#include "String.h"
using namespace Debounce;

const uint32_t Debouncer::_debounceTimeUs = 500UL;

Debouncer::Debouncer(void) :
    _unknownState(this),
    _activeState(this),
    _activatingState(this),
    _inactiveState(this),
    _inactivatingState(this)
{
    _currentState = &_unknownState;
    _listener = NULL;
}

void Debouncer::handleActivated(void)
{
    _currentState->handleActivated();
}

void Debouncer::handleDeactivated(void)
{
    _currentState->handleDeactivated();
}

bool Debouncer::isActivated(void) const
{
    return _currentState->isActivated();
}

void Debouncer::registerListener(InputListener* toRegister)
{
    _listener = toRegister;
}

void Debouncer::deregisterListener(void)
{
    _listener = NULL;
}

uint32_t Debouncer::getDebounceTimeUs(void)
{
    return _debounceTimeUs;
}

uint32_t Debouncer::getTransitionStartTime(void)
{
    return _transitionStartTime;
}

void Debouncer::resetTransitionStartTime(void)
{
    _transitionStartTime = Osal::GetTimeInMicroseconds();
}

/*

    STATE DEFINITIONS

*/

State::State(Debouncer* machine) : _machine(machine) {}

void State::changeState(State* newState)
{
    LOG_DEBUG("state -> ");
    LOG_DEBUG(newState->getName());
    LOG_DEBUG("\n");
    _machine->_currentState = newState;
    _machine->resetTransitionStartTime();
}

bool State::transitionOccurred(void)
{
    uint32_t elapsed =
        Osal::GetTimeInMicroseconds() - _machine->getTransitionStartTime();
    return elapsed > _machine->getDebounceTimeUs();
}

Debouncer* State::getMachine(void)
{
    return _machine;
}

bool UnknownState::isActivated(void)
{
    // this state cannot know whether the input is activated or not. For safety
    // it assumes that the input is inactive until the debouncer has determined
    // its state
    return false;
}

void UnknownState::handleActivated(void)
{
    changeState(&(getMachine()->_activatingState));
}

void UnknownState::handleDeactivated(void) {}

STRING UnknownState::getName(void)
{
    return "UnknownState";
}

bool ActivatingState::isActivated(void)
{
    return false;
}

void ActivatingState::handleActivated(void)
{
    if (transitionOccurred())
    {
        // Not sure about the way we need to lookup from the machine. Perhaps
        // the states should have independent knowledge of each other?
        changeState(&(getMachine()->_activeState));
        if (getMachine() ->_listener)
        {
            getMachine()->_listener->handleActivated();
        }
    }
}

void ActivatingState::handleDeactivated(void)
{
    // input has gone low again so go back to inactive...
    changeState(&(getMachine()->_inactiveState));
}

STRING ActivatingState::getName(void)
{
    return "ActivatingState";
}

bool ActiveState::isActivated(void)
{
    return true;
}

void ActiveState::handleActivated(void)
{}

void ActiveState::handleDeactivated(void)
{
    changeState(&(getMachine()->_inactivatingState));
}

STRING ActiveState::getName(void)
{
    return "ActiveState";
}

bool InactiveState::isActivated(void)
{
    return false;
}

void InactiveState::handleActivated(void)
{
    changeState(&(getMachine()->_activatingState));
}

void InactiveState::handleDeactivated(void) {}

STRING InactiveState::getName(void)
{
    return "InactiveState";
}

bool InactivatingState::isActivated(void)
{
    return true;
}

void InactivatingState::handleActivated(void)
{
    changeState(&(getMachine()->_activeState));
}

void InactivatingState::handleDeactivated(void)
{
    if (transitionOccurred())
    {
        changeState(&(getMachine()->_inactiveState));
        if (getMachine()->_listener)
        {
            getMachine()->_listener->handleDeactivated();
        }
    }
}

STRING InactivatingState::getName(void)
{
    return "InactivatingState";
}
