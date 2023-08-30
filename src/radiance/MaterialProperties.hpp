/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef RADIANCE_MATERIALPROPERTIES_HPP
#define RADIANCE_MATERIALPROPERTIES_HPP

#include "RadianceAPI.hpp"

#include <vector>

namespace openstudio {
namespace radiance {

  /** MaterialProperties represents the color, texture, transmittance, reflectance of opaque and glazing materials.
    */
  class RADIANCE_API MaterialProperties
  {
   public:
  };

  // vector of MaterialProperties
  using LMaterialPropertiesVector = std::vector<MaterialProperties>;

}  // namespace radiance
}  // namespace openstudio

#endif  //RADIANCE_MATERIALPROPERTIES_HPP
