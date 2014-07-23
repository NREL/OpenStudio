/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/
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
    SolarRadiation(const TimeFrame &frame, const EpwData &wdata, double tilt=3.141592653589);
    ~SolarRadiation(void);

    static const int NUM_SURFACES = 8;
    static const int MONTHS = 12;
    static const int HOURS = 24;

    void Calculate();

    //outputs
    const std::vector<std::vector<double> > &eglobe() const {return m_eglobe;}//total solar radiation from direct beam, ground reflect and diffuse
    //averages
    const std::vector<double> &monthlyDryBulbTemp() const {return m_monthlyDryBulbTemp;}
    const std::vector<double> &monthlyDewPointTemp() const {return m_monthlyDewPointTemp;}
    const std::vector<double> &monthlyRelativeHumidity() const {return m_monthlyRelativeHumidity;}
    const std::vector<double> &monthlyWindspeed() const {return m_monthlyWindspeed;}
    const std::vector<double> &monthlyGlobalHorizontalRadiation() const {return m_monthlyGlobalHorizontalRadiation;}
    const std::vector<std::vector<double> > &monthlySolarRadiation() const {return m_monthlySolarRadiation;}
    const std::vector<std::vector<double> > &hourlyDryBulbTemp() const {return m_hourlyDryBulbTemp;}
    const std::vector<std::vector<double> > &hourlyDewPointTemp() const {return m_hourlyDewPointTemp;}
    const std::vector<std::vector<double> > &hourlyGlobalHorizontalRadiation() const {return m_hourlyGlobalHorizontalRadiation;}

  protected:
    openstudio::isomodel::TimeFrame m_frame;
    openstudio::isomodel::EpwData m_weatherData;
    void calculateSurfaceSolarRadiation();
    void calculateAverages();
    void calculateMonthAvg(int midx, int cnt);
    void clearMonthlyAvg(int midx);

    //inputs
    double m_surfaceTilt;
    double m_localMeridian;//LSM
    double m_longitude;
    double m_latitude;//latitude in radians

    //outputs
    std::vector<std::vector<double> > m_eglobe;//total solar radiation from direct beam, ground reflect and diffuse

    //averages
    std::vector<double> m_monthlyDryBulbTemp;
    std::vector<double> m_monthlyDewPointTemp;
    std::vector<double> m_monthlyRelativeHumidity;
    std::vector<double> m_monthlyWindspeed;
    std::vector<double> m_monthlyGlobalHorizontalRadiation;
    std::vector<std::vector<double> > m_monthlySolarRadiation;
    std::vector<std::vector<double> > m_hourlyDryBulbTemp;
    std::vector<std::vector<double> > m_hourlyDewPointTemp;
    std::vector<std::vector<double> > m_hourlyGlobalHorizontalRadiation;

  private:
    REGISTER_LOGGER("openstudio.isomodel.SolarRadiation");

};

}
}
#endif // ISOMODEL_SOLARRADIATION_HPP
