/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"

#include <utilities/idd/Output_Meter_FieldEnums.hxx>
#include <utilities/idd/Output_Meter_Cumulative_FieldEnums.hxx>
#include <utilities/idd/Output_Meter_MeterFileOnly_FieldEnums.hxx>
#include <utilities/idd/Output_Meter_Cumulative_MeterFileOnly_FieldEnums.hxx>
#include "../../utilities/core/StringHelpers.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputMeter(OutputMeter& modelObject) {
    boost::optional<IdfObject> idfObject;

    auto name = modelObject.name();

    if (modelObject.meterFileOnly() && modelObject.cumulative()) {
      idfObject = IdfObject(openstudio::IddObjectType::Output_Meter_Cumulative_MeterFileOnly);
      m_idfObjects.push_back(*idfObject);

      idfObject->setString(Output_Meter_Cumulative_MeterFileOnlyFields::KeyName, name);

      if (!modelObject.isReportingFrequencyDefaulted()) {
        idfObject->setString(Output_Meter_Cumulative_MeterFileOnlyFields::ReportingFrequency, modelObject.reportingFrequency());
      }

    } else if (modelObject.meterFileOnly()) {
      idfObject = IdfObject(openstudio::IddObjectType::Output_Meter_MeterFileOnly);
      m_idfObjects.push_back(*idfObject);

      idfObject->setString(Output_Meter_MeterFileOnlyFields::KeyName, name);

      if (!modelObject.isReportingFrequencyDefaulted()) {
        idfObject->setString(Output_Meter_MeterFileOnlyFields::ReportingFrequency, modelObject.reportingFrequency());
      }

    } else if (modelObject.cumulative()) {
      idfObject = IdfObject(openstudio::IddObjectType::Output_Meter_Cumulative);
      m_idfObjects.push_back(*idfObject);

      idfObject->setString(Output_Meter_CumulativeFields::KeyName, name);

      if (!modelObject.isReportingFrequencyDefaulted()) {
        idfObject->setString(Output_Meter_CumulativeFields::ReportingFrequency, modelObject.reportingFrequency());
      }

    } else {
      idfObject = IdfObject(openstudio::IddObjectType::Output_Meter);
      m_idfObjects.push_back(*idfObject);

      idfObject->setString(Output_MeterFields::KeyName, name);

      if (!modelObject.isReportingFrequencyDefaulted()) {
        idfObject->setString(Output_MeterFields::ReportingFrequency, modelObject.reportingFrequency());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
