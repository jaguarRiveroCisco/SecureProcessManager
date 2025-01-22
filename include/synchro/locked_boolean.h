#pragma once

#ifndef SYNCHRO_LOCKED_BOOLEAN_H
#define SYNCHRO_LOCKED_BOOLEAN_H

#include "locked_variable.h"

namespace concurrency
{
    struct LockedBoolean final : public LockedVariable<bool> {};
} // namespace concurrency


#endif // SYNCHRO_LOCKED_BOOLEAN_H