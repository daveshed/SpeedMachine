#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

class Transmitter {
public:
    virtual bool IsEnabled(void) const = 0;
    virtual void Enable(void) = 0;
    virtual void Disable(void) = 0;
    virtual unsigned long GetPingElapsedTimeMicroseconds(void) const = 0;
};

class EchoListener {
public:
    virtual void HandleEcho(void) = 0;
};

class Receiver {
public:
    virtual bool IsEnabled(void) const = 0;
    virtual void Enable(void) = 0;
    virtual void Disable(void) = 0;
    virtual void RegisterListener(EchoListener*) = 0;
};

class UltrasonicSensor : public EchoListener
{
public:
    UltrasonicSensor(Receiver& receiver, Transmitter& transmitter);
    void Enable(void);
    void Disable(void);
    bool IsEnabled(void) const {return enabled; }
    double GetDistanceCm(void);
    void HandleEcho(void);
private:
    Receiver& receiver;
    Transmitter& transmitter;
    double distance = 0.0;
    bool enabled = false;
    unsigned long ElapsedTime(void);
    void Reset(void);
};

#endif // ULTRASONICSENSOR_H