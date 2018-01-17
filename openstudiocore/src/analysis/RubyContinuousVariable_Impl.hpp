/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

    virtual AnalysisObject clone() const override;

    //@}
    /** @name Virtual Methods */
    //@{

    /** Get the variable value from a dataPoint. Throws an openstudio::Exception if the Variable
     *  cannot be evaluated for dataPoint. */
    virtual double getValue(const DataPoint& dataPoint) const override;

    virtual boost::optional<FileReferenceType> inputFileType() const override;

    virtual boost::optional<FileReferenceType> outputFileType() const override;

    /** Returns true if value is valid for this Variable. If returns false, createJob(value) should
     *  throw. */
    virtual bool isValid(const QVariant& value) const override;

    /** Returns true if udesc is valid for this Variable. If returns false,
     *  setUncertaintyDescription will fail. */
    virtual bool isValid(const UncertaintyDescription& udesc) const override;

    virtual runmanager::WorkItem createWorkItem(const QVariant& value,
                                                const openstudio::path& rubyIncludeDirectory) const override;

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

    virtual QVariant toVariant() const override;

    static RubyContinuousVariable fromVariant(const QVariant& variant, const VersionString& version);

    static RubyContinuousVariable fromVariant(const QVariant& variant, const Measure& measure, const VersionString& version);

    /// Relocate path data from originalBase to newBase.
    virtual void updateInputPathData(const openstudio::path& originalBase,
                                     const openstudio::path& newBase) override;

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

