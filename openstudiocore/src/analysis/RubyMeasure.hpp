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

#ifndef ANALYSIS_RUBYMEASURE_HPP
#define ANALYSIS_RUBYMEASURE_HPP

#include "AnalysisAPI.hpp"
#include "Measure.hpp"

namespace openstudio {

class BCLMeasure;
class FileReference;
class FileReferenceType;

namespace ruleset {
  class OSArgument;
}

namespace analysis {

class RubyContinuousVariable;

namespace detail {

  class Problem_Impl;
  class RubyMeasure_Impl;
  class RubyContinuousVariable_Impl;

} // detail

/** RubyMeasure is a Measure that modifies a model (OSM or IDF file) using a Ruby
 *  script. The preferred way to create a RubyMeasure is with a BCLMeasure, see
 *  openstudio/bcl/BCLMeasure, ruleset::ModelUserScript, and ruleset::WorkspaceUserScript. In brief,
 *  a BCLMeasure is a specially structured folder, the contents of which includes a measure.rb file,
 *  which in turn contains the definition of a class that derives from ruleset::UserScript. Such 
 *  scripts may be parameterized with ruleset::OSArguments, and can be run from various entry points
 *  in OpenStudio, including from this class within the analysis framework (with the help of the 
 *  runmanager and analysisdriver sub-projects).
 *
 *  For now, \link RubyMeasure RubyMeasures\endlink can also be defined by referencing a
 *  Ruby script directly (free-form or following the interface specified by the ruleset::UserScript 
 *  classes). However, these features will be deprecated in time. */
class ANALYSIS_API RubyMeasure : public Measure {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructor from BCLMeasure. */
  RubyMeasure(const BCLMeasure& bclMeasure, bool isSelected = true);

  /** Constructor. Measure script can be self-contained script that handles its own arguments
   *  or a file that contains a class derived from ruleset::UserScript. \deprecated
   *
   *  If perturbationScript contains a rulset::UserScript, its type should match inputFileType and
   *  outputFileType. That is, if the UserScript is a ModelUserScript, inputFileType ==
   *  outputFileType == FileReferenceType::OSM. The isUserScript boolean should also be set to
   *  true in this case. */
  RubyMeasure(const openstudio::path& perturbationScript,
              const FileReferenceType& inputFileType,
              const FileReferenceType& outputFileType,
              bool isUserScript = false,
              bool isSelected = true);

  /** Constructor provided for deserialization; not for general use. */
  RubyMeasure(const UUID& uuid,
              const UUID& versionUUID,
              const std::string& name,
              const std::string& displayName,
              const std::string& description,
              bool isSelected,
              const FileReference& perturbationScriptOrBCLMeasureDir,
              const FileReferenceType& inputFileType,
              const FileReferenceType& outputFileType,
              bool isUserScript,
              const std::vector<ruleset::OSArgument>& arguments,
              bool usesBCLMeasure);

  virtual ~RubyMeasure() {}

  //@}
  /** @name Getters and Queries */
  //@{

  /** Temporary boolean to indicate whether this perturbation works off of a BCLMeasure
   *  or a FileReferenece. */
  bool usesBCLMeasure() const;

  /** Returns the BCLMeasure. Throws if not usesBCLMeasure(). Returns boost::none if measure
   *  cannot be located on file system. */
  boost::optional<BCLMeasure> bclMeasure() const;

  /** \deprecated */
  boost::optional<BCLMeasure> measure() const;

  /** Returns the BCLMeasure's directory. Throws if not usesBCLMeasure(). */
  openstudio::path bclMeasureDirectory() const;

  /** \deprecated */
  openstudio::path measureDirectory() const;

  /** Returns the BCLMeasure's UUID. Throws if not usesBCLMeasure(). */
  UUID bclMeasureUUID() const;

  /** \deprecated */
  UUID measureUUID() const;

  /** Returns the BCLMeasure's version UUID. Throws if not usesBCLMeasure(). */
  UUID bclMeasureVersionUUID() const;

  /** \deprecated */
  UUID measureVersionUUID() const;

