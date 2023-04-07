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

#ifndef ISOMODEL_SOLARRADIATION_HPP
#define ISOMODEL_SOLARRADIATION_HPP

#include <math.h>
#include <algorithm>
#include <vector>
#include "TimeFrame.hpp"
#include "EpwData.hpp"
#include "../utilities/core/Logger.hpp"

namespace openstudio {
namespace isomodel {

  class SolarRadiation
  {
   public:
    SolarRadiation(const TimeFrame& frame, const EpwData& wdata, double tilt = 3.141592653589);

    static const int NUM_SURFACES = 8;
    static const int MONTHS = 12;
    static const int HOURS = 24;

    void Calculate();

    //outputs
    const std::vector<std::vector<double>>& eglobe() const {
      return m_eglobe;
    }  //total solar radiation from direct beam, ground reflect and diffuse
    //averages
    const std::vector<double>& monthlyDryBulbTemp() const {
      return m_monthlyDryBulbTemp;
    }
    const std::vector<double>& monthlyDewPointTemp() const {
      return m_monthlyDewPointTemp;
    }
    const std::vector<double>& monthlyRelativeHumidity() const {
      return m_monthlyRelativeHumidity;
    }
    const std::vector<double>& monthlyWindspeed() const {
      return m_monthlyWindspeed;
    }
    const std::vector<double>& monthlyGlobalHorizontalRadiation() const {
      return m_monthlyGlobalHorizontalRadiation;
    }
    const std::vector<std::vector<double>>& monthlySolarRadiation() const {
      return m_monthlySolarRadiation;
    }
    const std::vector<std::vector<double>>& hourlyDryBulbTemp() const {
      return m_hourlyDryBulbTemp;
    }
    const std::vector<std::vector<double>>& hourlyDewPointTemp() const {
      return m_hourlyDewPointTemp;
    }
    const std::vector<std::vector<double>>& hourlyGlobalHorizontalRadiation() const {
      return m_hourlyGlobalHorizontalRadiation;
    }

   protected:
    openstudio::isomodel::TimeFrame m_frame;
    openstudio::isomodel::EpwData m_weatherData;
    void calculateSurfaceSolarRadiation();
    void calculateAverages();
    void calculateMonthAvg(int midx, int cnt);
    void clearMonthlyAvg(int midx);

    //inputs
    double m_surfaceTilt;
    double m_localMeridian;  //LSM
    double m_longitude;
    double m_latitude;  //latitude in radians

    //outputs
    std::vector<std::vector<double>> m_eglobe;  //total solar radiation from direct beam, ground reflect and diffuse

    //averages
    std::vector<double> m_monthlyDryBulbTemp;
    std::vector<double> m_monthlyDewPointTemp;
    std::vector<double> m_monthlyRelativeHumidity;
    std::vector<double> m_monthlyWindspeed;
    std::vector<double> m_monthlyGlobalHorizontalRadiation;
    std::vector<std::vector<double>> m_monthlySolarRadiation;
    std::vector<std::vector<double>> m_hourlyDryBulbTemp;
    std::vector<std::vector<double>> m_hourlyDewPointTemp;
    std::vector<std::vector<double>> m_hourlyGlobalHorizontalRadiation;

   private:
    REGISTER_LOGGER("openstudio.isomodel.SolarRadiation");
  };

}  // namespace isomodel
}  // namespace openstudio
#endif  // ISOMODEL_SOLARRADIATION_HPP
