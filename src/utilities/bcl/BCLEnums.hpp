/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
