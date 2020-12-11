/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ForwardTranslator.hpp"
#include "../../model/WaterUseConnections.hpp"
#include "../../model/WaterUseConnections_Impl.hpp"
#include "../../model/WaterUseEquipment.hpp"
#include "../../model/WaterUseEquipment_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/WaterUse_Connections_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateWaterUseConnections(WaterUseConnections& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    OptionalSchedule schedule;

    IdfObject idfObject(IddObjectType::WaterUse_Connections);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // InletNodeName

    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(WaterUse_ConnectionsFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(WaterUse_ConnectionsFields::OutletNodeName, node->name().get());
      }
    }

    // Hot Water Supply Temperature Schedule Name

    if (boost::optional<Schedule> s = modelObject.hotWaterSupplyTemperatureSchedule()) {
      translateAndMapModelObject(s.get());

      idfObject.setString(WaterUse_ConnectionsFields::HotWaterSupplyTemperatureScheduleName, s->name().get());
    }

    // Cold Water Supply Temperature Schedule Name

    if (boost::optional<Schedule> s = modelObject.coldWaterSupplyTemperatureSchedule()) {
      translateAndMapModelObject(s.get());

      idfObject.setString(WaterUse_ConnectionsFields::ColdWaterSupplyTemperatureScheduleName, s->name().get());
    }

    // Drain Water Heat Exchanger Type

    s = modelObject.drainWaterHeatExchangerType();
    if (s) {
      idfObject.setString(WaterUse_ConnectionsFields::DrainWaterHeatExchangerType, *s);
    }

    // Drain Water Heat Exchanger Destination

    s = modelObject.drainWaterHeatExchangerDestination();
    if (s) {
      idfObject.setString(WaterUse_ConnectionsFields::DrainWaterHeatExchangerDestination, *s);
    }

    // Drain Water Heat Exchanger U-Factor Times Area

    value = modelObject.drainWaterHeatExchangerUFactorTimesArea();
    if (value) {
      idfObject.setDouble(WaterUse_ConnectionsFields::DrainWaterHeatExchangerUFactorTimesArea, value.get());
    }

    //  Water Use Equipment 1 Name

    std::vector<WaterUseEquipment> equipment = modelObject.waterUseEquipment();

    for (auto& elem : equipment) {
      boost::optional<IdfObject> _equipment = translateAndMapModelObject(elem);

      if (_equipment) {
        IdfExtensibleGroup group = idfObject.pushExtensibleGroup();

        group.setString(WaterUse_ConnectionsExtensibleFields::WaterUseEquipmentName, _equipment->name().get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
