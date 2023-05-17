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

#ifndef UTILITIES_BCL_BCLXML_HPP
#define UTILITIES_BCL_BCLXML_HPP

#include "BCLEnums.hpp"
#include "BCLFileReference.hpp"
#include "BCLMeasureArgument.hpp"
#include "BCLMeasureOutput.hpp"
#include "../core/Optional.hpp"
#include "../core/Path.hpp"
#include "../core/UUID.hpp"
#include "../data/Attribute.hpp"
#include "../core/Deprecated.hpp"
#include "../UtilitiesAPI.hpp"

#include <compare>
#include <vector>

namespace pugi {
class xml_document;
}  // namespace pugi

namespace openstudio {

class BCLComponent;
class BCLMeasure;
class DateTime;

/** BCLXML is a class for accessing the common XML structures of BCLComponent and BCLMeasure.
  **/
class UTILITIES_API BCLXML
{

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

  // Returns the component/measure XSD schema version currently in use when writting a new xml.
  static VersionString currentSchemaVersion();

  //@}
  /** @name Destructor */
  //@{

  /// Virtual destructor
  virtual ~BCLXML() = default;

  //@}
  /** @name Getters */
  //@{

  /// Returns path to XML file.
  openstudio::path path() const;

  /// Returns parent path of XML file.
  openstudio::path directory() const;

  boost::optional<std::string> error() const;

  std::string uid() const;

  std::string versionId() const;

  boost::optional<DateTime> versionModified() const;

  std::string xmlChecksum() const;

  std::string name() const;

  std::string displayName() const;

  std::string className() const;

  std::string description() const;

  std::string modelerDescription() const;

  std::vector<BCLMeasureArgument> arguments() const;

  std::vector<BCLMeasureOutput> outputs() const;

  std::vector<BCLFileReference> files() const;

  /// Returns references to all files of given type.
  std::vector<BCLFileReference> files(const std::string& filetype) const;

  std::vector<Attribute> attributes() const;

  /// get attributes by name
  std::vector<Attribute> getAttributes(const std::string& name) const;

  std::vector<std::string> tags() const;

  //@}
  /** @name Setters */
  //@{

  void resetXMLChecksum();

  void setError(const std::string& error);

  void resetError();

  void setName(const std::string& name);

  void setDisplayName(const std::string& displayName);

  void setClassName(const std::string& className);

  void setDescription(const std::string& description);

  void setModelerDescription(const std::string& modelerDescription);

  void setArguments(const std::vector<BCLMeasureArgument>& arguments);

  void setOutputs(const std::vector<BCLMeasureOutput>& outputs);

  /// adds file to list, file with same full path will be removed
  void addFile(const BCLFileReference& file);

  /// check if has file reference by full path, returns true if file was found
  bool hasFile(const openstudio::path& path) const;

  /// remove file reference by full path, returns true if file was found and removed
  bool removeFile(const openstudio::path& path);

  /// clear all files
  void clearFiles();

  /// adds attribute to attribute list
  void addAttribute(const Attribute& attribute);

  /// removes all attributes with name, returns true if attributes were found and removed
  bool removeAttributes(const std::string& name);

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
  /// TODO: does it? it wouldn't be const if it did
  bool save() const;

  /// Save the XML to a new path, always increments version id.
  bool saveAs(const openstudio::path& xmlPath);

  /// Returns a string representation of the XML
  std::string toString() const;

  void changeUID();

  void incrementVersionId();

  /// Check for updates to the xml, will increment versionID and xmlChecksum then return true
  /// if any xml fields (other than uid, version id, or xml checksum) have changed
  /// The xml file must still be saved to disk to preserve the new versionID
  bool checkForUpdatesXML();

  //@}
 protected:
  /** Prints BCLXML to os. \relates BCLXML */
  UTILITIES_API friend std::ostream& operator<<(std::ostream& os, const BCLXML& bclXML);

 private:
  // configure logging
  REGISTER_LOGGER("utilities.bcl.BCLXML");

  /// Compute the current xml checksum
  std::string computeXMLChecksum() const;

  pugi::xml_document toXML() const;

  openstudio::path m_path;
  BCLXMLType m_bclXMLType;
  boost::optional<std::string> m_error;
  std::string m_name;
  std::string m_displayName;
  std::string m_className;  // only for measures
  std::string m_uid;
  std::string m_versionId;
  std::string m_versionModified;
  std::string m_xmlChecksum;
  std::string m_description;
  std::string m_modelerDescription;             // only for measures
  std::vector<BCLMeasureArgument> m_arguments;  // only for measures
  std::vector<BCLMeasureOutput> m_outputs;      // only for measures
  std::vector<BCLFileReference> m_files;
  std::vector<std::string> m_filetypes;
  std::vector<Attribute> m_attributes;
  std::vector<std::string> m_tags;
};

}  // namespace openstudio

#endif  // UTILITIES_BCL_BCLXML_HPP
