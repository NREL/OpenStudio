/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirLoopHVACReturnPlenum.hpp"
#include "../../model/AirLoopHVACReturnPlenum_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneHVACIdealLoadsAirSystem.hpp"
#include "../../model/ZoneHVACIdealLoadsAirSystem_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include "../../model/PortList_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_ReturnPlenum_FieldEnums.hxx>
#include <utilities/idd/NodeList_FieldEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACReturnPlenum(AirLoopHVACReturnPlenum& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    std::string s;

    IdfObject idfObject(openstudio::IddObjectType::AirLoopHVAC_ReturnPlenum);

    m_idfObjects.push_back(idfObject);

    // Name
    s = modelObject.name().get();
    idfObject.setName(s);

    //((ZoneName)(Zone Name))
    //((ZoneNodeName)(Zone Node Name))

    //  InducedAirOutletNodeorNodeListName
    // This is only the zoneHVAC that meets a very specific criteria,
    // see the method documentation in AirLoopHVACReturnPlenum_Impl
    auto zoneHVAC = modelObject.getImpl<model::detail::AirLoopHVACReturnPlenum_Impl>()->zoneHVACIdealLoadsAirSystems();
    // If zoneHVAC is not empty we do a workaround,
    // and we don't use the inducedAirOutletPortList because of an EnergyPlus bug
    if (zoneHVAC.empty()) {
      PortList po = modelObject.getImpl<model::detail::AirLoopHVACReturnPlenum_Impl>()->inducedAirOutletPortList();
      std::vector<ModelObject> inducedNodes = po.modelObjects();
      if (!inducedNodes.empty()) {
        IdfObject nodeList(openstudio::IddObjectType::NodeList);
        m_idfObjects.push_back(nodeList);
        nodeList.setName(s + " Induced Air Node List");
        idfObject.setString(AirLoopHVAC_ReturnPlenumFields::InducedAirOutletNodeorNodeListName, nodeList.name().get());
        for (auto it = inducedNodes.begin(); it != inducedNodes.end(); ++it) {
          IdfExtensibleGroup eg = nodeList.pushExtensibleGroup();
          eg.setString(NodeListExtensibleFields::NodeName, it->name().get());
        }
      }
    }

    // OutletNodeName
    if (!zoneHVAC.empty()) {
      auto node = zoneHVAC.front().airInletModelObject();
      OS_ASSERT(node);
      idfObject.setString(AirLoopHVAC_ReturnPlenumFields::OutletNodeName, node->nameString());
    } else if (boost::optional<model::ModelObject> node = modelObject.outletModelObject()) {
      idfObject.setString(AirLoopHVAC_ReturnPlenumFields::OutletNodeName, node->name().get());
    } else {
      idfObject.setString(AirLoopHVAC_ReturnPlenumFields::OutletNodeName, modelObject.nameString() + " Outlet Node");
    }

    //// ZoneName
    //// and
    //// ZoneNodeName
    if (boost::optional<model::ThermalZone> zone = modelObject.thermalZone()) {
      if (boost::optional<IdfObject> _zone = translateAndMapModelObject(zone.get())) {
        idfObject.setString(AirLoopHVAC_ReturnPlenumFields::ZoneName, _zone->name().get());

        model::Node node = zone->zoneAirNode();
        idfObject.setString(AirLoopHVAC_ReturnPlenumFields::ZoneNodeName, node.name().get());
      }
    }

    // InletNodeName
    auto inletModelObjects = modelObject.inletModelObjects();
    for (auto& inletModelObject : inletModelObjects) {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setString(AirLoopHVAC_ReturnPlenumExtensibleFields::InletNodeName, inletModelObject.name().get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
