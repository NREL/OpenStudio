/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_WATERUSECONNECTIONS_HPP
#define MODEL_WATERUSECONNECTIONS_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class WaterUseEquipment;

namespace detail {

class WaterUseConnections_Impl;

} // detail

/** WaterUseConnections is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:WaterUse:Connections'. */
class MODEL_API WaterUseConnections : public StraightComponent {

  public:

  explicit WaterUseConnections(const Model& model);

  virtual ~WaterUseConnections() {}

  static IddObjectType iddObjectType();

  boost::optional<Schedule> hotWaterSupplyTemperatureSchedule() const;

  bool setHotWaterSupplyTemperatureSchedule(Schedule& hotWaterSupplyTemperatureSchedule);

  void resetHotWaterSupplyTemperatureSchedule();

  boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

  bool setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule);

  void resetColdWaterSupplyTemperatureSchedule();

  std::vector<WaterUseEquipment> waterUseEquipment() const;

  bool addWaterUseEquipment(const WaterUseEquipment & waterUseEquipment);

  // Detaches waterUseEquipment from this object but does not remove it from the model
  bool removeWaterUseEquipment(WaterUseEquipment & waterUseEquipment);

  unsigned inletPort() const;

  unsigned outletPort() const;

  protected:

  /// @cond
  typedef detail::WaterUseConnections_Impl ImplType;

  explicit WaterUseConnections(std::shared_ptr<detail::WaterUseConnections_Impl> impl);

  friend class detail::WaterUseConnections_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

  private:

  REGISTER_LOGGER("openstudio.model.WaterUseConnections");
};

/** \relates WaterUseConnections*/
typedef boost::optional<WaterUseConnections> OptionalWaterUseConnections;

/** \relates WaterUseConnections*/
typedef std::vector<WaterUseConnections> WaterUseConnectionsVector;

} // model
} // openstudio

#endif // MODEL_WATERUSECONNECTIONS_HPP

