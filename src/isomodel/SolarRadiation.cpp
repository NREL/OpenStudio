/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "SolarRadiation.hpp"

#define PI 3.141592653589

namespace openstudio {
namespace isomodel {
  /**
   * Surface Azimuths of the "building" to calculate solar radiation for
   */
  double SurfaceAzimuths[] = {0, -45, -90, -135, 180, 135, 90, 45};

  SolarRadiation::SolarRadiation(const TimeFrame& frame, const EpwData& wdata, double tilt)
    : m_frame(frame),
      m_weatherData(wdata),

      m_surfaceTilt(tilt / 2.0),                 //surface tilt in radians (pi/2 is vertical, 0 is horizontal)
      m_localMeridian(wdata.timezone() * 15.0),  //compute the local meridian from the time zone.  Negative is W of the prime meridian
      m_longitude(wdata.longitude()),
      m_latitude(wdata.latitude() * PI / 180.0),  //convert latitute to radians

      m_eglobe(TimeFrame::TIMESLICES, std::vector<double>(NUM_SURFACES, 0)),

      m_monthlyDryBulbTemp(MONTHS, 0),
      m_monthlyDewPointTemp(MONTHS, 0),
      m_monthlyRelativeHumidity(MONTHS, 0),
      m_monthlyWindspeed(MONTHS, 0),
      m_monthlyGlobalHorizontalRadiation(MONTHS, 0),
      m_monthlySolarRadiation(MONTHS, std::vector<double>(NUM_SURFACES, 0)),
      m_hourlyDryBulbTemp(MONTHS, std::vector<double>(HOURS, 0)),
      m_hourlyDewPointTemp(MONTHS, std::vector<double>(HOURS, 0)),
      m_hourlyGlobalHorizontalRadiation(MONTHS, std::vector<double>(HOURS, 0)) {}

