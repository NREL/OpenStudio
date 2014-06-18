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

#ifndef ANALYSIS_DAKOTAALGORITHMOPTIONS_IMPL_HPP
#define ANALYSIS_DAKOTAALGORITHMOPTIONS_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "AlgorithmOptions_Impl.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  /** DakotaAlgorithmOptions_Impl is a AlgorithmOptions_Impl that is the implementation class for DakotaAlgorithmOptions.*/
  class ANALYSIS_API DakotaAlgorithmOptions_Impl : public AlgorithmOptions_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    DakotaAlgorithmOptions_Impl();

    /** Constructor provided for deserialization; not for general use. */
    explicit DakotaAlgorithmOptions_Impl(const std::vector<Attribute>& options);

    virtual ~DakotaAlgorithmOptions_Impl() {}

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.DakotaAlgorithmOptions");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_DAKOTAALGORITHMOPTIONS_IMPL_HPP

