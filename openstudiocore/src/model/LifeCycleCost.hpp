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

#ifndef MODEL_LIFECYCLECOST_HPP
#define MODEL_LIFECYCLECOST_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "ModelExtensibleGroup.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class LifeCycleCost_Impl;
}

/** LifeCycleCost derives from ModelObject and is an interface to the OpenStudio IDD object named "OS:LifeCycleCost".
 *
 *  LifeCycleCost objects are used to add costs related to building construction and operation. 
 *  The LifeCycleCost's lineItemType will be set based on the type of ModelObject.
 */
class MODEL_API LifeCycleCost : public ModelObject {

public:
    
  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new LifeCycleCost object in the model.
  /// This will throw if the object type is not known
  explicit LifeCycleCost(const ModelObject& modelObject);

  virtual ~LifeCycleCost() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validCategoryValues();

  static std::vector<std::string> validItemTypeValues();

  /// Create a LifeCycleCost for the given ModelObject with cost, costUnits, category, and optional repeatPeriodYear.
  /// Will return an empty result if ModelObject type is unknown or if costUnits or category are not valid
  static boost::optional<LifeCycleCost> createLifeCycleCost(const std::string& name, const ModelObject& modelObject, double cost, const std::string& costUnits, const std::string& category, int repeatPeriodYears = 0, int yearsFromStart = 0);

  //@}
  /** @name Getters */
  //@{

  /// Returns the category
  std::string category() const;

  /// Returns the type of ModelObject this LifeCycleCost applies to
  std::string itemType() const;

  /// Returns the ModelObject this LifeCycleCost applies to
  ModelObject item() const;

  /// Returns the cost
  double cost() const;

  /// Returns a list of valid cost units based on lineItemType.
  std::vector<std::string> validCostUnitsValues() const;

  /// Returns the cost units
  std::string costUnits() const;

  /// Years from start is added to months from start to give the time at which the costs start to occur.
  int yearsFromStart() const;
  bool isYearsFromStartDefaulted() const;
  int monthsFromStart() const;
  bool isMonthsFromStartDefaulted() const;

  /// Repeat period years is added to repeat period months to give the interval at which this cost reccurs. 
  /// If both repeat period years and repeat period months are zero (default), this cost does not recurr.
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

  /// Returns the total cost based on number of units.
  double totalCost() const;

  /// Attemps to convert cost units to "CostPerEach" and set cost to totalCost
  bool convertToCostPerEach();

  /// Compute the number of objects that would be used for CostPerEach.
  /// Includes space and instance multipliers.
  boost::optional<int> costedQuantity() const;

  /// Compute the area that would be used for CostPerArea.
  /// Includes space and instance multipliers.
  boost::optional<double> costedArea() const;

  /// Compute the number of ThermalZone objects that would be used for CostPerThermalZone.
  /// Includes space and instance multipliers.
  boost::optional<int> costedThermalZones() const;

protected:

  /// @cond
  typedef detail::LifeCycleCost_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit LifeCycleCost(std::shared_ptr<detail::LifeCycleCost_Impl> impl);

private:
  REGISTER_LOGGER("openstudio.model.LifeCycleCost");

  /// @endcond

};

/** \relates LifeCycleCost */
typedef boost::optional<LifeCycleCost> OptionalLifeCycleCost;

/** \relates LifeCycleCost */
typedef std::vector<LifeCycleCost> LifeCycleCostVector;

} // model
} // openstudio

#endif // MODEL_LIFECYCLECOST_HPP
