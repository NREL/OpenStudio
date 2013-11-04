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

#ifndef MODEL_UTILITYCOST_CHARGE_SIMPLE_IMPL_HPP
#define MODEL_UTILITYCOST_CHARGE_SIMPLE_IMPL_HPP


#include <model/UtilityCost_Charge_Simple.hpp>

#include <model/ParentObject_Impl.hpp>
#include <model/UtilityCost_Charge_Simple.hpp>
#include <utilities/core/Optional.hpp>

namespace openstudio {
namespace model {
namespace detail {

class MODEL_API UtilityCost_Charge_Simple_Impl : public ParentObject_Impl{
  
public:
  // constructor
  UtilityCost_Charge_Simple_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  // construct from workspace
  UtilityCost_Charge_Simple_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                 Model_Impl* model,
                 bool keepHandle);

  // clone copy constructor
  UtilityCost_Charge_Simple_Impl(const UtilityCost_Charge_Simple_Impl& other,Model_Impl* model,bool keepHandle);

  // virtual destructor
  virtual ~UtilityCost_Charge_Simple_Impl(){}

  OptionalString tariffName() const;
  void setTariffName(const std::string& str);

  OptionalString sourceVariable() const;
  void setSourceVariable(const std::string& str);

  OptionalString season() const;
  void setSeason(const std::string& str);

  OptionalString categoryVariableName() const;
  void setCategoryVariableName(const std::string& str);

  OptionalString costPerUnitValueOrVariableName() const;
  void setCostPerUnitValueOrVariableName(const std::string& str);

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

  virtual IddObjectType iddObjectType() const {return UtilityCost_Charge_Simple::iddObjectType();}

private:
  REGISTER_LOGGER("openstudio.model.UtilityCost_Charge_Simple");

};

} // detail
} // model
} // openstudio

#endif // MODEL_UTILITYCOST_CHARGE_SIMPLE_IMPL_HPP