  /**
   * compute the monthly average solar radiation incident on the vertical surfaces for the
   * eight primary directions (N, S, E, W, NW, SW, NE, SE)
   * these computations come from ASHRAE2007  Fundamentals , chapter 14
   * or Duffie and Beckman "Solar engineering of thermal processes, 3rd ed",
   * Wiley 2006
   */
  void SolarRadiation::calculateSurfaceSolarRadiation() {
    double rhog = 0.14;  //ground reflectivity coefficient

    const std::vector<std::vector<double>>& data = m_weatherData.data();
    const std::vector<double>& vecEB = data[EB];
    const std::vector<double>& vecED = data[ED];
    for (int i = 0; i < TimeFrame::TIMESLICES; i++) {
      // First compute the solar azimuth for each hour of the year for our location
      double Revolution =
        2.0 * PI * (static_cast<double>(m_frame.YTD[i]) - 1.0) / 365.0;  //should be .25? //calculation revolution angle around sun in radians
      double EquationOfTime = 2.2918
                              * (0.0075 + 0.1868 * cos(Revolution) - 3.2077 * sin(Revolution) - 1.4615 * cos(2 * Revolution)
                                 - 4.089 * sin(2 * Revolution));                                                    //equation of time??
      double ApparentSolarTime = m_frame.Hour[i] + EquationOfTime / 60.0 + (m_longitude - m_localMeridian) / 15.0;  // Apparent Solar Time in hours

      //the following is a more accurate formula for declination as taken from - Duffie and Beckman P. 14
      double SolarDeclination = 0.006918 - 0.399913 * cos(Revolution) + 0.070257 * sin(Revolution) - 0.006758 * cos(2.0 * Revolution)
                                + 0.00907 * sin(2.0 * Revolution) - 0.002679 * cos(3.0 * Revolution)
                                + 0.00148 * sin(3.0 * Revolution);          //solar declination in radians
      double SolarHourAngles = 15 * (ApparentSolarTime - 12) * PI / 180.0;  //solar hour angle in radians
      double SolarAltitudeAngles = asin(cos(m_latitude) * cos(SolarDeclination) * cos(SolarHourAngles)
                                        + sin(m_latitude) * sin(SolarDeclination));  //solar altitude angle in radians

      double SolarAzimuthSin = sin(SolarHourAngles) * cos(SolarDeclination) / cos(SolarAltitudeAngles);  //sin of the solar azimuth
      double SolarAzimuthCos = (cos(SolarHourAngles) * cos(SolarDeclination) * sin(m_latitude) - sin(SolarDeclination) * cos(m_latitude))
                               / cos(SolarAltitudeAngles);            //cosine of solar azimuth
      double SolarAzimuth = atan2(SolarAzimuthSin, SolarAzimuthCos);  //compute solar azimuth in radians

      double GroundReflected = (vecEB[i] * sin(SolarAltitudeAngles) + vecED[i]) * rhog * (1 - cos(m_surfaceTilt)) / 2;  // ground reflected component

      //LOG(Trace, "surfaceRad " << i << " " << Revolution << " " << EquationOfTime << " " << ApparentSolarTime << " " << SolarDeclination << " " << SolarHourAngles << " " << SolarAltitudeAngles << " " << SolarAzimuthSin << " " << SolarAzimuthCos << " " << SolarAzimuth << " " << GroundReflected);
      std::vector<double>& vecEGI = m_eglobe[i];
      //then compute the hourly radiation on each vertical surface given the solar azimuth for each hour
      //std::stringstream ss;
      for (int s = 0; s < NUM_SURFACES; s++) {
        double SurfaceSolarAzimuth =
          ::fabs(SolarAzimuth - (SurfaceAzimuths[s] * (PI / 180.0)));  //surface - solar azimuth in degrees, >pi/2 means surface is in shade

        double AngleOfIncidence = acos(cos(SolarAltitudeAngles) * cos(SurfaceSolarAzimuth) * sin(m_surfaceTilt)
                                       + sin(SolarAltitudeAngles) * cos(m_surfaceTilt));  //ancle of incidence of sun's rays on surface in rad

        double DirectBeam = vecEB[i] * std::max(cos(AngleOfIncidence), 0.0);  //Beam component of radiation

        double DiffuseRadiation =
          std::max(0.45, 0.55 + 0.437 * cos(AngleOfIncidence) + 0.313 * pow(cos(AngleOfIncidence), 2.0));  //Diffuse component of radiation
        //diffuse component for sigma> pi/2 meaning it is a wall tilted outward, for sigma<= pi/2 meaning wall vertical or tilted inward
        double DiffuseComponent = (m_surfaceTilt > PI / 2) ? vecED[i] * DiffuseRadiation * sin(m_surfaceTilt)
                                                           : vecED[i] * (DiffuseRadiation * sin(m_surfaceTilt) + cos(m_surfaceTilt));

        vecEGI[s] = DirectBeam + DiffuseComponent + GroundReflected;  // add up all the components
        //ss << s << " " << vecEB[i] << " " << SurfaceSolarAzimuth << " " << AngleOfIncidence << " " << DirectBeam << " " << DiffuseRadiation << " " << DiffuseComponent << vecEGI[s] << " ";
      }

      //LOG(Trace, "SurfaceRadDetail " << ss.str());
    }
  }
  //average the data in the bins over the count or days
  void SolarRadiation::calculateMonthAvg(int midx, int cnt) {
    if (midx > -1) {
      //average rate by month
      m_monthlyDryBulbTemp[midx] /= cnt;
      m_monthlyDewPointTemp[midx] /= cnt;
      m_monthlyRelativeHumidity[midx] /= cnt;
      m_monthlyWindspeed[midx] /= cnt;
      m_monthlyGlobalHorizontalRadiation[midx] /= cnt;
      for (int s = 0; s < NUM_SURFACES; s++) {
        m_monthlySolarRadiation[midx][s] /= cnt;
      }
      //hours are averaged over days in the month
      int days = m_frame.monthLength(midx + 1);
      for (int h = 0; h < 24; h++) {
        m_hourlyDryBulbTemp[midx][h] /= days;
        m_hourlyDewPointTemp[midx][h] /= days;
        m_hourlyGlobalHorizontalRadiation[midx][h] /= days;
      }
    }
  }
  //Empty bins to start accumulating data for monthly averages
  void SolarRadiation::clearMonthlyAvg(int midx) {
    for (int h = 0; h < 24; h++) {
      m_hourlyDryBulbTemp[midx][h] = 0;
      m_hourlyDewPointTemp[midx][h] = 0;
      m_hourlyGlobalHorizontalRadiation[midx][h] = 0;
    }
    for (int s = 0; s < NUM_SURFACES; s++) {
      m_monthlySolarRadiation[midx][s] = 0;
    }
    m_monthlyDryBulbTemp[midx] = 0;
    m_monthlyDewPointTemp[midx] = 0;
    m_monthlyRelativeHumidity[midx] = 0;
    m_monthlyWindspeed[midx] = 0;
    m_monthlyGlobalHorizontalRadiation[midx] = 0;
  }

