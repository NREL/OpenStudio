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

#include "DDACEAlgorithm.hpp"
#include "DDACEAlgorithm_Impl.hpp"

#include "DDACEAlgorithmOptions.hpp"
#include "DDACEAlgorithmOptions_Impl.hpp"

#include "Problem.hpp"

#include "../runmanager/lib/JSON.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  // TODO: Generate name from options data.
  DDACEAlgorithm_Impl::DDACEAlgorithm_Impl(const DDACEAlgorithmOptions& options)
    : DakotaAlgorithm_Impl("DDACE",options)
  {}

  DDACEAlgorithm_Impl::DDACEAlgorithm_Impl(const UUID& uuid,
                                           const UUID& versionUUID,
                                           const std::string& displayName,
                                           const std::string& description,
                                           bool complete,
                                           bool failed,
                                           int iter,
                                           const DDACEAlgorithmOptions& options,
                                           const boost::optional<FileReference>& restartFileReference,
                                           const boost::optional<FileReference>& outFileReference, 
                                           const boost::optional<openstudio::runmanager::Job>& job)
    : DakotaAlgorithm_Impl(uuid,
                           versionUUID,
                           "DDACE",
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

  DDACEAlgorithm_Impl::DDACEAlgorithm_Impl(const DDACEAlgorithm_Impl &other)
    : DakotaAlgorithm_Impl(other)
  {}

  AnalysisObject DDACEAlgorithm_Impl::clone() const {
    std::shared_ptr<DDACEAlgorithm_Impl> impl(new DDACEAlgorithm_Impl(*this));
    return DDACEAlgorithm(impl);
  }

  bool DDACEAlgorithm_Impl::isCompatibleProblemType(const Problem& problem) const {

    DDACEAlgorithmOptions options = ddaceAlgorithmOptions();

    // Some types only work for continuous problems.
    if (!problem.allVariablesAreContinuousOrStaticTransformations()) {
      switch (options.algorithmType().value()) {
        case DDACEAlgorithmType::central_composite :
        case DDACEAlgorithmType::box_behnken :
        case DDACEAlgorithmType::grid :
          LOG(Warn,"DDACE Central Composite, Box-Behnken and Grid algorithms only work with continuous "
              << "variables. (All discrete variables must be down-selected to 0-1 selected "
              << "perturbations.) DesignOfExperiments can be used to run a grid design on discrete "
              << "variables.");
          return false;
        default :
          break;
      }
    }

    return true;
  }

  bool DDACEAlgorithm_Impl::requiresContinuousVariables() const {
    return true;
  }

  std::string DDACEAlgorithm_Impl::dakotaInFileDescription() const {

    DDACEAlgorithmOptions options = ddaceAlgorithmOptions();
    
    std::stringstream ss;
    ss << "method," << std::endl;
    ss << "        dace " << options.algorithmType().valueName() << std::endl;
    if (OptionalInt seed = options.seed()) {
      ss << "        seed = " << seed.get() << std::endl;
    }
    if (OptionalInt samples = options.samples()) {
      ss << "        samples = " << *samples << std::endl;
    }
    if (OptionalInt symbols = options.symbols()) {
      ss << "        symbols = " << *symbols << std::endl;
    }
    ss << std::endl;

    return ss.str();
  }

  DDACEAlgorithmOptions DDACEAlgorithm_Impl::ddaceAlgorithmOptions() const {
    return options().cast<DDACEAlgorithmOptions>();
  }

  QVariant DDACEAlgorithm_Impl::toVariant() const {
    QVariantMap map = DakotaAlgorithm_Impl::toVariant().toMap();

    map["algorithm_type"] = QString("DDACEAlgorithm");

    return QVariant(map);
  }

  DDACEAlgorithm DDACEAlgorithm_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();
    DDACEAlgorithmOptions options = DDACEAlgorithmOptions_Impl::fromVariant(map["options"],version);
    return DDACEAlgorithm(toUUID(map["uuid"].toString().toStdString()),
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

DDACEAlgorithm::DDACEAlgorithm(const DDACEAlgorithmOptions& options)
  : DakotaAlgorithm(std::shared_ptr<detail::DDACEAlgorithm_Impl>(
        new detail::DDACEAlgorithm_Impl(options)))
{
  createCallbackForOptions();
}

DDACEAlgorithm::DDACEAlgorithm(const UUID& uuid,
                               const UUID& versionUUID,
                               const std::string& displayName,
                               const std::string& description,
                               bool complete,
                               bool failed,
                               int iter,
                               const DDACEAlgorithmOptions& options,
                               const boost::optional<FileReference>& restartFileReference,
                               const boost::optional<FileReference>& outFileReference,
                               const boost::optional<openstudio::runmanager::Job>& job)
  : DakotaAlgorithm(std::shared_ptr<detail::DDACEAlgorithm_Impl>(
        new detail::DDACEAlgorithm_Impl(uuid,
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

DDACEAlgorithmOptions DDACEAlgorithm::ddaceAlgorithmOptions() const {
  return getImpl<detail::DDACEAlgorithm_Impl>()->ddaceAlgorithmOptions();
}

/// @cond
DDACEAlgorithm::DDACEAlgorithm(std::shared_ptr<detail::DDACEAlgorithm_Impl> impl)
  : DakotaAlgorithm(impl)
{}
/// @endcond

} // analysis
} // openstudio

