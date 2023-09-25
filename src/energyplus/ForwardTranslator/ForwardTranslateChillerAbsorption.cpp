/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ChillerAbsorption.hpp"
#include "../../model/ChillerAbsorption_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Chiller_Absorption_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateChillerAbsorption(ChillerAbsorption& modelObject) {
    IdfObject idfObject(IddObjectType::Chiller_Absorption);

    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // ChilledWaterInletNodeName
    if (auto mo = modelObject.supplyInletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(Chiller_AbsorptionFields::ChilledWaterInletNodeName, node->name().get());
      }
    }

    // ChilledWaterOutletNodeName
    if (auto mo = modelObject.supplyOutletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(Chiller_AbsorptionFields::ChilledWaterOutletNodeName, node->name().get());
      }
    }

    // CondenserInletNodeName
    if (auto mo = modelObject.demandInletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(Chiller_AbsorptionFields::CondenserInletNodeName, node->name().get());
      }
    }

    // CondenserOutletNodeName
    if (auto mo = modelObject.demandOutletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(Chiller_AbsorptionFields::CondenserOutletNodeName, node->name().get());
      }
    }

    if (auto mo = modelObject.tertiaryInletModelObject()) {
      idfObject.setString(Chiller_AbsorptionFields::GeneratorInletNodeName, mo->name().get());
    }

    if (auto mo = modelObject.tertiaryOutletModelObject()) {
      idfObject.setString(Chiller_AbsorptionFields::GeneratorOutletNodeName, mo->name().get());
    }

    if (modelObject.isNominalCapacityAutosized()) {
      idfObject.setString(Chiller_AbsorptionFields::NominalCapacity, "Autosize");
    } else if (auto value = modelObject.nominalCapacity()) {
      idfObject.setDouble(Chiller_AbsorptionFields::NominalCapacity, value.get());
    }

    if (modelObject.isNominalPumpingPowerAutosized()) {
      idfObject.setString(Chiller_AbsorptionFields::NominalPumpingPower, "Autosize");
    } else if (auto value = modelObject.nominalPumpingPower()) {
      idfObject.setDouble(Chiller_AbsorptionFields::NominalPumpingPower, value.get());
    }

    {
      auto value = modelObject.minimumPartLoadRatio();
      idfObject.setDouble(Chiller_AbsorptionFields::MinimumPartLoadRatio, value);
    }

    {
      auto value = modelObject.maximumPartLoadRatio();
      idfObject.setDouble(Chiller_AbsorptionFields::MaximumPartLoadRatio, value);
    }

    {
      auto value = modelObject.optimumPartLoadRatio();
      idfObject.setDouble(Chiller_AbsorptionFields::OptimumPartLoadRatio, value);
    }

    {
      auto value = modelObject.designCondenserInletTemperature();
      idfObject.setDouble(Chiller_AbsorptionFields::DesignCondenserInletTemperature, value);
    }

    if (modelObject.isDesignChilledWaterFlowRateAutosized()) {
      idfObject.setString(Chiller_AbsorptionFields::DesignChilledWaterFlowRate, "Autosize");
    } else if (auto value = modelObject.designChilledWaterFlowRate()) {
      idfObject.setDouble(Chiller_AbsorptionFields::DesignChilledWaterFlowRate, value.get());
    }

    if (modelObject.isDesignCondenserWaterFlowRateAutosized()) {
      idfObject.setString(Chiller_AbsorptionFields::DesignCondenserWaterFlowRate, "Autosize");
    } else if (auto value = modelObject.designCondenserWaterFlowRate()) {
      idfObject.setDouble(Chiller_AbsorptionFields::DesignCondenserWaterFlowRate, value.get());
    }

    {
      auto value = modelObject.coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve();
      idfObject.setDouble(Chiller_AbsorptionFields::Coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve, value);
    }

    {
      auto value = modelObject.coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve();
      idfObject.setDouble(Chiller_AbsorptionFields::Coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve, value);
    }

    {
      auto value = modelObject.coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve();
      idfObject.setDouble(Chiller_AbsorptionFields::Coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve, value);
    }

    {
      auto value = modelObject.coefficient1ofthePumpElectricUsePartLoadRatioCurve();
      idfObject.setDouble(Chiller_AbsorptionFields::Coefficient1ofthePumpElectricUsePartLoadRatioCurve, value);
    }

    {
      auto value = modelObject.coefficient2ofthePumpElectricUsePartLoadRatioCurve();
      idfObject.setDouble(Chiller_AbsorptionFields::Coefficient2ofthePumpElectricUsePartLoadRatioCurve, value);
    }

    {
      auto value = modelObject.coefficient3ofthePumpElectricUsePartLoadRatioCurve();
      idfObject.setDouble(Chiller_AbsorptionFields::Coefficient3ofthePumpElectricUsePartLoadRatioCurve, value);
    }

    {
      auto value = modelObject.chilledWaterOutletTemperatureLowerLimit();
      idfObject.setDouble(Chiller_AbsorptionFields::ChilledWaterOutletTemperatureLowerLimit, value);
    }

    {
      auto value = modelObject.chillerFlowMode();
      idfObject.setString(Chiller_AbsorptionFields::ChillerFlowMode, value);
    }

    {
      auto value = modelObject.generatorHeatSourceType();
      idfObject.setString(Chiller_AbsorptionFields::GeneratorHeatSourceType, value);
    }

    if (modelObject.generatorLoop()) {
      if (modelObject.isDesignGeneratorFluidFlowRateAutosized()) {
        idfObject.setString(Chiller_AbsorptionFields::DesignGeneratorFluidFlowRate, "Autosize");
      } else if (auto value = modelObject.designGeneratorFluidFlowRate()) {
        idfObject.setDouble(Chiller_AbsorptionFields::DesignGeneratorFluidFlowRate, value.get());
      }
    } else {
      idfObject.setString(Chiller_AbsorptionFields::DesignGeneratorFluidFlowRate, "");
    }

    {
      auto value = modelObject.degreeofSubcoolinginSteamGenerator();
      idfObject.setDouble(Chiller_AbsorptionFields::DegreeofSubcoolinginSteamGenerator, value);
    }

    {
      auto value = modelObject.sizingFactor();
      idfObject.setDouble(Chiller_AbsorptionFields::SizingFactor, value);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
