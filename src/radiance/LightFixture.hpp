/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef RADIANCE_LIGHTFIXTURE_HPP
#define RADIANCE_LIGHTFIXTURE_HPP

#include "RadianceAPI.hpp"

#include "../utilities/data/Vector.hpp"
#include "../utilities/core/Path.hpp"

namespace openstudio {
namespace radiance {

  /** LightFixture represents the physical location, orientation, and illuminance distribution of a light fixture.
  */
  class RADIANCE_API LightFixture
  {
   public:
    openstudio::Vector location;
    openstudio::Vector orientation;
    openstudio::path iesFile;
  };

  // vector of LightFixture
  using LightFixtureVector = std::vector<LightFixture>;

}  // namespace radiance
}  // namespace openstudio

#endif  //RADIANCE_LIGHTFIXTURE_HPP
