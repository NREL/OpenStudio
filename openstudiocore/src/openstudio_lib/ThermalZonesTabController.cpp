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

#include "ThermalZonesTabController.hpp"
#include "ThermalZonesController.hpp"
#include "ThermalZonesView.hpp"
#include "ThermalZonesTabView.hpp"

namespace openstudio {

ThermalZonesTabController::ThermalZonesTabController(bool isIP, const model::Model& model)
  : MainTabController(new ThermalZonesTabView())
{
  m_thermalZonesController = std::shared_ptr<ThermalZonesController>(new ThermalZonesController(isIP,model));

  this->mainContentWidget()->addTabWidget(m_thermalZonesController->subTabView());

  connect( m_thermalZonesController.get(),SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )),
           this,SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )) );

  connect( this,SIGNAL(toggleUnitsClicked( bool )),
           m_thermalZonesController.get(),SIGNAL(toggleUnitsClicked( bool )) );
}

} // openstudio

