/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef ENERGYPLUS_ERRORFILE_HPP
#define ENERGYPLUS_ERRORFILE_HPP

#include "EnergyPlusAPI.hpp"

#include "../utilities/core/Path.hpp"
#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Logger.hpp"


#include <string>
#include <vector>

namespace openstudio {
namespace energyplus {

  /** \class ErrorLevel
   *  \brief EnergyPlus warning/error levels */
  OPENSTUDIO_ENUM(ErrorLevel,
      ((Warning))
      ((Severe))
      ((Fatal)) );

  class ENERGYPLUS_API ErrorFile {
   public:

    /// constructor
    ErrorFile(const openstudio::path& errPath);

    /// get warnings
    std::vector<std::string> warnings() const;

    /// get severe errors
    std::vector<std::string> severeErrors() const;

    /// get fatal errors
    std::vector<std::string> fatalErrors() const;

    /// did EnergyPlus complete or crash
    bool completed() const;

    /// completed successfully
    bool completedSuccessfully() const;

   private:

    REGISTER_LOGGER("energyplus.ErrorFile");

    void parse(openstudio::filesystem::ifstream& is);

    std::vector<std::string> m_warnings;
    std::vector<std::string> m_severeErrors;
    std::vector<std::string> m_fatalErrors;
    bool m_completed;
    bool m_completedSuccessfully;

  };

} // energyplus
} // openstudio

#endif // ENERGYPLUS_ERRORFILE_HPP
