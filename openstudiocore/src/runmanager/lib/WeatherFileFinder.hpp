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

