/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_CENTRALHEATPUMPSYSTEM_HPP
#define MODEL_CENTRALHEATPUMPSYSTEM_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class ModelObjectList;
class CentralHeatPumpSystemModule;

namespace detail {

  class CentralHeatPumpSystem_Impl;

} // detail

/** CentralHeatPumpSystem is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:CentralHeatPumpSystem'. */
class MODEL_API CentralHeatPumpSystem : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CentralHeatPumpSystem(const Model& model);

  virtual ~CentralHeatPumpSystem() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlMethodValues();

  /** @name Getters */
  //@{

  std::string controlMethod() const;

  double ancillaryPower() const;

  boost::optional<Schedule> ancillaryOperationSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlMethod(std::string controlMethod);

  bool setAncillaryPower(double ancillaryPower);

  bool setAncillaryOperationSchedule(Schedule& schedule);

  void resetAncillaryOperationSchedule();

  //@}
  /** @name Other */
  //@{

  bool addModule( const CentralHeatPumpSystemModule & centralHeatPumpSystemModule);

  void removeModule( const CentralHeatPumpSystemModule & centralHeatPumpSystemModule);

  void removeAllModules();

  std::vector<CentralHeatPumpSystemModule> modules() const;

  //@}
 protected:
  /// @cond
  typedef detail::CentralHeatPumpSystem_Impl ImplType;

  explicit CentralHeatPumpSystem(std::shared_ptr<detail::CentralHeatPumpSystem_Impl> impl);

  friend class detail::CentralHeatPumpSystem_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CentralHeatPumpSystem");
};

/** \relates CentralHeatPumpSystem*/
typedef boost::optional<CentralHeatPumpSystem> OptionalCentralHeatPumpSystem;

/** \relates CentralHeatPumpSystem*/
typedef std::vector<CentralHeatPumpSystem> CentralHeatPumpSystemVector;

} // model
} // openstudio

#endif // MODEL_CENTRALHEATPUMPSYSTEM_HPP

