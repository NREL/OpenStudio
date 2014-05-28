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

#ifndef ANALYSIS_RUBYCONTINUOUSVARIABLE_IMPL_HPP
#define ANALYSIS_RUBYCONTINUOUSVARIABLE_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "ContinuousVariable_Impl.hpp"
#include "RubyMeasure.hpp"

#include "../ruleset/OSArgument.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  /** RubyContinuousVariable_Impl is a ContinuousVariable_Impl that is the implementation class
   *  for RubyContinuousVariable.*/
  class ANALYSIS_API RubyContinuousVariable_Impl : public ContinuousVariable_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RubyContinuousVariable_Impl(const std::string& name,
                                const ruleset::OSArgument& argument,
                                const RubyMeasure& measure);

    RubyContinuousVariable_Impl(const UUID& uuid,
                                const UUID& versionUUID,
                                const std::string& name,
                                const std::string& displayName,
                                const std::string& description,
                                const boost::optional<UncertaintyDescription>& udesc,
                                boost::optional<double> minimum,
                                boost::optional<double> maximum,
                                boost::optional<double> increment,
                                boost::optional<int> nSteps,
                                const ruleset::OSArgument& argument,
                                const RubyMeasure& pertubation);

    RubyContinuousVariable_Impl(const RubyContinuousVariable_Impl& other);

    virtual ~RubyContinuousVariable_Impl() {}

    virtual AnalysisObject clone() const;

    //@}
    /** @name Virtual Methods */
    //@{

    /** Get the variable value from a dataPoint. Throws an openstudio::Exception if the Variable
     *  cannot be evaluated for dataPoint. */
    virtual double getValue(const DataPoint& dataPoint) const;

    virtual boost::optional<FileReferenceType> inputFileType() const;

    virtual boost::optional<FileReferenceType> outputFileType() const;

    /** Returns true if value is valid for this Variable. If returns false, createJob(value) should
     *  throw. */
    virtual bool isValid(const QVariant& value) const;

    /** Returns true if udesc is valid for this Variable. If returns false,
     *  setUncertaintyDescription will fail. */
    virtual bool isValid(const UncertaintyDescription& udesc) const;

    virtual runmanager::WorkItem createWorkItem(const QVariant& value,
                                                const openstudio::path& rubyIncludeDirectory) const;

    //@}
    /** @name Getters and Queries */
    //@{

    ruleset::OSArgument argument() const;

    RubyMeasure measure() const;

    //@}
    /** @name Setters */
    //@{

    void setArgument(const ruleset::OSArgument& argument);

    bool setRubyMeasure(const RubyMeasure& measure);

    //@}
    /** @name Protected in Public Class */
    //@{

    bool fileTypesAreCompatible(const RubyMeasure& childRubyMeasure,
                                const boost::optional<FileReferenceType>& proposedInputFileType,
                                const boost::optional<FileReferenceType>& proposedOutputFileType) const;

    //@}
    /** @name Protected in or Absent from Public Class */
    //@{

    virtual QVariant toVariant() const;

    static RubyContinuousVariable fromVariant(const QVariant& variant, const VersionString& version);

    static RubyContinuousVariable fromVariant(const QVariant& variant, const Measure& measure, const VersionString& version);

    /// Relocate path data from originalBase to newBase.
    virtual void updateInputPathData(const openstudio::path& originalBase,
                                     const openstudio::path& newBase);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.RubyContinuousVariable");

    ruleset::OSArgument m_argument;
    RubyMeasure m_measure;
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_RUBYCONTINUOUSVARIABLE_IMPL_HPP

