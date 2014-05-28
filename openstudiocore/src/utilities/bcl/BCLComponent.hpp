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

#ifndef UTILITIES_BCL_BCLCOMPONENT_HPP
#define UTILITIES_BCL_BCLCOMPONENT_HPP

#include "../core/Optional.hpp"
#include "../core/Path.hpp"
#include "../core/UUID.hpp"
#include "../data/Attribute.hpp"
#include "../UtilitiesAPI.hpp"

#include <vector>

// Add more methods for editing components, opening local component constructor

namespace openstudio{

  class UTILITIES_API BCLComponent {

  public:

    /** @name Constructors */
    //@{

    /// Default constructor for a new component
    BCLComponent();

    // DLM: TODO, this should really take an openstudio::path, other parts of this class should be converted to path as well
    /// Constructor for downloaded components, path is to directory containing "component.xml" file
    BCLComponent(const std::string& dir);

    //@}
    /** @name Destructor */
    //@{

    /// Virtual destructor
    virtual ~BCLComponent();

    //@}
    /** @name Getters */
    //@{

    std::string directory() const;

    std::string uid() const;

    std::string versionId() const;

    std::string name() const;

    std::string description() const;

    std::vector<std::string> files() const;

    std::vector<std::string> files(std::string filetype) const;

    std::vector<std::string> filetypes() const;

    std::vector<Attribute> attributes() const;

    //@}
    /** @name Setters */
    //@{

    void setName(const std::string& name);

    //@}
    /** @name Operators */
    //@{

    /// Equality test - Compares the uids and versionIds of two separate components
    bool operator==(const BCLComponent& other) const;

    /// Negation of ==.
    bool operator!=(const BCLComponent& other) const;

//@}
  private:
    std::string m_directory;
    std::string m_name;
    std::string m_uid;
    std::string m_versionId;
    std::string m_description;
    std::vector<std::string> m_files;
    std::vector<std::string> m_filetypes;
    std::vector<Attribute> m_attributes;
  };

  /** \relates BCLComponent */
  typedef boost::optional<BCLComponent> OptionalBCLComponent;

  /** \relates BCLComponent */
  typedef std::vector<BCLComponent> BCLComponentVector;

} // openstudio

#endif // UTILITIES_BCL_BCLCOMPONENT_HPP
