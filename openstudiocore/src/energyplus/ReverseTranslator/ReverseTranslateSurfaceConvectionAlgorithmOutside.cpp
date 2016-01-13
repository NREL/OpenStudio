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

#include "../ReverseTranslator.hpp"
#include "../../model/OutsideSurfaceConvectionAlgorithm.hpp"
#include "../../model/OutsideSurfaceConvectionAlgorithm_Impl.hpp"
#include <utilities/idd/SurfaceConvectionAlgorithm_Outside_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateSurfaceConvectionAlgorithmOutside( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::SurfaceConvectionAlgorithm_Outside )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: SurfaceConvectionAlgorithm_Outside");
     return boost::none;
  }

  OutsideSurfaceConvectionAlgorithm mo = m_model.getUniqueModelObject<OutsideSurfaceConvectionAlgorithm>();

  boost::optional<std::string> s = workspaceObject.getString(SurfaceConvectionAlgorithm_OutsideFields::Algorithm);
  if( s )
  {
    mo.setAlgorithm(s.get());
  }

  return mo;
}

} // energyplus

} // openstudio

