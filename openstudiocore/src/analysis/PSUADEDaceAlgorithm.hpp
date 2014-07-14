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

#ifndef ANALYSIS_PSUADEDACEALGORITHM_HPP
#define ANALYSIS_PSUADEDACEALGORITHM_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithm.hpp"

namespace openstudio {
namespace analysis {

class PSUADEDaceAlgorithmOptions;

namespace detail {

  class PSUADEDaceAlgorithm_Impl;

} // detail

/** PSUADEDaceAlgorithm is a DakotaAlgorithm that implements dakotaInFileDescription so as to 
 *  specify a particular Problem Solving Environment for Uncertainty Analysis and Design 
 *  Exploration Morris One-At-A-Time method and related options. */
class ANALYSIS_API PSUADEDaceAlgorithm : public DakotaAlgorithm {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct from options. */
  explicit PSUADEDaceAlgorithm(const PSUADEDaceAlgorithmOptions& options);

  /** Constructor provided for deserialization; not for general use. */
  PSUADEDaceAlgorithm(const UUID& uuid,
                      const UUID& versionUUID,
                      const std::string& displayName,
                      const std::string& description,
                      bool complete,
                      bool failed,
                      int iter,
                      const PSUADEDaceAlgorithmOptions& options,
                      const boost::optional<FileReference>& restartFileReference,
                      const boost::optional<FileReference>& outFileReference,
                      const boost::optional<openstudio::runmanager::Job>& job);

  virtual ~PSUADEDaceAlgorithm() {}

  //@}
  /** @name Getters and Queries */
  //@{

   PSUADEDaceAlgorithmOptions psuadedaceAlgorithmOptions() const;

  //@}
   protected:
  /// @cond
  typedef detail::PSUADEDaceAlgorithm_Impl ImplType;

  explicit PSUADEDaceAlgorithm(std::shared_ptr<detail::PSUADEDaceAlgorithm_Impl> impl);

  friend class detail::PSUADEDaceAlgorithm_Impl;
  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.PSUADEDaceAlgorithm");
};

/** \relates PSUADEDaceAlgorithm*/
typedef boost::optional<PSUADEDaceAlgorithm> OptionalPSUADEDaceAlgorithm;

/** \relates PSUADEDaceAlgorithm*/
typedef std::vector<PSUADEDaceAlgorithm> PSUADEDaceAlgorithmVector;

} // analysis
} // openstudio

#endif // ANALYSIS_PSUADEDACEALGORITHM_HPP

