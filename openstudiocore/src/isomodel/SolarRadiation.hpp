/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#ifndef ISOMODEL_SOLAR_RADIATION_HPP
#define ISOMODEL_SOLAR_RADIATION_HPP

#include <math.h>
#include <algorithm>
#include <vector>
#include <isomodel/TimeFrame.hpp>
#include <isomodel/EpwData.hpp>


namespace openstudio {
namespace isomodel {
const double PI = 3.1416;//3.1415926535897932384626433832795028841971693993751058209 
const int NUM_SURFACES = 8;
const int MONTHS = 12;
const int HOURS = 24;

class EpwData;
class SolarRadiation
{
protected:
	openstudio::isomodel::TimeFrame* frame;
  openstudio::isomodel::EpwData* weatherData;
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
public:
	void Calculate();

	SolarRadiation(TimeFrame* frame, EpwData* wdata, double tilt=PI);
	~SolarRadiation(void);
  //outputs
	std::vector<std::vector<double> > eglobe(){return m_eglobe;}//total solar radiation from direct beam, ground reflect and diffuse
	//averages
	std::vector<double> monthlyDryBulbTemp(){return m_monthlyDryBulbTemp;}
	std::vector<double> monthlyDewPointTemp(){return m_monthlyDewPointTemp;}
	std::vector<double> monthlyRelativeHumidity(){return m_monthlyRelativeHumidity;}
	std::vector<double> monthlyWindspeed(){return m_monthlyWindspeed;}
	std::vector<double> monthlyGlobalHorizontalRadiation(){return m_monthlyGlobalHorizontalRadiation;}
	std::vector<std::vector<double> > monthlySolarRadiation(){return m_monthlySolarRadiation;}
	std::vector<std::vector<double> > hourlyDryBulbTemp(){return m_hourlyDryBulbTemp;}
	std::vector<std::vector<double> > hourlyDewPointTemp(){return m_hourlyDewPointTemp;}
	std::vector<std::vector<double> > hourlyGlobalHorizontalRadiation(){return m_hourlyGlobalHorizontalRadiation;}

};

}
}
#endif