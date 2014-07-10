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

#include "SequentialSearch.hpp"
#include "SequentialSearch_Impl.hpp"

#include "SequentialSearchOptions.hpp"
#include "SequentialSearchOptions_Impl.hpp"

#include "Analysis.hpp"
#include "DataPoint.hpp"
#include "OptimizationProblem.hpp"
#include "OptimizationProblem_Impl.hpp"
#include "OptimizationDataPoint.hpp"
#include "OptimizationDataPoint_Impl.hpp"
#include "DiscreteVariable.hpp"
#include "DiscreteVariable_Impl.hpp"

#include "../utilities/math/FloatCompare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  SequentialSearch_Impl::SequentialSearch_Impl(const SequentialSearchOptions& options)
    : OpenStudioAlgorithm_Impl(SequentialSearch::standardName(),options)
  {}

  SequentialSearch_Impl::SequentialSearch_Impl(const UUID& uuid,
                                               const UUID& versionUUID,
                                               const std::string& displayName,
                                               const std::string& description,
                                               bool complete,
                                               bool failed,
                                               int iter,
                                               const SequentialSearchOptions& options)
    : OpenStudioAlgorithm_Impl(uuid,
                               versionUUID,
                               SequentialSearch::standardName(),
                               displayName,
                               description,
                               complete,
                               failed,
                               iter,
                               options)
  {}

  SequentialSearch_Impl::SequentialSearch_Impl(const SequentialSearch_Impl& other)
    : OpenStudioAlgorithm_Impl(other)
  {}

  AnalysisObject SequentialSearch_Impl::clone() const {
    std::shared_ptr<SequentialSearch_Impl> impl(new SequentialSearch_Impl(*this));
    return SequentialSearch(impl);
  }

  bool SequentialSearch_Impl::isCompatibleProblemType(const Problem& problem) const {
    OptionalOptimizationProblem optimizationProblem = problem.optionalCast<OptimizationProblem>();
    if (!optimizationProblem) {
      return false;
    }
    if (!optimizationProblem->allVariablesAreDiscrete()) {
      return false;
    }
    if (optimizationProblem->numObjectives() != 2) {
      return false;
    }
    return true;
  }

  int SequentialSearch_Impl::createNextIteration(Analysis& analysis) {
    OS_ASSERT(analysis.algorithm().get() == getPublicObject<SequentialSearch>());

    int numAdded(0);
    if (isComplete()) {
      LOG(Info,"Algorithm is already marked as complete. Returning without creating new points.");
      return numAdded;
    }

    SequentialSearchOptions options = sequentialSearchOptions();
    OptionalInt maxIter = options.maxIter();
    OptionalInt maxSims = options.maxSims();
    DataPointVector ssDataPoints = analysis.getDataPoints("ss");
    // return without doing anything if any ssDataPoints are incomplete
    DataPointVector incompletePoints = analysis.dataPointsToQueue();
    DataPointVector::const_iterator it = std::find_if(incompletePoints.begin(),
                                                      incompletePoints.end(),
                                                      std::bind(&DataPoint::isTag,std::placeholders::_1,"ss"));
    if (it != incompletePoints.end()) {
      LOG(Info,"Returning because the last iteration has not yet been run.");
      return numAdded;
    }
    // last iteration appears complete
    int nSims = ssDataPoints.size();
    if (maxSims && (nSims >= *maxSims)) {
      LOG(Info,"Returning because maxSims " << *maxSims << " reached. No DataPoints will be added "
          << "to Analysis '"<< analysis.name() << "', and the Algorithm will be marked complete.");
      markComplete();
      return numAdded;
    }
    if (maxIter && (m_iter+1 == *maxIter)) {
      LOG(Info,"Returning because maxIter " << *maxIter << " reached. No DataPoints will be added "
          << "to Analysis '"<< analysis.name() << "', and the Algorithm will be marked complete.");
      markComplete();
      return numAdded;
    }
    onChange(AnalysisObject_Impl::Benign);
    ++m_iter;

    if (m_iter == 0) {
      // run baseline point, which is just the 0th perturbation of each variable.
      OptimizationProblem problem = analysis.problem().cast<OptimizationProblem>();
      std::vector<QVariant> baselineValues(problem.numVariables(),0);
      // see if already in analysis
      DataPointVector baselines = analysis.getDataPoints(baselineValues);
      if (!baselines.empty()) {
        OS_ASSERT(baselines.size() == 1u);
        DataPoint baseline = baselines[0];
        OS_ASSERT(baseline.optionalCast<OptimizationDataPoint>());
        OS_ASSERT(baseline.problem() == problem);
        if (!baseline.isTag("ss")) {
          baseline.addTag("ss");
        }
        if (!baseline.isTag("iter0")) {
          baseline.addTag("iter0");
        }
        if (!baseline.isTag("current")) {
          baseline.addTag("current");
        }
        if (!baseline.isComplete()) {
          ++numAdded;
        }
      }
      else {
        OptimizationDataPoint dataPoint = problem.createDataPoint(baselineValues).get().cast<OptimizationDataPoint>();
        dataPoint.addTag("ss");
        dataPoint.addTag("iter0");
        dataPoint.addTag("current");
        analysis.addDataPoint(dataPoint);
        ++numAdded;
      }
    }
    else {
      OptimizationDataPointVector minimumCurve =
          getMinimumCurve(options.objectiveToMinimizeFirst(),analysis);
      OS_ASSERT(!minimumCurve.empty());
      OS_ASSERT(minimumCurve.back().isTag("current"));
      if (!minimumCurve.back().isTag("explored")) {
        std::vector< std::vector<QVariant> > candidateVariableValues = getCandidateCombinations(minimumCurve.back());
        std::stringstream ss;
        ss << "iter" << m_iter;
        std::string iterTag(ss.str()); ss.str("");
        for (const std::vector<QVariant>& candidate : candidateVariableValues) {
          DataPoint newDataPoint = analysis.problem().createDataPoint(candidate).get();
          OS_ASSERT(newDataPoint.optionalCast<OptimizationDataPoint>());
          newDataPoint.addTag("ss");
          newDataPoint.addTag(iterTag);
          bool added = analysis.addDataPoint(newDataPoint);
          if (added) {
            ++numAdded;
            if (maxSims && (nSims + numAdded == *maxSims)) {
              break;
            }
          }
        }
        minimumCurve.back().addTag("explored");
      }
    }

    if (numAdded == 0) {
      LOG(Trace,"No new points were added, so marking this SequentialSearch complete.");
      markComplete();
    }

    return numAdded;
  }

  SequentialSearchOptions SequentialSearch_Impl::sequentialSearchOptions() const {
    return options().cast<SequentialSearchOptions>();
  }

  std::vector<OptimizationDataPoint> SequentialSearch_Impl::getMinimumCurve(
      unsigned i, Analysis& analysis) const
  {
    std::stringstream ss;
    ss << "curve" << i;
    std::string curveTag(ss.str()); ss.str("");

    OptimizationDataPointVector lastCurve = castVector<OptimizationDataPoint>(
        analysis.getDataPoints(curveTag));
    OptimizationDataPointVector lastCurrent = castVector<OptimizationDataPoint>(
        analysis.getDataPoints("current"));
    OptimizationDataPointVector result = castVector<OptimizationDataPoint>(
        analysis.getDataPoints("iter0")); // baseline point
    OS_ASSERT(result.size() < 2);
    OptimizationDataPointVector successfulPoints = castVector<OptimizationDataPoint>(
        analysis.successfulDataPoints());

    // construct curve
    OptionalOptimizationDataPoint current;
    if (!result.empty()) {
      current = result.back();
      if (!current->isTag(curveTag)) {
        current->addTag(curveTag);
      }
      auto it = std::find(successfulPoints.begin(),
                          successfulPoints.end(),
                          result.back());
      successfulPoints.erase(it);
    }
    int otherIndex(0);
    if (i == 0) {
      otherIndex = 1;
    }
    else {
      OS_ASSERT(i == 1);
    }
    while (current) {
      OptionalOptimizationDataPoint candidate;
      if (current->isTag("explored") && current->isComplete() && !current->failed()) {
        // candidates have objective function at otherIndex < current's
        DoubleVector currentValues = current->objectiveValues();
        OptionalDouble candidateSlope;
        DoubleVector candidateValues;
        for (auto it = successfulPoints.begin();
             it != successfulPoints.end(); )
        {
          DoubleVector values = it->objectiveValues();
          if (lessThanOrEqual(values[otherIndex],currentValues[otherIndex])) {
            // take maximum slope as calculated on graph i vs. otherIndex
            double slope = (values[i] - currentValues[i])/
                           (values[otherIndex] - currentValues[otherIndex]);
            // infinite slope exception for start of algorithm
            bool infiniteSlopeException =
                current->isTag("iter0") &&
                equal(values[otherIndex],currentValues[otherIndex]) &&
                (values[i] < currentValues[i]);
            if (infiniteSlopeException && candidateSlope) {
              infiniteSlopeException = infiniteSlopeException &&
                  ((candidateValues[otherIndex] < values[otherIndex]) ||
                   (values[i] < candidateValues[i]));
            }
            if (infiniteSlopeException ||
                (!equal(values[otherIndex],currentValues[otherIndex]) &&
                 ((!candidateSlope) || (slope > *candidateSlope) ||
                  (equal(slope,*candidateSlope) && (values[i] < candidateValues[i])))))
            {
              candidate = *it;
              candidateSlope = slope;
              candidateValues = values;
              if (infiniteSlopeException) {
                candidateSlope = std::numeric_limits<double>::max();
              }
            }
            ++it;
          }
          else {
            // erase it from consideration--with other objective function > current,
            // will never be candidate.
            it = successfulPoints.erase(it);
          }
        }
      }
      if (candidate) {
        result.push_back(*candidate);
        if (!result.back().isTag(curveTag)) {
          result.back().addTag(curveTag);
        }
        auto it = std::find(successfulPoints.begin(),
                            successfulPoints.end(),
                            result.back());
        successfulPoints.erase(it);
      }
      current = candidate;
    }

    // remove outdated tags
    for (OptimizationDataPoint& point : lastCurve) {
      OptimizationDataPointVector::const_iterator it = std::find(result.begin(),result.end(),point);
      if (it == result.end()) {
        point.deleteTag(curveTag);
      }
    }
    if (!lastCurrent.empty()) {
      OS_ASSERT(lastCurrent.size() == 1u);
      if (lastCurrent[0] != result.back()) {
        lastCurrent[0].deleteTag("current");
      }
    }

    // add current tag
    if (!result.empty() && !result.back().isTag("current")) {
      result.back().addTag("current");
    }

    return result;
  }

  std::vector<OptimizationDataPoint> SequentialSearch_Impl::getParetoFront(
      Analysis& analysis) const
  {
    DataPointVector temp = analysis.getDataPoints("pareto");
    OptimizationDataPointVector lastParetoFront = castVector<OptimizationDataPoint>(temp);
    OptimizationDataPointVector result;
    temp = analysis.successfulDataPoints();
    OptimizationDataPointVector successfulPoints = castVector<OptimizationDataPoint>(temp);

    // sort by objective function options().objectiveToMinimizeFirst()
    int i = sequentialSearchOptions().objectiveToMinimizeFirst();
    int otherIndex(0);
    if (i == 0) {
      otherIndex = 1;
    }
    else {
      OS_ASSERT(i == 1);
    }
    OptimizationDataPointObjectiveFunctionLess predicate(i);
    std::sort(successfulPoints.begin(),successfulPoints.end(),predicate);

    // non-dominated means that you cannot improve one objective without harming the other
    auto it = successfulPoints.begin();
    DoubleVector currentValues;
    while (it != successfulPoints.end()) {
      // it has next-worst objective i
      DoubleVector candidateValues = it->objectiveValues();
      if (!currentValues.empty()) {
        OS_ASSERT(greaterThanOrEqual(candidateValues[i],currentValues[i]));
      }
      // is Pareto if improves objective otherIndex, and
      if (!currentValues.empty() &&
          greaterThanOrEqual(candidateValues[otherIndex],currentValues[otherIndex]))
      {
        it = successfulPoints.erase(it);
        continue;
      }
      // is Pareto if there is no other point with same objective i and better objective otherIndex
      OptimizationDataPointVector::const_iterator jt = it; ++jt;
      bool erasedIt(false);
      while (jt != successfulPoints.end()) {
        DoubleVector nextValues = jt->objectiveValues();
        if (equal(candidateValues[i],nextValues[i])) {
          if (nextValues[otherIndex] < candidateValues[otherIndex]) {
            it = successfulPoints.erase(it);
            erasedIt = true;
            break;
          }
          ++jt;
        }
        else {
          break;
        }
      }
      if (erasedIt) {
        continue;
      }
      result.push_back(*it);
      it = successfulPoints.erase(it);
      currentValues = result.back().objectiveValues();
      if (!result.back().isTag("pareto")) {
        result.back().addTag("pareto");
      }
    }

    // remove outdated tags
    for (OptimizationDataPoint& point : lastParetoFront) {
      OptimizationDataPointVector::const_iterator it = std::find(result.begin(),result.end(),point);
      if (it == result.end()) {
        point.deleteTag("pareto");
      }
    }

    return result;
  }

  std::vector< std::vector<QVariant> > SequentialSearch_Impl::getCandidateCombinations(
      const DataPoint& dataPoint) const
  {
    Problem problem = dataPoint.problem();
    std::vector<QVariant> currentValues = dataPoint.variableValues();
    std::vector< std::vector<QVariant> > result;
    for (int i = 0, n = problem.numVariables(); i < n; ++i) {
      DiscreteVariable variable = problem.getVariable(i).cast<DiscreteVariable>();
      // only use selected items
      int currentValue = currentValues[i].toInt();
      for (int j : variable.validValues(true)) {
        if (currentValue != j) {
          std::vector<QVariant> newValues = currentValues;
          newValues[i] = j;
          result.push_back(newValues);
        }
      }
    }
    return result;
  }

  QVariant SequentialSearch_Impl::toVariant() const {
    QVariantMap map = Algorithm_Impl::toVariant().toMap();

    map["algorithm_type"] = QString("SequentialSearch");

    return QVariant(map);
  }

  SequentialSearch SequentialSearch_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();
    SequentialSearchOptions options = SequentialSearchOptions_Impl::fromVariant(map["options"],version);
    return SequentialSearch(toUUID(map["uuid"].toString().toStdString()),
                            toUUID(map["version_uuid"].toString().toStdString()),
                            map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                            map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                            map["complete"].toBool(),
                            map["failed"].toBool(),
                            map["iter"].toInt(),
                            options);
  }

} // detail

