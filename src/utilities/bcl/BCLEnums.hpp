/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_BCL_BCLENUMS_HPP
#define UTILITIES_BCL_BCLENUMS_HPP

#include "../core/Enum.hpp"

namespace openstudio {

// clang-format off

/** \class MeasureType
 *  \brief Enumeration of the types of BCLMeasure, by input file type.
 *  \details ModelMeasures accept OpenStudio Models as input; EnergyPlusMeasures accept
 *  EnergyPlus IDF files as input; and UtilityMeasures do not operate on any sort of energy
 *  model. See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
 *  call is:
 *  \code
OPENSTUDIO_ENUM( MeasureType,
  ((ModelMeasure)(Model Measure))
  ((EnergyPlusMeasure)(EnergyPlus Measure))
  ((UtilityMeasure)(Utility Measure))
  ((ReportingMeasure)(Reporting Measure))
);
 *  \endcode */
OPENSTUDIO_ENUM( MeasureType,
  ((ModelMeasure)(Model Measure))
  ((EnergyPlusMeasure)(EnergyPlus Measure))
  ((UtilityMeasure)(Utility Measure))
  ((ReportingMeasure)(Reporting Measure))
);

OPENSTUDIO_ENUM( MeasureBadgeType,
  ((BCLMeasure))
  ((MyMeasure))
  ((OSMeasure))
);

OPENSTUDIO_ENUM( MeasureLanguage,
  ((Ruby))
  ((Python))
);

/** \class BCLXMLType
 *  \brief Enumeration of the BCL XML file types.
 *  \details The Building Component Library (BCL) hosts both components and measures. The
 *  meta-data for individual instances of these two types of items are transmitted using XML
 *  files that with slightly different structures. Thus, this enum helps distinguish between the
 *  expected schema.
 *
 *  See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is:
 *  \code
OPENSTUDIO_ENUM(BCLXMLType,
  ((ComponentXML)(ComponentXML))
  ((MeasureXML)(MeasureXML))
);
 *  \endcode */
OPENSTUDIO_ENUM(BCLXMLType,
  ((ComponentXML)(ComponentXML))
  ((MeasureXML)(MeasureXML))
);

// clang-format on

}  // namespace openstudio

#endif  // UTILITIES_BCL_BCLENUMS_HPP
