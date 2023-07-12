/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef RADIANCE_PHOTOSENSOR_HPP
#define RADIANCE_PHOTOSENSOR_HPP

#include "RadianceAPI.hpp"

#include "../utilities/data/Vector.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/data/TimeSeries.hpp"
#include <vector>

namespace openstudio {
namespace radiance {

  /** Photosensor represents the physical location, orientation, and sensitivity pattern of a photocell. As well as
   *   output from simulation.
   */
  class RADIANCE_API Photosensor
  {
   public:
    openstudio::Vector location;
    openstudio::Vector orientation;
    openstudio::path iesFile;
    openstudio::TimeSeries illuminanceValues();
  };

  // vector of Photosensor
  using PhotosensorVector = std::vector<Photosensor>;

}  // namespace radiance
}  // namespace openstudio

#endif  //RADIANCE_PHOTOSENSOR_HPP
