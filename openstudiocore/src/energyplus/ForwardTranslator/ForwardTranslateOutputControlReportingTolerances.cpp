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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/OutputControlReportingTolerances.hpp>
#include <utilities/idd/OutputControl_ReportingTolerances_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateOutputControlReportingTolerances( OutputControlReportingTolerances& modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::OutputControl_ReportingTolerances );

  m_idfObjects.push_back(idfObject);

  OptionalString s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  boost::optional<double> value;

  if( value = modelObject.toleranceforTimeCoolingSetpointNotMet() )
  {
    idfObject.setDouble(OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet,value.get());
  }

  if( value = modelObject.toleranceforTimeHeatingSetpointNotMet() )
  {
    idfObject.setDouble(OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet,value.get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

