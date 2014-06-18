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

#ifndef ANALYSIS_PARAMETERSTUDYALGORITHMOPTIONS_HPP
#define ANALYSIS_PARAMETERSTUDYALGORITHMOPTIONS_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithmOptions.hpp"

#include "../utilities/core/Enum.hpp"

namespace openstudio {
namespace analysis {

class Problem;

namespace detail {

  class ParameterStudyAlgorithmOptions_Impl;

} // detail

/** \class ParameterStudyAlgorithmType 
 *  \brief Lists the types of sampling methods offered by the Parameter Study library.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM( ParameterStudyAlgorithmType,
  ((vector_parameter_study)(Vector Parameter Study))
  ((list_parameter_study)(List Parameter Study))
  ((centered_parameter_study)(Centered Parameter Study))
  ((multidim_parameter_study)(Multidimensional Parameter Study))
); 
 *  \endcode
 *  The valueName is the DAKOTA key for this algorithm type. The valueDescription is for 
 *  reading and display. */
OPENSTUDIO_ENUM( ParameterStudyAlgorithmType,
  ((vector_parameter_study)(Vector Parameter Study))
  ((list_parameter_study)(List Parameter Study))
  ((centered_parameter_study)(Centered Parameter Study))
  ((multidim_parameter_study)(Multidimensional Parameter Study))
);

/** ParameterStudyAlgorithmOptions is a DakotaAlgorithmOptions that defines options specific to 
 *  ParameterStudyAlgorithm. */
class ANALYSIS_API ParameterStudyAlgorithmOptions : public DakotaAlgorithmOptions {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Every ParameterStudyAlgorithmType has at least one required input. If one of the required
   *  inputs is not specified, dakotaInFileDescription() will not write out that option in the .in
   *  file, and will log a warning. The required inputs are as follows:
   *  <ul>
   *  <li>vector_parameter_study requires either finalPoint or stepVector, and numSteps. If you
   *  want to use stepVector, then finalPoint must be empty.
   *  <li>list_parameter_study requires listOfPoints.
   *  <li>centered_parameter_study requires stepVector and stepsPerVariable.
   *  <li>multidim_parameter_study requires partitions.
   *  </ul> */
  explicit ParameterStudyAlgorithmOptions(const ParameterStudyAlgorithmType& algorithmType);
  
  /** Constructor provided for deserialization; not for general use. */
  ParameterStudyAlgorithmOptions(const ParameterStudyAlgorithmType& algorithmType,
                                 const std::vector<Attribute>& options);

  virtual ~ParameterStudyAlgorithmOptions() {}

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
 protected:
  /// @cond
  typedef detail::ParameterStudyAlgorithmOptions_Impl ImplType;

  explicit ParameterStudyAlgorithmOptions(std::shared_ptr<detail::ParameterStudyAlgorithmOptions_Impl> impl);

  friend class detail::ParameterStudyAlgorithmOptions_Impl;
  friend class AlgorithmOptions;
  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.ParameterStudyAlgorithmOptions");
};

/** \relates ParameterStudyAlgorithmOptions*/
typedef boost::optional<ParameterStudyAlgorithmOptions> OptionalParameterStudyAlgorithmOptions;

/** \relates ParameterStudyAlgorithmOptions*/
typedef std::vector<ParameterStudyAlgorithmOptions> ParameterStudyAlgorithmOptionsVector;

} // analysis
} // openstudio

#endif // ANALYSIS_PARAMETERSTUDYALGORITHMOPTIONS_HPP

