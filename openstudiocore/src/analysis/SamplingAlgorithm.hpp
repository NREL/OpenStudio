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

#ifndef ANALYSIS_SAMPLINGALGORITHM_HPP
#define ANALYSIS_SAMPLINGALGORITHM_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithm.hpp"

namespace openstudio {
namespace analysis {

class SamplingAlgorithmOptions;

namespace detail {

  class SamplingAlgorithm_Impl;

} // detail

/** SamplingAlgorithm is a DakotaAlgorithm that implements dakotaInFileDescription so as to specify
 *  a particular Sampling method and related options. The purpose of SamplingAlgorithm is not so 
 *  much to provide sampling algorithms (the sampling algorithms available are also available in 
 *  DDACE), but to enable proper sampling from a number of \link UncertaintyDescription 
 *  UncertaintyDescriptions\endlink. */
class ANALYSIS_API SamplingAlgorithm : public DakotaAlgorithm {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct from options. */
  explicit SamplingAlgorithm(const SamplingAlgorithmOptions& options);

  /** Constructor provided for deserialization; not for general use. */
  SamplingAlgorithm(const UUID& uuid,
                    const UUID& versionUUID,
                    const std::string& displayName,
                    const std::string& description,
                    bool complete,
                    bool failed,
                    int iter,
                    const SamplingAlgorithmOptions& options,
                    const boost::optional<FileReference>& restartFileReference,
                    const boost::optional<FileReference>& outFileReference,
                    const boost::optional<openstudio::runmanager::Job>& job);

  virtual ~SamplingAlgorithm() {}

  //@}

  /** @name Getters and Queries */
  //@{

  SamplingAlgorithmOptions samplingAlgorithmOptions() const;

  //@}
 protected:
  /// @cond
  typedef detail::SamplingAlgorithm_Impl ImplType;

  explicit SamplingAlgorithm(std::shared_ptr<detail::SamplingAlgorithm_Impl> impl);

  friend class detail::SamplingAlgorithm_Impl;
  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.SamplingAlgorithm");
};

/** \relates SamplingAlgorithm*/
typedef boost::optional<SamplingAlgorithm> OptionalSamplingAlgorithm;

/** \relates SamplingAlgorithm*/
typedef std::vector<SamplingAlgorithm> SamplingAlgorithmVector;

} // analysis
} // openstudio

#endif // ANALYSIS_SAMPLINGALGORITHM_HPP

