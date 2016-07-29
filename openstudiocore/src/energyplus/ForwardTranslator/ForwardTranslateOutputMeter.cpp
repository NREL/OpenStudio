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
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"

#include <utilities/idd/Output_Meter_FieldEnums.hxx>
#include <utilities/idd/Output_Meter_Cumulative_FieldEnums.hxx>
#include <utilities/idd/Output_Meter_MeterFileOnly_FieldEnums.hxx>
#include <utilities/idd/Output_Meter_Cumulative_MeterFileOnly_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateOutputMeter( OutputMeter & modelObject )
{
  boost::optional<IdfObject> idfObject;

  QString name = toQString(modelObject.name()).replace(QString("FuelOil_"), QString("FuelOil#"));

  if (modelObject.meterFileOnly() && modelObject.cumulative()){
    idfObject = IdfObject (openstudio::IddObjectType::Output_Meter_Cumulative_MeterFileOnly);
    m_idfObjects.push_back(*idfObject);

    idfObject->setString(Output_Meter_Cumulative_MeterFileOnlyFields::Name, toString(name));

    if (!modelObject.isReportingFrequencyDefaulted()){
      idfObject->setString(Output_Meter_Cumulative_MeterFileOnlyFields::ReportingFrequency, modelObject.reportingFrequency());
    }

  }else if (modelObject.meterFileOnly()){
    idfObject = IdfObject (openstudio::IddObjectType::Output_Meter_MeterFileOnly);
    m_idfObjects.push_back(*idfObject);

    idfObject->setString(Output_Meter_MeterFileOnlyFields::Name, toString(name));

    if (!modelObject.isReportingFrequencyDefaulted()){
      idfObject->setString(Output_Meter_MeterFileOnlyFields::ReportingFrequency, modelObject.reportingFrequency());
    }

  }else if (modelObject.cumulative()){
    idfObject = IdfObject (openstudio::IddObjectType::Output_Meter_Cumulative);
    m_idfObjects.push_back(*idfObject);

    idfObject->setString(Output_Meter_CumulativeFields::Name, toString(name));

    if (!modelObject.isReportingFrequencyDefaulted()){
      idfObject->setString(Output_Meter_CumulativeFields::ReportingFrequency, modelObject.reportingFrequency());
    }

  }else{
    idfObject = IdfObject (openstudio::IddObjectType::Output_Meter);
    m_idfObjects.push_back(*idfObject);

    idfObject->setString(Output_MeterFields::Name, toString(name));

    if (!modelObject.isReportingFrequencyDefaulted()){
      idfObject->setString(Output_MeterFields::ReportingFrequency, modelObject.reportingFrequency());
    }

  }

  return idfObject;
}

} // energyplus

} // openstudio

