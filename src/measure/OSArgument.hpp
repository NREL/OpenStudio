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

#ifndef MEASURE_OSARGUMENT_HPP
#define MEASURE_OSARGUMENT_HPP

#include "MeasureAPI.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idd/IddEnums.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Logger.hpp"

#include <variant>

namespace openstudio {
namespace measure {

  // clang-format off

  /** \class OSArgumentType
 *  \brief Listing of OSArgument data types. Quantity type is deprecated (will map to Double instead).
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM( OSArgumentType,
  ((Boolean)(Bool)(0))
  ((Double)(Double)(1))
  ((Quantity)(Quantity)(2))
  ((Integer)(Int)(3))
  ((String)(String)(4))
  ((Choice)(Choice)(5))
  ((Path)(Path)(6))
  ((Separator)(Separator)(7))
);
 *  \endcode */
  OPENSTUDIO_ENUM(OSArgumentType,
    ((Boolean)(Bool)(0))
    ((Double)(Double)(1))
    ((Quantity)(Quantity)(2))
    ((Integer)(Int)(3))
    ((String)(String)(4))
    ((Choice)(Choice)(5))
    ((Path)(Path)(6))
    ((Separator)(Separator)(7))
  );

  // ETH@20121211 - Perhaps add MeanStdDev or StatisticalMoments. Could even tack on an
  // UncertaintyDescriptionType string, or move the whole UncertaintyDescription class down
  // into measure.

  /** \class OSDomainType
 *  \brief Methods for restricting the domain of an OSArgument.
 *  \details Numerical OSArguments (Double, Integer) can be given either an Interval
 *  (range) or an Enumeration. The other OSArgument types can only be enumerated.
 *
 *  See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is:
 *  \code
OPENSTUDIO_ENUM( OSDomainType,
  ((Interval))
  ((Enumeration)(Enum))
);
 *  \endcode */
  OPENSTUDIO_ENUM(OSDomainType,
    ((Interval))
    ((Enumeration)(Enum))
  );

  // clang-format on

  /** Preserves old name for OSArgumentType. \deprecated */
  // TODO: JM 2018-11-28: Time to remove no?
  using UserScriptArgumentType = OSArgumentType;

  /** OSArgument is an argument to an OSMeasure. **/
  class MEASURE_API OSArgument
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    // ETH@20121211 - Consider adding a constructor or otherwise tweaking the code to fix
    // the issue of having to explicitly call clone (at least in Ruby) in order to setValue.
    // It would be incorrect to always clone on copy since these objects go back and forth
    // to ProjectDatabase.

    OSArgument();
    OSArgument(const std::string& name, const OSArgumentType& type, bool required, bool modelDependent);

    /** Creates a copy with new UUIDs. */
    OSArgument clone() const;

    //@}

    /** @name Factory methods */
    //@{

    /** Creates an OSArgument for bool values. Defaults domainType() to OSDomainType::Enumeration. */
    static OSArgument makeBoolArgument(const std::string& name, bool required = true, bool modelDependent = false);

    /** Creates an OSArgument for double values. Defaults domaintType() to OSDomainType::Interval. */
    static OSArgument makeDoubleArgument(const std::string& name, bool required = true, bool modelDependent = false);

    /** Creates an OSArgument for int values. Defaults domaintType() to OSDomainType::Interval. */
    static OSArgument makeIntegerArgument(const std::string& name, bool required = true, bool modelDependent = false);

    /** Creates an OSArgument for string values. Defaults domainType() to
   *  OSDomainType::Enumeration. */
    static OSArgument makeStringArgument(const std::string& name, bool required = true, bool modelDependent = false);

    /** Creates an OSArgument for choice values. Defaults domainType() to
   *  OSDomainType::Enumeration. */
    static OSArgument makeChoiceArgument(const std::string& name, const std::vector<std::string>& choices, bool required = true,
                                         bool modelDependent = false);

    /** Creates an OSArgument for choice values. Defaults domainType() to
   *  OSDomainType::Enumeration. */
    static OSArgument makeChoiceArgument(const std::string& name, const std::vector<std::string>& choices,
                                         const std::vector<std::string>& displayNames, bool required = true, bool modelDependent = false);

    /** Creates an OSArgument for path values. Defaults domainType() to OSDomainType::Enumeration. */
    static OSArgument makePathArgument(const std::string& name, bool isRead, const std::string& extension, bool required = true,
                                       bool modelDependent = false);

