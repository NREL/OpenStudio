/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
