/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/SurfacePropertySurroundingSurfaces.hpp"

#include "../../model/Schedule.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/SurfaceProperty_SurroundingSurfaces_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSurfacePropertySurroundingSurfaces(model::SurfacePropertySurroundingSurfaces& modelObject) {

    if (modelObject.numberofSurroundingSurfaceGroups() == 0) {
      return boost::none;
    }

    // Instantiate an IdfObject of the class to store the values,
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SurfaceProperty_SurroundingSurfaces, modelObject);

    // Sky View Factor: Optional Double
    if (boost::optional<double> skyViewFactor_ = modelObject.skyViewFactor()) {
      idfObject.setDouble(SurfaceProperty_SurroundingSurfacesFields::SkyViewFactor, skyViewFactor_.get());
    }

    // Sky Temperature Schedule Name: Optional Object
    if (boost::optional<Schedule> _skyTemperatureSchedule = modelObject.skyTemperatureSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_skyTemperatureSchedule.get())) {
        idfObject.setString(SurfaceProperty_SurroundingSurfacesFields::SkyTemperatureScheduleName, _owo->nameString());
      }
    }

    // Ground View Factor: Optional Double
    if (boost::optional<double> groundViewFactor_ = modelObject.groundViewFactor()) {
      idfObject.setDouble(SurfaceProperty_SurroundingSurfacesFields::GroundViewFactor, groundViewFactor_.get());
    }

    // Ground Temperature Schedule Name: Optional Object
    if (boost::optional<Schedule> _groundTemperatureSchedule = modelObject.groundTemperatureSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_groundTemperatureSchedule.get())) {
        idfObject.setString(SurfaceProperty_SurroundingSurfacesFields::GroundTemperatureScheduleName, _owo->nameString());
      }
    }

    for (const auto& group : modelObject.surroundingSurfaceGroups()) {

      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setString(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceName, group.surroundingSurfaceName());
      eg.setDouble(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceViewFactor, group.viewFactor());
      auto sch = group.temperatureSchedule();
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(sch)) {
        eg.setString(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceTemperatureScheduleName, _owo->nameString());
      }  // TODO: handle failure?
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
