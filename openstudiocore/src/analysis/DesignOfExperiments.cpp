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

#include "DesignOfExperiments.hpp"
#include "DesignOfExperiments_Impl.hpp"

#include "DesignOfExperimentsOptions.hpp"
#include "DesignOfExperimentsOptions_Impl.hpp"

#include "Analysis.hpp"
#include "Problem.hpp"
#include "DataPoint.hpp"
#include "DiscreteVariable.hpp"
#include "DiscreteVariable_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Containers.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  DesignOfExperiments_Impl::DesignOfExperiments_Impl(const DesignOfExperimentsOptions& options)
    : OpenStudioAlgorithm_Impl(DesignOfExperiments::standardName(),options)
  {}

  DesignOfExperiments_Impl::DesignOfExperiments_Impl(const UUID& uuid, 
                                                     const UUID& versionUUID,
                                                     const std::string& displayName,
                                                     const std::string& description,
                                                     bool complete,
                                                     bool failed,
                                                     int iter,
                                                     const DesignOfExperimentsOptions& options)
    : OpenStudioAlgorithm_Impl(uuid,
                               versionUUID,
                               DesignOfExperiments::standardName(),
                               displayName,
                               description,
                               complete,
                               failed,
                               iter,
                               options)
  {}

  DesignOfExperiments_Impl::DesignOfExperiments_Impl(const DesignOfExperiments_Impl& other)
    : OpenStudioAlgorithm_Impl(other)
  {}

  AnalysisObject DesignOfExperiments_Impl::clone() const {
    std::shared_ptr<DesignOfExperiments_Impl> impl(new DesignOfExperiments_Impl(*this));
    return DesignOfExperiments(impl);
  }

  bool DesignOfExperiments_Impl::isCompatibleProblemType(const Problem& problem) const {
    if (!problem.allVariablesAreDiscrete()) {
      LOG(Info,"DesignOfExperiments only operates on Problems composed of DiscreteVariables.");
      return false;
    }
    return true;
  }
  
  int DesignOfExperiments_Impl::createNextIteration(Analysis& analysis) {
    int result(0);

    // to make sure problem type check has already occurred. this is stated usage in header.
    OS_ASSERT(analysis.algorithm().get() == getPublicObject<DesignOfExperiments>());
    // nothing else is supported yet
    DesignOfExperimentsOptions options = designOfExperimentsOptions();
    OS_ASSERT(options.designType() == DesignOfExperimentsType::FullFactorial);

    if (isComplete()) {
      LOG(Info,"Algorithm is already marked as complete. Returning without creating new points.");
      return result;
    }

    if (options.maxIter() && options.maxIter().get() < 1) {
      LOG(Info,"Maximum iterations set to less than one. No DataPoints will be added to Analysis '"
          << analysis.name() << "', and the Algorithm will be marked complete.");
      markComplete();
      return result;
    }

    OptionalInt mxSim = options.maxSims();
    DataPointVector dataPoints = analysis.getDataPoints("DOE");
    int totPoints = dataPoints.size();
    if (mxSim && (totPoints >= *mxSim)) {
      LOG(Info,"Analysis '" << analysis.name() << "' already contains " << totPoints
          << " DataPoints added by the DesignOfExperiments algorithm, which meets or exceeds the "
          << "maximum number specified in this algorithm's options object, " << *mxSim << ". "
          << "No data points will be added and the Algorithm will be marked complete.");
      markComplete();
      return result;
    } 

    m_iter = 1;

    // determine all combinations
    std::vector< std::vector<QVariant> > variableValues;
    for (const Variable& variable : analysis.problem().variables()) {
      // variable must be DiscreteVariable, otherwise !isCompatibleProblemType(analysis.problem())
      DiscreteVariable discreteVariable = variable.cast<DiscreteVariable>();
      IntVector dvValues = discreteVariable.validValues(true);
      std::vector< std::vector<QVariant> > currentValues = variableValues;
      for (IntVector::const_iterator it = dvValues.begin(), itEnd = dvValues.end();
           it != itEnd; ++it)
      {
        std::vector< std::vector<QVariant> > nextSet = currentValues;
        if (currentValues.empty()) {
          variableValues.push_back(std::vector<QVariant>(1u,QVariant(*it)));
        }
        else {
          for (std::vector<QVariant>& point : nextSet) {
            point.push_back(QVariant(*it));
          }
          if (it == dvValues.begin()) {
            variableValues = nextSet;
          }
          else {
            variableValues.insert(variableValues.end(),nextSet.begin(),nextSet.end());
          }
        }
      }
    }

    // create data points and add to analysis
    for (const std::vector<QVariant>& value : variableValues) {
      DataPoint dataPoint = analysis.problem().createDataPoint(value).get();
      dataPoint.addTag("DOE");
      bool added = analysis.addDataPoint(dataPoint);
      if (added) {
        ++result;
        ++totPoints;
        if (mxSim && (totPoints == mxSim.get())) {
          break;
        }
      }
    }

    if (result == 0) {
      LOG(Trace,"No new points were added, so marking this DesignOfExperiments complete.");
      markComplete();
    }

    return result;
  }

  DesignOfExperimentsOptions DesignOfExperiments_Impl::designOfExperimentsOptions() const {
    return m_options.cast<DesignOfExperimentsOptions>();
  }

  QVariant DesignOfExperiments_Impl::toVariant() const {
    QVariantMap map = Algorithm_Impl::toVariant().toMap();

    map["algorithm_type"] = QString("DesignOfExperiments");

    return QVariant(map);
  }

  DesignOfExperiments DesignOfExperiments_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();
    DesignOfExperimentsOptions options = DesignOfExperimentsOptions_Impl::fromVariant(map["options"],version);
    return DesignOfExperiments(toUUID(map["uuid"].toString().toStdString()),
                               toUUID(map["version_uuid"].toString().toStdString()),
                               map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                               map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                               map["complete"].toBool(),
                               map["failed"].toBool(),
                               map["iter"].toInt(),
                               options);
  }

} // detail

DesignOfExperiments::DesignOfExperiments(const DesignOfExperimentsOptions& options)
  : OpenStudioAlgorithm(std::shared_ptr<detail::DesignOfExperiments_Impl>(
        new detail::DesignOfExperiments_Impl(options)))
{
  createCallbackForOptions();
}

DesignOfExperiments::DesignOfExperiments(const UUID& uuid, 
                                         const UUID& versionUUID,
                                         const std::string& displayName,
                                         const std::string& description,
                                         bool complete,
                                         bool failed,
                                         int iter,
                                         const DesignOfExperimentsOptions& options)
  : OpenStudioAlgorithm(std::shared_ptr<detail::DesignOfExperiments_Impl>(
        new detail::DesignOfExperiments_Impl(uuid,
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

std::string DesignOfExperiments::standardName() {
  return std::string("Design of Experiments");
}

DesignOfExperimentsOptions DesignOfExperiments::designOfExperimentsOptions() const {
  return getImpl<detail::DesignOfExperiments_Impl>()->designOfExperimentsOptions();
}

/// @cond
DesignOfExperiments::DesignOfExperiments(std::shared_ptr<detail::DesignOfExperiments_Impl> impl)
  : OpenStudioAlgorithm(impl)
{}
/// @endcond

} // analysis
} // openstudio

