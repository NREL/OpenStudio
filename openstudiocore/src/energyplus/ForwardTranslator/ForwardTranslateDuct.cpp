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
#include "../../model/Duct.hpp"
#include "../../model/Duct_Impl.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Duct_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDuct(Duct & modelObject)
{
  IdfObject idfObject(openstudio::IddObjectType::Duct);

  m_idfObjects.push_back(idfObject);

  OptionalString s;
  OptionalDouble d;
  OptionalModelObject mo;

  ///////////////////////////////////////////////////////////////////////////
  s = modelObject.name();
  if (s)
  {
    idfObject.setName(*s);
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  mo = modelObject.inletModelObject();
  if (mo)
  {
    s = mo->name();
    if (s)
    {
      idfObject.setString(openstudio::DuctFields::InletNodeName, *s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  mo = modelObject.outletModelObject();
  if (mo)
  {
    s = mo->name();
    if (s)
    {
      idfObject.setString(openstudio::DuctFields::OutletNodeName, *s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

