/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>

#include <model/Model.hpp>
#include <model/DaylightingDeviceShelf.hpp>
#include <model/DaylightingDeviceShelf_Impl.hpp>
#include <utilities/idd/OS_DaylightingDevice_Shelf_FieldEnums.hxx>

#include <utilities/idd/DaylightingDevice_Shelf_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateDaylightingDeviceShelf( model::DaylightingDeviceShelf & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::DaylightingDevice_Shelf);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(DaylightingDevice_ShelfFields::Name, modelObject.name().get());

  OptionalString s = modelObject.getString(OS_DaylightingDevice_ShelfFields::WindowName, false, true);
  if (s){
    idfObject.setString(DaylightingDevice_ShelfFields::WindowName, *s);
  }else{
    LOG(Error, "Missing required input 'Window Name' for DaylightingDevice:Shelf named '" << modelObject.name().get() << "'");
  }

  // TODO: make sure inside shelf is converted to a surface
  s = modelObject.getString(OS_DaylightingDevice_ShelfFields::InsideShelfName, false, true);
  if (s){
    idfObject.setString(DaylightingDevice_ShelfFields::InsideShelfName, *s);
  }

  s = modelObject.getString(OS_DaylightingDevice_ShelfFields::OutsideShelfName, false, true);
  if (s){
    idfObject.setString(DaylightingDevice_ShelfFields::OutsideShelfName, *s);
  }

  // TODO: map construction from shading surface
  s.reset();
  if (s){
    idfObject.setString(DaylightingDevice_ShelfFields::OutsideShelfConstructionName, *s);
  }

  OptionalDouble d = modelObject.getDouble(OS_DaylightingDevice_ShelfFields::ViewFactortoOutsideShelf, false);
  if (d){
    idfObject.setDouble(DaylightingDevice_ShelfFields::ViewFactortoOutsideShelf, *d);
  }

  return boost::none;
}

} // energyplus

} // openstudio

