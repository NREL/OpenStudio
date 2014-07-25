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

#ifndef ANALYSIS_DAKOTAALGORITHMOPTIONS_HPP
#define ANALYSIS_DAKOTAALGORITHMOPTIONS_HPP

#include "AnalysisAPI.hpp"
#include "AlgorithmOptions.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class DakotaAlgorithmOptions_Impl;

} // detail

/** DakotaAlgorithmOptions is an AlgorithmOptions class for \link DakotaAlgorithm 
 *  DakotaAlgorithms\endlink. \relates DakotaAlgorithm */
class ANALYSIS_API DakotaAlgorithmOptions : public AlgorithmOptions {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~DakotaAlgorithmOptions() {}

  //@}
 protected:
  /// @cond
  typedef detail::DakotaAlgorithmOptions_Impl ImplType;

  explicit DakotaAlgorithmOptions(std::shared_ptr<detail::DakotaAlgorithmOptions_Impl> impl);

  friend class detail::DakotaAlgorithmOptions_Impl;
  friend class AlgorithmOptions;
  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.DakotaAlgorithmOptions");
};

/** \relates DakotaAlgorithmOptions*/
typedef boost::optional<DakotaAlgorithmOptions> OptionalDakotaAlgorithmOptions;

/** \relates DakotaAlgorithmOptions*/
typedef std::vector<DakotaAlgorithmOptions> DakotaAlgorithmOptionsVector;

} // analysis
} // openstudio

#endif // ANALYSIS_DAKOTAALGORITHMOPTIONS_HPP

