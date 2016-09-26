/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef RUNMANAGER_LIB_WEATHERFILEFINDER_HPP
#define RUNMANAGER_LIB_WEATHERFILEFINDER_HPP

#include "../../utilities/core/Path.hpp"
#include "JobParam.hpp"
#include "ToolInfo.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/core/Logger.hpp"

namespace openstudio {
namespace runmanager {

  /// Used to determine the weather file that needs to be used with a given IDF
  class RUNMANAGER_API WeatherFileFinder
  {
    public:
      /// Returns the path to the weather file that should be used for the EnergyPlus simulation
      ///
      /// \param[in] t_params Checked for epwdir param to look for weather file search path
      /// \param[in] t_filelocationname Location name returned from extractDetails
      /// \param[in] t_weatherfilename Weather file name returned from extractDetails
      static openstudio::path find(const JobParams &t_params,
          const boost::optional<std::string> &t_filelocationname,
          const boost::optional<std::string> &t_weatherfilename);

      /// Extracts the details of the IDF needed for simulation and weather file finding
      ///
      /// \param[in] t_idffile IDF to extract details form
      /// \param[out] t_version ToolVersion information extracted from IdfFile
      /// \param[out] t_filelocationname Weather file location name if provided in the IDf
      /// \param[out] t_weatherfilename Weather file name, if provided by the IDF
      static void extractDetails(
          const IdfFile &t_idffile,
          ToolVersion &t_version,
          boost::optional<std::string> &t_filelocationname,
          boost::optional<std::string> &t_weatherfilename);

    private:
          /// Return an ordered set of name parts for a weather name
      static std::set<std::string> getNameComponents(const std::string &name);

      REGISTER_LOGGER("openstudio.runmanager.WeatherFileFinder");

  };


}
}


#endif // RUNMANAGER_LIB_WEATHERFILEFINDER_HPP

