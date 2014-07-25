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

#ifndef MODEL_SCHEDULETYPELIMITS_HPP
#define MODEL_SCHEDULETYPELIMITS_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
class Quantity;
class OSOptionalQuantity;
namespace model {

namespace detail {

  class ScheduleTypeLimits_Impl;

} // detail

/** ScheduleTypeLimits is a ResourceObject that wraps the OpenStudio IDD object 'OS:ScheduleTypeLimits'. 
 *  Once attached to an object of type ScheduleBase, it defines the units and bounds for that object's 
 *  values. */
class MODEL_API ScheduleTypeLimits : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ScheduleTypeLimits(const Model& model);

  virtual ~ScheduleTypeLimits() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> numericTypeValues();

  static std::vector<std::string> unitTypeValues();

  static boost::optional<Unit> units(std::string unitType, bool returnIP=false);

  /** @name Getters */
  //@{

  boost::optional<double> lowerLimitValue() const;

  OSOptionalQuantity getLowerLimitValue(bool returnIP=false) const;

  boost::optional<double> upperLimitValue() const;

  OSOptionalQuantity getUpperLimitValue(bool returnIP=false) const;

  boost::optional<std::string> numericType() const;

  std::string unitType() const;

  bool isUnitTypeDefaulted() const;

  //@}
  /** @name Setters */
  //@{
  
  // ETH@20120919 TODO: Have these setters fail if they are going to invalidate the current 
  // use of a ScheduleBase object. 

  void setLowerLimitValue(double lowerLimitValue);

  bool setLowerLimitValue(const Quantity& lowerLimitValue);

  void resetLowerLimitValue();

  void setUpperLimitValue(double upperLimitValue);

  bool setUpperLimitValue(const Quantity& upperLimitValue);

  void resetUpperLimitValue();

  bool setNumericType(std::string numericType);

  void resetNumericType();

  bool setUnitType(std::string unitType);

  void resetUnitType();

  //@}
 protected:
  /// @cond
  typedef detail::ScheduleTypeLimits_Impl ImplType;

  explicit ScheduleTypeLimits(std::shared_ptr<detail::ScheduleTypeLimits_Impl> impl);

  friend class detail::ScheduleTypeLimits_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ScheduleTypeLimits");
};

/** \relates ScheduleTypeLimits */
typedef boost::optional<ScheduleTypeLimits> OptionalScheduleTypeLimits;

/** \relates ScheduleTypeLimits */
typedef std::vector<ScheduleTypeLimits> ScheduleTypeLimitsVector;

/** Returns true if candidate is compatible with parentLimits, that is, if it is the same
 *  type and has bounds that are as least as strict as parentLimits's. 
 *  \relates ScheduleTypeLimits */
MODEL_API bool isCompatible(const ScheduleTypeLimits& parentLimits,
                            const ScheduleTypeLimits& candidate);

} // model
} // openstudio

#endif // MODEL_SCHEDULETYPELIMITS_HPP

