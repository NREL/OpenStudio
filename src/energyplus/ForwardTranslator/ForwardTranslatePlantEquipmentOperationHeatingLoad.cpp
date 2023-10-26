/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/PlantEquipmentOperationHeatingLoad.hpp"
#include "../../model/PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/PlantEquipmentOperation_HeatingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

// Special case
#include "../../model/GeneratorMicroTurbineHeatRecovery.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery_Impl.hpp"
#include "../../model/GeneratorMicroTurbine.hpp"
#include "../../model/GeneratorMicroTurbine_Impl.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePlantEquipmentOperationHeatingLoad(PlantEquipmentOperationHeatingLoad& modelObject) {
    IdfObject idfObject(IddObjectType::PlantEquipmentOperation_HeatingLoad);
    m_idfObjects.push_back(idfObject);

    // Name
    auto name = modelObject.nameString();
    idfObject.setName(name);

    double lowerLimit = modelObject.minimumLowerLimit();
    int i = 1;
    for (auto upperLimit : modelObject.loadRangeUpperLimits()) {
      auto equipment = modelObject.equipment(upperLimit);
      if (!equipment.empty()) {
        auto eg = idfObject.pushExtensibleGroup();
        eg.setDouble(PlantEquipmentOperation_HeatingLoadExtensibleFields::LoadRangeLowerLimit, lowerLimit);
        eg.setDouble(PlantEquipmentOperation_HeatingLoadExtensibleFields::LoadRangeUpperLimit, upperLimit);

        IdfObject equipmentList(IddObjectType::PlantEquipmentList);
        m_idfObjects.push_back(equipmentList);
        auto equipmentListName = name + " equipment list " + std::to_string(i);
        equipmentList.setName(equipmentListName);
        eg.setString(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName, equipmentListName);

        for (auto& component : equipment) {

          // TODO: Find the right way to deal with this
          // For now, "dirty" (?) fix for Generator:MicroTurbine
          // @kbenne, FYI
          boost::optional<IdfObject> idf_component;

          // If you find a mCHPHR
          if (boost::optional<GeneratorMicroTurbineHeatRecovery> mchpHR = component.optionalCast<GeneratorMicroTurbineHeatRecovery>()) {
            // Get the parent mchp and translate that, which will pull the appropriate fields from the mchpHR
            // But we need the name of the mchp for the plant equipment list, not the mchpHR
            GeneratorMicroTurbine mchp = mchpHR->generatorMicroTurbine();
            idf_component = translateAndMapModelObject(mchp);
            LOG(Trace, "Found a mchpHR, instead translated " << idf_component->briefDescription());
          } else {
            idf_component = translateAndMapModelObject(component);
          }

          auto eg2 = equipmentList.pushExtensibleGroup();
          OS_ASSERT(idf_component);
          eg2.setString(PlantEquipmentListExtensibleFields::EquipmentObjectType, idf_component->iddObject().name());
          eg2.setString(PlantEquipmentListExtensibleFields::EquipmentName, idf_component->nameString());
        }
      }

      lowerLimit = upperLimit;
      ++i;
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
