/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef RADIANCE_ANNUALILLUMINANCEMAP_HPP
#define RADIANCE_ANNUALILLUMINANCEMAP_HPP

#include "RadianceAPI.hpp"

#include "../utilities/data/Vector.hpp"
#include "../utilities/data/Matrix.hpp"
#include "../utilities/time/DateTime.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Path.hpp"

namespace openstudio {
namespace radiance {

  /** AnnualIlluminanceMap represents illuminance map for an entire year.
  *   We assume that the output files is from SPOT, with length in meters and illuminance
  *   values in footcandles.  All illuminance values are converted to lux.
  */
  class RADIANCE_API AnnualIlluminanceMap
  {
   private:
    // map of DateTime to Matrix (illuminance map)
    using DateTimeIlluminanceMap = std::map<openstudio::DateTime, openstudio::Matrix>;

   public:
    /// default constructor
    AnnualIlluminanceMap();

    /// constructor with path
    AnnualIlluminanceMap(const openstudio::path& path);

    /// virtual destructor
    virtual ~AnnualIlluminanceMap() = default;

    /// get the dates and times for which illuminance maps are available
    openstudio::DateTimeVector dateTimes() const {
      return m_dateTimes;
    }
    //openstudio::DateTime::ConstVec dateTimes() const {return openstudio::DateTime::ConstVec(m_dateTimes.begin(), m_dateTimes.end());}

    /// get the x points corresponding to illuminance matrix columns in meters
    openstudio::Vector xVector() const {
      return m_xVector;
    }

    /// get the y points corresponding to illuminance matrix rows in meters
    openstudio::Vector yVector() const {
      return m_yVector;
    }

    /// get the illuminance map in lux corresponding to date and time
    openstudio::Matrix illuminanceMap(const openstudio::DateTime& dateTime) const;

   private:
    REGISTER_LOGGER("radiance.AnnualIlluminanceMap");

    void init(const openstudio::path& path);

    openstudio::DateTimeVector m_dateTimes;
    openstudio::Vector m_xVector;
    openstudio::Vector m_yVector;
    openstudio::Matrix m_nullIlluminanceMap;  // used when there is no data
    DateTimeIlluminanceMap m_dateTimeIlluminanceMap;
  };

}  // namespace radiance
}  // namespace openstudio

#endif  //RADIANCE_ANNUALILLUMINANCEMAP_HPP
