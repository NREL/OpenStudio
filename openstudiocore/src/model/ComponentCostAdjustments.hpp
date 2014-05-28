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

#ifndef MODEL_COMPONENTCOSTADJUSTMENTS_HPP
#define MODEL_COMPONENTCOSTADJUSTMENTS_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class ComponentCostAdjustments_Impl;
}

/** ComponentCostAdjustments derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:ComponentCost:Adjustments".
 *
 *  ComponentCostAdjustments is a unique object which modifies the construction costs calculated by ComponentCost_LineItem
 *  objects.
 *
 *  \todo The IDD object "OS:ComponentCost:Adjustments" should be marked as unique
 *   
 */
class MODEL_API ComponentCostAdjustments : public ParentObject {
public:

  /** @name Constructors and Destructors */
  //@{

  virtual ~ComponentCostAdjustments() {}

  //@}
  /** @name Getters */
  //@{
 
  boost::optional<double> miscellaneousCostPerConditionedArea() const;
  
  boost::optional<double> designAndEngineeringFees() const;

  boost::optional<double> contractorFee() const;

  boost::optional<double> contingency() const;
  
  boost::optional<double> permitsBondingAndInsurance() const;

  boost::optional<double> commissioningFee() const;
  
  boost::optional<double> regionalAdjustmentFactor() const;

  //@}
  /** @name Setters */
  //@{

  void setMiscellaneousCostPerConditionedArea(double miscellaneousCostPerConditionedArea);

  void setDesignAndEngineeringFees(double designAndEngineeringFees);

  void setContractorFee(double contractorFee);

  void setContingency(double contingency);

  void setPermitsBondingAndInsurance(double permitsBondingAndInsurance);

  void setCommissioningFee(double commissioningFee);

  void setRegionalAdjustmentFactor(double regionalAdjustmentFactor);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

protected:

  /// @cond

  typedef detail::ComponentCostAdjustments_Impl ImplType;

  friend class Model;
  friend class IdfObject;
  
  /// Constructs a new ComponentCostAdjustments object in the model.
  explicit ComponentCostAdjustments(const Model& model);
  
  // constructor
  explicit ComponentCostAdjustments(std::shared_ptr<detail::ComponentCostAdjustments_Impl> impl);

private:
  REGISTER_LOGGER("openstudio.model.ComponentCostAdjustments");

  /// @endcond

};

/** \relates ComponentCostAdjustments */
typedef boost::optional<ComponentCostAdjustments> OptionalComponentCostAdjustments;

/** \relates ComponentCostAdjustments */
typedef std::vector<ComponentCostAdjustments> ComponentCostAdjustmentsVector;

} // model
} // openstudio

#endif // MODEL_COMPONENTCOSTADJUSTMENTS_HPP
