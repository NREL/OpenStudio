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

#include "../ReverseTranslator.hpp"

#include "../../model/PhotovoltaicPerformanceSandia.hpp"

#include <utilities/idd/PhotovoltaicPerformance_Sandia_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translatePhotovoltaicPerformanceSandia(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;

    // Instantiate an object of the class to store the values,
    // but we don't return it until we know it's ok
    // TODO: check constructor, it might need other objects
    openstudio::model::PhotovoltaicPerformanceSandia modelObject(m_model);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Active Area: Optional Double
    if (boost::optional<double> _activeArea = workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::ActiveArea)) {
      modelObject.setActiveArea(_activeArea.get());
    }

    // Number of Cells in Series: Optional Integer
    if (boost::optional<int> _numberofCellsinSeries = workspaceObject.getInt(PhotovoltaicPerformance_SandiaFields::NumberofCellsinSeries)) {
      modelObject.setNumberofCellsinSeries(_numberofCellsinSeries.get());
    }

    // Number of Cells in Parallel: Optional Integer
    if (boost::optional<int> _numberofCellsinParallel = workspaceObject.getInt(PhotovoltaicPerformance_SandiaFields::NumberofCellsinParallel)) {
      modelObject.setNumberofCellsinParallel(_numberofCellsinParallel.get());
    }

    // Short Circuit Current: Optional Double
    if (boost::optional<double> _shortCircuitCurrent = workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::ShortCircuitCurrent)) {
      modelObject.setShortCircuitCurrent(_shortCircuitCurrent.get());
    }

    // Open Circuit Voltage: Optional Double
    if (boost::optional<double> _openCircuitVoltage = workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::OpenCircuitVoltage)) {
      modelObject.setOpenCircuitVoltage(_openCircuitVoltage.get());
    }

    // Current at Maximum Power Point: Optional Double
    if (boost::optional<double> _currentatMaximumPowerPoint =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::CurrentatMaximumPowerPoint)) {
      modelObject.setCurrentatMaximumPowerPoint(_currentatMaximumPowerPoint.get());
    }

    // Voltage at Maximum Power Point: Optional Double
    if (boost::optional<double> _voltageatMaximumPowerPoint =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::VoltageatMaximumPowerPoint)) {
      modelObject.setVoltageatMaximumPowerPoint(_voltageatMaximumPowerPoint.get());
    }

    // Sandia Database Parameter aIsc: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameteraIsc =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraIsc)) {
      modelObject.setSandiaDatabaseParameteraIsc(_sandiaDatabaseParameteraIsc.get());
    }

    // Sandia Database Parameter aImp: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameteraImp =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraImp)) {
      modelObject.setSandiaDatabaseParameteraImp(_sandiaDatabaseParameteraImp.get());
    }

    // Sandia Database Parameter c0: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterc0 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc0)) {
      modelObject.setSandiaDatabaseParameterc0(_sandiaDatabaseParameterc0.get());
    }

    // Sandia Database Parameter c1: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterc1 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc1)) {
      modelObject.setSandiaDatabaseParameterc1(_sandiaDatabaseParameterc1.get());
    }

    // Sandia Database Parameter BVoc0: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterBVoc0 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVoc0)) {
      modelObject.setSandiaDatabaseParameterBVoc0(_sandiaDatabaseParameterBVoc0.get());
    }

    // Sandia Database Parameter mBVoc: Optional Double
    if (boost::optional<double> _sandiaDatabaseParametermBVoc =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVoc)) {
      modelObject.setSandiaDatabaseParametermBVoc(_sandiaDatabaseParametermBVoc.get());
    }

    // Sandia Database Parameter BVmp0: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterBVmp0 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVmp0)) {
      modelObject.setSandiaDatabaseParameterBVmp0(_sandiaDatabaseParameterBVmp0.get());
    }

    // Sandia Database Parameter mBVmp: Optional Double
    if (boost::optional<double> _sandiaDatabaseParametermBVmp =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVmp)) {
      modelObject.setSandiaDatabaseParametermBVmp(_sandiaDatabaseParametermBVmp.get());
    }

    // Diode Factor: Optional Double
    if (boost::optional<double> _diodeFactor = workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::DiodeFactor)) {
      modelObject.setDiodeFactor(_diodeFactor.get());
    }

    // Sandia Database Parameter c2: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterc2 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc2)) {
      modelObject.setSandiaDatabaseParameterc2(_sandiaDatabaseParameterc2.get());
    }

    // Sandia Database Parameter c3: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterc3 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc3)) {
      modelObject.setSandiaDatabaseParameterc3(_sandiaDatabaseParameterc3.get());
    }

    // Sandia Database Parameter a0: Optional Double
    if (boost::optional<double> _sandiaDatabaseParametera0 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera0)) {
      modelObject.setSandiaDatabaseParametera0(_sandiaDatabaseParametera0.get());
    }

    // Sandia Database Parameter a1: Optional Double
    if (boost::optional<double> _sandiaDatabaseParametera1 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera1)) {
      modelObject.setSandiaDatabaseParametera1(_sandiaDatabaseParametera1.get());
    }

    // Sandia Database Parameter a2: Optional Double
    if (boost::optional<double> _sandiaDatabaseParametera2 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera2)) {
      modelObject.setSandiaDatabaseParametera2(_sandiaDatabaseParametera2.get());
    }

    // Sandia Database Parameter a3: Optional Double
    if (boost::optional<double> _sandiaDatabaseParametera3 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera3)) {
      modelObject.setSandiaDatabaseParametera3(_sandiaDatabaseParametera3.get());
    }

    // Sandia Database Parameter a4: Optional Double
    if (boost::optional<double> _sandiaDatabaseParametera4 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera4)) {
      modelObject.setSandiaDatabaseParametera4(_sandiaDatabaseParametera4.get());
    }

    // Sandia Database Parameter b0: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterb0 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb0)) {
      modelObject.setSandiaDatabaseParameterb0(_sandiaDatabaseParameterb0.get());
    }

    // Sandia Database Parameter b1: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterb1 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb1)) {
      modelObject.setSandiaDatabaseParameterb1(_sandiaDatabaseParameterb1.get());
    }

    // Sandia Database Parameter b2: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterb2 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb2)) {
      modelObject.setSandiaDatabaseParameterb2(_sandiaDatabaseParameterb2.get());
    }

    // Sandia Database Parameter b3: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterb3 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb3)) {
      modelObject.setSandiaDatabaseParameterb3(_sandiaDatabaseParameterb3.get());
    }

    // Sandia Database Parameter b4: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterb4 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb4)) {
      modelObject.setSandiaDatabaseParameterb4(_sandiaDatabaseParameterb4.get());
    }

    // Sandia Database Parameter b5: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterb5 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb5)) {
      modelObject.setSandiaDatabaseParameterb5(_sandiaDatabaseParameterb5.get());
    }

    // Sandia Database Parameter Delta(Tc): Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterDeltaTc =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterDelta_Tc_)) {
      modelObject.setSandiaDatabaseParameterDeltaTc(_sandiaDatabaseParameterDeltaTc.get());
    }

    // Sandia Database Parameter fd: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterfd =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterfd)) {
      modelObject.setSandiaDatabaseParameterfd(_sandiaDatabaseParameterfd.get());
    }

    // Sandia Database Parameter a: Optional Double
    if (boost::optional<double> _sandiaDatabaseParametera =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera)) {
      modelObject.setSandiaDatabaseParametera(_sandiaDatabaseParametera.get());
    }

    // Sandia Database Parameter b: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterb =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb)) {
      modelObject.setSandiaDatabaseParameterb(_sandiaDatabaseParameterb.get());
    }

    // Sandia Database Parameter c4: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterc4 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc4)) {
      modelObject.setSandiaDatabaseParameterc4(_sandiaDatabaseParameterc4.get());
    }

    // Sandia Database Parameter c5: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterc5 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc5)) {
      modelObject.setSandiaDatabaseParameterc5(_sandiaDatabaseParameterc5.get());
    }

    // Sandia Database Parameter Ix0: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterIx0 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIx0)) {
      modelObject.setSandiaDatabaseParameterIx0(_sandiaDatabaseParameterIx0.get());
    }

    // Sandia Database Parameter Ixx0: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterIxx0 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIxx0)) {
      modelObject.setSandiaDatabaseParameterIxx0(_sandiaDatabaseParameterIxx0.get());
    }

    // Sandia Database Parameter c6: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterc6 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc6)) {
      modelObject.setSandiaDatabaseParameterc6(_sandiaDatabaseParameterc6.get());
    }

    // Sandia Database Parameter c7: Optional Double
    if (boost::optional<double> _sandiaDatabaseParameterc7 =
          workspaceObject.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc7)) {
      modelObject.setSandiaDatabaseParameterc7(_sandiaDatabaseParameterc7.get());
    }

    result = modelObject;
    return result;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
