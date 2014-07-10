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

#ifndef ANALYSIS_DISCRETEVARIABLE_IMPL_HPP
#define ANALYSIS_DISCRETEVARIABLE_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "InputVariable_Impl.hpp"

namespace openstudio {
namespace analysis {

class DiscreteVariable;

namespace detail {

  /** DiscreteVariable_Impl is an InputVariable_Impl that is the implementation class
   *  for DiscreteVariable. */
  class ANALYSIS_API DiscreteVariable_Impl : public InputVariable_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    DiscreteVariable_Impl(const std::string& name);

    /** Constructor provided for deserialization; not for general use. */
    DiscreteVariable_Impl(const UUID& uuid,
                          const UUID& versionUUID,
                          const std::string& name,
                          const std::string& displayName,
                          const std::string& description,
                          const boost::optional<UncertaintyDescription>& udesc);

    DiscreteVariable_Impl(const DiscreteVariable_Impl& other);

    virtual ~DiscreteVariable_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Get the variable value from a dataPoint. */
    virtual double getValue(const DataPoint& dataPoint) const;

    /** Returns true if udesc is valid for this Variable. If returns false,
     *  setUncertaintyDescription will fail. */
    virtual bool isValid(const UncertaintyDescription& udesc) const;

    //@}
    /** @name Getters and Queries */
    //@{

    /** Returns the valid integer values for this variable. (All discrete variables
     *  are mappable to integers. Some discrete variables allow users to downselect from
     *  their overall range.) */
    virtual std::vector<int> validValues(bool selectedOnly) const = 0;

    int numValidValues(bool selectedOnly) const;

    //@}
    /** @name Serialization */
    //@{

    virtual QVariant toServerFormulationVariant() const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.analysis.DiscreteVariable");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_DISCRETEVARIABLE_IMPL_HPP
