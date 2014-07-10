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

#ifndef ENERGYPLUS_ERRORFILE_HPP
#define ENERGYPLUS_ERRORFILE_HPP

#include "EnergyPlusAPI.hpp"

#include "../utilities/core/Path.hpp"
#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Logger.hpp"

#include <boost/filesystem/fstream.hpp>
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

    void parse(boost::filesystem::ifstream& is);

    std::vector<std::string> m_warnings;
    std::vector<std::string> m_severeErrors;
    std::vector<std::string> m_fatalErrors;
    bool m_completed;
    bool m_completedSuccessfully;

  };

} // energyplus
} // openstudio

#endif // ENERGYPLUS_ERRORFILE_HPP
