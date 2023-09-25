/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
