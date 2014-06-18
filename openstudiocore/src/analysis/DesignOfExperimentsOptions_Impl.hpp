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

#ifndef ANALYSIS_DESIGNOFEXPERIMENTSOPTIONS_IMPL_HPP
#define ANALYSIS_DESIGNOFEXPERIMENTSOPTIONS_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "AlgorithmOptions_Impl.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  /** DesignOfExperimentsOptions_Impl is a AlgorithmOptions_Impl that is the implementation class for DesignOfExperimentsOptions.*/
  class ANALYSIS_API DesignOfExperimentsOptions_Impl : public AlgorithmOptions_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    DesignOfExperimentsOptions_Impl(const DesignOfExperimentsType& designType);

    /** Constructor provided for deserialization; not for general use. */
    DesignOfExperimentsOptions_Impl(const DesignOfExperimentsType& designType,
                                    const std::vector<Attribute>& options);

    virtual ~DesignOfExperimentsOptions_Impl() {}

    virtual AlgorithmOptions clone() const;

    //@}
    /** @name Getters */
    //@{

    DesignOfExperimentsType designType() const;

    //@}
    /** @name Setters */
    //@{

    void setDesignType(const DesignOfExperimentsType& designType);

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    virtual QVariant toVariant() const;

    static DesignOfExperimentsOptions fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   protected:
    DesignOfExperimentsType m_designType;
   private:
    REGISTER_LOGGER("openstudio.analysis.DesignOfExperimentsOptions");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_DESIGNOFEXPERIMENTSOPTIONS_IMPL_HPP

