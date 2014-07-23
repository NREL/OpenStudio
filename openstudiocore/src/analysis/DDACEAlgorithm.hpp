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

#ifndef ANALYSIS_DDACEALGORITHM_HPP
#define ANALYSIS_DDACEALGORITHM_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithm.hpp"

namespace openstudio {
namespace analysis {

class DDACEAlgorithmOptions;

namespace detail {

  class DDACEAlgorithm_Impl;

} // detail

/** DDACEAlgorithm is a DakotaAlgorithm that implements dakotaInFileDescription so as to specify
 *  a particular Distributed Design and Analysis for Computer Experiments (DDACE) method and 
 *  related options. This family of algorithms works best with continuous variables, but we have 
 *  made it so most of the methods (all but DDACEAlgorithmType::central_composite, 
 *  DDACEAlgorithmType::box_behnken, and DDACEAlgorithmType::grid) will also operate on discrete
 *  variables (by presenting them as continuous variables to DAKOTA). */
class ANALYSIS_API DDACEAlgorithm : public DakotaAlgorithm {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct from options. */
  explicit DDACEAlgorithm(const DDACEAlgorithmOptions& options);

  /** Constructor provided for deserialization; not for general use. */
  DDACEAlgorithm(const UUID& uuid,
                 const UUID& versionUUID,
                 const std::string& displayName,
                 const std::string& description,
                 bool complete,
                 bool failed,
                 int iter,
                 const DDACEAlgorithmOptions& options,
                 const boost::optional<FileReference>& restartFileReference,
                 const boost::optional<FileReference>& outFileReference,
                 const boost::optional<openstudio::runmanager::Job>& job);

  virtual ~DDACEAlgorithm() {}

  //@}
  /** @name Getters and Queries */
  //@{

  DDACEAlgorithmOptions ddaceAlgorithmOptions() const;

  //@}
 protected:
  /// @cond
  typedef detail::DDACEAlgorithm_Impl ImplType;

  explicit DDACEAlgorithm(std::shared_ptr<detail::DDACEAlgorithm_Impl> impl);

  friend class detail::DDACEAlgorithm_Impl;
  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.DDACEAlgorithm");
};

/** \relates DDACEAlgorithm*/
typedef boost::optional<DDACEAlgorithm> OptionalDDACEAlgorithm;

/** \relates DDACEAlgorithm*/
typedef std::vector<DDACEAlgorithm> DDACEAlgorithmVector;

} // analysis
} // openstudio

#endif // ANALYSIS_DDACEALGORITHM_HPP

