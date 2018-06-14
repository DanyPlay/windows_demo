#include "stdafx.h"
#include "AtomicBoolean.h"


AtomicBoolean::AtomicBoolean()
{
    m_bValue = false;
}

AtomicBoolean::AtomicBoolean(bool initialValue)
{
    m_bValue = initialValue;
}

bool AtomicBoolean::Get()
{
    AutoThreadSection lock(&m_oThreadSection);

    return m_bValue;
}

void AtomicBoolean::Set(bool value)
{
    AutoThreadSection lock(&m_oThreadSection);

    m_bValue = value;
}

bool AtomicBoolean::GetAndSet(bool newValue)
{
    return CompareAndSet(true, newValue);
}

bool AtomicBoolean::CompareAndSet(bool expect, bool update)
{
    AutoThreadSection lock(&m_oThreadSection);

    bool old = m_bValue;

    if (expect)
    {
        m_bValue = update;
    }

    return old;
}
