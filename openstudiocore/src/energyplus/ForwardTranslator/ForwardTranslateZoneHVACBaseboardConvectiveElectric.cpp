/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ModelObject_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACBaseboardConvectiveElectric.hpp"
#include "../../model/ZoneHVACBaseboardConvectiveElectric_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include <utilities/idd/ZoneHVAC_Baseboard_Convective_Electric_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACBaseboardConvectiveElectric(
    ZoneHVACBaseboardConvectiveElectric & modelObject )
{
  // Makes sure the modelObject gets put in the map, and that the new idfObject gets put in
  // the final file. Also set's the idfObject's name.
  IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneHVAC_Baseboard_Convective_Electric,modelObject);

  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<ModelObject> temp;

  // AvailabilityScheduleName
  Schedule availabilitySchedule = modelObject.availabilitySchedule();
  translateAndMapModelObject(availabilitySchedule);
  idfObject.setString(ZoneHVAC_Baseboard_Convective_ElectricFields::AvailabilityScheduleName,
                      availabilitySchedule.name().get() );

  // NominalCapacity

  if( modelObject.isNominalCapacityAutosized() )
  {
    idfObject.setString(ZoneHVAC_Baseboard_Convective_ElectricFields::HeatingDesignCapacity,"Autosize");
  }
  else if( (value = modelObject.nominalCapacity()) )
  {
    idfObject.setDouble(ZoneHVAC_Baseboard_Convective_ElectricFields::HeatingDesignCapacity,value.get());
  }

  // Efficiency

  if( (value = modelObject.efficiency()) )
  {
    idfObject.setDouble(ZoneHVAC_Baseboard_Convective_ElectricFields::Efficiency,value.get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