  void SolarRadiation::calculateAverages() {
    int month = 0;
    int midx = -1;
    int cnt = 0;
    // int days = 0;

    const std::vector<std::vector<double>>& data = m_weatherData.data();
    const std::vector<double>& vecDBT = data[DBT];
    const std::vector<double>& vecDPT = data[DPT];
    const std::vector<double>& vecRH = data[RH];

    const std::vector<double>& vecEGH = data[EGH];
    const std::vector<double>& vecWSPD = data[WSPD];

    for (int i = 0; i < TimeFrame::TIMESLICES; i++, cnt++) {
      if (m_frame.Month[i] != month) {
        month = m_frame.Month[i];
        //on month change, average bin out over count
        calculateMonthAvg(midx, cnt);
        midx++;
        //reset accumulators
        clearMonthlyAvg(midx);
        cnt = 0;
      }
      //accumulate data into bins
      // cppcheck-suppress negativeContainerIndex
      m_monthlyDryBulbTemp[midx] += vecDBT[i];
      // cppcheck-suppress negativeContainerIndex
      m_monthlyDewPointTemp[midx] += vecDPT[i];
      // cppcheck-suppress negativeContainerIndex
      m_monthlyRelativeHumidity[midx] += vecRH[i];
      // cppcheck-suppress negativeContainerIndex
      m_monthlyGlobalHorizontalRadiation[midx] += vecEGH[i];
      // cppcheck-suppress negativeContainerIndex
      m_monthlyWindspeed[midx] += vecWSPD[i];

      //std::stringstream ss;
      for (int s = 0; s < NUM_SURFACES; s++) {
        m_monthlySolarRadiation[midx][s] += m_eglobe[i][s];
        //ss << s << " " << m_monthlySolarRadiation[midx][s] << " " << m_eglobe[i][s] << " ";
      }
      //LOG(Trace, "solarRad / eglobe " << ss.str());

      int h = m_frame.Hour[i] - 1;
      // cppcheck-suppress negativeContainerIndex
      m_hourlyDryBulbTemp[midx][h] += vecDBT[i];
      // cppcheck-suppress negativeContainerIndex
      m_hourlyDewPointTemp[midx][h] += vecDPT[i];
      // cppcheck-suppress negativeContainerIndex
      m_hourlyGlobalHorizontalRadiation[midx][h] += vecEGH[i];
    }
    //final month average
    calculateMonthAvg(midx, cnt);
  }

  //Calculate hourly solar radiation for each surface
  //and then calculate the monthly/hourly averages
  void SolarRadiation::Calculate() {
    calculateSurfaceSolarRadiation();
    calculateAverages();
  }

}  // namespace isomodel
}  // namespace openstudio
