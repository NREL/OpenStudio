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

#ifndef MEASURE_OSOUTPUT_HPP
#define MEASURE_OSOUTPUT_HPP

#include "MeasureAPI.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idd/IddEnums.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Logger.hpp"

namespace openstudio {
namespace measure {

  // clang-format off

  /** \class OSOutputType
   *  \brief Listing of OSOutput data types, values match OSArgumentType.
   *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
   *  macro call is:
   *  \code
OPENSTUDIO_ENUM(OSOutputType,
  ((Boolean)(Bool)(0))
  ((Double)(Double)(1))
  ((Integer)(Int)(3))
  ((String)(String)(4))
);
   *  \endcode */
  OPENSTUDIO_ENUM(OSOutputType,
    ((Boolean)(Bool)(0))
    ((Double)(Double)(1))
    ((Integer)(Int)(3))
    ((String)(String)(4))
  );

  // clang-format on

  /** OSOutput is an output of an OSMeasure. **/
  class MEASURE_API OSOutput
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    OSOutput();
    OSOutput(const std::string& name, const OSOutputType& type, bool modelDependent);

    //@}

    /** Creates an OSOutput for bool values. */
    static OSOutput makeBoolOutput(const std::string& name, bool modelDependent = false);

    /** Creates an OSOutput for double values. */
    static OSOutput makeDoubleOutput(const std::string& name, bool modelDependent = false);

    /** Creates an OSOutput for int values. */
    static OSOutput makeIntegerOutput(const std::string& name, bool modelDependent = false);

    /** Creates an OSOutput for string values. */
    static OSOutput makeStringOutput(const std::string& name, bool modelDependent = false);

    /** @name Getters */
    //@{

    /** Returns the name of this output. */
    std::string name() const;

    /** Returns the display name of this output. */
    std::string displayName() const;

    /** Returns the short name of this output. */
    std::string shortName() const;

    /** Returns the description of this output. */
    boost::optional<std::string> description() const;

    /** Returns this output's type. */
    OSOutputType type() const;

    /** Returns the units of this output. */
    boost::optional<std::string> units() const;

    /** Returns true if this output's properties can changed based on the specific model used in the
   *  measure. */
    bool modelDependent() const;

    //@}
    /** @name Setters */
    //@{

    /** Set the display name of this output. */
    void setDisplayName(const std::string& displayName);

    /** Set the short name of this output. */
    void setShortName(const std::string& shortName);

    /** Set the description of this output. */
    void setDescription(const std::string& description);

    /** Set the units for this output. */
    void setUnits(const std::string& units);

    //@}
    /** @name Serialization */
    //@{

    /** Prints output data for debugging purposes. */
    std::string print() const;

    //@}
   private:
    REGISTER_LOGGER("openstudio.measure.OSOutput");

    std::string m_name;
    std::string m_displayName;
    std::string m_shortName;
    boost::optional<std::string> m_description;
    OSOutputType m_type;
    boost::optional<std::string> m_units;
    bool m_modelDependent;
  };

  /** \relates OSOutput */
  using OptionalOSOutput = boost::optional<OSOutput>;

  /** \relates OSOutput */
  using OSOutputVector = std::vector<OSOutput>;

  /** Prints argument data to ostream for debugging purposes. \relates OSOutput */
  MEASURE_API std::ostream& operator<<(std::ostream& os, const OSOutput& arg);

}  // namespace measure
}  // namespace openstudio

#endif  // MEASURE_OSOUTPUT_HPP