    /** Creates a separator OSArgument, cannot be used to store a value, cannot be required. */
    static OSArgument makeSeparatorArgument(const std::string& name, bool modelDependent = false);

    //@}

    /** @name Getters */
    //@{

    //openstudio::UUID uuid() const;

    //openstudio::UUID versionUUID() const;

    /** Returns the name of this argument. */
    std::string name() const;

    /** Returns the display name of this argument. */
    std::string displayName() const;

    /** Returns the description of this argument. */
    boost::optional<std::string> description() const;

    /** Returns this argument's type. */
    OSArgumentType type() const;

    /** Returns the units of this argument. */
    boost::optional<std::string> units() const;

    /** Returns true if this argument is required, that is, if the argument must have a value or
   *  default value for the measure to run properly. */
    bool required() const;

    /** Returns true if this argument's properties can changed based on the specific model used in the
   *  measure.  A choice argument which lists objects in the model would be model dependent. */
    bool modelDependent() const;

    /** Returns true if this argument's value has been set. */
    bool hasValue() const;

    /** Returns this argument's value as a bool. Throws if not hasValue() or if type() !=
   *  OSArgumentType::Boolean. */
    bool valueAsBool() const;

    /** Returns this argument's value as a double. Throws if not hasValue() or if type() !=
   *  OSArgumentType::Double. */
    double valueAsDouble() const;

    /** Returns this argument's value as an int. Throws if not hasValue() or if type() !=
   *  OSArgumentType::Integer. */
    int valueAsInteger() const;

    /** Returns this argument's value as a std::string. If type() != OSArgumentType::String, prints
   *  this argument's value to produce the result. Throws if not hasValue(). */
    std::string valueAsString() const;

    /** Returns this argument's value as an openstudio::path. Throws if not hasValue() or if
   *  type() != OSArgumentType::Path. */
    openstudio::path valueAsPath() const;

    /** Returns true if this argument's default value has been set. */
    bool hasDefaultValue() const;

    /** Returns this argument's default value as a bool. Throws if not hasDefaultValue() or if
   *  type() != OSArgumentType::Boolean. */
    bool defaultValueAsBool() const;

    /** Returns this argument's default value as a double. Throws if not hasDefaultValue() or if
   *  type() != OSArgumentType::Double. */
    double defaultValueAsDouble() const;

    /** Returns this argument's default value as an int. Throws if not hasDefaultValue() or if
   *  type() != OSArgumentType::Integer. */
    int defaultValueAsInteger() const;

    /** Returns this argument's default value as a std::string. If type() != OSArgumentType::String,
   *  prints this argument's default value to produce the result. Throws if not hasDefaultValue(). */
    std::string defaultValueAsString() const;

    /** Returns this argument's default value as an openstudio::path. Throws if not
   *  hasDefaultValue() or if type() != OSArgumentType::Path. */
    openstudio::path defaultValueAsPath() const;

    /** Returns true if this argument has a non-null domain. */
    bool hasDomain() const;

    /** Returns the OSDomainType, which is defaulted to a particular value for each type().
   *  If domainType == OSDomainType::Interval then the domain getters will return two
   *  elements. The first will be the minimum value, and the second will be the maximum. If
   *  domainType == OSDomainType::Enumeration, then all of the possible/acceptable values
   *  will be explicitly listed. */
    OSDomainType domainType() const;

    /** Returns the domain as a vector of bools. Will throw if not hasDomain() or type() !=
   *  OSArgumentType::Boolean. */
    std::vector<bool> domainAsBool() const;

    /** Returns the domain as a vector of doubles. Will throw if not hasDomain() or type() !=
   *  OSArgumentType::Double. */
    std::vector<double> domainAsDouble() const;

    /** Returns the domain as a vector of ints. Will throw if not hasDomain() or type() !=
   *  OSArgumentType::Integer. */
    std::vector<int> domainAsInteger() const;

    /** Returns the domain as a vector of strings. Will throw if not hasDomain(). */
    std::vector<std::string> domainAsString() const;

    /** Returns the domain as a vector of paths. Will throw if not hasDomain() or type() !=
   *  OSArgumentType::Path. */
    std::vector<openstudio::path> domainAsPath() const;

    //@}
    /** @name Choice Argument Getters */
    //@{

