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
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../../model/AirLoopHVACSupplyPlenum.hpp"
#include "../../model/AirLoopHVACSupplyPlenum_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/AirLoopHVAC_SupplyPath_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::createAirLoopHVACSupplyPath( AirLoopHVAC & airLoopHVAC )
{
  IdfObject supplyPathIdf(openstudio::IddObjectType::AirLoopHVAC_SupplyPath);
  m_idfObjects.push_back(supplyPathIdf);

  supplyPathIdf.setName(airLoopHVAC.name().get() + " Supply Path");

  model::Node node = airLoopHVAC.demandInletNode();
  supplyPathIdf.setString(openstudio::AirLoopHVAC_SupplyPathFields::SupplyAirPathInletNodeName,node.name().get());

  model::AirLoopHVACZoneSplitter zoneSplitter = airLoopHVAC.zoneSplitter();

  boost::optional<IdfObject> _zoneSplitter = translateAndMapModelObject(zoneSplitter);
  OS_ASSERT(_zoneSplitter);
  IdfExtensibleGroup eg = supplyPathIdf.pushExtensibleGroup();
  eg.setString(AirLoopHVAC_SupplyPathExtensibleFields::ComponentObjectType,_zoneSplitter->iddObject().name());
  eg.setString(AirLoopHVAC_SupplyPathExtensibleFields::ComponentName,_zoneSplitter->name().get());

  std::vector<ModelObject> supplyPlenums = airLoopHVAC.demandComponents(AirLoopHVACSupplyPlenum::iddObjectType());
  for( auto & supplyPlenum : supplyPlenums )
  {
    eg = supplyPathIdf.pushExtensibleGroup();
    boost::optional<IdfObject> _supplyPlenum = translateAndMapModelObject(supplyPlenum);
    OS_ASSERT(_supplyPlenum);
    eg.setString(AirLoopHVAC_SupplyPathExtensibleFields::ComponentObjectType,_supplyPlenum->iddObject().name());
    eg.setString(AirLoopHVAC_SupplyPathExtensibleFields::ComponentName,_supplyPlenum->name().get());
  }

  return boost::optional<IdfObject>(supplyPathIdf);
}

} // energyplus

} // openstudio

