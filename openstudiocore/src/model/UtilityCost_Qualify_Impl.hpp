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

#ifndef MODEL_UTILITYCOST_QUALIFY_IMPL_HPP
#define MODEL_UTILITYCOST_QUALIFY_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "UtilityCost_Qualify.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {
namespace detail {

class MODEL_API UtilityCost_Qualify_Impl : public ParentObject_Impl{
  
public:
  // constructor
  UtilityCost_Qualify_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  // construct from workspace
  UtilityCost_Qualify_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                 Model_Impl* model,
                 bool keepHandle);

  // clone copy constructor
  UtilityCost_Qualify_Impl(const UtilityCost_Qualify_Impl& other,Model_Impl* model,bool keepHandle);

  // virtual destructor
  virtual ~UtilityCost_Qualify_Impl(){}

  OptionalString tariffName() const;
  void setTariffName(const std::string& str);

  OptionalString variableName() const;
  void setVariableName(const std::string& str);

  OptionalString qualifyType() const;
  void setQualifyType(const std::string& str);

  OptionalString thresholdValueOrVariableName() const;
  void setThresholdValueOrVariableName(const std::string& str);

  OptionalString season() const;
  void setSeason(const std::string& str);

  OptionalString thresholdTest() const;
  void setThresholdTest(const std::string& str);

  OptionalInt numberOfMonths() const;
  void setNumberOfMonths(int num);

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

  virtual IddObjectType iddObjectType() const {return UtilityCost_Qualify::iddObjectType();}

private:
  REGISTER_LOGGER("openstudio.model.UtilityCost_Qualify");

};

} // detail
} // model
} // openstudio

#endif // MODEL_UTILITYCOST_QUALIFY_IMPL_HPP
