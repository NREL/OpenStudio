/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#include "../../model/SizingPlant.hpp"
#include "../../model/SizingPlant_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Sizing_Plant_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSizingPlant( SizingPlant & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Sizing_Plant);

  // PlantorCondenserLoopName

  s = modelObject.plantLoop().name();
  if( s )
  {
    idfObject.setString(Sizing_PlantFields::PlantorCondenserLoopName,s.get());
  }

  // LoopType

  s = modelObject.loopType();
  if( s )
  {
    idfObject.setString(Sizing_PlantFields::LoopType,s.get());
  }
  
  // DesignLoopExitTemperature

  value = modelObject.designLoopExitTemperature();
  if( value )
  {
    idfObject.setDouble(Sizing_PlantFields::DesignLoopExitTemperature,value.get());
  }

  // LoopDesignTemperatureDifference

  value = modelObject.loopDesignTemperatureDifference();
  if( value )
  {
    idfObject.setDouble(Sizing_PlantFields::LoopDesignTemperatureDifference,value.get());
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

