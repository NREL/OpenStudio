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

#ifndef UTILITIES_BCL_BCLMEASURE_HPP
#define UTILITIES_BCL_BCLMEASURE_HPP

#include "BCLXML.hpp"
#include "BCLEnums.hpp"
#include "../core/Optional.hpp"
#include "../core/Path.hpp"
#include "../core/Deprecated.hpp"
#include "../data/Attribute.hpp"
#include "../UtilitiesAPI.hpp"

#include <vector>

namespace openstudio {

class FileReferenceType;

/** BCLMeasure is a class for managing the contents of a BCL Measure directory including the xml description file.
  **/
class UTILITIES_API BCLMeasure
{
 public:
  /** @name Constructors */
  //@{

  /// Constructor for a new measure with required data
  /// Will throw exception unless dir is a directory that does not exist or an empty directory.
  /// A template measure will be created, including a blank user script.
  BCLMeasure(const std::string& name, const std::string& className, const openstudio::path& dir, const std::string& taxonomyTag,
             MeasureType measureType, const std::string& description, const std::string& modelerDescription,
             MeasureLanguage measureLanguage = MeasureLanguage::Ruby);

  /// Constructor for downloaded measures, path is to directory containing "measure.xml" file
  /// Will throw exception if directory does not exist or does not represent a valid measure.
  BCLMeasure(const openstudio::path& dir);

  /// Convert a measure name to a valid Ruby class name
  static std::string makeClassName(const std::string& name);

  /// Try to load a BCLMeasure from a directory containing a "measure.xml" file.
  static boost::optional<BCLMeasure> load(const openstudio::path& dir);

  /// Suggested values for "Intended Software Tool" attribute
  static std::vector<std::string> suggestedIntendedSoftwareTools();

  /// Suggested values for "Intended Use Case" attribute
  static std::vector<std::string> suggestedIntendedUseCases();

  /// Suggested values for the first level taxonomy tag
  // DLM: we should move the TID code out of BuildingComponentDialog into BCLComponent
  static std::vector<std::string> suggestedFirstLevelTaxonomyTerms();

  /// Suggested values for the second level taxonomy tag
  static std::vector<std::string> suggestedSecondLevelTaxonomyTerms(const std::string& firstLevelTaxonomyTerm);

  static bool isIgnoredFileName(const std::string& fileName);

  static bool isApprovedFile(const openstudio::path& absoluteFilePath, const openstudio::path& measureDir);

  //@}
  /** @name Destructor */
  //@{

  /// Virtual destructor
  virtual ~BCLMeasure() = default;

  //@}
  /** @name Getters */
  //@{

  openstudio::path directory() const;

  boost::optional<std::string> error() const;

  std::string uid() const;

  UUID uuid() const;

  std::string versionId() const;

  UUID versionUUID() const;

  boost::optional<DateTime> versionModified() const;

  std::string xmlChecksum() const;

  // string representation of the underlying BCLXML
  std::string xmlString() const;

  std::string name() const;

  std::string displayName() const;

  std::string className() const;

  std::string description() const;

  std::string modelerDescription() const;

  std::vector<BCLMeasureArgument> arguments() const;

  std::vector<BCLMeasureOutput> outputs() const;

  // ETH: provenance?
  // DLM: we can do this later

  std::vector<std::string> tags() const;

  std::vector<Attribute> attributes() const;

  std::vector<BCLFileReference> files() const;

  //@}
  /** @name Setters */
  //@{

  void setError(const std::string& error);

  void resetError();

  void setName(const std::string& name);

  void setDisplayName(const std::string& displayName);

  void setClassName(const std::string& className);

  void setDescription(const std::string& description);

  void setModelerDescription(const std::string& modelerDescription);

  void setArguments(const std::vector<BCLMeasureArgument>& arguments);

  void setOutputs(const std::vector<BCLMeasureOutput>& outputs);

  //@}
  /** @name Operators */
  //@{

  /// Returns the single taxonomy tag allowed
  std::string taxonomyTag() const;

  // DLM: might want to deprecate this as this is set manually in XML
  /// Sets the single taxonomy tag allowed
  void setTaxonomyTag(const std::string& taxonomyTag);

