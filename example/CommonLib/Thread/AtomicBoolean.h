#pragma once
#include "ThreadLock.h"

class AtomicBoolean
{
public:
    /**
    * Creates a new {@code AtomicBoolean} with initial value {@code false}.
    */
    AtomicBoolean();

    /**
    * Creates a new {@code AtomicBoolean} with the given initial value.
    *
    * @param initialValue the initial value
    */
    AtomicBoolean(bool initialValue);

public:
    /**
    * Returns the current value.
    *
    * @return the current value
    */
    bool Get();

    /**
    * Unconditionally sets to the given value.
    *
    * @param newValue the new value
    */
    void Set(bool value);

    /**
    * Atomically sets to the given value and returns the previous value.
    *
    * @param newValue the new value
    * @return the previous value
    */
    bool GetAndSet(bool newValue);

    /**
    * Atomically sets the value to the given updated value
    * if the current value {@code ==} the expected value.
    *
    * @param expect the expected value
    * @param update the new value
    * @return {@code true} if successful. False return indicates that
    * the actual value was not equal to the expected value.
    */
    bool CompareAndSet(bool expect, bool update);

private:
    ThreadSection m_oThreadSection;

    bool m_bValue;
};