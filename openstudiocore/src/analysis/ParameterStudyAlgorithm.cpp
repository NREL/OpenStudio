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

#include <analysis/ParameterStudyAlgorithm.hpp>
#include <analysis/ParameterStudyAlgorithm_Impl.hpp>

#include <analysis/ParameterStudyAlgorithmOptions.hpp>
#include <analysis/ParameterStudyAlgorithmOptions_Impl.hpp>

#include <analysis/Problem.hpp>

#include <runmanager/lib/JSON.hpp>

#include <utilities/core/Optional.hpp>
#include <utilities/core/String.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  ParameterStudyAlgorithm_Impl::ParameterStudyAlgorithm_Impl(const ParameterStudyAlgorithmOptions& options)
    : DakotaAlgorithm_Impl("ParameterStudy",options)
  {}

  ParameterStudyAlgorithm_Impl::ParameterStudyAlgorithm_Impl(const UUID& uuid,
                                                             const UUID& versionUUID,
                                                             const std::string& displayName,
                                                             const std::string& description,
                                                             bool complete,
                                                             bool failed,
                                                             int iter,
                                                             const ParameterStudyAlgorithmOptions& options,
                                                             const boost::optional<FileReference>& restartFileReference,
                                                             const boost::optional<FileReference>& outFileReference, 
                                                             const boost::optional<openstudio::runmanager::Job>& job)
    : DakotaAlgorithm_Impl(uuid,
                           versionUUID,
                           "ParameterStudy",
                           displayName,
                           description,
                           complete,
                           failed,
                           iter,
                           options,
                           restartFileReference,
                           outFileReference, 
                           job)
  {}

  ParameterStudyAlgorithm_Impl::ParameterStudyAlgorithm_Impl(const ParameterStudyAlgorithm_Impl &other)
    : DakotaAlgorithm_Impl(other)
  {}

  AnalysisObject ParameterStudyAlgorithm_Impl::clone() const {
    boost::shared_ptr<ParameterStudyAlgorithm_Impl> impl(new ParameterStudyAlgorithm_Impl(*this));
    return ParameterStudyAlgorithm(impl);
  }

  bool ParameterStudyAlgorithm_Impl::isCompatibleProblemType(const Problem& problem) const {
    return true;  
  }

  bool ParameterStudyAlgorithm_Impl::requiresContinuousVariables() const {
    return true;
  }

  /** Every ParameterStudyAlgorithmType has at least one required input. If one of the required
   *  inputs is not specified, dakotaInFileDescription() will not write out that option in the .in
   *  file, and will log a warning. */
  std::string ParameterStudyAlgorithm_Impl::dakotaInFileDescription() const {

    ParameterStudyAlgorithmOptions options = parameterStudyAlgorithmOptions();
    
    std::stringstream ss;
    ss << "method," << std::endl;
    ss << "        " << options.algorithmType().valueName() << std::endl;
    if (options.algorithmType() == ParameterStudyAlgorithmType::vector_parameter_study) {
      if (!options.finalPoint().empty()) {
        ss << "          final_point = ";
        std::vector<double> finalPoint = options.finalPoint();
        for (std::vector<double>::iterator i = finalPoint.begin(); i != finalPoint.end(); ++i) {
          ss << toString(*i) << " ";
        }
        ss << std::endl;
      }
      else if (!options.stepVector().empty()) {
        ss << "          step_vector = ";
        std::vector<double> stepVector = options.stepVector();
        for (std::vector<double>::iterator i = stepVector.begin(); i != stepVector.end(); ++i) {
          ss << toString(*i) << " ";
        }
        ss << std::endl;
      }
      else {
        LOG(Warn,"Both the finalPoint vector and the stepVector are empty.");
      }
      ss << "          num_steps = " << *(options.numSteps()) << std::endl;
    }
    else if (options.algorithmType() == ParameterStudyAlgorithmType::list_parameter_study) {
      if (!options.listOfPoints().empty()) {
        ss << "          list_of_points = ";
        std::vector<double> listOfPoints = options.listOfPoints();
        for (std::vector<double>::iterator i = listOfPoints.begin(); i != listOfPoints.end(); ++i) {
          ss << toString(*i) << " ";
        }
        ss << std::endl;
      }
      else {
        LOG(Warn,"The listOfPoints vector is empty.");
      }
    }
    else if (options.algorithmType() == ParameterStudyAlgorithmType::centered_parameter_study) {
      if (!options.stepVector().empty() && !options.stepVector().empty()) {
        ss << "          step_vector = ";
        std::vector<double> stepVector = options.stepVector();
        for (std::vector<double>::iterator i = stepVector.begin(); i != stepVector.end(); ++i) {
          ss << toString(*i) << " ";
        }
        ss << std::endl;
        ss << "          steps_per_variable = ";
        std::vector<int> stepsPerVariable = options.stepsPerVariable();
        for (std::vector<int>::iterator i = stepsPerVariable.begin(); i != stepsPerVariable.end(); ++i) {
          ss << *i << " ";
        }
        ss << std::endl;
      }
      else {
        LOG(Warn,"The stepVector and/or the stepsPerVariable vector are/is empty.");
      }
    }
    else {
      if (!options.partitions().empty()) {
        ss << "          partitions = ";
        std::vector<int> partitions = options.partitions();
        for (std::vector<int>::iterator i = partitions.begin(); i != partitions.end(); ++i) {
          ss << *i << " ";
        }
        ss << std::endl;
      }
      else {
        LOG(Warn,"The partitions vector is empty.");
      }
    }
    ss << std::endl;

    return ss.str();
  }

  ParameterStudyAlgorithmOptions ParameterStudyAlgorithm_Impl::parameterStudyAlgorithmOptions() const {
    return options().cast<ParameterStudyAlgorithmOptions>();
  }

  QVariant ParameterStudyAlgorithm_Impl::toVariant() const {
    QVariantMap map = DakotaAlgorithm_Impl::toVariant().toMap();

    map["algorithm_type"] = QString("ParameterStudyAlgorithm");

    return QVariant(map);
  }

  ParameterStudyAlgorithm ParameterStudyAlgorithm_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();
    ParameterStudyAlgorithmOptions options = ParameterStudyAlgorithmOptions_Impl::fromVariant(map["options"],version);
    return ParameterStudyAlgorithm(toUUID(map["uuid"].toString().toStdString()),
                                   toUUID(map["version_uuid"].toString().toStdString()),
                                   map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                                   map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                                   map["complete"].toBool(),
                                   map["failed"].toBool(),
                                   map["iter"].toInt(),
                                   options,
                                   map.contains("restart_file_reference") ? openstudio::detail::toFileReference(map["restart_file_reference"],version) : OptionalFileReference(),
                                   map.contains("out_file_reference") ? openstudio::detail::toFileReference(map["out_file_reference"],version) : OptionalFileReference(),
                                   map.contains("job") ? runmanager::detail::JSON::toJob(variant,version, true) : boost::optional<runmanager::Job>());
  }

} // detail

