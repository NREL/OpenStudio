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

#ifndef ANALYSIS_RUBYMEASURE_IMPL_HPP
#define ANALYSIS_RUBYMEASURE_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "Measure_Impl.hpp"

#include "../ruleset/OSArgument.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"

#include "../utilities/core/FileReference.hpp"

namespace openstudio {
namespace analysis {

class RubyMeasure;

namespace detail {

  /** RubyMeasure_Impl is a Measure_Impl that is the implementation class for RubyMeasure.*/
  class ANALYSIS_API RubyMeasure_Impl : public Measure_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructor from BCLMeasure. */
    RubyMeasure_Impl(const BCLMeasure& bclMeasure, bool isSelected);

    /** Constructor. Measure script can be self-contained script that handles its own arguments
     *  or a file that contains a class derived from ruleset::UserScript. \deprecated
     *
     *  If perturbationScript contains a rulset::UserScript, its type should match inputFileType and
     *  outputFileType. That is, if the UserScript is a ModelUserScript, inputFileType ==
     *  outputFileType == FileReferenceType::OSM. The isUserScript boolean should also be set to
     *  true in this case. */
    RubyMeasure_Impl(const openstudio::path& perturbationScript,
                     const FileReferenceType& inputFileType,
                     const FileReferenceType& outputFileType,
                     bool isUserScript,
                     bool isSelected);

    /** Constructor provided for deserialization; not for general use. */
    RubyMeasure_Impl(const UUID& uuid,
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

    RubyMeasure_Impl(const RubyMeasure_Impl& other);

    virtual ~RubyMeasure_Impl() {}

    virtual AnalysisObject clone() const;

    //@}
    /** @name Virtual Methods */
    //@{

    virtual boost::optional<FileReferenceType> inputFileType() const;

    virtual boost::optional<FileReferenceType> outputFileType() const;

    virtual runmanager::WorkItem createWorkItem(const openstudio::path& rubyIncludeDirectory) const;

    //@}
    /** @name Getters and Queries */
    //@{

    /** Temporary boolean to indicate whether this perturbation works off of a BCLMeasure
     *  or a FileReferenece. */
    bool usesBCLMeasure() const;

    /** Returns the BCLMeasure. Throws if not usesBCLMeasure(). Returns boost::none if measure
     *  cannot be located on file system. */
    boost::optional<BCLMeasure> bclMeasure() const;

    /** Returns the measure's directory. Throws if not usesBCLMeasure(). */
    openstudio::path bclMeasureDirectory() const;

    /** Returns the measure's UUID. Throws if not usesBCLMeasure(). */
    UUID bclMeasureUUID() const;

    /** Returns the measure's version UUID. Throws if not usesBCLMeasure(). */
    UUID bclMeasureVersionUUID() const;

    /** Returns the file reference. Throws if usesBCLMeasure(). \deprecated */
    FileReference perturbationScript() const;

    /** Return the arguments to be passed to the perturbation ruby script as a vector of
     *  Attributes. */
    std::vector<ruleset::OSArgument> arguments() const;

    /** If not usesBCLMeasure, indicates whether the file pointed to follows ruleset::UserScript
     *  syntax or not. \deprecated */
    bool isUserScript() const;

    bool hasIncompleteArguments() const;

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
     *  use of this RubyMeasure. Does nothing and returns false otherwise. */
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

    /** Removes argument named argumentName and returns true if such an argument is found.
     *  returns false otherwise. */
    bool removeArgument(const std::string& argumentName);

    void clearArguments();

    //@}
    /** @name Protected in or Absent from Public Class */
    //@{

    virtual QVariant toVariant() const;

    static RubyMeasure fromVariant(const QVariant& variant, const VersionString& version);

    /// Relocate path data from originalBase to newBase.
    virtual void updateInputPathData(const openstudio::path& originalBase,
                                     const openstudio::path& newBase);

    //@}
   protected:
    mutable boost::optional<BCLMeasure> m_bclMeasure; // cache for BCLMeasure.
                                                   // measure actually defined by data below.
    boost::optional<openstudio::path> m_bclMeasureDirectory;
    boost::optional<UUID> m_bclMeasureUUID, m_bclMeasureVersionUUID;
    boost::optional<FileReference> m_perturbationScript;
    FileReferenceType m_inputFileType;
    FileReferenceType m_outputFileType;
    bool m_isUserScript;
    std::vector<ruleset::OSArgument> m_arguments;

   private:
    REGISTER_LOGGER("openstudio.analysis.RubyMeasure");

    bool fileTypesAreCompatible(const FileReferenceType& proposedInputFileType,
                                const FileReferenceType& proposedOutputFileType) const;
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_RUBYMEASURE_IMPL_HPP

