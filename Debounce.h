#ifndef DEBOUNCE_H
#define DEBOUNCE_H
#include <stdint.h>
#include "String.h"

using namespace std;

namespace Debounce
{
    class Debouncer;
    class Input;
    class InputListener;
    class State;

    class InputListener
    {
    public:
        virtual void handleActivated(void) = 0;
        virtual void handleDeactivated(void) = 0;
    };

    class State
    {
    public:
        explicit State(Debouncer*);
        virtual void handleActivated(void) = 0;
        virtual void handleDeactivated(void) = 0;
        virtual bool isActivated(void) = 0;
        virtual STRING getName(void) = 0;
    protected:
        void changeState(State*);
        bool transitionOccurred(void);
        Debouncer* getMachine(void);
    private:
        Debouncer* _machine;
    };

    class UnknownState : public State
    {
    public:
        using State::State;
        void handleActivated(void);
        void handleDeactivated(void);
        bool isActivated(void);
        STRING getName(void);
    };

    class ActivatingState : public State
    {
    public:
        using State::State;
        void handleActivated(void);
        void handleDeactivated(void);
        bool isActivated(void);
        STRING getName(void);
    };

    class ActiveState : public State
    {
    public:
        using State::State;
        void handleActivated(void);
        void handleDeactivated(void);
        bool isActivated(void);
        STRING getName(void);
    };

    class InactiveState : public State
    {
    public:
        using State::State;
        void handleActivated(void);
        void handleDeactivated(void);
        bool isActivated(void);
        STRING getName(void);
    };

    class InactivatingState : public State
    {
    public:
        using State::State;
        void handleActivated(void);
        void handleDeactivated(void);
        bool isActivated(void);
        STRING getName(void);
    };

    class Debouncer : public InputListener
    {
    public:
        Debouncer(void);
        void handleActivated(void);
        void handleDeactivated(void);
        bool isActivated(void) const;
        void registerListener(InputListener*);
        void deregisterListener(void);
        uint32_t getDebounceTimeUs(void);

    private:
        void changeState(State*);
        uint32_t getTransitionStartTime(void);
        void resetTransitionStartTime(void);

    private:
        friend class State;
        friend class UnknownState;
        friend class ActivatingState;
        friend class ActiveState;
        friend class InactivatingState;
        friend class InactiveState;

    private:
        static const uint32_t _debounceTimeUs;
        uint32_t _transitionStartTime;
        InputListener* _listener;
        Input* _input;
        State* _currentState;
        UnknownState _unknownState;
        ActivatingState _activatingState;
        ActiveState _activeState;
        InactiveState _inactiveState;
        InactivatingState _inactivatingState;
    };

}
#endif // DEBOUNCE_H