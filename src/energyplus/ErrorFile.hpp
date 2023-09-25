/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

  // clang-format off

  /** \class ErrorLevel
   *  \brief EnergyPlus warning/error levels */
  OPENSTUDIO_ENUM(ErrorLevel,
    ((Warning))
    ((Severe))
    ((Fatal))
  );

  // clang-format on

  class ENERGYPLUS_API ErrorFile
  {
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

}  // namespace energyplus
}  // namespace openstudio

#endif  // ENERGYPLUS_ERRORFILE_HPP
