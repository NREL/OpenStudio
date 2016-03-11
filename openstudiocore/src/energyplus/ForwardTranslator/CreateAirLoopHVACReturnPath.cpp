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
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include "../../model/AirLoopHVACZoneMixer_Impl.hpp"
#include "../../model/AirLoopHVACReturnPlenum.hpp"
#include "../../model/AirLoopHVACReturnPlenum_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"
#include "../../utilities/geometry/EulerAngles.hpp"
#include "../../utilities/geometry/Transformation.hpp"
#include <utilities/idd/AirLoopHVAC_ReturnPath_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::createAirLoopHVACReturnPath( AirLoopHVAC & airLoopHVAC )
{
  IdfObject returnPathIdf(openstudio::IddObjectType::AirLoopHVAC_ReturnPath);
  m_idfObjects.push_back(returnPathIdf);

  returnPathIdf.setName(airLoopHVAC.name().get() + " Return Path");

  Node node = airLoopHVAC.demandOutletNode();
  returnPathIdf.setString(openstudio::AirLoopHVAC_ReturnPathFields::ReturnAirPathOutletNodeName,node.name().get());

  std::vector<ModelObject> returnPlenums = airLoopHVAC.demandComponents(AirLoopHVACReturnPlenum::iddObjectType());
  for( auto & returnPlenum : returnPlenums )
  {
    IdfExtensibleGroup eg = returnPathIdf.pushExtensibleGroup();
    boost::optional<IdfObject> _returnPlenum = translateAndMapModelObject(returnPlenum);
    OS_ASSERT(_returnPlenum);
    eg.setString(AirLoopHVAC_ReturnPathExtensibleFields::ComponentObjectType,_returnPlenum->iddObject().name());
    eg.setString(AirLoopHVAC_ReturnPathExtensibleFields::ComponentName,_returnPlenum->name().get());
  }

  AirLoopHVACZoneMixer zoneMixer = airLoopHVAC.zoneMixer();
  boost::optional<IdfObject> _zoneMixer = translateAndMapModelObject(zoneMixer);
  OS_ASSERT(_zoneMixer);
  IdfExtensibleGroup eg = returnPathIdf.pushExtensibleGroup();
  eg.setString(AirLoopHVAC_ReturnPathExtensibleFields::ComponentObjectType,_zoneMixer->iddObject().name());
  eg.setString(AirLoopHVAC_ReturnPathExtensibleFields::ComponentName,_zoneMixer->name().get());

  return boost::optional<IdfObject>(returnPathIdf);
}

} // energyplus

} // openstudio

