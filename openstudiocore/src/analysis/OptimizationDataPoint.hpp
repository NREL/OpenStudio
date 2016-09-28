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

#ifndef ANALYSIS_OPTIMIZATIONDATAPOINT_HPP
#define ANALYSIS_OPTIMIZATIONDATAPOINT_HPP

#include "AnalysisAPI.hpp"
#include "DataPoint.hpp"

namespace openstudio {
namespace analysis {

class OptimizationProblem;

namespace detail {

  class OptimizationDataPoint_Impl;
  class OptimizationProblem_Impl;

} // detail

/** OptimizationDataPoint is a DataPoint for use with \link OptimizationProblem
 *  OptimizationProblems \endlink. In addition to all the data provided by DataPoint,
 *  OptimizationDataPoint also has objective function values (calculated by OptimizationProblem
 *  post-simulation). As a user, OptimizationDataPoints are automatically created for you by
 *  calls to OptimizationProblem::createDataPoint either directly, or through the use of an
 *  Algorithm. (And Problem::createDataPoint works as well, because of the virtual implementation
 *  of the method.) */
class ANALYSIS_API OptimizationDataPoint : public DataPoint {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructor provided for deserialization; not for general use. */
  OptimizationDataPoint(const UUID& uuid,
                        const UUID& versionUUID,
                        const std::string& name,
                        const std::string& displayName,
                        const std::string& description,
                        const OptimizationProblem& optimizationProblem,
                        bool complete,
                        bool failed,
                        bool selected,
                        DataPointRunType runType,
                        const std::vector<QVariant>& variableValues,
                        const std::vector<double>& responseValues,
                        const std::vector<double>& objectiveValues,
                        const openstudio::path& directory,
                        const boost::optional<FileReference>& osmInputData,
                        const boost::optional<FileReference>& idfInputData,
                        const boost::optional<FileReference>& sqlOutputData,
                        const boost::optional<runmanager::Job>& topLevelJob,
                        const std::vector<openstudio::path>& dakotaParametersFiles,
                        const std::vector<Tag>& tags,
                        const std::vector<Attribute>& outputAttributes);

  /** Constructor provided for deserialization; not for general use. */
  OptimizationDataPoint(const UUID& uuid,
                        const UUID& versionUUID,
                        const std::string& name,
                        const std::string& displayName,
                        const std::string& description,
                        const UUID& problemUUID,
                        const boost::optional<UUID>& analysisUUID,
                        bool complete,
                        bool failed,
                        bool selected,
                        DataPointRunType runType,
                        const std::vector<QVariant>& variableValues,
                        const std::vector<double>& responseValues,
                        const std::vector<double>& objectiveValues,
                        const openstudio::path& directory,
                        const boost::optional<FileReference>& osmInputData,
                        const boost::optional<FileReference>& idfInputData,
                        const boost::optional<FileReference>& sqlOutputData,
                        const boost::optional<runmanager::Job>& topLevelJob,
                        const std::vector<openstudio::path>& dakotaParametersFiles,
                        const std::vector<Tag>& tags,
                        const std::vector<Attribute>& outputAttributes);

  virtual ~OptimizationDataPoint() {}

  //@}
  /** @name Getters and Queries */
  //@{

  /** Equivalent to .problem().cast<OptimizationProblem>(). Provided for convenience. */
  OptimizationProblem optimizationProblem() const;

  std::vector<double> objectiveValues() const;

  //@}
 protected:
  /// @cond
  typedef detail::OptimizationDataPoint_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::OptimizationDataPoint_Impl;

  explicit OptimizationDataPoint(std::shared_ptr<detail::OptimizationDataPoint_Impl> impl);
  /// @endcond

  friend class detail::OptimizationProblem_Impl;

  /** Constructor from variableValues. Called by OptimizationProblem::createDataPoint. After
   *  construction isComplete() == false. */
  OptimizationDataPoint(const OptimizationProblem& optimizationProblem,
                        const std::vector<QVariant>& variableValues);

  void setObjectiveValues(const std::vector<double> values);

 private:

  REGISTER_LOGGER("openstudio.analysis.OptimizationDataPoint");
};

/** \relates OptimizationDataPoint*/
typedef boost::optional<OptimizationDataPoint> OptionalOptimizationDataPoint;

/** \relates OptimizationDataPoint*/
typedef std::vector<OptimizationDataPoint> OptimizationDataPointVector;

/** Predicate struct for comparing \link OptimizationDataPoint OptimizationDataPoints \endlink by
 *  objective function value. \relates OptimizationDataPoint */
struct ANALYSIS_API OptimizationDataPointObjectiveFunctionLess {
 public:
  /** Construct a predicate object. index refers to the index of the objective function to sort by
   *  in OptimizationProblem::objectiveFunctions(). */
  OptimizationDataPointObjectiveFunctionLess(unsigned index=0);

  /** Returns true if left.objectiveFunctionValues()[index] < right.objectiveFunctionValues()[index]. */
  bool operator()(const OptimizationDataPoint& left, const OptimizationDataPoint& right) const;
 private:
  unsigned m_index;
};

} // analysis
} // openstudio

#endif // ANALYSIS_OPTIMIZATIONDATAPOINT_HPP

