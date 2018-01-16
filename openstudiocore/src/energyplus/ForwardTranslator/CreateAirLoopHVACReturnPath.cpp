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