    /** If this is an argument of type() OSArgument::Choice, returns the allowable values.
   *  Otherwise the returned vector will be .empty(). */
    std::vector<std::string> choiceValues() const;

    /** Returns a vector the same length as choiceValues() that can be used to display
   *  choices to a user. Each element is the explicit choice display name, if that
   *  exists, or the actual choice value, otherwise. */
    std::vector<std::string> choiceValueDisplayNames() const;

    /** Returns the display name or actual value for arguments of type() OSArgument::Choice,
   *  if a value is set. Otherwise throws. */
    std::string valueDisplayName() const;

    /** Returns the display name or actual default value for arguments of type()
   *  OSArgument::Choice, if a default value is set. Otherwise throws. */
    std::string defaultValueDisplayName() const;

    //@}
    /** @name Path Argument Getters */
    //@{

    /** Returns true if type() == OSArgumentType::Path and if the path is to a file that is to be
   *  read. */
    bool isRead() const;

    /** If type() == OSArgumentType::Path, returns the expected file extension of the path.
   *  Otherwise, the return value is .empty(). */
    std::string extension() const;

    //@}
    /** @name Setters */
    //@{

    /** Set the display name of this argument. */
    void setDisplayName(const std::string& displayName);

    /** Set the description of this argument. */
    void setDescription(const std::string& description);

    /** Set the units for this argument. */
    void setUnits(const std::string& units);

    /** Set the value of this argument by passing in data of a particular type. The method will do
   *  nothing and return false if the data is of an incorrect type. These methods do not check
   *  value against the domain (if set), as the domain is just a guideline for users. The string
   *  setter will try to convert the string to the correct type for this argument. Integers can
   *  be used to set the values of arguments of type double. A double  that is really an int (eg: 1.0)
   *  can also be accepted to set integer arguments. */
    bool setValue(bool value);
    /// \overload
    bool setValue(double value);
    /// \overload
    bool setValue(int value);
    /// \overload forwards to std::string& one
    bool setValue(const char* value);
    /// \overload
    bool setValue(const std::string& value);
    /// \overload
    bool setValue(const openstudio::path& value);

    void clearValue();

    /** Set the default value of this argument by passing in data of a particular type. The method
   *  will do nothing and return false if the data is of an incorrect type. These methods do not
   *  check defaultValue against the domain (if set), as the domain is just a guideline for users.
   *  The string setter will try to convert the string to the correct type for this argument.
   *  Integers can be used to set the default values of arguments of type double.
   *  A double  that is really an int (eg: 1.0) can also be accepted to set integer arguments. */
    bool setDefaultValue(bool defaultValue);
    /// \overload
    bool setDefaultValue(double defaultValue);
    /// \overload
    bool setDefaultValue(int defaultValue);
    /// \overload forwards to std::string& one
    bool setDefaultValue(const char* defaultValue);
    /// \overload
    bool setDefaultValue(const std::string& defaultValue);
    /// \overload
    bool setDefaultValue(const openstudio::path& defaultValue);

    /** Sets the domainType and returns true if domainType is consistent with type, otherwise returns
   *  false. If the domainType changes, the domain will be cleared before returning. */
    bool setDomainType(const OSDomainType& domainType);

    /** Sets the domain of this argument by passing in data of a particular type. The method will do
   *  nothing and return false if the data is of an incorrect type or size. A vector of size 2 is
   *  expected for OSDomainType::Interval. */
    bool setDomain(const std::vector<bool>& domain);
    /// \overload
    bool setDomain(const std::vector<double>& domain);
    /// \overload
    bool setDomain(const std::vector<int>& domain);
    /// \overload
    bool setDomain(const std::vector<std::string>& domain);
    /// \overload
    bool setDomain(const std::vector<openstudio::path>& domain);

    void clearDomain();

    /** Sets the domain type to OSDomainType::Interval and sets minimum value.
   *  Preserves existing maximum value or sets it to infinity if it does not exist.
   *  Does not check for compatibility with current value, default value, or that min < max. */
    bool setMinValue(double minValue);
    /// \overload
    bool setMinValue(int minValue);

    /** Sets the domain type to OSDomainType::Interval and sets maximum value.
  *  Preserves existing minimum value or sets it to infinity if it does not exist. */
    bool setMaxValue(double maxValue);
    bool setMaxValue(int maxValue);

    //@}
    /** @name Serialization */
    //@{

