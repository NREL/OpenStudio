/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef ANALYSIS_NULLPERTURBATION_HPP
#define ANALYSIS_NULLPERTURBATION_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/DiscretePerturbation.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  class NullPerturbation_Impl;

} // detail

/** NullPerturbation is a DiscretePerturbation that does not change the input model in any way. 
 *  It should be used if one of the design options for the DiscreteVariable is to leave the seed 
 *  model unchanged. */
class ANALYSIS_API NullPerturbation : public DiscretePerturbation {
 public:
  /** @name Constructors and Destructors */
  //@{

  NullPerturbation(bool isSelected=true);

  /** Constructor provided for deserialization; not for general use. */
  NullPerturbation(const UUID& uuid, 
                   const UUID& versionUUID, 
                   const std::string& name,
                   const std::string& displayName,
                   const std::string& description,
                   bool isSelected);

  virtual ~NullPerturbation() {}

  //@}
 protected:
  /// @cond
  typedef detail::NullPerturbation_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::NullPerturbation_Impl;

  explicit NullPerturbation(boost::shared_ptr<detail::NullPerturbation_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.NullPerturbation");
};

/** \relates NullPerturbation*/
typedef boost::optional<NullPerturbation> OptionalNullPerturbation;

/** \relates NullPerturbation*/
typedef std::vector<NullPerturbation> NullPerturbationVector;

} // analysis
} // openstudio

#endif // ANALYSIS_NULLPERTURBATION_HPP

