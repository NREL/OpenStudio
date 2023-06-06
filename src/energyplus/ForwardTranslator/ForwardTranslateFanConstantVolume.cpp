/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateFanConstantVolume(FanConstantVolume& modelObject) {
    OptionalString s;
    OptionalDouble d;
    OptionalModelObject temp;

    // Create a new IddObjectType::Fan_ConstantVolume
    IdfObject idfObject(IddObjectType::Fan_ConstantVolume);

    ///////////////////////////////////////////////////////////////////////////
    // Field: Name ////////////////////////////////////////////////////////////
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }
    ///////////////////////////////////////////////////////////////////////////

    // hook up required objects
    try {
      if (boost::optional<model::AirLoopHVAC> airLoopHVAC = modelObject.airLoopHVAC()) {
        Schedule sched = airLoopHVAC->availabilitySchedule();
        boost::optional<IdfObject> schedIdf = translateAndMapModelObject(sched);
        if (schedIdf) {
          idfObject.setString(Fan_ConstantVolumeFields::AvailabilityScheduleName, schedIdf->name().get());
        }
      } else {
        Schedule sched = modelObject.availabilitySchedule();
        translateAndMapModelObject(sched);
        idfObject.setString(Fan_ConstantVolumeFields::AvailabilityScheduleName, sched.name().get());
      }
    } catch (std::exception& e) {
      LOG(Error, "Could not translate " << modelObject.briefDescription() << ", because " << e.what() << ".");
      return boost::none;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Fan Total Efficiency ///////////////////////////////////////////////////
    idfObject.setDouble(openstudio::Fan_ConstantVolumeFields::FanTotalEfficiency, modelObject.fanEfficiency());
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Pressure Rise //////////////////////////////////////////////////////////
    idfObject.setDouble(openstudio::Fan_ConstantVolumeFields::PressureRise, modelObject.pressureRise());
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Maximum Flow Rate //////////////////////////////////////////////////////
    d = modelObject.maximumFlowRate();
    if (d) {
      idfObject.setDouble(openstudio::Fan_ConstantVolumeFields::MaximumFlowRate, *d);
    } else {
      idfObject.setString(openstudio::Fan_ConstantVolumeFields::MaximumFlowRate, "AutoSize");
    }
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Motor Efficiency ///////////////////////////////////////////////////////
    idfObject.setDouble(openstudio::Fan_ConstantVolumeFields::MotorEfficiency, modelObject.motorEfficiency());
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Motor In Airstream Fraction ////////////////////////////////////////////
    idfObject.setDouble(openstudio::Fan_ConstantVolumeFields::MotorInAirstreamFraction, modelObject.motorInAirstreamFraction());
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Air Inlet Node Name ////////////////////////////////////////////////////
    temp = modelObject.inletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(openstudio::Fan_ConstantVolumeFields::AirInletNodeName, *s);
      }
    }
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Air Outlet Node Name ///////////////////////////////////////////////////
    temp = modelObject.outletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(openstudio::Fan_ConstantVolumeFields::AirOutletNodeName, *s);
      }
    }
    ///
    ////////////////////////////////////////////////////////////////////////

    // End Use Subcategory
    idfObject.setString(Fan_ConstantVolumeFields::EndUseSubcategory, modelObject.endUseSubcategory());

    m_idfObjects.push_back(idfObject);
    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
