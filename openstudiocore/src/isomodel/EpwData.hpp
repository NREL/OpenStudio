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
#ifndef ISOMODEL_EPWDATA_HPP
#define ISOMODEL_EPWDATA_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "TimeFrame.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/data/Matrix.hpp"
#include "../utilities/data/Vector.hpp"

namespace openstudio {
namespace isomodel {

const int DBT = 0;
const int DPT = 1;
const int RH = 2;
const int EGH = 3;
const int EB = 4;
const int ED = 5;
const int WSPD = 6;


class EpwData
{
  public:
    EpwData(const openstudio::path &t_path);

    std::string location() const {return m_location;}
    std::string stationid() const {return m_stationid;}
    int timezone() const {return m_timezone;}
    double latitude() const {return m_latitude;}
    double longitude() const {return m_longitude;}
    const std::vector< std::vector<double> > &data()  const {return m_data;}

    std::string toISOData() const;
    void toISOData(Matrix &_msolar, Matrix &_mhdbt, Matrix &_mhEgh, Vector &_mEgh, Vector &_mdbt, Vector &_mwind) const;

  protected:
    void loadData(const openstudio::path &t_path);
    void parseHeader(const std::string &line);
    void parseData(const std::string &line, size_t row);
    std::string m_location,m_stationid;
    int m_timezone;
    double m_latitude,m_longitude;
    std::vector< std::vector<double> > m_data;

};

}
}

#endif // ISOMODEL_EPWDATA_HPP
