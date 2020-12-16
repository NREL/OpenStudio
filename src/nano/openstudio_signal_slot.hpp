#pragma once

#include "nano_signal_slot.hpp"

namespace openstudio
{
  // Creating aliases when using nano-signal-slot will increase the maintainability of your code
  // especially if you are choosing to use the alternative policies.
  //
  using OS_Policy = Nano::ST_Policy_Safe;

  template <typename RT>
  using Signal = Nano::Signal<RT, OS_Policy>;

  using Observer = Nano::Observer<OS_Policy>;

} // namespace openstudio

