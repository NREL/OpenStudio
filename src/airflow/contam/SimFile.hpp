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

#ifndef AIRFLOW_CONTAM_SIMFILE_HPP
#define AIRFLOW_CONTAM_SIMFILE_HPP

#include "PrjDefines.hpp"

#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/core/Path.hpp"

#include "../AirflowAPI.hpp"

namespace openstudio {
namespace contam {

  class AIRFLOW_API SimFile
  {
   public:
    explicit SimFile(openstudio::path path);

    // These are provided for advanced use
    std::vector<std::vector<double>> dP() const {
      return m_dP;
    }
    std::vector<std::vector<double>> F0() const {
      return m_F0;
    }
    std::vector<std::vector<double>> F1() const {
      return m_F1;
    }
    std::vector<std::vector<double>> T() const {
      return m_T;
    }
    std::vector<std::vector<double>> P() const {
      return m_P;
    }
    std::vector<std::vector<double>> D() const {
      return m_D;
    }

    // Most use should be confined to these
    boost::optional<openstudio::TimeSeries> pathDeltaP(int nr) const;
    boost::optional<openstudio::TimeSeries> pathFlow0(int nr) const;
    boost::optional<openstudio::TimeSeries> pathFlow1(int nr) const;
    boost::optional<openstudio::TimeSeries> pathFlow(int nr) const;
    boost::optional<openstudio::TimeSeries> nodeTemperature(int nr) const;
    boost::optional<openstudio::TimeSeries> nodePressure(int nr) const;
    boost::optional<openstudio::TimeSeries> nodeDensity(int nr) const;
    /** Returns a vector of DateTime objects that give the EnergyPlus-style
   *  end of interval times. These are not the actual times in the SIM file */
    std::vector<openstudio::DateTime> dateTimes() const;

    /** Returns a vector of DateTime objects that the SIM file contains data for.
   *  CONTAM always includes a start time result, so a yearly simulation will
   *  result in 8761 times. */
    std::vector<openstudio::DateTime> fileDateTimes() const {
      return m_dateTimes;
    }

   private:
    void clearLfr();
    bool readLfr(const std::string& fileName);
    void clearNfr();
    bool readNfr(const std::string& fileName);
    bool computeDateTimes(const std::vector<std::string>& day, const std::vector<std::string>& time);

    std::vector<int> m_pathNr;  // the CONTAM path index
    std::vector<std::vector<double>> m_dP;
    std::vector<std::vector<double>> m_F0;
    std::vector<std::vector<double>> m_F1;
    std::vector<int> m_nodeNr;  // the CONTAM node index
    std::vector<std::vector<double>> m_T;
    std::vector<std::vector<double>> m_P;
    std::vector<std::vector<double>> m_D;
    std::vector<openstudio::DateTime> m_dateTimes;

    bool m_hasLfr;
    bool m_hasNfr;
    bool m_hasNcr;

    REGISTER_LOGGER("openstudio.contam.SimFile");
  };

}  // namespace contam
}  // namespace openstudio

#endif  // AIRFLOW_CONTAM_SIMFILE_HPP
