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

#ifndef ANALYSIS_FSUDACEALGORITHM_HPP
#define ANALYSIS_FSUDACEALGORITHM_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithm.hpp"

namespace openstudio {
namespace analysis {

class FSUDaceAlgorithmOptions;

namespace detail {

  class FSUDaceAlgorithm_Impl;

} // detail

/** FSUDaceAlgorithm is a DakotaAlgorithm that implements dakotaInFileDescription so as to 
 *  specify a particular Florida State University Design and Analysis of Computer Experiments 
 *  method and related options. Discrete variables are sampled by making them look continuous 
 *  to DAKOTA and then rounding the requested values. */
class ANALYSIS_API FSUDaceAlgorithm : public DakotaAlgorithm {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct from options. */
  explicit FSUDaceAlgorithm(const FSUDaceAlgorithmOptions& options);

  /** Constructor provided for deserialization; not for general use. */
  FSUDaceAlgorithm(const UUID& uuid,
                   const UUID& versionUUID,
                   const std::string& displayName,
                   const std::string& description,
                   bool complete,
                   bool failed,
                   int iter,
                   const FSUDaceAlgorithmOptions& options,
                   const boost::optional<FileReference>& restartFileReference,
                   const boost::optional<FileReference>& outFileReference,
                   const boost::optional<openstudio::runmanager::Job>& job);

  virtual ~FSUDaceAlgorithm() {}

  //@}

  /** @name Getters and Queries */
  //@{

  FSUDaceAlgorithmOptions fsudaceAlgorithmOptions() const;

  //@}
 protected:
  /// @cond
  typedef detail::FSUDaceAlgorithm_Impl ImplType;

  explicit FSUDaceAlgorithm(std::shared_ptr<detail::FSUDaceAlgorithm_Impl> impl);

  friend class detail::FSUDaceAlgorithm_Impl;
  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.FSUDaceAlgorithm");
};

/** \relates FSUDaceAlgorithm*/
typedef boost::optional<FSUDaceAlgorithm> OptionalFSUDaceAlgorithm;

/** \relates FSUDaceAlgorithm*/
typedef std::vector<FSUDaceAlgorithm> FSUDaceAlgorithmVector;

} // analysis
} // openstudio

#endif // ANALYSIS_FSUDACEALGORITHM_HPP