ParameterStudyAlgorithm::ParameterStudyAlgorithm(const ParameterStudyAlgorithmOptions& options)
  : DakotaAlgorithm(boost::shared_ptr<detail::ParameterStudyAlgorithm_Impl>(
        new detail::ParameterStudyAlgorithm_Impl(options)))
{
  createCallbackForOptions();
}

ParameterStudyAlgorithm::ParameterStudyAlgorithm(const UUID& uuid,
                                                 const UUID& versionUUID,
                                                 const std::string& displayName,
                                                 const std::string& description,
                                                 bool complete,
                                                 bool failed,
                                                 int iter,
                                                 const ParameterStudyAlgorithmOptions& options,
                                                 const boost::optional<FileReference>& restartFileReference,
                                                 const boost::optional<FileReference>& outFileReference,
                                                 const boost::optional<openstudio::runmanager::Job>& job)
  : DakotaAlgorithm(boost::shared_ptr<detail::ParameterStudyAlgorithm_Impl>(
        new detail::ParameterStudyAlgorithm_Impl(uuid,
                                                 versionUUID,
                                                 displayName,
                                                 description,
                                                 complete,
                                                 failed,
                                                 iter,
                                                 options,
                                                 restartFileReference,
                                                 outFileReference,
                                                 job)))
{
  createCallbackForOptions();
}

ParameterStudyAlgorithmOptions ParameterStudyAlgorithm::parameterStudyAlgorithmOptions() const {
  return getImpl<detail::ParameterStudyAlgorithm_Impl>()->parameterStudyAlgorithmOptions();
}

/// @cond
ParameterStudyAlgorithm::ParameterStudyAlgorithm(boost::shared_ptr<detail::ParameterStudyAlgorithm_Impl> impl)
  : DakotaAlgorithm(impl)
{}
/// @endcond

} // analysis
} // openstudio

