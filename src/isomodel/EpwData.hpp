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
