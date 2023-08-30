/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/DistrictHeatingSteam.hpp"
#include "../../model/DistrictHeatingSteam_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/DistrictHeating_Steam_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDistrictHeatingSteam(DistrictHeatingSteam& modelObject) {
    OptionalString s;
    OptionalDouble d;
    OptionalModelObject temp;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::DistrictHeating_Steam);

    m_idfObjects.push_back(idfObject);

    ///////////////////////////////////////////////////////////////////////////
    // Field: Name ////////////////////////////////////////////////////////////
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Inlet Node Name ////////////////////////////////////////////////////
    temp = modelObject.inletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(openstudio::DistrictHeating_SteamFields::SteamInletNodeName, *s);
      }
    }
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Outlet Node Name ///////////////////////////////////////////////////
    temp = modelObject.outletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(openstudio::DistrictHeating_SteamFields::SteamOutletNodeName, *s);
      }
    }
    ///
    ////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    //Nominal Capacity ///////////////////////////////////////////////////
    if (modelObject.isNominalCapacityAutosized()) {
      idfObject.setString(DistrictHeating_SteamFields::NominalCapacity, "Autosize");
    } else if ((value = modelObject.nominalCapacity())) {
      idfObject.setDouble(DistrictHeating_SteamFields::NominalCapacity, value.get());
    }
    //
    ////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    //Capacity Fraction Schedule Name ///////////////////////////////////////////////////
    boost::optional<Schedule> capacityFractionSchedule = modelObject.capacityFractionSchedule();

    if (capacityFractionSchedule) {
      boost::optional<IdfObject> _capacityFractionSchedule = translateAndMapModelObject(capacityFractionSchedule.get());

      if (_capacityFractionSchedule && _capacityFractionSchedule->name()) {
        idfObject.setString(DistrictHeating_SteamFields::CapacityFractionScheduleName, _capacityFractionSchedule->name().get());
      }
    }
    //
    ////////////////////////////////////////////////////////////////////////

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
