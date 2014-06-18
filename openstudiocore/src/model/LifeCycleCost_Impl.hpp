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

#ifndef MODEL_LIFECYCLECOST_IMPL_HPP
#define MODEL_LIFECYCLECOST_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "LifeCycleCost.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {
  
  class SpaceLoadDefinition;

namespace detail {

class MODEL_API LifeCycleCost_Impl : public ModelObject_Impl
{
  Q_OBJECT;
  Q_PROPERTY(const std::string& itemType READ itemType);
  Q_PROPERTY(double cost READ cost WRITE setCost);
  Q_PROPERTY(std::string costUnits READ costUnits WRITE setCostUnits);
public:
  // constructor
  LifeCycleCost_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  // construct from workspace
  LifeCycleCost_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                 Model_Impl* model,
                 bool keepHandle);

  // clone copy constructor
  LifeCycleCost_Impl(const LifeCycleCost_Impl& other,Model_Impl* model,bool keepHandle);

  // virtual destructor
  virtual ~LifeCycleCost_Impl(){}

  virtual IddObjectType iddObjectType() const {return LifeCycleCost::iddObjectType();}

  virtual const std::vector<std::string>& outputVariableNames() const;

  /** @name Getters */
  //@{

  std::string category() const;

  std::string itemType() const;

  ModelObject item() const;

  double cost() const;

  std::vector<std::string> validCostUnitsValues() const;

  std::string costUnits() const;

  int yearsFromStart() const;
  bool isYearsFromStartDefaulted() const;
  int monthsFromStart() const;
  bool isMonthsFromStartDefaulted() const;

  int repeatPeriodYears() const;
  bool isRepeatPeriodYearsDefaulted() const;
  int repeatPeriodMonths() const;
  bool isRepeatPeriodMonthsDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setCategory(const std::string& category);
  
  bool setCost(double cost);

  bool setCostUnits(const std::string& costUnits);

  bool setYearsFromStart(int yearsFromStart);
  void resetYearsFromStart();
  bool setMonthsFromStart(int monthsFromStart);
  void resetMonthsFromStart();

  bool setRepeatPeriodYears(int repeatPeriodYears);
  void resetRepeatPeriodYears();
  bool setRepeatPeriodMonths(int repeatPeriodMonths);
  void resetRepeatPeriodMonths();

  //@}

  double totalCost() const;

  bool convertToCostPerEach();

  boost::optional<int> costedQuantity() const;

  boost::optional<double> costedArea() const;

  boost::optional<int> costedThermalZones() const;
  
private:
  REGISTER_LOGGER("openstudio.model.LifeCycleCost");

  //double getArea(const SpaceLoadInstance& spaceLoadInstance) const;
  //double getArea(const SpaceLoadDefinition& spaceLoadDefinition) const;
};

} // detail
} // model
} // openstudio

#endif // MODEL_LIFECYCLECOST_IMPL_HPP
