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
#include <isomodel/TimeFrame.hpp>
#include <isomodel/EpwData.hpp>


namespace openstudio {
namespace isomodel {
#define PI 3.1416//3.1415926535897932384626433832795028841971693993751058209 
#define NUM_SURFACES 8
#define MONTHS 12
#define HOURS 24

using namespace std;
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

public:
	//inputs
	double SurfaceTilt;
	double LocalMeridian;//LSM
	double Longitude;
	double Latitude;//latitude in radians
	
	//outputs
	double Eglobe[TIMESLICES][NUM_SURFACES];//total solar radiation from direct beam, ground reflect and diffuse
	//averages
	double MonthlyDryBulbTemp[MONTHS];
	double MonthlyDewPointTemp[MONTHS];
	double MonthlyRelativeHumidity[MONTHS];
	double MonthlyWindspeed[MONTHS];
	double MonthlyGlobalHorizontalRadiation[MONTHS];
	double MonthlySolarRadiation[MONTHS][NUM_SURFACES];
	double HourlyDryBulbTemp[MONTHS][HOURS];
	double HourlyDewPointTemp[MONTHS][HOURS];
	double HourlyGlobalHorizontalRadiation[MONTHS][HOURS];

	void Calculate();

	SolarRadiation(TimeFrame* frame, EpwData* wdata, double tilt=PI);
	~SolarRadiation(void);
};

}
}
#endif