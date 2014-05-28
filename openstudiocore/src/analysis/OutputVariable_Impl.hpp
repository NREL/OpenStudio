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

#ifndef ANALYSIS_OUTPUTVARIABLE_IMPL_HPP
#define ANALYSIS_OUTPUTVARIABLE_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "Variable_Impl.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class ANALYSIS_API OutputVariable_Impl : public Variable_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    OutputVariable_Impl(const std::string& name);

    /** Constructor provided for deserialization; not for general use. */
    OutputVariable_Impl(const UUID& uuid,
                        const UUID& versionUUID,
                        const std::string& name,
                        const std::string& displayName,
                        const std::string& description);

    OutputVariable_Impl(const OutputVariable_Impl& other);

    virtual ~OutputVariable_Impl() {}

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.OutputVariable");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_OUTPUTVARIABLE_IMPL_HPP
