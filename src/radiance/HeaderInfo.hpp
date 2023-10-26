/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef RADIANCE_HEADERINFO_HPP
#define RADIANCE_HEADERINFO_HPP

#include "RadianceAPI.hpp"

#include "../utilities/data/Vector.hpp"
#include "../utilities/core/Logger.hpp"

namespace openstudio {
namespace radiance {

  /** HeaderInfo represents the header of a radiance output file from SPOT.
   *   It's primary function is to define the x and y axis of the illuminance map.
   */
  class RADIANCE_API HeaderInfo
  {
   public:
    /// header consists of two lines
    HeaderInfo(const std::string& line1, const std::string& line2);

    /// virtual destructor
    virtual ~HeaderInfo() = default;

    /// get the x points corresponding to illuminance matrix columns in meters
    openstudio::Vector xVector() const {
      return m_xVector;
    }

    /// get the y points corresponding to illuminance matrix rows in meters
    openstudio::Vector yVector() const {
      return m_yVector;
    }

   private:
    REGISTER_LOGGER("radiance.HeaderInfo");

    openstudio::Vector m_xVector;
    openstudio::Vector m_yVector;
  };

}  // namespace radiance
}  // namespace openstudio

#endif  //RADIANCE_HEADERINFO_HPP