  /** Returns the file reference. Throws if usesBCLMeasure(). \deprecated */
  FileReference perturbationScript() const;

  /** Return the arguments to be passed to the perturbation ruby script as a vector of
   *  ruleset::OSArguments. If this RubyMeasure is used by one or more \link
   *  RubyContinuousVariable RubyContinuousVariables\endlink, this may be an incomplete listing 
   *  from the script's perspective. */
  std::vector<ruleset::OSArgument> arguments() const;

  /** If not usesBCLMeasure(), indicates whether the file pointed to follows ruleset::UserScript
   *  syntax or not. \deprecated */
  bool isUserScript() const;

  /** Returns !incompleteArguments().empty(). */
  bool hasIncompleteArguments() const;

  /** Returns any arguments() that are required but do not have a value or default value set. */
  std::vector<ruleset::OSArgument> incompleteArguments() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets measure to measure, calls clearArguments, and returns true if measure's file types
   *  are okay with the current use of this RubyMeasure. Does nothing and returns false
   *  otherwise. */
  bool setMeasure(const BCLMeasure& measure);

  /** Updates measure to newVersion and merges current argument values with newArguments if
   *  newVersion has the same UUID as measure() and has compatible file types for the current
   *  use of this RubyMeasure. Does nothing and returns false otherwise. Upon successful
   *  completion, the return value of arguments() is in the same order as newArguments. */
  bool updateMeasure(const BCLMeasure& newVersion, std::vector<ruleset::OSArgument> newArguments);

  /** Sets the perturbationScript to script and clears all arguments. \deprecated */
  bool setPerturbationScript(const openstudio::path& script,
                             const FileReferenceType& inputFileType,
                             const FileReferenceType& outputFileType,
                             bool isUserScript);

  /** \deprecated */
  void setIsUserScript(bool isUserScript);

  /** Add an argument to this script. The argument will be stored as a ruleset::OSArgument,
   *  and passed in as --#{name}. \deprecated */
  void addArgument(const std::string& name);

  /** Add an argument to this script. The argument will be stored as a ruleset::OSArgument,
   *  and passed in as --#{name}=#{value}. \deprecated */
  void addArgument(const std::string& name, const std::string& value);

  /** Add argument to this script. Returns false if !argument.hasValue() &&
   *  !argument.hasDefaultValue(), but will still register the argument. \deprecated */
  bool addArgument(const ruleset::OSArgument& argument);

  /** Sets argument. Replaces any existing argument with the same name, otherwise, adds it
   *  to the list. */
  void setArgument(const ruleset::OSArgument& argument);

  /** Replaces current arguments with the provided vector. */
  void setArguments(const std::vector<ruleset::OSArgument>& arguments);

  /** Removes argument named argumentName. */
  bool removeArgument(const std::string& argumentName);

  void clearArguments();

  //@}
 protected:
  /// @cond
  typedef detail::RubyMeasure_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::RubyMeasure_Impl;

  explicit RubyMeasure(std::shared_ptr<detail::RubyMeasure_Impl> impl);

  friend class RubyContinuousVariable; // to call setParent
  friend class detail::RubyContinuousVariable_Impl; // to call setParent
  friend class detail::Problem_Impl; // to call toVariant
  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.RubyMeasure");
};

/** \relates RubyMeasure*/
typedef boost::optional<RubyMeasure> OptionalRubyMeasure;

/** \relates RubyMeasure*/
typedef std::vector<RubyMeasure> RubyMeasureVector;

// ETH@20130723 - These typedefs are just to preserve the old names for the near to
// medium-term future (to avoid breaking existing scripts).

/** \relates RubyMeasure*/
typedef RubyMeasure RubyPerturbation;

/** \relates RubyMeasure*/
typedef boost::optional<RubyMeasure> OptionalRubyPerturbation;

/** \relates RubyMeasure*/
typedef std::vector<RubyMeasure> RubyPerturbationVector;

} // analysis
} // openstudio

#endif // ANALYSIS_RUBYMEASURE_HPP

