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

#ifndef UTILITIES_BCL_ONDEMANDGENERATOR_HPP
#define UTILITIES_BCL_ONDEMANDGENERATOR_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Enum.hpp"
#include "../core/Path.hpp"

#include <QVariant>

class QDomElement;

namespace openstudio{

  class OnDemandGeneratorArgument;
  class OnDemandGenerator;
  
  /// This class represents any restrictions on argument to an on demand generator script.
  /// Currently, the only value restriction allowed is that of a string enumeration.  
  /// In this case each OnDemandGeneratorValueRestriction represents an allowable value for the enumeration.
  /// Nested arguments represent additional arguments which are present only if this value 
  class UTILITIES_API OnDemandGeneratorValueRestriction  {
  public:

    /** @name Constructor */
    //@{

    /// Constructs an OnDemandGeneratorValueRestriction from its xml description
    OnDemandGeneratorValueRestriction(const QDomElement& valueElement);

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~OnDemandGeneratorValueRestriction();

    //@}
    /** @name Class members */
    //@{

    /// Returns the name of this restriction, if this restriction represents an allowable value
    /// for a string enumeration then name returns the allowable value
    std::string name() const;

    /// Returns any additional arguments which are accepted if a value covered by this restriction is selected.
    std::vector<OnDemandGeneratorArgument> nestedArguments() const;
  
    //@}
  private:

    friend class OnDemandGeneratorArgument;

    /// Sets the argument value, this will search through the active argument tree
    bool setArgumentValue(const std::string& name, const std::string& value);
    bool setArgumentValue(const std::string& name, double value);
    bool setArgumentValue(const std::string& name, int value);

    std::string m_name; // if inputType is select this is one of the options
    std::vector<OnDemandGeneratorArgument> m_nestedArguments;
  };

  /** \class OnDemandGeneratorArgumentType
   *  \brief Enumeration of the types of OnDemandGeneratorArgumentType
   *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
   *  call is:
   *
   *  \code
  OPENSTUDIO_ENUM( OnDemandGeneratorArgumentType,
    ((String)(String)(0))
    ((Float)(Float)(1))
    ((Integer)(Integer)(2))
  );
   *  \endcode
   */
  OPENSTUDIO_ENUM( OnDemandGeneratorArgumentType,
    ((String)(String)(0))
    ((Float)(Float)(1))
    ((Integer)(Integer)(2))
  );

  /** \class OnDemandGeneratorInputType
   *  \brief Enumeration of the types of OnDemandGeneratorInputType
   *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
   *  call is:
   *
   *  \code
  OPENSTUDIO_ENUM( OnDemandGeneratorInputType,
    ((Select)(Select)(0))
    ((Text)(Text)(1))
    ((Component)(Component)(2))
  );
   *  \endcode
   */
  OPENSTUDIO_ENUM( OnDemandGeneratorInputType,
    ((Select)(Select)(0))
    ((Text)(Text)(1))
    ((Component)(Component)(2))
  );
  
  /// This class represents an argument to an on demand generator script.
  class UTILITIES_API OnDemandGeneratorArgument {
  public:

    /** @name Constructor */
    //@{

    /// Constructs an OnDemandGenerator from its xml description
    OnDemandGeneratorArgument(const QDomElement& argumentElement);

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~OnDemandGeneratorArgument();

    //@}
    /** @name Class members */
    //@{

    /// Get the name of this argument
    std::string name() const;

    /// Get the display name of this argument
    std::string displayName() const;

    /// Get the data type of this argument
    OnDemandGeneratorArgumentType dataType() const;

    /// Returns true if this is a required argument
    bool required() const;

    /// Get the input type of this argument
    OnDemandGeneratorInputType inputType() const;

    /// Get the units of this argument if any
    std::string units() const;

    /// Get the component type (e.g. taxonomy term such as 'Lighting.Indoor Lighting.Lamp') of this argument if any
    std::string componentType() const;

    /// Get any restrictions on the value of this argument
    std::vector<OnDemandGeneratorValueRestriction> valueRestrictions() const;

