/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef ANALYSIS_OPTIMIZATIONDATAPOINT_HPP
#define ANALYSIS_OPTIMIZATIONDATAPOINT_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/DataPoint.hpp>

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
                        const std::vector<FileReference>& xmlOutputData,
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
                        const std::vector<FileReference>& xmlOutputData,
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

  explicit OptimizationDataPoint(boost::shared_ptr<detail::OptimizationDataPoint_Impl> impl);
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

