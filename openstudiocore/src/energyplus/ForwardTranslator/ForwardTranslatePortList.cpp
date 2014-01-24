/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/PortList.hpp>
#include <model/PortList_Impl.hpp>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/NodeList_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translatePortList( PortList & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  // Create a new IddObjectType::NodeList
  // If you don't want a node list based on the port list, don't use this translator

  std::vector<ModelObject> modelObjects = modelObject.modelObjects();

  if (modelObjects.empty()){
    // do not write out this object
    return boost::none;
  }

  IdfObject idfObject(IddObjectType::NodeList);

  // Name
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  for( std::vector<ModelObject>::iterator it = modelObjects.begin();
       it != modelObjects.end();
       ++it )
  {
    if( boost::optional<Node> node = it->optionalCast<Node>() )
    {
      IdfExtensibleGroup group = idfObject.pushExtensibleGroup();

      group.setString(NodeListExtensibleFields::NodeName,node->name().get());
    }
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

