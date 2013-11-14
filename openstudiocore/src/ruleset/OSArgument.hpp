/**********************************************************************
 *  Copyright (c) 2008-2012, Alliance for Sustainable Energy.
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

#ifndef RULESET_OSARGUMENT_HPP
#define RULESET_OSARGUMENT_HPP

#include <ruleset/RulesetAPI.hpp>

#include <utilities/idf/Workspace.hpp>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/core/Path.hpp>
#include <utilities/core/Enum.hpp>
#include <utilities/core/Logger.hpp>

#include <QVariant>

namespace openstudio {
namespace ruleset {

/** \class OSArgumentType
 *  \brief Listing of OSArgument data types.
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
);
 *  \endcode */
OPENSTUDIO_ENUM( OSArgumentType,
  ((Boolean)(Bool)(0))
  ((Double)(Double)(1))
  ((Quantity)(Quantity)(2))
  ((Integer)(Int)(3))
  ((String)(String)(4))
  ((Choice)(Choice)(5))
  ((Path)(Path)(6))
);

// ETH@20121211 - Perhaps add MeanStdDev or StatisticalMoments. Could even tack on an
// UncertaintyDescriptionType string, or move the whole UncertaintyDescription class down
// into ruleset.

/** \class OSDomainType
 *  \brief Methods for restricting the domain of an OSArgument.
 *  \details Numerical OSArguments (Double, Quantity, Integer) can be given either an Interval
 *  (range) or an Enumeration. The other OSArgument types can only be enumerated.
 *
 *  See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is:
 *  \code
OPENSTUDIO_ENUM( OSDomainType,
  ((Interval))
  ((Enumeration)(Enum))
);
 *  \endcode */
OPENSTUDIO_ENUM( OSDomainType,
  ((Interval))
  ((Enumeration)(Enum))
);

/** Preserves old name for OSArgumentType. \deprecated */
typedef OSArgumentType UserScriptArgumentType;

/** OSArgument is an argument to a UserScript or a Ruleset. **/
class RULESET_API OSArgument {
 public:
  /** @name Construtors and Destructors */
  //@{

  // ETH@20121211 - Consider adding a constructor or otherwise tweaking the code to fix
  // the issue of having to explicitly call clone (at least in Ruby) in order to setValue.
  // It would be incorrect to always clone on copy since these objects go back and forth
  // to ProjectDatabase.

  /** Constructor provided for deserialization; not for general use. */
  OSArgument(const UUID& uuid,
             const UUID& versionUUID,
             const std::string& name,
             const std::string& displayName,
             const OSArgumentType& type,
             bool required,
             const boost::optional<std::string>& value,
             const boost::optional<std::string>& defaultValue,
             const OSDomainType& domainType,
             std::vector<std::string>& domain,
             const std::vector<std::string>& choices,
             const std::vector<std::string>& choiceDisplayNames,
             bool isRead,
             const std::string& extension);

  /** Constructor provided for deserialization; not for general use. */
  OSArgument(const UUID& uuid,
             const UUID& versionUUID,
             const std::string& name,
             const std::string& displayName,
             const OSArgumentType& type,
             bool required,
             const QVariant& value,
             const QVariant& defaultValue,
             const OSDomainType& domainType,
             std::vector<QVariant>& domain,
             const std::vector<std::string>& choices,
             const std::vector<std::string>& choiceDisplayNames,
             bool isRead,
             const std::string& extension);

  /** Creates a copy with new UUIDs. */
  OSArgument clone() const;

  //@}

  /** Creates an OSArgument for bool values. Defaults domainType() to OSDomainType::Enumeration. */
  static OSArgument makeBoolArgument(const std::string& name, bool required = true);

  /** Creates an OSArgument for double values. Defaults domaintType() to OSDomainType::Interval. */
  static OSArgument makeDoubleArgument(const std::string& name, bool required = true);

  /** Creates an OSArgument for Quantity values. Defaults domaintType() to
   *  OSDomainType::Interval. */
  static OSArgument makeQuantityArgument(const std::string& name, bool required = true);

  /** Creates an OSArgument for int values. Defaults domaintType() to OSDomainType::Interval. */
  static OSArgument makeIntegerArgument(const std::string& name, bool required = true);

  /** Creates an OSArgument for string values. Defaults domainType() to
   *  OSDomainType::Enumeration. */
  static OSArgument makeStringArgument(const std::string& name, bool required = true);

  /** Creates an OSArgument for choice values. Defaults domainType() to
   *  OSDomainType::Enumeration. */
  static OSArgument makeChoiceArgument(const std::string& name,
                                       const std::vector<std::string>& choices,
                                       bool required = true);

  /** Creates an OSArgument for choice values. Defaults domainType() to
   *  OSDomainType::Enumeration. */
  static OSArgument makeChoiceArgument(const std::string& name,
                                       const std::vector<std::string>& choices,
                                       const std::vector<std::string>& displayNames,
                                       bool required = true);

  /** Creates an OSArgument for path values. Defaults domainType() to OSDomainType::Enumeration. */
  static OSArgument makePathArgument(const std::string& name,
                                     bool isRead,
                                     const std::string& extension,
                                     bool required = true);

