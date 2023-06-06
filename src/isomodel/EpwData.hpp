/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

  constexpr int DBT = 0;
  constexpr int DPT = 1;
  constexpr int RH = 2;
  constexpr int EGH = 3;
  constexpr int EB = 4;
  constexpr int ED = 5;
  constexpr int WSPD = 6;

  class EpwData
  {
   public:
    EpwData(const openstudio::path& t_path);

    std::string location() const {
      return m_location;
    }
    std::string stationid() const {
      return m_stationid;
    }
    int timezone() const {
      return m_timezone;
    }
    double latitude() const {
      return m_latitude;
    }
    double longitude() const {
      return m_longitude;
    }
    const std::vector<std::vector<double>>& data() const {
      return m_data;
    }

    std::string toISOData() const;
    void toISOData(Matrix& _msolar, Matrix& _mhdbt, Matrix& _mhEgh, Vector& _mEgh, Vector& _mdbt, Vector& _mwind) const;

   protected:
    void loadData(const openstudio::path& t_path);
    void parseHeader(const std::string& line);
    void parseData(const std::string& line, size_t row);
    std::string m_location, m_stationid;
    int m_timezone;
    double m_latitude, m_longitude;
    std::vector<std::vector<double>> m_data;
  };

}  // namespace isomodel
}  // namespace openstudio

#endif  // ISOMODEL_EPWDATA_HPP
