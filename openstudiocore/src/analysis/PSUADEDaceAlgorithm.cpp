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

#include "PSUADEDaceAlgorithm.hpp"
#include "PSUADEDaceAlgorithm_Impl.hpp"

#include "PSUADEDaceAlgorithmOptions.hpp"
#include "PSUADEDaceAlgorithmOptions_Impl.hpp"

#include "Problem.hpp"

#include "../runmanager/lib/JSON.hpp"

#include "../utilities/core/Json.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  PSUADEDaceAlgorithm_Impl::PSUADEDaceAlgorithm_Impl(const PSUADEDaceAlgorithmOptions& options)
    : DakotaAlgorithm_Impl("PSUADEDace",options)
  {}

  PSUADEDaceAlgorithm_Impl::PSUADEDaceAlgorithm_Impl(const UUID& uuid,
                                                     const UUID& versionUUID,
                                                     const std::string& displayName,
                                                     const std::string& description,
                                                     bool complete,
                                                     bool failed,
                                                     int iter,
                                                     const PSUADEDaceAlgorithmOptions& options,
                                                     const boost::optional<FileReference>& restartFileReference,
                                                     const boost::optional<FileReference>& outFileReference, 
                                                     const boost::optional<openstudio::runmanager::Job>& job)
    : DakotaAlgorithm_Impl(uuid,
                           versionUUID,
                           "PSUADEDace",
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

  PSUADEDaceAlgorithm_Impl::PSUADEDaceAlgorithm_Impl(const PSUADEDaceAlgorithm_Impl &other)
    : DakotaAlgorithm_Impl(other)
  {}

  AnalysisObject PSUADEDaceAlgorithm_Impl::clone() const {
    std::shared_ptr<PSUADEDaceAlgorithm_Impl> impl(new PSUADEDaceAlgorithm_Impl(*this));
    return PSUADEDaceAlgorithm(impl);
  }

  bool PSUADEDaceAlgorithm_Impl::isCompatibleProblemType(const Problem& problem) const {
    return true;  
  }

  bool PSUADEDaceAlgorithm_Impl::requiresContinuousVariables() const {
    return true;
  }  

  std::string PSUADEDaceAlgorithm_Impl::dakotaInFileDescription() const {

    PSUADEDaceAlgorithmOptions options = psuadedaceAlgorithmOptions();

    std::stringstream ss;
    ss << "method," << std::endl;
    ss << "        psuade_moat " << std::endl;
    if (OptionalInt seed = options.seed()) {
      ss << "          seed = " << *seed << std::endl;
    }
    if (OptionalInt samples = options.samples()) {
      ss << "          samples = " << *samples << std::endl;
    }
    if (OptionalInt partitions = options.partitions()) {
      ss << "          partitions = " << *partitions << std::endl;
    }
    ss << std::endl;

    return ss.str();
  }

  PSUADEDaceAlgorithmOptions PSUADEDaceAlgorithm_Impl::psuadedaceAlgorithmOptions() const {
    return options().cast<PSUADEDaceAlgorithmOptions>();
  }

  QVariant PSUADEDaceAlgorithm_Impl::toVariant() const {
    QVariantMap map = DakotaAlgorithm_Impl::toVariant().toMap();

    map["algorithm_type"] = QString("PSUADEDaceAlgorithm");

    return QVariant(map);
  }

  PSUADEDaceAlgorithm PSUADEDaceAlgorithm_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();
    PSUADEDaceAlgorithmOptions options = PSUADEDaceAlgorithmOptions_Impl::fromVariant(map["options"],version);
    return PSUADEDaceAlgorithm(toUUID(map["uuid"].toString().toStdString()),
                               toUUID(map["version_uuid"].toString().toStdString()),
                               map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                               map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                               map["complete"].toBool(),
                               map["failed"].toBool(),
                               map["iter"].toInt(),
                               options,
                               map.contains("restart_file_reference") ? openstudio::detail::toFileReference(map["restart_file_reference"],version) : OptionalFileReference(),
                               map.contains("out_file_reference") ? openstudio::detail::toFileReference(map["out_file_reference"],version) : OptionalFileReference(),
                               map.contains("job") ? runmanager::detail::JSON::toJob(variant,version,true) : boost::optional<runmanager::Job>());
  }

} // detail

PSUADEDaceAlgorithm::PSUADEDaceAlgorithm(const PSUADEDaceAlgorithmOptions& options)
  : DakotaAlgorithm(std::shared_ptr<detail::PSUADEDaceAlgorithm_Impl>(
        new detail::PSUADEDaceAlgorithm_Impl(options)))
{
  createCallbackForOptions();
}

PSUADEDaceAlgorithm::PSUADEDaceAlgorithm(const UUID& uuid,
                                         const UUID& versionUUID,
                                         const std::string& displayName,
                                         const std::string& description,
                                         bool complete,
                                         bool failed,
                                         int iter,
                                         const PSUADEDaceAlgorithmOptions& options,
                                         const boost::optional<FileReference>& restartFileReference,
                                         const boost::optional<FileReference>& outFileReference,
                                         const boost::optional<openstudio::runmanager::Job>& job)
  : DakotaAlgorithm(std::shared_ptr<detail::PSUADEDaceAlgorithm_Impl>(
        new detail::PSUADEDaceAlgorithm_Impl(uuid,
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

PSUADEDaceAlgorithmOptions PSUADEDaceAlgorithm::psuadedaceAlgorithmOptions() const {
  return getImpl<detail::PSUADEDaceAlgorithm_Impl>()->psuadedaceAlgorithmOptions();
}

/// @cond
PSUADEDaceAlgorithm::PSUADEDaceAlgorithm(std::shared_ptr<detail::PSUADEDaceAlgorithm_Impl> impl)
  : DakotaAlgorithm(impl)
{}
/// @endcond

} // analysis
} // openstudio