  /** @name Getters */
  //@{

  openstudio::UUID uuid() const;

  openstudio::UUID versionUUID() const;

  /** Returns the name of this argument. */
  std::string name() const;

  /** Returns the display name of this argument. */
  std::string displayName() const;

  /** Returns this argument's type. */
  OSArgumentType type() const;

  /** Returns true if this argument is required, that is, if the argument must have a value or
   *  default value for the UserScript or Ruleset to run properly. */
  bool required() const;

  /** Returns true if this argument's value has been set. */
  bool hasValue() const;

  /** Returns this argument's value as a bool. Throws if not hasValue() or if type() != 
   *  OSArgumentType::Boolean. */
  bool valueAsBool() const;

  /** Returns this argument's value as a double. Throws if not hasValue() or if type() != 
   *  OSArgumentType::Double. */
  double valueAsDouble() const;

  /** Returns this argument's value as a Quantity. Throws if not hasValue() or if type() != 
   *  OSArgumentType::Quantity. */
  Quantity valueAsQuantity() const;

  /** Returns this argument's value as an int. Throws if not hasValue() or if type() != 
   *  OSArgumentType::Integer. */
  int valueAsInteger() const;

  /** Returns this argument's value as a std::string. If type() != OSArgumentType::String, prints
   *  this argument's value to produce the result. Throws if not hasValue(). */
  std::string valueAsString() const;

  /** Returns this argument's value as an openstudio::path. Throws if not hasValue() or if 
   *  type() != OSArgumentType::Path. */
  openstudio::path valueAsPath() const;

  /** For serialization, not for general use. */
  QVariant valueAsQVariant() const;

  /** Returns true if this argument's default value has been set. */
  bool hasDefaultValue() const;

  /** Returns this argument's default value as a bool. Throws if not hasDefaultValue() or if 
   *  type() != OSArgumentType::Boolean. */
  bool defaultValueAsBool() const;

  /** Returns this argument's default value as a double. Throws if not hasDefaultValue() or if 
   *  type() != OSArgumentType::Double. */
  double defaultValueAsDouble() const;

  /** Returns this argument's default value as a Quantity. Throws if not hasDefaultValue() or if 
   *  type() != OSArgumentType::Quantity. */
  Quantity defaultValueAsQuantity() const;

  /** Returns this argument's default value as an int. Throws if not hasDefaultValue() or if 
   *  type() != OSArgumentType::Integer. */
  int defaultValueAsInteger() const;

  /** Returns this argument's default value as a std::string. If type() != OSArgumentType::String,
   *  prints this argument's default value to produce the result. Throws if not hasDefaultValue(). */
  std::string defaultValueAsString() const;

  /** Returns this argument's default value as an openstudio::path. Throws if not 
   *  hasDefaultValue() or if type() != OSArgumentType::Path. */
  openstudio::path defaultValueAsPath() const;

  /** For serialization, not for general use. */
  QVariant defaultValueAsQVariant() const;

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

  /** Returns the domain as a vector of Quantities. Will throw if not hasDomain() or type() !=
   *  OSArgumentType::Quantity. */
  std::vector<Quantity> domainAsQuantity() const;

  /** Returns the domain as a vector of ints. Will throw if not hasDomain() or type() !=
   *  OSArgumentType::Integer. */
  std::vector<int> domainAsInteger() const;

  /** Returns the domain as a vector of strings. Will throw if not hasDomain(). */
  std::vector<std::string> domainAsString() const;

  /** Returns the domain as a vector of paths. Will throw if not hasDomain() or type() !=
   *  OSArgumentType::Path. */
  std::vector<openstudio::path> domainAsPath() const;

  /** For serialization, not for general use. */
  std::vector<QVariant> domainAsQVariant() const;

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

  /** Set the value of this argument by passing in data of a particular type. The method will do
   *  nothing and return false if the data is of an incorrect type. These methods do not check
   *  value against the domain (if set), as the domain is just a guideline for users. The string
   *  setter will try to convert the string to the correct type for this argument. Integers can
   *  be used to set the values of arguments of type double. */
  bool setValue(bool value);
  /// \overload
  bool setValue(double value);
  /// \overload
  bool setValue(const Quantity& value);
  /// \overload
  bool setValue(int value);
  /// \overload
  bool setValue(const std::string& value);
  /// \overload
  bool setValue(const char* value);
  /// \overload
  bool setValue(const openstudio::path& value);

  void clearValue();

  /** Set the default value of this argument by passing in data of a particular type. The method
   *  will do nothing and return false if the data is of an incorrect type. These methods do not
   *  check defaultValue against the domain (if set), as the domain is just a guideline for users.
   *  The string setter will try to convert the string to the correct type for this argument. 
   *  Integers can be used to set the default values of arguments of type double. */
  bool setDefaultValue(bool defaultValue);
  /// \overload
  bool setDefaultValue(double defaultValue);
  /// \overload
  bool setDefaultValue(const Quantity& value);
  /// \overload
  bool setDefaultValue(int defaultValue);
  /// \overload
  bool setDefaultValue(const std::string& defaultValue);
  /// \overload
  bool setDefaultValue(const char* defaultValue);
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
  bool setDomain(const std::vector<Quantity>& domain);
  /// \overload
  bool setDomain(const std::vector<int>& domain);
  /// \overload
  bool setDomain(const std::vector<std::string>& domain);
  /// \overload
  bool setDomain(const std::vector<openstudio::path>& domain);

