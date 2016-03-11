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

#ifndef MODEL_CENTRALHEATPUMPSYSTEMMODULE_HPP
#define MODEL_CENTRALHEATPUMPSYSTEMMODULE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {
namespace model {

class ChillerHeaterPerformanceElectricEIR;
class Schedule;

namespace detail {

  class CentralHeatPumpSystemModule_Impl;

} // detail

/** CentralHeatPumpSystemModule is a ParentObject that wraps the OpenStudio IDD object 'OS:CentralHeatPumpSystem:Module'. */
class MODEL_API CentralHeatPumpSystemModule : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CentralHeatPumpSystemModule(const Model& model);

  virtual ~CentralHeatPumpSystemModule() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  ChillerHeaterPerformanceElectricEIR chillerHeaterModulesPerformanceComponent() const;

  Schedule chillerHeaterModulesControlSchedule() const;

  int numberofChillerHeaterModules() const;

  //@}
  /** @name Setters */
  //@{

  bool setChillerHeaterModulesPerformanceComponent(const ChillerHeaterPerformanceElectricEIR& chillerHeaterPerformanceElectricEIR);

  bool setChillerHeaterModulesControlSchedule(Schedule& schedule);

  bool setNumberofChillerHeaterModules(int numberofChillerHeaterModules);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CentralHeatPumpSystemModule_Impl ImplType;

  explicit CentralHeatPumpSystemModule(std::shared_ptr<detail::CentralHeatPumpSystemModule_Impl> impl);

  friend class detail::CentralHeatPumpSystemModule_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CentralHeatPumpSystemModule");
};

/** \relates CentralHeatPumpSystemModule*/
typedef boost::optional<CentralHeatPumpSystemModule> OptionalCentralHeatPumpSystemModule;

/** \relates CentralHeatPumpSystemModule*/
typedef std::vector<CentralHeatPumpSystemModule> CentralHeatPumpSystemModuleVector;

} // model
} // openstudio

#endif // MODEL_CENTRALHEATPUMPSYSTEMMODULE_HPP

