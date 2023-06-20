/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PlantEquipmentOperationOutdoorDryBulbDifference.hpp"
#include "../../model/PlantEquipmentOperationOutdoorDryBulbDifference_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/PlantEquipmentOperation_OutdoorDryBulbDifference_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translatePlantEquipmentOperationOutdoorDryBulbDifference(PlantEquipmentOperationOutdoorDryBulbDifference& modelObject) {
    IdfObject idfObject(IddObjectType::PlantEquipmentOperation_OutdoorDryBulbDifference);
    m_idfObjects.push_back(idfObject);

    // Name
    auto name = modelObject.name().get();
    idfObject.setName(name);

    // ReferenceTemperatureNodeName
    if (const auto& node = modelObject.referenceTemperatureNode()) {
      idfObject.setString(PlantEquipmentOperation_OutdoorDryBulbDifferenceFields::ReferenceTemperatureNodeName, node->name().get());
    }

    double lowerLimit = modelObject.minimumLowerLimit();
    int i = 1;
    for (auto upperLimit : modelObject.loadRangeUpperLimits()) {
      auto equipment = modelObject.equipment(upperLimit);
      if (!equipment.empty()) {
        auto eg = idfObject.pushExtensibleGroup();
        eg.setDouble(PlantEquipmentOperation_OutdoorDryBulbDifferenceExtensibleFields::DryBulbTemperatureDifferenceRangeLowerLimit, lowerLimit);
        eg.setDouble(PlantEquipmentOperation_OutdoorDryBulbDifferenceExtensibleFields::DryBulbTemperatureDifferenceRangeUpperLimit, upperLimit);

        IdfObject equipmentList(IddObjectType::PlantEquipmentList);
        m_idfObjects.push_back(equipmentList);
        auto equipmentListName = name + " equipment list " + std::to_string(i);
        equipmentList.setName(equipmentListName);
        eg.setString(PlantEquipmentOperation_OutdoorDryBulbDifferenceExtensibleFields::RangeEquipmentListName, equipmentListName);

        for (auto& component : equipment) {
          auto eg2 = equipmentList.pushExtensibleGroup();
          auto idf_component = translateAndMapModelObject(component);
          OS_ASSERT(idf_component);
          eg2.setString(PlantEquipmentListExtensibleFields::EquipmentObjectType, idf_component->iddObject().name());
          eg2.setString(PlantEquipmentListExtensibleFields::EquipmentName, idf_component->name().get());
        }
      }

      lowerLimit = upperLimit;
      ++i;
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
