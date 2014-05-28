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

#ifndef ANALYSIS_PARAMETERSTUDYALGORITHM_HPP
#define ANALYSIS_PARAMETERSTUDYALGORITHM_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithm.hpp"

namespace openstudio {
namespace analysis {

class ParameterStudyAlgorithmOptions;

namespace detail {

  class ParameterStudyAlgorithm_Impl;

} // detail

/** ParameterStudyAlgorithm is a DakotaAlgorithm that implements dakotaInFileDescription so as to 
 *  specify a particular Parameter Study method and related options. */
class ANALYSIS_API ParameterStudyAlgorithm : public DakotaAlgorithm {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct from options. */
  ParameterStudyAlgorithm(const ParameterStudyAlgorithmOptions& options);

  /** Constructor provided for deserialization; not for general use. */
  ParameterStudyAlgorithm(const UUID& uuid,
                          const UUID& versionUUID,
                          const std::string& displayName,
                          const std::string& description,
                          bool complete,
                          bool failed,
                          int iter,
                          const ParameterStudyAlgorithmOptions& options,
                          const boost::optional<FileReference>& restartFileReference,
                          const boost::optional<FileReference>& outFileReference,
                          const boost::optional<openstudio::runmanager::Job>& job);

  virtual ~ParameterStudyAlgorithm() {}

  //@}

  /** @name Getters and Queries */
  //@{

  ParameterStudyAlgorithmOptions parameterStudyAlgorithmOptions() const;

  //@}
 protected:
  /// @cond
  typedef detail::ParameterStudyAlgorithm_Impl ImplType;

  explicit ParameterStudyAlgorithm(std::shared_ptr<detail::ParameterStudyAlgorithm_Impl> impl);

  friend class detail::ParameterStudyAlgorithm_Impl;
  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.ParameterStudyAlgorithm");
};

/** \relates ParameterStudyAlgorithm*/
typedef boost::optional<ParameterStudyAlgorithm> OptionalParameterStudyAlgorithm;

/** \relates ParameterStudyAlgorithm*/
typedef std::vector<ParameterStudyAlgorithm> ParameterStudyAlgorithmVector;

} // analysis
} // openstudio

#endif // ANALYSIS_PARAMETERSTUDYALGORITHM_HPP

