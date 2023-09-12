/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/DistrictCooling.hpp"
#include "../../model/DistrictCooling_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/DistrictCooling_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDistrictCooling(DistrictCooling& modelObject) {
    OptionalString s;
    OptionalModelObject temp;
    boost::optional<double> value;

    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::DistrictCooling, modelObject);

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
        idfObject.setString(openstudio::DistrictCoolingFields::ChilledWaterInletNodeName, *s);
      }
    }
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Outlet Node Name ///////////////////////////////////////////////////
    temp = modelObject.outletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(openstudio::DistrictCoolingFields::ChilledWaterOutletNodeName, *s);
      }
    }
    ///
    ////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    //Nominal Capacity ///////////////////////////////////////////////////
    if (modelObject.isNominalCapacityAutosized()) {
      idfObject.setString(DistrictCoolingFields::NominalCapacity, "Autosize");
    } else if ((value = modelObject.nominalCapacity())) {
      idfObject.setDouble(DistrictCoolingFields::NominalCapacity, value.get());
    }
    //
    ////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    //Capacity Fraction Schedule Name ///////////////////////////////////////////////////
    boost::optional<Schedule> capacityFractionSchedule = modelObject.capacityFractionSchedule();

    if (capacityFractionSchedule) {
      boost::optional<IdfObject> _capacityFractionSchedule = translateAndMapModelObject(capacityFractionSchedule.get());

      if (_capacityFractionSchedule && _capacityFractionSchedule->name()) {
        idfObject.setString(DistrictCoolingFields::CapacityFractionScheduleName, _capacityFractionSchedule->name().get());
      }
    }
    //
    ////////////////////////////////////////////////////////////////////////

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
