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

#ifndef UTILITIES_BCL_BCLXML_HPP
#define UTILITIES_BCL_BCLXML_HPP

#include "BCLFileReference.hpp"
#include "../core/Optional.hpp"
#include "../core/Path.hpp"
#include "../core/UUID.hpp"
#include "../data/Attribute.hpp"
#include "../UtilitiesAPI.hpp"

#include <vector>

namespace openstudio{

  class BCLComponent;
  class BCLMeasure;

  /** \class BCLXMLType
   *  \brief Enumeration of the BCL XML file types.
   *  \details The Building Component Library (BCL) hosts both components and measures. The 
   *  meta-data for individual instances of these two types of items are transmitted using XML
   *  files that with slightly different structures. Thus, this enum helps distinguish between the 
   *  expected schema. 
   *
   *  See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is: 
   *  \code
  OPENSTUDIO_ENUM( BCLXMLType,
    ((ComponentXML)(ComponentXML)) 
    ((MeasureXML)(MeasureXML)) 
  );
   *  \endcode */
  OPENSTUDIO_ENUM( BCLXMLType,
    ((ComponentXML)(ComponentXML)) 
    ((MeasureXML)(MeasureXML)) 
  );

  /** BCLXML is a class for accessing the common XML structures of BCLComponent and BCLMeasure.
  **/
  class UTILITIES_API BCLXML {

  public:

    /** @name Constructors */
    //@{

    /// Constructor for a new XML
    explicit BCLXML(const BCLXMLType& bclXMLType);

    /// Constructor from file path, will throw exception if file does not exist or is incorrect.
    explicit BCLXML(const openstudio::path& xmlPath);

    /// Try to load a BCLXML from disk.
    static boost::optional<BCLXML> load(const openstudio::path& xmlPath);

    /// Escape a string to write to xml
    static std::string escapeString(const std::string& txt);

    /// Decode a string written in xml
    static std::string decodeString(const std::string& txt);

    //@}
    /** @name Destructor */
    //@{

    /// Virtual destructor
    virtual ~BCLXML();

    //@}
    /** @name Getters */
    //@{

    /// Returns path to XML file.
    openstudio::path path() const;

    /// Returns parent path of XML file.
    openstudio::path directory() const;

    std::string uid() const;

    std::string versionId() const;

    std::string name() const;

    std::string description() const;

    std::string modelerDescription() const;

    std::vector<BCLFileReference> files() const;

    /// Returns references to all files of given type.
    std::vector<BCLFileReference> files(const std::string& filetype) const;

    std::vector<Attribute> attributes() const;

    /// get attribute by name
    boost::optional<Attribute> getAttribute(const std::string& name) const;

    std::vector<std::string> tags() const;

    //@}
    /** @name Setters */
    //@{

    void setName(const std::string& name);

    void setDescription(const std::string& description);

    void setModelerDescription(const std::string& modelerDescription);

    /// adds file to list, file with same full path will be removed
    void addFile(const BCLFileReference& file);

    /// check if has file reference by full path, returns true if file was found
    bool hasFile(const openstudio::path& path);

    /// remove file reference by full path, returns true if file was found and removed
    bool removeFile(const openstudio::path& path);

    /// clear all files
    void clearFiles();

    /// adds attribute to attribute list, existing attribute with same name will be removed
    void addAttribute(const Attribute& attribute);

    /// removes attribute by name, returns true if attribute was found and removed
    bool removeAttribute(const std::string& name);

    /// removes all attributes
    void clearAttributes();

    /// adds a tag, existing tag with same name will be removed
    void addTag(const std::string& tagName);

    /// removes tag by name, returns true if tag was found and removed
    bool removeTag(const std::string& tagName);

    /// removes all tags
    void clearTags();

    //@}
    /** @name Operators */
    //@{

    /// Save the XML back to the original path, always increments version id.
    bool save() const;

    /// Save the XML to a new path, always increments version id.
    bool saveAs(const openstudio::path& xmlPath);

    void changeUID();

    void incrementVersionId();

    //@}

  private:
    // configure logging
    REGISTER_LOGGER("utilities.bcl.BCLXML"); 

    openstudio::path m_path;
    BCLXMLType m_bclXMLType;
    std::string m_name;
    std::string m_uid;
    std::string m_versionId;
    std::string m_description;
    std::string m_modelerDescription; // only for measures
    std::vector<BCLFileReference> m_files; 
    std::vector<std::string> m_filetypes;
    std::vector<Attribute> m_attributes;
    std::vector<std::string> m_tags;
  };

} // openstudio

#endif // UTILITIES_BCL_BCLXML_HPP
