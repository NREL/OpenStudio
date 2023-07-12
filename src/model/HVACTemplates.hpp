/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HVACTEMPLATES_HPP
#define MODEL_HVACTEMPLATES_HPP

#include "ModelAPI.hpp"
#include <vector>

namespace openstudio {

namespace model {

  class Loop;

  class Model;

  class ThermalZone;

  MODEL_API void addSystemType1(Model& model, std::vector<ThermalZone> zones);

  MODEL_API void addSystemType2(Model& model, std::vector<ThermalZone> zones);

  MODEL_API Loop addSystemType3(Model& model);

  MODEL_API Loop addSystemType4(Model& model);

  MODEL_API Loop addSystemType5(Model& model);

  MODEL_API Loop addSystemType6(Model& model);

  MODEL_API Loop addSystemType7(Model& model);

  MODEL_API Loop addSystemType8(Model& model);

  MODEL_API Loop addSystemType9(Model& model);

  MODEL_API Loop addSystemType10(Model& model);

  /** Add an example Service Hot Water (SHW) Loop */
  MODEL_API Loop addSHWLoop(Model& model);

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_HVACTEMPLATES_HPP