  void clearDomain();

  //@}
  /** @name Serialization */
  //@{

  /** Prints argument data for debugging purposes. */
  std::string print() const;

  /** Returns this argument's value printed to string. If printDefault, returns the default value if 
   *  in the case of !hasValue() && hasDefaultValue(). If there is no value to print, the returned 
   *  string will be .empty(). */
  std::string printValue(bool printDefault=true) const;

  /** Returns this argument's default value printed to string, if possible. If there is no default 
   *  value, the string will be .empty(). */
  std::string printDefaultValue() const;

  //@}
 private:
  REGISTER_LOGGER("openstudio.ruleset.OSArgument");

  // ETH@20121211 - Why are these friend declarations here?!
  // JMT@20130124 - Because all of the constructors are private and the SWIG wrapper for pair needs 
  // some constructors

  friend class std::map<std::string, OSArgument>;
  friend struct std::pair<std::string, OSArgument>;

#if _MSC_VER >= 1600
  friend class std::pair<const std::string, OSArgument>;
  friend class std::_Pair_base<std::string, OSArgument>;
#endif


  OSArgument();
  OSArgument(const std::string& name, const OSArgumentType& type, bool required);

  bool setStringInternal(QVariant& variant, const std::string& value);

  std::string printQVariant(const QVariant& toPrint) const;

  void onChange();

  openstudio::UUID m_uuid;
  openstudio::UUID m_versionUUID;
  std::string m_name;
  std::string m_displayName;
  OSArgumentType m_type;
  bool m_required;
  QVariant m_value;
  QVariant m_defaultValue;
  OSDomainType m_domainType;
  std::vector<QVariant> m_domain;
  std::vector<std::string> m_choices;
  std::vector<std::string> m_choiceDisplayNames;
  bool m_isRead;
  std::string m_extension;
};

/** \relates OSArgument */
typedef boost::optional<OSArgument> OptionalOSArgument;

/** \relates OSArgument */
typedef std::vector<OSArgument> OSArgumentVector;

/** \relates OSArgument */
typedef std::map<std::string,OSArgument> OSArgumentMap;

/** \relates OSArgument */
typedef std::pair< openstudio::path,std::vector<OSArgument> > UserScriptInfo;

/** Prints argument data to ostream for debugging purposes. \relates OSArgument */
RULESET_API std::ostream& operator<<(std::ostream& os, const OSArgument& arg);

// ETH@20121211 - These typedefs are just to preserve the old names for the near to
// medium-term future (to avoid breaking existing scripts).

/** \relates OSArgument \deprecated */
typedef OSArgument UserScriptArgument;

/** \relates OSArgument \deprecated */
typedef boost::optional<OSArgument> OptionalUserScriptArgument;

/** \relates OSArgument \deprecated */
typedef std::vector<OSArgument> UserScriptArgumentVector;

/** \relates OSArgument \deprecated */
typedef std::map<std::string,OSArgument> UserScriptArgumentMap;

/** Creates a choice argument with object handles as its values and object names as
 *  its display names by querying workspace for all of its objects of type iddObjectType.
 *  The resulting argument does not attempt to preserve workspace, so the handles and
 *  names will only be valid if the workspace to which the argument is applied is the same
 *  or closely related to workspace. \relates OSArgument */
RULESET_API OSArgument makeChoiceArgumentOfWorkspaceObjects(
    const std::string& name,
    const IddObjectType& iddObjectType,
    const Workspace& workspace,
    bool required=true);

/** Creates a choice argument with object handles as its values and object names as
 *  its display names by querying workspace for all of its objects with reference
 *  referenceName. The resulting argument does not attempt to preserve workspace, so the
 *  handles and names will only be valid if the workspace to which the argument is applied
 *  is the same or closely related to workspace. \relates OSArgument */
RULESET_API OSArgument makeChoiceArgumentOfWorkspaceObjects(
    const std::string& name,
    const std::string& referenceName,
    const Workspace& workspace,
    bool required=true);

/** Converts a vector of OSArgument to a map of OSArgument using name as the key. \relates OSArgument */
RULESET_API std::map<std::string,OSArgument> convertOSArgumentVectorToMap(const std::vector<OSArgument>& arguments);

namespace detail {

  /** Non-member function to convert argument into a QJSON-ready QVariant. */
  RULESET_API QVariant toVariant(const OSArgument& argument);

  RULESET_API OSArgument toOSArgument(const QVariant& variant, const VersionString& version);

  QVariant toQuantityQVariant(const QVariantMap& map,
                              const std::string& valueKey,
                              const std::string& unitsKey);

}

} // ruleset
} // openstudio

#endif // RULESET_OSARGUMENT_HPP
