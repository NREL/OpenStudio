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

#ifndef MODEL_LIFECYCLECOSTUSEPRICEESCALATION_IMPL_HPP
#define MODEL_LIFECYCLECOSTUSEPRICEESCALATION_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "LifeCycleCostUsePriceEscalation.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {
namespace detail {

class MODEL_API LifeCycleCostUsePriceEscalation_Impl : public ParentObject_Impl{
 public:
  // constructor
  LifeCycleCostUsePriceEscalation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  // construct from workspace
  LifeCycleCostUsePriceEscalation_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                 Model_Impl* model,
                 bool keepHandle);

  // clone copy constructor
  LifeCycleCostUsePriceEscalation_Impl(const LifeCycleCostUsePriceEscalation_Impl& other,Model_Impl* model,bool keepHandle);

  // virtual destructor
  virtual ~LifeCycleCostUsePriceEscalation_Impl(){}

  OptionalString resource() const;
  void setResource(const std::string& str);

  OptionalString escalationStartMonth() const;
  void setEscalationStartMonth(const std::string& str);

  OptionalInt escalationStartYear() const;
  void setEscalationStartYear(int num);

  boost::optional<double> yearEscalation(unsigned index) const;
  bool setYearEscalation(unsigned index, double num);

  unsigned numYears() const;

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

  virtual IddObjectType iddObjectType() const {return LifeCycleCostUsePriceEscalation::iddObjectType();}

private:
  REGISTER_LOGGER("openstudio.model.LifeCycleCostUsePriceEscalation");

};

} // detail
} // model
} // openstudio

#endif // MODEL_LIFECYCLECOSTUSEPRICEESCALATION_IMPL_HPP
