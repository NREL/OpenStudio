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
#include <sstream>
#include <isomodel/SolarRadiation.hpp>
#include <isomodel/TimeFrame.hpp>

namespace openstudio {
namespace isomodel {

#define DBT  0
#define DPT  1
#define RH   2
#define EGH  3
#define EB   4
#define ED   5
#define WSPD 6

class SolarRadiation;
using namespace std;
class EpwData
{
protected:
	void parseHeader(string line);
	void parseData(string line, int row);
public:
	string location,stationid;
	int timezone;
	double latitude,longitude;
	double** data;
	EpwData(void);
	~EpwData(void);

	void loadData(const char*);
  std::string toISOData();

};

}
}

#endif