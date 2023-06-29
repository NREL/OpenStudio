/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_SQL_SQLFILEENUMS_HPP
#define UTILITIES_SQL_SQLFILEENUMS_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Enum.hpp"

#include <boost/optional.hpp>

namespace openstudio {

// clang-format off

/** \class ReportingFrequency
 *  \brief Frequencies at which report variables may be specified.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(ReportingFrequency,
  ((Detailed)(HVAC System Timestep)(1))
  ((Timestep)(Zone Timestep))
  ((Hourly))
  ((Daily))
  ((Monthly))
  ((RunPeriod))
  ((Annual))
);
 *  \endcode */
OPENSTUDIO_ENUM(ReportingFrequency,
  ((Detailed)(HVAC System Timestep)(1))
  ((Timestep)(Zone Timestep))
  ((Hourly))
  ((Daily))
  ((Monthly))
  ((RunPeriod)(Run Period))
  ((Annual))
);

// clang-format on

using OptionalReportingFrequency = boost::optional<ReportingFrequency>;
using ReportingFrequencyVector = std::vector<ReportingFrequency>;
using ReportingFrequencySet = std::set<ReportingFrequency>;

/** \class EnvironmentType
 *  \brief Frequencies at which report variables may be specified.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(EnvironmentType,
          ((DesignDay)(DesignDay)(1))
          ((DesignRunPeriod))
          ((WeatherRunPeriod)) );
 *  \endcode */
OPENSTUDIO_ENUM(EnvironmentType, ((DesignDay)(DesignDay)(1))((DesignRunPeriod))((WeatherRunPeriod)));

using OptionalEnvironmentType = boost::optional<EnvironmentType>;

}  // namespace openstudio

#endif  // UTILITIES_SQL_SQLFILEENUMS_HPP
