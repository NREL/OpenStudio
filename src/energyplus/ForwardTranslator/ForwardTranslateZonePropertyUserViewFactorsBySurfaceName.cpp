/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/ZonePropertyUserViewFactorsBySurfaceName.hpp"
#include "../../model/ZonePropertyUserViewFactorsBySurfaceName_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/ZoneProperty_UserViewFactors_BySurfaceName_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateZonePropertyUserViewFactorsBySurfaceName(ZonePropertyUserViewFactorsBySurfaceName& modelObject) {
    ThermalZone zone = modelObject.thermalZone();
    boost::optional<IdfObject> _zone = translateAndMapModelObject(zone);
    if (!_zone) {
      LOG(Error, "ZonePropertyUserViewFactorsBySurfaceName references a zone '" << zone.nameString()
                                                                                << "' that was not translated, so it will not be translated either");
    }

    // No need to translate it if it doesn't have at least one view factor
    std::vector<ViewFactor> viewFactors = modelObject.viewFactors();
    if (viewFactors.empty()) {
      LOG(Error, "ZonePropertyUserViewFactorsBySurfaceName for zone '" << zone.nameString()
                                                                       << "' doesn't have at least one view factor, it will not be translated.");
      return boost::none;
    }

    IdfObject idfObject(openstudio::IddObjectType::ZoneProperty_UserViewFactors_BySurfaceName);
    m_idfObjects.push_back(idfObject);

    idfObject.setString(ZoneProperty_UserViewFactors_BySurfaceNameFields::ZoneorZoneListName, _zone->name().get());

    for (const ViewFactor& viewFactor : viewFactors) {

      ModelObject fromSurface = viewFactor.fromSurface();
      boost::optional<IdfObject> _fromSurface = translateAndMapModelObject(fromSurface);
      ModelObject toSurface = viewFactor.toSurface();
      boost::optional<IdfObject> _toSurface = translateAndMapModelObject(toSurface);

      if (_fromSurface && _toSurface) {
        // Push group only if ok
        auto eg = idfObject.pushExtensibleGroup();
        eg.setString(ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::FromSurface, _fromSurface->name().get());
        eg.setString(ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ToSurface, _toSurface->name().get());
        eg.setDouble(ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ViewFactor, viewFactor.viewFactor());
      } else {
        LOG(Error, "Could not translate a viewFactor group for " << modelObject.briefDescription() << ". Continuing with the rest.");
      }
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
