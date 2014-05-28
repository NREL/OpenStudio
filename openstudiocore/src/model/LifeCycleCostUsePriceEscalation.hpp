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

#ifndef MODEL_LIFECYCLECOSTUSEPRICEESCALATION_HPP
#define MODEL_LIFECYCLECOSTUSEPRICEESCALATION_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class LifeCycleCostUsePriceEscalation_Impl;
}

/** LifeCycleCostUsePriceEscalation derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:LifeCycleCost:UsePriceEscalation".
 *
 *  LifeCycleCostUsePriceEscalation specifies escalation about energy and water costs separately from general inflation assumptions.
 */
class MODEL_API LifeCycleCostUsePriceEscalation : public ParentObject{
 
public:

  /** @name Constructors and Destructors */
  //@{
  virtual ~LifeCycleCostUsePriceEscalation() {} 

  /// Constructs a new LifeCycleCostUsePriceEscalation object in the model.
  explicit LifeCycleCostUsePriceEscalation(const Model& model);
  
  //@}
  /** @name Getters */
  //@{  

  boost::optional<std::string> resource() const;

  boost::optional<std::string> escalationStartMonth() const;

  boost::optional<int> escalationStartYear() const;

  /** Get the escalation for the year at index. Indexing starts at 0. */
  boost::optional<double> yearEscalation(unsigned index) const;

  //@}
  /** @name Setters */
  //@{

  void setResource(const std::string& str);

  void setEscalationStartMonth(const std::string& str);

  /** Set the escalation for the year at index. Indexing starts at 0. */
  void setEscalationStartYear(int num);

  bool setYearEscalation(unsigned index, double num);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();
  
  /// Returns the number of years for which escalation data is specified, e.g. the number of extensible fields.
  unsigned numYears() const;

protected:

  /// @cond

  typedef detail::LifeCycleCostUsePriceEscalation_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit LifeCycleCostUsePriceEscalation(std::shared_ptr<detail::LifeCycleCostUsePriceEscalation_Impl> impl);

private:

  REGISTER_LOGGER("openstudio.model.LifeCycleCostUsePriceEscalation");

  /// @endcond

};

/** \relates LifeCycleCostUsePriceEscalation */
typedef boost::optional<LifeCycleCostUsePriceEscalation> OptionalLifeCycleCostUsePriceEscalation;

/** \relates LifeCycleCostUsePriceEscalation */
typedef std::vector<LifeCycleCostUsePriceEscalation> LifeCycleCostUsePriceEscalationVector;

} // model
} // openstudio

#endif // MODEL_LIFECYCLECOSTUSEPRICEESCALATION_HPP
