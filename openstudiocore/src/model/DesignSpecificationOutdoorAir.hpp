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

#ifndef MODEL_DESIGNSPECIFICATIONOUTDOORAIR_HPP
#define MODEL_DESIGNSPECIFICATIONOUTDOORAIR_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

class DesignSpecificationOutdoorAir_Impl;

} // detail

/** DesignSpecificationOutdoorAir is a ModelObject that wraps the OpenStudio IDD
 *  object 'OS:DesignSpecification:OutdoorAir'. */
class MODEL_API DesignSpecificationOutdoorAir : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit DesignSpecificationOutdoorAir(const Model& model);

  virtual ~DesignSpecificationOutdoorAir() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> outdoorAirMethodValues();

  /** \deprecated */
  static std::vector<std::string> validOutdoorAirMethodValues();

  /** @name Getters */
  //@{

  std::string outdoorAirMethod() const;

  bool isOutdoorAirMethodDefaulted() const;

  double outdoorAirFlowperPerson() const;

  Quantity getOutdoorAirFlowperPerson(bool returnIP=false) const;

  bool isOutdoorAirFlowperPersonDefaulted() const;

  double outdoorAirFlowperFloorArea() const;

  Quantity getOutdoorAirFlowperFloorArea(bool returnIP=false) const;

  bool isOutdoorAirFlowperFloorAreaDefaulted() const;

  double outdoorAirFlowRate() const;

  Quantity getOutdoorAirFlowRate(bool returnIP=false) const;

  bool isOutdoorAirFlowRateDefaulted() const;

  double outdoorAirFlowAirChangesperHour() const;

  Quantity getOutdoorAirFlowAirChangesperHour(bool returnIP=false) const;

  bool isOutdoorAirFlowAirChangesperHourDefaulted() const;

  boost::optional<Schedule> outdoorAirFlowRateFractionSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setOutdoorAirMethod(std::string outdoorAirMethod);

  void resetOutdoorAirMethod();

  bool setOutdoorAirFlowperPerson(double outdoorAirFlowperPerson);

  bool setOutdoorAirFlowperPerson(const Quantity& outdoorAirFlowperPerson);

  void resetOutdoorAirFlowperPerson();

  bool setOutdoorAirFlowperFloorArea(double outdoorAirFlowperFloorArea);

  bool setOutdoorAirFlowperFloorArea(const Quantity& outdoorAirFlowperFloorArea);

  void resetOutdoorAirFlowperFloorArea();

  bool setOutdoorAirFlowRate(double outdoorAirFlowRate);

  bool setOutdoorAirFlowRate(const Quantity& outdoorAirFlowRate);

  void resetOutdoorAirFlowRate();

  bool setOutdoorAirFlowAirChangesperHour(double outdoorAirFlowAirChangesperHour);

  bool setOutdoorAirFlowAirChangesperHour(const Quantity& outdoorAirFlowAirChangesperHour);

  void resetOutdoorAirFlowAirChangesperHour();

  bool setOutdoorAirFlowRateFractionSchedule(Schedule& schedule);

  void resetOutdoorAirFlowRateFractionSchedule();

  //@}
 protected:
  /// @cond
  typedef detail::DesignSpecificationOutdoorAir_Impl ImplType;

  explicit DesignSpecificationOutdoorAir(std::shared_ptr<detail::DesignSpecificationOutdoorAir_Impl> impl);

  friend class detail::DesignSpecificationOutdoorAir_Impl;

  friend class Model;

  friend class IdfObject;

  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.DesignSpecificationOutdoorAir");
};

/** \relates DesignSpecificationOutdoorAir*/
typedef boost::optional<DesignSpecificationOutdoorAir> OptionalDesignSpecificationOutdoorAir;

/** \relates DesignSpecificationOutdoorAir*/
typedef std::vector<DesignSpecificationOutdoorAir> DesignSpecificationOutdoorAirVector;

} // model
} // openstudio

#endif // MODEL_DESIGNSPECIFICATIONOUTDOORAIR_HPP