    /** Prints argument data for debugging purposes. */
    std::string print() const;

    /** Returns this argument's value printed to string. If printDefault, returns the default value if
   *  in the case of !hasValue() && hasDefaultValue(). If there is no value to print, the returned
   *  string will be .empty(). */
    std::string printValue(bool printDefault = true) const;

    /** Returns this argument's default value printed to string, if possible. If there is no default
   *  value, the string will be .empty(). */
    std::string printDefaultValue() const;

    //@}
   private:
    REGISTER_LOGGER("openstudio.measure.OSArgument");

    // ETH@20121211 - Why are these friend declarations here?!
    // JMT@20130124 - Because all of the constructors are private and the SWIG wrapper for pair needs
    // some constructors

    friend class std::map<std::string, OSArgument>;
    friend struct std::pair<std::string, OSArgument>;

#ifdef _LIBCPP_VERSION
    friend struct std::allocator_traits<std::allocator<std::__tree_node<std::__value_type<std::string, OSArgument>, void*>>>;
#endif

#if _MSC_VER >= 1800
    friend struct std::pair<const std::string, OSArgument>;
#elif _MSC_VER >= 1600
    friend struct std::pair<const std::string, OSArgument>;
    friend struct std::_Pair_base<std::string, OSArgument>;
#endif

    // Note JM 2018-11-28:
    // typedef for the std::variant we will use for value, default value, and domain
    // we add std::monostate to allow the variant to be empty basically
    using OSArgumentVariant = std::variant<std::monostate, bool, double, int, std::string, openstudio::path>;

    bool setStringInternal(OSArgumentVariant& variant, const std::string& value);

    std::string printOSArgumentVariant(const OSArgumentVariant& toPrint) const;

    // This also OS App related
    void onChange();

    openstudio::UUID m_uuid;
    openstudio::UUID m_versionUUID;
    std::string m_name;
    std::string m_displayName;
    boost::optional<std::string> m_description;
    OSArgumentType m_type;
    boost::optional<std::string> m_units;
    bool m_required;
    bool m_modelDependent;
    OSArgumentVariant m_value;
    OSArgumentVariant m_defaultValue;
    OSDomainType m_domainType;
    std::vector<OSArgumentVariant> m_domain;
    std::vector<std::string> m_choices;
    std::vector<std::string> m_choiceDisplayNames;
    bool m_isRead;
    std::string m_extension;
  };

  /** \relates OSArgument */
  using OptionalOSArgument = boost::optional<OSArgument>;

  /** \relates OSArgument */
  using OSArgumentVector = std::vector<OSArgument>;

  /** \relates OSArgument */
  using OSArgumentMap = std::map<std::string, OSArgument>;

  /** \relates OSArgument */
  using UserScriptInfo = std::pair<openstudio::path, std::vector<OSArgument>>;

  /** Prints argument data to ostream for debugging purposes. \relates OSArgument */
  MEASURE_API std::ostream& operator<<(std::ostream& os, const OSArgument& arg);

  /** Creates a choice argument with object handles as its values and object names as
 *  its display names by querying workspace for all of its objects of type iddObjectType.
 *  The resulting argument does not attempt to preserve workspace, so the handles and
 *  names will only be valid if the workspace to which the argument is applied is the same
 *  or closely related to workspace. \relates OSArgument */
  MEASURE_API OSArgument makeChoiceArgumentOfWorkspaceObjects(const std::string& name, const IddObjectType& iddObjectType, const Workspace& workspace,
                                                              bool required = true);

  /** Creates a choice argument with object handles as its values and object names as
 *  its display names by querying workspace for all of its objects with reference
 *  referenceName. The resulting argument does not attempt to preserve workspace, so the
 *  handles and names will only be valid if the workspace to which the argument is applied
 *  is the same or closely related to workspace. \relates OSArgument */
  MEASURE_API OSArgument makeChoiceArgumentOfWorkspaceObjects(const std::string& name, const std::string& referenceName, const Workspace& workspace,
                                                              bool required = true);

  /** Converts a vector of OSArgument to a map of OSArgument using name as the key. \relates OSArgument */
  MEASURE_API std::map<std::string, OSArgument> convertOSArgumentVectorToMap(const std::vector<OSArgument>& arguments);

}  // namespace measure
}  // namespace openstudio

#endif  // MEASURE_OSARGUMENT_HPP
