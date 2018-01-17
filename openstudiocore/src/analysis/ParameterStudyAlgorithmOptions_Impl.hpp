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

#ifndef ANALYSIS_PARAMETERSTUDYALGORITHMOPTIONS_IMPL_HPP
#define ANALYSIS_PARAMETERSTUDYALGORITHMOPTIONS_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithmOptions_Impl.hpp"

namespace openstudio {
namespace analysis {

class Problem;

namespace detail {

  /** ParameterStudyAlgorithmOptions_Impl is a DakotaAlgorithmOptions_Impl that is the implementation class for ParameterStudyAlgorithmOptions.*/
  class ANALYSIS_API ParameterStudyAlgorithmOptions_Impl : public DakotaAlgorithmOptions_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Every ParameterStudyAlgorithmType has at least one required input. If one of the required
     *  inputs is not specified, dakotaInFileDescription() will not write out that option in the
     *  .in file, and will log a warning. The required inputs are as follows:
     *  <ul>
     *  <li>vector_parameter_study requires either finalPoint or stepVector, and numSteps. If you
     *  want to use stepVector, then finalPoint must be empty.
     *  <li>list_parameter_study requires listOfPoints.
     *  <li>centered_parameter_study requires stepVector and stepsPerVariable.
     *  <li>multidim_parameter_study requires partitions.
     *  </ul>*/
    ParameterStudyAlgorithmOptions_Impl(const ParameterStudyAlgorithmType& algorithmType);

    /** Constructor provided for deserialization; not for general use. */
    ParameterStudyAlgorithmOptions_Impl(const ParameterStudyAlgorithmType& algorithmType,
                                        const std::vector<Attribute>& options);

    virtual ~ParameterStudyAlgorithmOptions_Impl() {}

    virtual AlgorithmOptions clone() const override;

    //@}
    /** @name Getters */
    //@{

    ParameterStudyAlgorithmType algorithmType() const;

    /** Returns the vector that defines the final point to be sampled. Only applies to
     *  ParameterStudyAlgorithmType::vector_parameter_study. */
    std::vector<double>  finalPoint() const;

    /** Returns the vector that defines the step vector for sampling. Only applies to
     *  ParameterStudyAlgorithmType::vector_parameter_study and 
     *  ParameterStudyAlgorithmType::centered_parameter_study. */
    std::vector<double>  stepVector() const;

    /** Returns the number of steps to be taken for sampling. Only applies to
     *  ParameterStudyAlgorithmType::vector_parameter_study. This value must be specified to use 
     *  finalPoint or stepVector. */
    boost::optional<int> numSteps() const;

    /** Returns the user-defined list of points that are to be sampled. Only applies to
     *  ParameterStudyAlgorithmType::list_parameter_study, and must be specified for this 
     *  ParameterStudyAlgorithmType. */
    std::vector<double> listOfPoints() const;

    /** Returns the number of steps to be taken for each variable. Only applies to
     *  ParameterStudyAlgorithmType::centered_parameter_study, and must be specified for this
     *  ParameterStudyAlgorithmType. */
    std::vector<int>  stepsPerVariable() const;

    /** Returns the number of partitions to be used for each variable. Only applies to
     *  ParameterStudyAlgorithmType::multidim_parameter_study, and must be specified for this
     *  ParameterStudyAlgorithmType. */
    std::vector<int>  partitions() const;

    //@}
    /** @name Setters */
    //@{

    /** Only applies to ParameterStudyAlgorithmType::vector_parameter_study. There must be one value
     *  for every variable.*/
    void setFinalPoint(const std::vector<double>& value);
  
    void clearFinalPoint();

    /** Only applies to ParameterStudyAlgorithmType::vector_parameter_study and 
     *  ParameterStudyAlgorithmType::centered_parameter_study. There must be one value for every 
     *  variable.*/
    void setStepVector(const std::vector<double>& value);
  
    void clearStepVector();

    /** Only applies to ParameterStudyAlgorithmType::vector_parameter_study. Value must be greater
     *  than zero.*/
    bool setNumSteps(const int value);
  
    void clearNumSteps();  

    /** Only applies to ParameterStudyAlgorithmType::list_parameter_study. There must be one value
     *  for every variable.*/
    void setListOfPoints(const std::vector<double>& value);
  
    void clearListOfPoints();  

    /** Only applies to ParameterStudyAlgorithmType::centered_parameter_study. There must be one
     *  value for every variable, and they must be nonnegative.*/
    bool setStepsPerVariable(const std::vector<int>& value);
  
    void clearStepsPerVariable();  

    /** Only applies to ParameterStudyAlgorithmType::multidim_parameter_study. There must be one
     *  value for every variable, and they must be nonnegative.*/
    bool setPartitions(const std::vector<int>& value);

    void clearPartitions();

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    virtual QVariant toVariant() const override;

    static ParameterStudyAlgorithmOptions fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   protected:
    ParameterStudyAlgorithmType m_algorithmType;

   private:
    REGISTER_LOGGER("openstudio.analysis.ParameterStudyAlgorithmOptions");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_PARAMETERSTUDYALGORITHMOPTIONS_IMPL_HPP

