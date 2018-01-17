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

#include "FSUDaceAlgorithm.hpp"
#include "FSUDaceAlgorithm_Impl.hpp"

#include "FSUDaceAlgorithmOptions.hpp"
#include "FSUDaceAlgorithmOptions_Impl.hpp"

#include "Problem.hpp"

#include "../runmanager/lib/JSON.hpp"

#include "../utilities/core/Optional.hpp"
#include "../utilities/core/FileReference.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  FSUDaceAlgorithm_Impl::FSUDaceAlgorithm_Impl(const FSUDaceAlgorithmOptions& options)
    : DakotaAlgorithm_Impl("FSUDace",options)
  {}

  FSUDaceAlgorithm_Impl::FSUDaceAlgorithm_Impl(const UUID& uuid,
                                               const UUID& versionUUID,
                                               const std::string& displayName,
                                               const std::string& description,
                                               bool complete,
                                               bool failed,
                                               int iter,
                                               const FSUDaceAlgorithmOptions& options,
                                               const boost::optional<FileReference>& restartFileReference,
                                               const boost::optional<FileReference>& outFileReference,
                                               const boost::optional<openstudio::runmanager::Job>& job)
    : DakotaAlgorithm_Impl(uuid,
                           versionUUID,
                           "FSUDace",
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

  FSUDaceAlgorithm_Impl::FSUDaceAlgorithm_Impl(const FSUDaceAlgorithm_Impl &other)
    : DakotaAlgorithm_Impl(other)
  {}

  AnalysisObject FSUDaceAlgorithm_Impl::clone() const {
    std::shared_ptr<FSUDaceAlgorithm_Impl> impl(new FSUDaceAlgorithm_Impl(*this));
    return FSUDaceAlgorithm(impl);
  }

  bool FSUDaceAlgorithm_Impl::isCompatibleProblemType(const Problem& problem) const {
    return true;  
  }

  bool FSUDaceAlgorithm_Impl::requiresContinuousVariables() const {
    return true;
  }  

  std::string FSUDaceAlgorithm_Impl::dakotaInFileDescription() const {

    FSUDaceAlgorithmOptions options = fsudaceAlgorithmOptions();
    
    std::stringstream ss;
    ss << "method," << std::endl;
    //This if statement is grouped so that only the options relevant to quasi_mc are printed will
    //be printed if halton or hammersley is selected, and will only print options relevant to cvt
    //when cvt is selected. Options outside the if statement are valid for any algorithm type.
    if (options.algorithmType() != FSUDaceAlgorithmType::cvt) {
      ss << "        fsu_quasi_mc " << options.algorithmType().valueName() << std::endl;
      if (!options.sequenceStart().empty()) {
        ss << "          sequence_start = ";
        std::vector<int> sequenceStart = options.sequenceStart();
        for (const auto & elem : sequenceStart) {
          ss << elem << " ";
        }
        ss << std::endl;
      }
      if (!options.sequenceLeap().empty()) {
        ss << "          sequence_leap = ";
        std::vector<int> sequenceLeap = options.sequenceLeap();
        for (const auto & elem : sequenceLeap) {
          ss << elem << " ";
        }
        ss << std::endl;
      }
      if (options.fixedSequence()) {
        ss << "          fixed_sequence" << std::endl;
      }
    }
    else { ss << "        fsu_cvt" << std::endl;
      if (OptionalInt seed = options.seed()) {
        ss << "          seed = " << *seed << std::endl;
      }
      if (options.fixedSeed()) {
        ss << "          fixed_seed" << std::endl;
      }
      if (OptionalInt numTrials = options.numTrials()) {
        ss << "          num_trials = " << *numTrials << std::endl;
      }
      if (boost::optional<FSUDaceCvtTrialType> trialType = options.trialType()) {
        ss << "          trial_type = " << (*trialType).valueName() << std::endl;
      }
    }
    if (options.latinize()) {
      ss << "          latinize" << std::endl;
    }
    if (options.varianceBasedDecomp()) {
      ss << "          variance_based_decomp" << std::endl;
    }
    if (options.qualityMetrics()) {
      ss << "          quality_metrics" << std::endl;
    }
    ss << "          samples = " << options.samples() << std::endl;
    ss << std::endl;

    return ss.str();
  }

  FSUDaceAlgorithmOptions FSUDaceAlgorithm_Impl::fsudaceAlgorithmOptions() const {
    return options().cast<FSUDaceAlgorithmOptions>();
  }

  QVariant FSUDaceAlgorithm_Impl::toVariant() const {
    QVariantMap map = DakotaAlgorithm_Impl::toVariant().toMap();

    map["algorithm_type"] = QString("FSUDaceAlgorithm");

    return QVariant(map);
  }

  FSUDaceAlgorithm FSUDaceAlgorithm_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();
    FSUDaceAlgorithmOptions options = FSUDaceAlgorithmOptions_Impl::fromVariant(map["options"],version);
    return FSUDaceAlgorithm(toUUID(map["uuid"].toString().toStdString()),
                            toUUID(map["version_uuid"].toString().toStdString()),
                            map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                            map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                            map["complete"].toBool(),
                            map["failed"].toBool(),
                            map["iter"].toInt(),
                            options,
                            map.contains("restart_file_reference") ? openstudio::detail::toFileReference(map["restart_file_reference"],version) : OptionalFileReference(),
                            map.contains("out_file_reference") ? openstudio::detail::toFileReference(map["out_file_reference"],version) : OptionalFileReference(),
                            map.contains("job") ? runmanager::detail::JSON::toJob(map["job"],version, true) : boost::optional<runmanager::Job>());
  }

} // detail

FSUDaceAlgorithm::FSUDaceAlgorithm(const FSUDaceAlgorithmOptions& options)
  : DakotaAlgorithm(std::shared_ptr<detail::FSUDaceAlgorithm_Impl>(
        new detail::FSUDaceAlgorithm_Impl(options)))
{
  createCallbackForOptions();
}

FSUDaceAlgorithm::FSUDaceAlgorithm(const UUID& uuid,
                                   const UUID& versionUUID,
                                   const std::string& displayName,
                                   const std::string& description,
                                   bool complete,
                                   bool failed,
                                   int iter,
                                   const FSUDaceAlgorithmOptions& options,
                                   const boost::optional<FileReference>& restartFileReference,
                                   const boost::optional<FileReference>& outFileReference,
                                   const boost::optional<openstudio::runmanager::Job>& job)
  : DakotaAlgorithm(std::shared_ptr<detail::FSUDaceAlgorithm_Impl>(
        new detail::FSUDaceAlgorithm_Impl(uuid,
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

FSUDaceAlgorithmOptions FSUDaceAlgorithm::fsudaceAlgorithmOptions() const {
  return getImpl<detail::FSUDaceAlgorithm_Impl>()->fsudaceAlgorithmOptions();
}

/// @cond
FSUDaceAlgorithm::FSUDaceAlgorithm(std::shared_ptr<detail::FSUDaceAlgorithm_Impl> impl)
  : DakotaAlgorithm(impl)
{}
/// @endcond

} // analysis
} // openstudio

