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

#include <standardsinterface/Title24Interface.hpp>

#include <model/Model_Impl.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>

#include <utilities/data/Attribute.hpp>

namespace openstudio {
namespace standardsinterface {

Title24Interface::Title24Interface(const model::Model& model) 
  : OpenStudioStandardsInterface(model)
{
  // provide CEC context for climate zone
  model::Site site = m_model.getUniqueModelObject<model::Site>();
  bool ok = site.setAttribute("activeClimateZoneInstitution", "CEC");
  BOOST_ASSERT(ok);
}

Title24Interface Title24Interface::clone() const {
  model::Model modelClone = m_model.clone().cast<model::Model>();
  return Title24Interface(modelClone);
}

} // standardsinterface
} // openstudio
