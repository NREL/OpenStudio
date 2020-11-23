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

#include "../../model/SolarCollectorPerformanceIntegralCollectorStorage.hpp"

#include <utilities/idd/SolarCollectorPerformance_IntegralCollectorStorage_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSolarCollectorPerformanceIntegralCollectorStorage(
    model::SolarCollectorPerformanceIntegralCollectorStorage& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::SolarCollectorPerformance_IntegralCollectorStorage);

    m_idfObjects.push_back(idfObject);

    boost::optional<double> d;

    std::string name = modelObject.name().get();
    idfObject.setName(name);

    idfObject.setString(SolarCollectorPerformance_IntegralCollectorStorageFields::ICSCollectorType, modelObject.iCSCollectorType());

    d = modelObject.grossArea();
    if (d) {
      idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::GrossArea, *d);
    }

    d = modelObject.collectorWaterVolume();
    if (d) {
      idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::CollectorWaterVolume, *d);
    }

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::BottomHeatLossConductance, modelObject.bottomHeatLossConductance());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::SideHeatLossConductance, modelObject.sideHeatLossConductance());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::AspectRatio, modelObject.aspectRatio());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::CollectorSideHeight, modelObject.collectorSideHeight());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::ThermalMassofAbsorberPlate,
                        modelObject.thermalMassOfAbsorberPlate());

    idfObject.setInt(SolarCollectorPerformance_IntegralCollectorStorageFields::NumberofCovers, modelObject.numberOfCovers());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::CoverSpacing, modelObject.coverSpacing());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::RefractiveIndexofOuterCover,
                        modelObject.refractiveIndexOfOuterCover());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::ExtinctionCoefficientTimesThicknessofOuterCover,
                        modelObject.extinctionCoefficientTimesThicknessOfOuterCover());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofOuterCover, modelObject.emissivityOfOuterCover());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::RefractiveIndexofInnerCover,
                        modelObject.refractiveIndexOfInnerCover());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::ExtinctionCoefficientTimesThicknessoftheinnerCover,
                        modelObject.extinctionCoefficientTimesThicknessOfTheInnerCover());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofInnerCover, modelObject.emissivityOfInnerCover());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::AbsorptanceofAbsorberPlate,
                        modelObject.absorptanceOfAbsorberPlate());

    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofAbsorberPlate, modelObject.emissivityOfAbsorberPlate());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