    /// Returns this argument and all nested arguments which are currently active in depth first search order
    /// (e.g. top level arguments + nested arguments directly underneath arguments with value set)
    std::vector<OnDemandGeneratorArgument> activeArguments() const;

    /// Returns this name, plus name of all arguments which are currently active in depth first search order
    /// (e.g. top level arguments + nested arguments directly underneath arguments with value set)
    std::vector<std::string> activeArgumentNames() const;

    /// Returns argument by name, searches through the active argument tree starting with this
    boost::optional<OnDemandGeneratorArgument> getActiveArgument(const std::string& name) const;

    /// Returns true if a value has been set for this argument
    bool hasValue() const;

    /// Returns the value as a string if possible
    boost::optional<std::string> valueAsString() const;

    /// Returns the value as a double if possible
    boost::optional<double> valueAsDouble() const;

    /// Returns the value as an integer if possible
    boost::optional<int> valueAsInteger() const;

    //@}
  private:

    friend class OnDemandGenerator;
    friend class OnDemandGeneratorValueRestriction;

    void clearValue();
    bool setValue(const std::string& value);
    bool setValue(double value);
    bool setValue(int value);

    /// Sets the argument value, this will search through the active argument tree
    bool setArgumentValue(const std::string& name, const std::string& value);
    bool setArgumentValue(const std::string& name, double value);
    bool setArgumentValue(const std::string& name, int value);

    std::string m_name;
    std::string m_displayName;
    OnDemandGeneratorArgumentType m_dataType; // {string, float, integer}
    bool m_required;
    OnDemandGeneratorInputType m_inputType; // {select, text, component}
    std::string m_units;
    std::string m_componentType;
    std::vector<OnDemandGeneratorValueRestriction> m_valueRestrictions;

    QVariant m_value;
  };

  /// Class for accessing on demand generators in the online BCL.  An on demand generator is a 
  /// script which accepts user arguments, is called through the online BCL API, and returns a BCLComponent.
  class UTILITIES_API OnDemandGenerator {
  public:

    /** @name Constructor */
    //@{

    /// Load an OnDemandGenerator from its xml description
    static boost::optional<OnDemandGenerator> load(const openstudio::path& path);

    /// Constructs an OnDemandGenerator from its xml description
    OnDemandGenerator(const QDomElement& generatorElement);

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~OnDemandGenerator();

    //@}
    /** @name Class members */
    //@{

    /// Get the name of the generator
    std::string name() const;

    /// Get the uid of the generator
    std::string uid() const;

    /// Get the version id of the generator
    std::string versionId() const;

    /// Get the description of the generator
    std::string description() const;

    /// Returns the top level arguments, user will have to navigate any nested arguments 
    /// on their own for 
    std::vector<OnDemandGeneratorArgument> arguments() const;

    /// Returns all arguments which are currently active in depth first search order
    /// (e.g. top level arguments + nested arguments directly underneath arguments with value set)
    std::vector<OnDemandGeneratorArgument> activeArguments() const;

    /// Returns the name of all arguments which are currently active in depth first search order
    /// (e.g. top level arguments + nested arguments directly underneath arguments with value set)
    std::vector<std::string> activeArgumentNames() const;

    /// Returns argument by name, searches through the active argument tree
    boost::optional<OnDemandGeneratorArgument> getActiveArgument(const std::string& name) const;

    /// Sets the argument value, this will search through the active argument tree
    /// Setting an argument to a value may change the active argument tree
    bool setArgumentValue(const std::string& name, const std::string& value);
    bool setArgumentValue(const std::string& name, double value);
    bool setArgumentValue(const std::string& name, int value);

    /// Returns false until valid values have been set for all required arguments
    bool checkArgumentValues() const;
    
    //@}

  private:
    std::string m_name;
    std::string m_uid;
    std::string m_versionId;
    std::string m_description;
    std::vector<OnDemandGeneratorArgument> m_arguments;
  };

} // openstudio

#endif // UTILITIES_BCL_ONDEMANDGENERATOR_HPP
