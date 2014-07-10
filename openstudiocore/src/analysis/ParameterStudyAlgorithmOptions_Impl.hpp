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

    virtual AlgorithmOptions clone() const;

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

    virtual QVariant toVariant() const;

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

