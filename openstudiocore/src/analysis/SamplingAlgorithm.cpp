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

#include "SamplingAlgorithm.hpp"
#include "SamplingAlgorithm_Impl.hpp"

#include "SamplingAlgorithmOptions.hpp"
#include "SamplingAlgorithmOptions_Impl.hpp"

#include "Problem.hpp"

#include "../runmanager/lib/JSON.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  SamplingAlgorithm_Impl::SamplingAlgorithm_Impl(const SamplingAlgorithmOptions& options)
    : DakotaAlgorithm_Impl("Sampling",options)
  {}

  SamplingAlgorithm_Impl::SamplingAlgorithm_Impl(const UUID& uuid,
                                               const UUID& versionUUID,
                                               const std::string& displayName,
                                               const std::string& description,
                                               bool complete,
                                               bool failed,
                                               int iter,
                                               const SamplingAlgorithmOptions& options,
                                               const boost::optional<FileReference>& restartFileReference,
                                               const boost::optional<FileReference>& outFileReference, 
                                               const boost::optional<openstudio::runmanager::Job>& job)
    : DakotaAlgorithm_Impl(uuid,
                           versionUUID,
                           "Sampling",
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

  SamplingAlgorithm_Impl::SamplingAlgorithm_Impl(const SamplingAlgorithm_Impl &other)
    : DakotaAlgorithm_Impl(other)
  {}

  AnalysisObject SamplingAlgorithm_Impl::clone() const {
    std::shared_ptr<SamplingAlgorithm_Impl> impl(new SamplingAlgorithm_Impl(*this));
    return SamplingAlgorithm(impl);
  }

  bool SamplingAlgorithm_Impl::isCompatibleProblemType(const Problem& problem) const {
    return true;  
  }

  bool SamplingAlgorithm_Impl::requiresContinuousVariables() const {
    return false;
  }  

  bool SamplingAlgorithm_Impl::isCompatible(const UncertaintyDescriptionType& udescType) const {
    return true;
  }

  std::string SamplingAlgorithm_Impl::dakotaInFileDescription() const {

    SamplingAlgorithmOptions options = samplingAlgorithmOptions();
    
    std::stringstream ss;
    ss << "method," << std::endl;
    ss << "        sampling" << std::endl;
    if (boost::optional<SamplingAlgorithmSampleType> sampleType = options.sampleType()) {
      ss << "          sample_type = " << (*sampleType).valueName() << std::endl;
    }
    if (options.allVariables()) {
      ss << "          all_variables" << std::endl;
    }
    if (OptionalInt seed = options.seed()) {
      ss << "          seed = " << *seed << std::endl;
    }
    if (options.fixedSeed()) {
      ss << "          fixed_seed" << std::endl;
    }
    if (boost::optional<SamplingAlgorithmRNGType> rngType = options.rngType()) {
      ss << "          rng = " << (*rngType).valueName() << std::endl;
    }
    if (options.varianceBasedDecomp()) {
      ss << "          variance_based_decomp" << std::endl;
    }
    if (options.dropTolerance()) {
      ss << "          drop_tolerance" << std::endl;
    }
    ss << "          samples = " << options.samples() << std::endl;
    ss << std::endl;

    return ss.str();
  }

  SamplingAlgorithmOptions SamplingAlgorithm_Impl::samplingAlgorithmOptions() const {
    return options().cast<SamplingAlgorithmOptions>();
  }

  QVariant SamplingAlgorithm_Impl::toVariant() const {
    QVariantMap map = DakotaAlgorithm_Impl::toVariant().toMap();

    map["algorithm_type"] = QString("SamplingAlgorithm");

    return QVariant(map);
  }

  SamplingAlgorithm SamplingAlgorithm_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();
    SamplingAlgorithmOptions options = SamplingAlgorithmOptions_Impl::fromVariant(map["options"],version);
    return SamplingAlgorithm(toUUID(map["uuid"].toString().toStdString()),
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

SamplingAlgorithm::SamplingAlgorithm(const SamplingAlgorithmOptions& options)
  : DakotaAlgorithm(std::shared_ptr<detail::SamplingAlgorithm_Impl>(
        new detail::SamplingAlgorithm_Impl(options)))
{
  createCallbackForOptions();
}

SamplingAlgorithm::SamplingAlgorithm(const UUID& uuid,
                                   const UUID& versionUUID,
                                   const std::string& displayName,
                                   const std::string& description,
                                   bool complete,
                                   bool failed,
                                   int iter,
                                   const SamplingAlgorithmOptions& options,
                                   const boost::optional<FileReference>& restartFileReference,
                                   const boost::optional<FileReference>& outFileReference,
                                   const boost::optional<openstudio::runmanager::Job>& job)
  : DakotaAlgorithm(std::shared_ptr<detail::SamplingAlgorithm_Impl>(
        new detail::SamplingAlgorithm_Impl(uuid,
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

SamplingAlgorithmOptions SamplingAlgorithm::samplingAlgorithmOptions() const {
  return getImpl<detail::SamplingAlgorithm_Impl>()->samplingAlgorithmOptions();
}

/// @cond
SamplingAlgorithm::SamplingAlgorithm(std::shared_ptr<detail::SamplingAlgorithm_Impl> impl)
  : DakotaAlgorithm(impl)
{}
/// @endcond

} // analysis
} // openstudio

