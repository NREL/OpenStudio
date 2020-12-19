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
#include "../../model/CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "../../model/CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"

#include "../../model/Model.hpp"

#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/CoilSystem_IntegratedHeatPump_AirSource_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateCoilSystemIntegratedHeatPumpAirSource(CoilSystemIntegratedHeatPumpAirSource& modelObject) {
    IdfObject idfObject(IddObjectType::CoilSystem_IntegratedHeatPump_AirSource);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // Supply Hot Water Flow Sensor Node Name

    // Space Cooling Coil Name

    // Space Heating Coil Name

    // Dedicated Water Heating Coil Name

    // SCWH Coil Name

    // SCDWH Cooling Coil Name

    // SCDWH Water Heating Coil Name

    // SHDWH Heating Coil Name

    // SHDWH Water Heating Coil Name

    // Enhanced Dehumidification Cooling Coil Name

    // Grid Response Cooling Coil Name

    // Grid Response Space Heating Coil Name

    // Indoor Temperature Limit for SCWH Mode

    // Ambient Temperature Limit for SCWH Mode

    // Indoor Temperature above Which WH has Higher Priority

    // Ambient Temperature above Which WH has Higher Priority

    // Flag to Indicate Load Control in SCWH Mode

    // Minimum Speed Level for SCWH Mode

    // Maximum Water Flow Volume before Switching from SCDWH to SCWH Mode

    // Minimum Speed Level for SCDWH Mode

    // Maximum Running Time before Allowing Electric Resistance Heat Use during SHDWH Mode

    // Minimum Speed Level for SHDWH Mode

    // Sizing Ratio of Space Heating Coil to Space Cooling Coil

    // Sizing Ratio of Dedicated Water Heating Coil to Space Cooling Coil

    // Sizing Ratio of Combined Space Cooling and Water Heating Coil with Full Condensing to Space Cooling Coil

    // Sizing Ratio of Combined Space Cooling and Water Heating Coil with Desuperheating - Cooling Capacity to Space Cooling Coil

    // Sizing Ratio of Combined Space Cooling and Water Heating Coil with Desuperheating - Water Heating Capacity to Space Cooling Coil

    // Sizing Ratio of Combined Space Heating and Water Heating Coil with Desuperheating - Space Heating Capacity to Space Cooling Coil

    // Sizing Ratio of Combined Space Heating and Water Heating Coil with Desuperheating - Water Heating Capacity to Space Cooling Coil

    // Sizing Ratio of Enhanced Dehumidification Coil to Space Cooling Coil

    // Sizing Ratio of Grid Response Cooling Coil to Space Cooling Coil

    // Sizing Ratio of Grid Response Heating Coil to Space Cooling Coil

    // Chiller Coil Name

    // Chiller Coil Belongs to a Single or Separate Unit

    // Chiller Compressor Run Speed

    // Sizing Ratio of Chiller Coil to Space Cooling Coil

    // Coil Object Type

    // Coil Object Name

    // Air Flow Ratio of Water Coil to the Space Cooling Coil

    // Water Flow Ratio of Water Coil to the Chiller Coil

    // Tank Object Type

    // Tank Name

    // Ice Fraction below which charging starts

    // Chiller Entering Temperature at 0 Tank Fraction

    // Temperature Deviation Curve Name, as a Function of the Tank Fraction

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
