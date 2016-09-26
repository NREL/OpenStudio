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

#include "ParameterStudyAlgorithm.hpp"
#include "ParameterStudyAlgorithm_Impl.hpp"

#include "ParameterStudyAlgorithmOptions.hpp"
#include "ParameterStudyAlgorithmOptions_Impl.hpp"

#include "Problem.hpp"

#include "../runmanager/lib/JSON.hpp"

#include "../utilities/core/Optional.hpp"
#include "../utilities/core/String.hpp"

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
    std::shared_ptr<ParameterStudyAlgorithm_Impl> impl(new ParameterStudyAlgorithm_Impl(*this));
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
        for (const auto & elem : finalPoint) {
          ss << toString(elem) << " ";
        }
        ss << std::endl;
      }
      else if (!options.stepVector().empty()) {
        ss << "          step_vector = ";
        std::vector<double> stepVector = options.stepVector();
        for (const auto & elem : stepVector) {
          ss << toString(elem) << " ";
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
        for (const auto & elem : listOfPoints) {
          ss << toString(elem) << " ";
        }
        ss << std::endl;
      }
      else {
        LOG(Warn,"The listOfPoints vector is empty.");
      }
    }
    else if (options.algorithmType() == ParameterStudyAlgorithmType::centered_parameter_study) {
      if (!options.stepVector().empty()) {
        ss << "          step_vector = ";
        std::vector<double> stepVector = options.stepVector();
        for (const auto & elem : stepVector) {
          ss << toString(elem) << " ";
        }
        ss << std::endl;
        ss << "          steps_per_variable = ";
        std::vector<int> stepsPerVariable = options.stepsPerVariable();
        for (const auto & elem : stepsPerVariable) {
          ss << elem << " ";
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
        for (const auto & elem : partitions) {
          ss << elem << " ";
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
  : DakotaAlgorithm(std::shared_ptr<detail::ParameterStudyAlgorithm_Impl>(
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
  : DakotaAlgorithm(std::shared_ptr<detail::ParameterStudyAlgorithm_Impl>(
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
ParameterStudyAlgorithm::ParameterStudyAlgorithm(std::shared_ptr<detail::ParameterStudyAlgorithm_Impl> impl)
  : DakotaAlgorithm(impl)
{}
/// @endcond

} // analysis
} // openstudio

