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

#ifndef MODEL_CONTROLLERMECHANICALVENTILATION_HPP
#define MODEL_CONTROLLERMECHANICALVENTILATION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ControllerOutdoorAir;

namespace detail {

  class ControllerMechanicalVentilation_Impl;

} // detail

/** ControllerMechanicalVentilation is a ModelObject that wraps the OpenStudio IDD object 'OS:Controller:MechanicalVentilation'. */
class MODEL_API ControllerMechanicalVentilation : public ModelObject {
 public:

  explicit ControllerMechanicalVentilation(const Model& model);

  virtual ~ControllerMechanicalVentilation() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> systemOutdoorAirMethodValues();

  Schedule availabilitySchedule() const;

  bool setAvailabilitySchedule(Schedule& schedule);

  bool demandControlledVentilation() const;

  bool isDemandControlledVentilationDefaulted() const;

  void setDemandControlledVentilation(bool demandControlledVentilation);

  void resetDemandControlledVentilation();

  std::string systemOutdoorAirMethod() const;

  bool isSystemOutdoorAirMethodDefaulted() const;

  bool setSystemOutdoorAirMethod(std::string systemOutdoorAirMethod);

  void resetSystemOutdoorAirMethod();

  ControllerOutdoorAir controllerOutdoorAir() const;

 protected:
  /// @cond
  typedef detail::ControllerMechanicalVentilation_Impl ImplType;

  explicit ControllerMechanicalVentilation(std::shared_ptr<detail::ControllerMechanicalVentilation_Impl> impl);

  friend class detail::ControllerMechanicalVentilation_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ControllerMechanicalVentilation");
};

/** \relates ControllerMechanicalVentilation*/
typedef boost::optional<ControllerMechanicalVentilation> OptionalControllerMechanicalVentilation;

/** \relates ControllerMechanicalVentilation*/
typedef std::vector<ControllerMechanicalVentilation> ControllerMechanicalVentilationVector;

} // model
} // openstudio

#endif // MODEL_CONTROLLERMECHANICALVENTILATION_HPP

