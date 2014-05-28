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

#ifndef ANALYSIS_NULLMEASURE_HPP
#define ANALYSIS_NULLMEASURE_HPP

#include "AnalysisAPI.hpp"
#include "Measure.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class NullMeasure_Impl;

} // detail

/** NullMeasure is a Measure that does not change the input model in any way.
 *  It should be used if one of the design options for the DiscreteVariable is to leave the seed 
 *  model unchanged. */
class ANALYSIS_API NullMeasure : public Measure {
 public:
  /** @name Constructors and Destructors */
  //@{

  NullMeasure(bool isSelected=true);

  /** Constructor provided for deserialization; not for general use. */
  NullMeasure(const UUID& uuid,
              const UUID& versionUUID,
              const std::string& name,
              const std::string& displayName,
              const std::string& description,
              bool isSelected);

  virtual ~NullMeasure() {}

  //@}
 protected:
  /// @cond
  typedef detail::NullMeasure_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::NullMeasure_Impl;

  explicit NullMeasure(std::shared_ptr<detail::NullMeasure_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.NullMeasure");
};

/** \relates NullMeasure*/
typedef boost::optional<NullMeasure> OptionalNullMeasure;

/** \relates NullMeasure*/
typedef std::vector<NullMeasure> NullMeasureVector;

// ETH@20130723 - These typedefs are just to preserve the old names for the near to
// medium-term future (to avoid breaking existing scripts).

/** \relates NullMeasure*/
typedef NullMeasure NullPerturbation;

/** \relates NullMeasure*/
typedef boost::optional<NullMeasure> OptionalNullPerturbation;

/** \relates NullMeasure*/
typedef std::vector<NullMeasure> NullPerturbationVector;

} // analysis
} // openstudio

#endif // ANALYSIS_NULLMEASURE_HPP

