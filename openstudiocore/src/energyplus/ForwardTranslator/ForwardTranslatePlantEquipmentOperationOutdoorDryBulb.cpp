/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/PlantEquipmentOperationOutdoorDryBulb.hpp"
#include "../../model/PlantEquipmentOperationOutdoorDryBulb_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/PlantEquipmentOperation_OutdoorDryBulb_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translatePlantEquipmentOperationOutdoorDryBulb( PlantEquipmentOperationOutdoorDryBulb & modelObject )
{
  IdfObject idfObject(IddObjectType::PlantEquipmentOperation_OutdoorDryBulb);
  m_idfObjects.push_back(idfObject);

  // Name
  auto name = modelObject.name().get();
  idfObject.setName(name);

  double lowerLimit = modelObject.minimumLowerLimit();
  int i = 1;
  for( auto upperLimit : modelObject.loadRangeUpperLimits() ) {
    auto equipment = modelObject.equipment(upperLimit);
    if( ! equipment.empty() ) {
      auto eg = idfObject.pushExtensibleGroup();
      eg.setDouble(PlantEquipmentOperation_OutdoorDryBulbExtensibleFields::DryBulbTemperatureRangeLowerLimit,lowerLimit);
      eg.setDouble(PlantEquipmentOperation_OutdoorDryBulbExtensibleFields::DryBulbTemperatureRangeUpperLimit,upperLimit);

      IdfObject equipmentList(IddObjectType::PlantEquipmentList);
      m_idfObjects.push_back(equipmentList);
      auto equipmentListName = name + " equipment list " + std::to_string(i);
      equipmentList.setName(equipmentListName);
      eg.setString(PlantEquipmentOperation_OutdoorDryBulbExtensibleFields::RangeEquipmentListName,equipmentListName);

      for( auto component : equipment ) {
        auto eg2 = equipmentList.pushExtensibleGroup();
        auto idf_component = translateAndMapModelObject(component);
        OS_ASSERT(idf_component);
        eg2.setString(PlantEquipmentListExtensibleFields::EquipmentObjectType,idf_component->iddObject().name());
        eg2.setString(PlantEquipmentListExtensibleFields::EquipmentName,idf_component->name().get());
      }
    }

    lowerLimit = upperLimit;
    ++i;
  }

  return idfObject;
}

} // energyplus

} // openstudio

