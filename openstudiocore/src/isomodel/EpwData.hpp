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
#ifndef ISOMODEL_EPW_DATA_HPP
#define ISOMODEL_EPW_DATA_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <isomodel/SolarRadiation.hpp>
#include <isomodel/TimeFrame.hpp>

namespace openstudio {
namespace isomodel {

const int DBT = 0;
const int DPT = 1;
const int RH = 2;
const int EGH = 3;
const int EB = 4;
const int ED = 5;
const int WSPD = 6;

class SolarRadiation;

class EpwData
{
protected:
	void parseHeader(std::string line);
	void parseData(std::string line, int row);
	std::string m_location,m_stationid;
	int m_timezone;
	double m_latitude,m_longitude;
	std::vector< std::vector<double> > m_data;
public:
	EpwData(void);
	~EpwData(void);
  std::string location(){return m_location;}
  std::string stationid(){return m_stationid;}
  int timezone(){return m_timezone;}
  double latitude(){return m_latitude;}
  double longitude(){return m_longitude;}
  std::vector< std::vector<double> > data(){return m_data;}


	void loadData(std::string);
  std::string toISOData();

};

}
}

#endif