/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include "../../model/AirLoopHVACZoneMixer_Impl.hpp"
#include "../../model/AirTerminalSingleDuctUncontrolled.hpp"
#include "../../model/AirTerminalSingleDuctUncontrolled_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_ZoneSplitter_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACZoneSplitter( AirLoopHVACZoneSplitter & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  std::string s;

  IdfObject idfObject(openstudio::IddObjectType::AirLoopHVAC_ZoneSplitter);

  m_idfObjects.push_back(idfObject);

  ///////////////////////////////////////////////////////////////////////////
  // Field: Name ////////////////////////////////////////////////////////////
  s = modelObject.name().get();
  idfObject.setName(s);

  ///////////////////////////////////////////////////////////////////////////
  // Field: Inlet Node Name /////////////////////////////////////////////////
  temp = modelObject.inletModelObject();
  if(temp)
  {
    optS = temp->name();
    if(optS)
    {
      idfObject.setString(openstudio::AirLoopHVAC_ZoneSplitterFields::InletNodeName,*optS);
    }
  }
  ///////////////////////////////////////////////////////////////////////////
  // Field: Outlet 1-N Node Name //////////////////////////////////////////////
  std::vector<ModelObject> outletModelObjects = modelObject.outletModelObjects();
  int i = 2;
  for( const auto & outletModelObject : outletModelObjects )
  {
    if( OptionalAirTerminalSingleDuctUncontrolled airTerminal =
        outletModelObject.optionalCast<AirTerminalSingleDuctUncontrolled>() )
    {
      temp = airTerminal->outletModelObject();
      if(temp)
      {
        optS = temp->name().get();
        if(optS)
          idfObject.setString(i,*optS);
      }
    }
    else
    {
      s = outletModelObject.name().get();
      idfObject.setString(i,s);
    }
    i++;
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

