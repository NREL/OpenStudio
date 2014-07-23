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

#ifndef ANALYSIS_INPUTVARIABLE_IMPL_HPP
#define ANALYSIS_INPUTVARIABLE_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "Variable_Impl.hpp"

#include "UncertaintyDescription.hpp"

namespace openstudio {

class FileReferenceType;

namespace runmanager {
  class WorkItem;
}

namespace analysis {

class InputVariable;
class Measure;
class WorkflowStep;

namespace detail {

  /** InputVariable_Impl is a Variable_Impl that is the implementation class for
   *  InputVariable */
  class ANALYSIS_API InputVariable_Impl : public Variable_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    InputVariable_Impl(const std::string& name);

    /** Constructor provided for deserialization; not for general use. */
    InputVariable_Impl(const UUID& uuid,
                       const UUID& versionUUID,
                       const std::string& name,
                       const std::string& displayName,
                       const std::string& description,
                       const boost::optional<UncertaintyDescription>& udesc);

    InputVariable_Impl(const InputVariable_Impl& other);

    virtual ~InputVariable_Impl() {}

    //@}
    /** @name Getters and Queries */
    //@{

    virtual boost::optional<FileReferenceType> inputFileType() const = 0;

    virtual boost::optional<FileReferenceType> outputFileType() const = 0;

    boost::optional<UncertaintyDescription> uncertaintyDescription() const;

    /** Returns true if value is valid for this Variable. If returns false, createWorkItem(value)
     *  should throw. */
    virtual bool isValid(const QVariant& value) const = 0;

    /** Returns true if udesc is valid for this Variable. If returns false,
     *  setUncertaintyDescription will fail. */
    virtual bool isValid(const UncertaintyDescription& udesc) const = 0;

    WorkflowStep asWorkflowStep() const;

    //@}
    /** @name Setters */
    //@{

    bool setUncertaintyDescription(const UncertaintyDescription& udesc);

    void resetUncertaintyDescription();

    //@}
    /** @name Actions */
    //@{

    /** Create a runmanager::WorkItem for instantiating this Variable at value. Will throw an
     *  openstudio::Exception if value is invalid for the variable. */
    virtual runmanager::WorkItem createWorkItem(const QVariant& value,
                                                const openstudio::path& rubyIncludeDirectory) const = 0;

    //@}
    /** @name Protected in or Absent from Public Class */
    //@{

    virtual QVariant toVariant() const;

    static InputVariable factoryFromVariant(const QVariant &variant, const VersionString &version);

    static InputVariable factoryFromVariant(const QVariant& variant, const Measure& measure, const VersionString& version);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.InputVariable");

    boost::optional<UncertaintyDescription> m_uncertaintyDescription;
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_INPUTVARIABLE_IMPL_HPP
