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

#ifndef MODEL_COMPONENTCOSTADJUSTMENTS_IMPL_HPP
#define MODEL_COMPONENTCOSTADJUSTMENTS_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "ComponentCostAdjustments.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {
namespace detail {

class MODEL_API ComponentCostAdjustments_Impl : public ParentObject_Impl{
 public:
  // constructor
  ComponentCostAdjustments_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  // construct from workspace
  ComponentCostAdjustments_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                 Model_Impl* model,
                 bool keepHandle);

  // clone copy constructor
  ComponentCostAdjustments_Impl(const ComponentCostAdjustments_Impl& other,Model_Impl* model,bool keepHandle);

  // virtual destructor
  virtual ~ComponentCostAdjustments_Impl(){}

  boost::optional<double> miscellaneousCostPerConditionedArea() const;
  void setMiscellaneousCostPerConditionedArea(double num);

  boost::optional<double> designAndEngineeringFees() const;
  void setDesignAndEngineeringFees(double num);

  boost::optional<double> contractorFee() const;
  void setContractorFee(double num);

  boost::optional<double> contingency() const;
  void setContingency(double num);

  boost::optional<double> permitsBondingAndInsurance() const;
  void setPermitsBondingAndInsurance(double num);

  boost::optional<double> commissioningFee() const;
  void setCommissioningFee(double num);

  boost::optional<double> regionalAdjustmentFactor() const;
  void setRegionalAdjustmentFactor(double num);

  // return the parent object in the hierarchy
  virtual boost::optional<ParentObject> parent() const;

  // set the parent, child may have to call methods on the parent
  virtual bool setParent(ParentObject& newParent);

  // return any children objects in the hierarchy
  virtual std::vector<ModelObject> children() const;

  /// get a vector of allowable children types
  virtual std::vector<IddObjectType> allowableChildTypes() const;

  // Get all output variable names that could be associated with this object.
  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const {return ComponentCostAdjustments::iddObjectType();}

private:
  REGISTER_LOGGER("openstudio.model.ComponentCostAdjustments");

};

} // detail
} // model
} // openstudio

#endif // MODEL_COMPONENTCOSTADJUSTMENTS_IMPL_HPP