SequentialSearch::SequentialSearch(const SequentialSearchOptions& options)
  : OpenStudioAlgorithm(std::shared_ptr<detail::SequentialSearch_Impl>(
        new detail::SequentialSearch_Impl(options)))
{
  createCallbackForOptions();
}

SequentialSearch::SequentialSearch(const UUID& uuid,
                                   const UUID& versionUUID,
                                   const std::string& displayName,
                                   const std::string& description,
                                   bool complete,
                                   bool failed,
                                   int iter,
                                   const SequentialSearchOptions& options)
  : OpenStudioAlgorithm(std::shared_ptr<detail::SequentialSearch_Impl>(
        new detail::SequentialSearch_Impl(uuid,
                                          versionUUID,
                                          displayName,
                                          description,
                                          complete,
                                          failed,
                                          iter,
                                          options)))
{
  createCallbackForOptions();
}

std::string SequentialSearch::standardName() {
  return std::string("Sequential Search");
}

SequentialSearchOptions SequentialSearch::sequentialSearchOptions() const {
  return getImpl<detail::SequentialSearch_Impl>()->sequentialSearchOptions();
}

std::vector<OptimizationDataPoint> SequentialSearch::getMinimumCurve(
    unsigned i, Analysis& analysis) const
{
  return getImpl<detail::SequentialSearch_Impl>()->getMinimumCurve(i,analysis);
}

std::vector<OptimizationDataPoint> SequentialSearch::getParetoFront(
    Analysis& analysis) const
{
  return getImpl<detail::SequentialSearch_Impl>()->getParetoFront(analysis);
}

std::vector< std::vector<QVariant> > SequentialSearch::getCandidateCombinations(
    const DataPoint& dataPoint) const
{
  return getImpl<detail::SequentialSearch_Impl>()->getCandidateCombinations(dataPoint);
}

/// @cond
SequentialSearch::SequentialSearch(std::shared_ptr<detail::SequentialSearch_Impl> impl)
  : OpenStudioAlgorithm(impl)
{}
/// @endcond

} // analysis
} // openstudio

