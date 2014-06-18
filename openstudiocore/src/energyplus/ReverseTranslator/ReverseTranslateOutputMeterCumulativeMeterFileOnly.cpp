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

#include "../ReverseTranslator.hpp"

#include "../../model/Meter.hpp"
#include "../../model/Meter_Impl.hpp"

#include <utilities/idd/Output_Meter_Cumulative_MeterFileOnly_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateOutputMeterCumulativeMeterFileOnly( const WorkspaceObject & workspaceObject )
{
  openstudio::model::Meter meter( m_model );

  OptionalString s = workspaceObject.getString(Output_Meter_Cumulative_MeterFileOnlyFields::Name);
  if(s){
    meter.setName(*s);
  }

  s = workspaceObject.getString(Output_Meter_Cumulative_MeterFileOnlyFields::ReportingFrequency);
  if(s){
    meter.setReportingFrequency(*s);
  }

  meter.setMeterFileOnly(true);

  meter.setCumulative(true);

  return meter;
}

} // energyplus

} // openstudio

