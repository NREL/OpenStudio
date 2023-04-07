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

#include "../../model/PhotovoltaicPerformanceSandia.hpp"

#include <utilities/idd/PhotovoltaicPerformance_Sandia_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePhotovoltaicPerformanceSandia(model::PhotovoltaicPerformanceSandia& modelObject) {
    boost::optional<IdfObject> result;

    // Instantiate an IdfObject of the class to store the values,
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::PhotovoltaicPerformance_Sandia, modelObject);

    if (boost::optional<std::string> moName = modelObject.name()) {
      idfObject.setName(*moName);
    }

    // Active Area: Optional Double
    double activeArea = modelObject.activeArea();
    idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::ActiveArea, activeArea);

    // Number of Cells in Series: Optional Integer
    int numberofCellsinSeries = modelObject.numberofCellsinSeries();
    idfObject.setInt(PhotovoltaicPerformance_SandiaFields::NumberofCellsinSeries, numberofCellsinSeries);

    // Number of Cells in Parallel: Optional Integer
    int numberofCellsinParallel = modelObject.numberofCellsinParallel();
    idfObject.setInt(PhotovoltaicPerformance_SandiaFields::NumberofCellsinParallel, numberofCellsinParallel);

    // Short Circuit Current: boost::optional<double>
    if (boost::optional<double> _shortCircuitCurrent = modelObject.shortCircuitCurrent()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::ShortCircuitCurrent, _shortCircuitCurrent.get());
    }

    // Open Circuit Voltage: boost::optional<double>
    if (boost::optional<double> _openCircuitVoltage = modelObject.openCircuitVoltage()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::OpenCircuitVoltage, _openCircuitVoltage.get());
    }

    // Current at Maximum Power Point: boost::optional<double>
    if (boost::optional<double> _currentatMaximumPowerPoint = modelObject.currentatMaximumPowerPoint()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::CurrentatMaximumPowerPoint, _currentatMaximumPowerPoint.get());
    }

    // Voltage at Maximum Power Point: boost::optional<double>
    if (boost::optional<double> _voltageatMaximumPowerPoint = modelObject.voltageatMaximumPowerPoint()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::VoltageatMaximumPowerPoint, _voltageatMaximumPowerPoint.get());
    }

    // Sandia Database Parameter aIsc: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameteraIsc = modelObject.sandiaDatabaseParameteraIsc()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraIsc, _sandiaDatabaseParameteraIsc.get());
    }

    // Sandia Database Parameter aImp: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameteraImp = modelObject.sandiaDatabaseParameteraImp()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraImp, _sandiaDatabaseParameteraImp.get());
    }

    // Sandia Database Parameter c0: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterc0 = modelObject.sandiaDatabaseParameterc0()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc0, _sandiaDatabaseParameterc0.get());
    }

    // Sandia Database Parameter c1: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterc1 = modelObject.sandiaDatabaseParameterc1()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc1, _sandiaDatabaseParameterc1.get());
    }

    // Sandia Database Parameter BVoc0: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterBVoc0 = modelObject.sandiaDatabaseParameterBVoc0()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVoc0, _sandiaDatabaseParameterBVoc0.get());
    }

    // Sandia Database Parameter mBVoc: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParametermBVoc = modelObject.sandiaDatabaseParametermBVoc()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVoc, _sandiaDatabaseParametermBVoc.get());
    }

    // Sandia Database Parameter BVmp0: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterBVmp0 = modelObject.sandiaDatabaseParameterBVmp0()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVmp0, _sandiaDatabaseParameterBVmp0.get());
    }

    // Sandia Database Parameter mBVmp: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParametermBVmp = modelObject.sandiaDatabaseParametermBVmp()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVmp, _sandiaDatabaseParametermBVmp.get());
    }

    // Diode Factor: boost::optional<double>
    if (boost::optional<double> _diodeFactor = modelObject.diodeFactor()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::DiodeFactor, _diodeFactor.get());
    }

    // Sandia Database Parameter c2: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterc2 = modelObject.sandiaDatabaseParameterc2()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc2, _sandiaDatabaseParameterc2.get());
    }

    // Sandia Database Parameter c3: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterc3 = modelObject.sandiaDatabaseParameterc3()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc3, _sandiaDatabaseParameterc3.get());
    }

    // Sandia Database Parameter a0: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParametera0 = modelObject.sandiaDatabaseParametera0()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera0, _sandiaDatabaseParametera0.get());
    }

    // Sandia Database Parameter a1: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParametera1 = modelObject.sandiaDatabaseParametera1()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera1, _sandiaDatabaseParametera1.get());
    }

    // Sandia Database Parameter a2: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParametera2 = modelObject.sandiaDatabaseParametera2()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera2, _sandiaDatabaseParametera2.get());
    }

    // Sandia Database Parameter a3: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParametera3 = modelObject.sandiaDatabaseParametera3()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera3, _sandiaDatabaseParametera3.get());
    }

    // Sandia Database Parameter a4: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParametera4 = modelObject.sandiaDatabaseParametera4()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera4, _sandiaDatabaseParametera4.get());
    }

    // Sandia Database Parameter b0: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterb0 = modelObject.sandiaDatabaseParameterb0()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb0, _sandiaDatabaseParameterb0.get());
    }

    // Sandia Database Parameter b1: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterb1 = modelObject.sandiaDatabaseParameterb1()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb1, _sandiaDatabaseParameterb1.get());
    }

    // Sandia Database Parameter b2: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterb2 = modelObject.sandiaDatabaseParameterb2()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb2, _sandiaDatabaseParameterb2.get());
    }

    // Sandia Database Parameter b3: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterb3 = modelObject.sandiaDatabaseParameterb3()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb3, _sandiaDatabaseParameterb3.get());
    }

    // Sandia Database Parameter b4: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterb4 = modelObject.sandiaDatabaseParameterb4()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb4, _sandiaDatabaseParameterb4.get());
    }

    // Sandia Database Parameter b5: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterb5 = modelObject.sandiaDatabaseParameterb5()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb5, _sandiaDatabaseParameterb5.get());
    }

    // Sandia Database Parameter Delta(Tc): boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterDeltaTc = modelObject.sandiaDatabaseParameterDeltaTc()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterDelta_Tc_, _sandiaDatabaseParameterDeltaTc.get());
    }

    // Sandia Database Parameter fd: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterfd = modelObject.sandiaDatabaseParameterfd()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterfd, _sandiaDatabaseParameterfd.get());
    }

    // Sandia Database Parameter a: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParametera = modelObject.sandiaDatabaseParametera()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera, _sandiaDatabaseParametera.get());
    }

    // Sandia Database Parameter b: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterb = modelObject.sandiaDatabaseParameterb()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb, _sandiaDatabaseParameterb.get());
    }

    // Sandia Database Parameter c4: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterc4 = modelObject.sandiaDatabaseParameterc4()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc4, _sandiaDatabaseParameterc4.get());
    }

    // Sandia Database Parameter c5: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterc5 = modelObject.sandiaDatabaseParameterc5()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc5, _sandiaDatabaseParameterc5.get());
    }

    // Sandia Database Parameter Ix0: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterIx0 = modelObject.sandiaDatabaseParameterIx0()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIx0, _sandiaDatabaseParameterIx0.get());
    }

    // Sandia Database Parameter Ixx0: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterIxx0 = modelObject.sandiaDatabaseParameterIxx0()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIxx0, _sandiaDatabaseParameterIxx0.get());
    }

    // Sandia Database Parameter c6: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterc6 = modelObject.sandiaDatabaseParameterc6()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc6, _sandiaDatabaseParameterc6.get());
    }

    // Sandia Database Parameter c7: boost::optional<double>
    if (boost::optional<double> _sandiaDatabaseParameterc7 = modelObject.sandiaDatabaseParameterc7()) {
      idfObject.setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc7, _sandiaDatabaseParameterc7.get());
    }

    result = idfObject;
    return result;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