  /// Returns value of the "Measure Type" attribute
  MeasureType measureType() const;

  MeasureLanguage measureLanguage() const;

  /// Sets value of the "Measure Type" attribute
  void setMeasureType(const MeasureType& measureType);

  void setMeasureLanguage(const MeasureLanguage& measureLanguage);

  /// Returns values of any "Intended Software Tool" attributes
  std::vector<std::string> intendedSoftwareTools() const;

  /// Returns values of any "Intended Use Case" attributes
  std::vector<std::string> intendedUseCases() const;

  /** Returns the path of the primary Ruby script if the file exists and is
    *  a regular file. Otherwise returns boost::none. */
  boost::optional<openstudio::path> primaryRubyScriptPath() const;

  boost::optional<openstudio::path> primaryPythonScriptPath() const;

  /** Returns the path of the primary script path, based on MeasureLanguage **/
  boost::optional<openstudio::path> primaryScriptPath() const;

  /// Returns the input file type for this measure
  FileReferenceType inputFileType() const;

  /// Returns the output file type for this measure
  FileReferenceType outputFileType() const;

  /// Clears all files
  void clearFiles();

  /// Adds attribute to attribute list
  void addAttribute(const Attribute& attribute);

  /// get attributes by name
  std::vector<Attribute> getAttributes(const std::string& name) const;

  /// Removes attribute by name, returns true if attribute was found and removed
  bool removeAttributes(const std::string& name);

  /// Removes all attributes
  void clearAttributes();

  /// Check for all required fields, return true if any are missing
  /// Does not update the XML
  bool missingRequiredFields() const;

  /// Check for updates to files, will increment versionID and return true
  /// if any files have changed, been added, or removed from the measure
  /// The measure must still be saved to disk to preserve the new versionID
  /// Typical usage is 1) checkForUpdatesFiles, 2) update content using embedded ruby interpreter, 3) checkForUpdatesXML
  bool checkForUpdatesFiles();

  /// Check for updates to the xml, will increment versionID and xmlChecksum then return true
  /// if any xml fields (other than uid, version id, or xml checksum) have changed
  /// The xml file must still be saved to disk to preserve the new versionID
  bool checkForUpdatesXML();

  /// Equality test - Compares the uids and versionIds of two separate measures
  bool operator==(const BCLMeasure& other) const;

  bool operator!=(const BCLMeasure& other) const;

  /// Less than
  bool operator<(const BCLMeasure& other) const;

  /// Save the measure.xml back to the directory
  bool save() const;

  /// Clone the measure from the original location to the new location
  /// newDir is a directory which either does not exist or is empty
  /// If the copy is successful a new BCLMeasure is returned
  boost::optional<BCLMeasure> clone(const openstudio::path& newDir) const;

  /// Change the UID for this measure
  void changeUID();

  /// Change the Version ID for this measure
  void incrementVersionId();

  /// Update's the ruby measure script to have this new information
  // cppcheck-suppress functionConst
  bool updateMeasureScript(const MeasureType& oldMeasureType, const MeasureType& newMeasureType, const MeasureLanguage& oldMeasureLanguage,
                           const MeasureLanguage& newMeasureLanguage, const std::string& oldClassName, const std::string& newClassName,
                           const std::string& name, const std::string& description, const std::string& modelerDescription);

  /// Update's the ruby measure tests to have this new information
  // cppcheck-suppress functionConst
  bool updateMeasureTests(const std::string& oldClassName, const std::string& newClassName);

  /// get all measures in an input directory
  static std::vector<BCLMeasure> getMeasuresInDir(const openstudio::path& dir);

  //@}
 private:
  // configure logging
  REGISTER_LOGGER("utilities.bcl.BCLMeasure");

  static void createDirectory(const openstudio::path& dir);

  std::string computeXMLChecksum() const;

  openstudio::path m_directory;
  BCLXML m_bclXML;
};

using OptionalBCLMeasure = boost::optional<BCLMeasure>;

using BCLMeasureVector = std::vector<BCLMeasure>;

}  // namespace openstudio

#endif  // UTILITIES_BCL_BCLMEASURE_HPP
