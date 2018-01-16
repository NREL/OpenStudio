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
#include "../../model/AirLoopHVACSupplyPlenum.hpp"
#include "../../model/AirLoopHVACSupplyPlenum_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirTerminalSingleDuctUncontrolled.hpp"
#include "../../model/AirTerminalSingleDuctUncontrolled_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_SupplyPlenum_FieldEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACSupplyPlenum( AirLoopHVACSupplyPlenum & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  std::string s;

  IdfObject idfObject(openstudio::IddObjectType::AirLoopHVAC_SupplyPlenum);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name().get();
  idfObject.setName(s);

  // Inlet Node Name
  if( boost::optional<model::ModelObject> node = modelObject.inletModelObject() )
  {
    idfObject.setString(openstudio::AirLoopHVAC_SupplyPlenumFields::InletNodeName,node->name().get());
  }

  // ZoneName
  // and
  // ZoneNodeName

  if( boost::optional<model::ThermalZone> zone = modelObject.thermalZone() )
  {
    if( boost::optional<IdfObject> _zone = translateAndMapModelObject(zone.get()) )
    {
      idfObject.setString(AirLoopHVAC_SupplyPlenumFields::ZoneName,_zone->name().get());

      model::Node node = zone->zoneAirNode();
      idfObject.setString(AirLoopHVAC_SupplyPlenumFields::ZoneNodeName,node.name().get());
    }
  }


  // OutletNodeName

  std::vector<ModelObject> outletModelObjects = modelObject.outletModelObjects();
  for( const auto & outletModelObject : outletModelObjects )
  {
    if( OptionalAirTerminalSingleDuctUncontrolled airTerminal =
        outletModelObject.optionalCast<AirTerminalSingleDuctUncontrolled>() )
    {
      if( boost::optional<model::ModelObject> node = airTerminal->outletModelObject() )
      {
        IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
        eg.setString(AirLoopHVAC_SupplyPlenumExtensibleFields::OutletNodeName,node->name().get());
      }
    }
    else
    {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setString(AirLoopHVAC_SupplyPlenumExtensibleFields::OutletNodeName,outletModelObject.name().get());
    }
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

