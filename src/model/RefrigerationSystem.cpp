/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "RefrigerationSystem.hpp"
#include "RefrigerationSystem_Impl.hpp"

#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "RefrigerationAirChiller.hpp"
#include "RefrigerationAirChiller_Impl.hpp"
#include "RefrigerationCondenserCascade.hpp"
#include "RefrigerationCondenserCascade_Impl.hpp"
#include "RefrigerationCase.hpp"
#include "RefrigerationCase_Impl.hpp"
#include "RefrigerationCompressor.hpp"
#include "RefrigerationCompressor_Impl.hpp"
#include "RefrigerationSecondarySystem.hpp"
#include "RefrigerationSecondarySystem_Impl.hpp"
#include "RefrigerationWalkIn.hpp"
#include "RefrigerationWalkIn_Impl.hpp"
#include "RefrigerationSubcoolerLiquidSuction.hpp"
#include "RefrigerationSubcoolerLiquidSuction_Impl.hpp"
#include "RefrigerationSubcoolerMechanical.hpp"
#include "RefrigerationSubcoolerMechanical_Impl.hpp"
#include "RefrigerationCondenserAirCooled.hpp"
#include "RefrigerationCondenserAirCooled_Impl.hpp"
#include "RefrigerationCondenserWaterCooled.hpp"
#include "RefrigerationCondenserWaterCooled_Impl.hpp"
#include "RefrigerationCondenserEvaporativeCooled.hpp"
#include "RefrigerationCondenserEvaporativeCooled_Impl.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_System_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/ContainersMove.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    RefrigerationSystem_Impl::RefrigerationSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == RefrigerationSystem::iddObjectType());
    }

    RefrigerationSystem_Impl::RefrigerationSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == RefrigerationSystem::iddObjectType());
    }

    RefrigerationSystem_Impl::RefrigerationSystem_Impl(const RefrigerationSystem_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& RefrigerationSystem_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        // TODO: Implement checks
        // THE FOLLOWING OUTPUTS ARE AVAILABLE FOR SYSTEMS THAT SERVE CASES AND/OR WALKINS:
        "Refrigeration System Total Compressor Electricity Rate", "Refrigeration System Total Compressor Electricity Energy",
        "Refrigeration System Average Compressor COP", "Refrigeration System Total Compressor Heat Transfer Rate",
        "Refrigeration System Total Compressor Heat Transfer Energy", "Refrigeration System Total Cases and Walk Ins Heat Transfer Rate",
        "Refrigeration System Total Cases and Walk Ins Heat Transfer Energy", "Refrigeration System Total Transferred Load Heat Transfer Rate",
        "Refrigeration System Total Transferred Load Heat Transfer Energy", "Refrigeration System Total Suction Pipe Heat Gain Rate",
        "Refrigeration System Total Suction Pipe Heat Gain Energy", "Refrigeration System Net Rejected Heat Transfer Rate",
        "Refrigeration System Net Rejected Heat Transfer Energy", "Refrigeration System Estimated Refrigerant Inventory Mass",
        "Refrigeration System Liquid Suction Subcooler Heat Transfer Rate", "Refrigeration System Liquid Suction Subcooler Heat Transfer Energy",
        "Refrigeration System Estimated Refrigerant Inventory", "Refrigeration System Estimated Refrigerant Mass Flow Rate",
        "Refrigeration System Condensing Temperature", "Refrigeration System Evaporating Temperature",
        "Refrigeration System Suction Pipe Suction Temperature", "Refrigeration System Thermostatic Expansion Valve Liquid Temperature",
        "Refrigeration System Estimated High Stage Refrigerant Mass Flow Rate", "Refrigeration System Total High Stage Compressor Electricity Energy",
        "Refrigeration System Total High Stage Compressor Electricity Rate", "Refrigeration System Total High Stage Compressor Heat Transfer Energy",
        "Refrigeration System Total High Stage Compressor Heat Transfer Rate",
        "Refrigeration System Total Low and High Stage Compressor Electricity Energy",
        "Refrigeration System Total Low Stage Compressor Electricity Energy", "Refrigeration System Total Low Stage Compressor Electricity Rate",
        "Refrigeration System Total Low Stage Compressor Heat Transfer Energy", "Refrigeration System Total Low Stage Compressor Heat Transfer Rate",
        "Refrigeration System Estimated Low Stage Refrigerant Mass Flow Rate",
        // THE FOLLOWING OUTPUTS ARE AVAILABLE FOR SYSTEMS THAT SERVE AIR CHILLERS:
        "Refrigeration Air Chiller System Total Compressor Electricity Rate", "Refrigeration Air Chiller System Total Compressor Electricity Energy",
        "Refrigeration Air Chiller System Average Compressor COP", "Refrigeration Air Chiller System Total Compressor Heat Transfer Rate",
        "Refrigeration Air Chiller System Total Compressor Heat Transfer Energy",
        "Refrigeration Air Chiller System Total Air Chiller Heat Transfer Rate",
        "Refrigeration Air Chiller System Total Case and Walk In Heat Transfer Energy",
        "Refrigeration Air Chiller System Total Transferred Load Heat Transfer Rate",
        "Refrigeration Air Chiller System Total Transferred Load Heat Transfer Energy",
        "Refrigeration Air Chiller System Total Suction Pipe Heat Gain Rate", "Refrigeration Air Chiller System Total Suction Pipe Heat Gain Energy",
        "Refrigeration Air Chiller System Net Rejected Heat Transfer Rate", "Refrigeration Air Chiller System Net Rejected Heat Transfer Energy",
        "Refrigeration Air Chiller System Liquid Suction Subcooler Heat Transfer Rate",
        "Refrigeration Air Chiller System Liquid Suction Subcooler Heat Transfer Energy",
        "Refrigeration Air Chiller System Estimated Refrigerant Inventory Mass",
        "Refrigeration Air Chiller System Estimated Refrigerant Mass Flow Rate", "Refrigeration Air Chiller System Intercooler Temperature",
        "Refrigeration Air Chiller System Intercooler Pressure", "Refrigeration Air Chiller System Condensing Temperature",
        "Refrigeration Air Chiller System Evaporating Temperature", "Refrigeration Air Chiller System Suction Temperature",
        "Refrigeration Air Chiller System TXV Liquid Temperature", "Refrigeration Air Chiller System Estimated High Stage Refrigerant Mass Flow Rate",
        "Refrigeration Air Chiller System Total High Stage Compressor Electricity Energy",
        "Refrigeration Air Chiller System Total High Stage Compressor Electricity Rate",
        "Refrigeration Air Chiller System Total High Stage Compressor Heat Transfer Energy",
        "Refrigeration Air Chiller System Total High Stage Compressor Heat Transfer Rate",
        "Refrigeration Air Chiller System Total Low and High Stage Compressor Electricity Energy",
        "Refrigeration Air Chiller System Total Low Stage Compressor Electricity Energy",
        "Refrigeration Air Chiller System Total Low Stage Compressor Electricity Rate",
        "Refrigeration Air Chiller System Total Low Stage Compressor Heat Transfer Energy",
        "Refrigeration Air Chiller System Total Low Stage Compressor Heat Transfer Rate",
        "Refrigeration Air Chiller System Estimated Low Stage Refrigerant Mass Flow Rate"};
      return result;
    }

    IddObjectType RefrigerationSystem_Impl::iddObjectType() const {
      return RefrigerationSystem::iddObjectType();
    }

    std::vector<IdfObject> RefrigerationSystem_Impl::remove() {
      std::vector<IdfObject> result;

      if (boost::optional<ModelObject> condenser = this->optionalRefrigerationCondenser()) {
        openstudio::detail::concat_helper(result, condenser->remove());
      }

      if (boost::optional<ModelObjectList> caseAndWalkinList = this->refrigeratedCaseAndWalkInList()) {
        openstudio::detail::concat_helper(result, caseAndWalkinList->remove());
      }

      if (boost::optional<ModelObjectList> transferLoadList = this->refrigerationTransferLoadList()) {
        openstudio::detail::concat_helper(result, transferLoadList->remove());
      }

      // We're clearing the compressor/HighStage compressor list objects first. otherwise a compressor on the high stage list will call
      // RefrgerationCompressor_Impl::system which will try to locate the compressorList which was deleted first
      for (auto& c : compressors()) {
        openstudio::detail::concat_helper(result, c.remove());
      }
      for (auto& c : highStageCompressors()) {
        openstudio::detail::concat_helper(result, c.remove());
      }
      if (boost::optional<ModelObjectList> compressorList = this->compressorList()) {
        openstudio::detail::concat_helper(result, compressorList->remove());
      }
      if (boost::optional<ModelObjectList> highStageCompressorList = this->highStageCompressorList()) {
        openstudio::detail::concat_helper(result, highStageCompressorList->remove());
      }

      if (boost::optional<RefrigerationSubcoolerMechanical> mechSubcooler = this->mechanicalSubcooler()) {
        openstudio::detail::concat_helper(result, mechSubcooler->remove());
      }

      if (boost::optional<RefrigerationSubcoolerLiquidSuction> liqSuctionSubcooler = this->liquidSuctionHeatExchangerSubcooler()) {
        openstudio::detail::concat_helper(result, liqSuctionSubcooler->remove());
      }

      openstudio::detail::concat_helper(result, ModelObject_Impl::remove());

      return result;
    }

    ModelObject RefrigerationSystem_Impl::clone(Model model) const {
      auto modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationSystem>();

      if (boost::optional<ModelObject> condenser = this->optionalRefrigerationCondenser()) {
        ModelObject condenserClone = condenser->clone(model);
        modelObjectClone.setRefrigerationCondenser(condenserClone);
      }

      if (boost::optional<ModelObjectList> caseAndWalkinList = this->refrigeratedCaseAndWalkInList()) {
        auto caseAndWalkinListClone = caseAndWalkinList->clone(model).cast<ModelObjectList>();
        modelObjectClone.getImpl<detail::RefrigerationSystem_Impl>()->setRefrigeratedCaseAndWalkInList(caseAndWalkinListClone);
      }

      if (boost::optional<ModelObjectList> transferLoadList = this->refrigerationTransferLoadList()) {
        auto transferLoadListClone = transferLoadList->clone(model).cast<ModelObjectList>();
        modelObjectClone.getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationTransferLoadList(transferLoadListClone);
      }

      if (boost::optional<ModelObjectList> compressorList = this->compressorList()) {
        auto compressorListClone = compressorList->clone(model).cast<ModelObjectList>();
        modelObjectClone.getImpl<detail::RefrigerationSystem_Impl>()->setCompressorList(compressorListClone);
      }

      if (boost::optional<RefrigerationSubcoolerMechanical> mechSubcooler = this->mechanicalSubcooler()) {
        auto mechSubClone = mechSubcooler->clone(model).cast<RefrigerationSubcoolerMechanical>();
        modelObjectClone.setMechanicalSubcooler(mechSubClone);
      }

      if (boost::optional<RefrigerationSubcoolerLiquidSuction> liqSuctionSubcooler = this->liquidSuctionHeatExchangerSubcooler()) {
        auto liqSuctionSubClone = liqSuctionSubcooler->clone(model).cast<RefrigerationSubcoolerLiquidSuction>();
        modelObjectClone.setLiquidSuctionHeatExchangerSubcooler(liqSuctionSubClone);
      }

      if (boost::optional<ModelObjectList> highStageCompressorList = this->highStageCompressorList()) {
        auto highStageCompressorListClone = highStageCompressorList->clone(model).cast<ModelObjectList>();
        modelObjectClone.getImpl<detail::RefrigerationSystem_Impl>()->setHighStageCompressorList(highStageCompressorListClone);
      }

      modelObjectClone.resetSuctionPipingZone();

      return std::move(modelObjectClone);
    }

    template <class T>
    std::vector<T> RefrigerationSystem_Impl::listTemplate(const boost::optional<ModelObjectList>& modelObjectList) const {
      std::vector<T> result;

      if (modelObjectList) {
        std::vector<ModelObject> modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          boost::optional<T> modelObject = elem.optionalCast<T>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
        }
      }

      return result;
    }

    std::vector<RefrigerationCase> RefrigerationSystem_Impl::cases() const {
      return RefrigerationSystem_Impl::listTemplate<RefrigerationCase>(refrigeratedCaseAndWalkInList());
    }

    std::vector<RefrigerationWalkIn> RefrigerationSystem_Impl::walkins() const {
      return RefrigerationSystem_Impl::listTemplate<RefrigerationWalkIn>(refrigeratedCaseAndWalkInList());
    }

    std::vector<RefrigerationCompressor> RefrigerationSystem_Impl::compressors() const {
      return RefrigerationSystem_Impl::listTemplate<RefrigerationCompressor>(compressorList());
    }

    std::vector<RefrigerationCompressor> RefrigerationSystem_Impl::highStageCompressors() const {
      return RefrigerationSystem_Impl::listTemplate<RefrigerationCompressor>(highStageCompressorList());
    }

    std::vector<RefrigerationSecondarySystem> RefrigerationSystem_Impl::secondarySystemLoads() const {
      return RefrigerationSystem_Impl::listTemplate<RefrigerationSecondarySystem>(refrigerationTransferLoadList());
    }

    std::vector<RefrigerationCondenserCascade> RefrigerationSystem_Impl::cascadeCondenserLoads() const {
      return RefrigerationSystem_Impl::listTemplate<RefrigerationCondenserCascade>(refrigerationTransferLoadList());
    }

    std::vector<RefrigerationAirChiller> RefrigerationSystem_Impl::airChillers() const {
      return RefrigerationSystem_Impl::listTemplate<RefrigerationAirChiller>(refrigeratedCaseAndWalkInList());
    }

    boost::optional<ModelObjectList> RefrigerationSystem_Impl::refrigeratedCaseAndWalkInList() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_SystemFields::RefrigeratedCaseAndWalkInListName);
    }

    boost::optional<ModelObjectList> RefrigerationSystem_Impl::refrigerationTransferLoadList() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_SystemFields::RefrigerationTransferLoadListName);
    }

    boost::optional<ModelObject> RefrigerationSystem_Impl::refrigerationCondenser() const {
      return optionalRefrigerationCondenser();
    }

    ModelObjectList RefrigerationSystem_Impl::compressorList() const {
      boost::optional<ModelObjectList> value = optionalCompressorList();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Compressor List attached.");
      }
      return value.get();
    }

    double RefrigerationSystem_Impl::minimumCondensingTemperature() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_SystemFields::MinimumCondensingTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string RefrigerationSystem_Impl::refrigerationSystemWorkingFluidType() const {
      boost::optional<std::string> value = getString(OS_Refrigeration_SystemFields::RefrigerationSystemWorkingFluidType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string RefrigerationSystem_Impl::suctionTemperatureControlType() const {
      boost::optional<std::string> value = getString(OS_Refrigeration_SystemFields::SuctionTemperatureControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationSystem_Impl::isSuctionTemperatureControlTypeDefaulted() const {
      return isEmpty(OS_Refrigeration_SystemFields::SuctionTemperatureControlType);
    }

    boost::optional<RefrigerationSubcoolerMechanical> RefrigerationSystem_Impl::mechanicalSubcooler() const {
      return getObject<ModelObject>().getModelObjectTarget<RefrigerationSubcoolerMechanical>(OS_Refrigeration_SystemFields::MechanicalSubcoolerName);
    }

    boost::optional<RefrigerationSubcoolerLiquidSuction> RefrigerationSystem_Impl::liquidSuctionHeatExchangerSubcooler() const {
      return getObject<ModelObject>().getModelObjectTarget<RefrigerationSubcoolerLiquidSuction>(
        OS_Refrigeration_SystemFields::LiquidSuctionHeatExchangerSubcoolerName);
    }

    double RefrigerationSystem_Impl::sumUASuctionPiping() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_SystemFields::SumUASuctionPiping, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationSystem_Impl::isSumUASuctionPipingDefaulted() const {
      return isEmpty(OS_Refrigeration_SystemFields::SumUASuctionPiping);
    }

    boost::optional<ThermalZone> RefrigerationSystem_Impl::suctionPipingZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Refrigeration_SystemFields::SuctionPipingZoneName);
    }

    std::string RefrigerationSystem_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_Refrigeration_SystemFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationSystem_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_Refrigeration_SystemFields::EndUseSubcategory);
    }

    std::string RefrigerationSystem_Impl::numberofCompressorStages() const {
      if (this->highStageCompressors().empty()) {
        return "1";
      } else {
        return "2";
      }
    }

    std::string RefrigerationSystem_Impl::intercoolerType() const {
      boost::optional<std::string> value = getString(OS_Refrigeration_SystemFields::IntercoolerType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationSystem_Impl::isIntercoolerTypeDefaulted() const {
      return isEmpty(OS_Refrigeration_SystemFields::IntercoolerType);
    }

    double RefrigerationSystem_Impl::shellandCoilIntercoolerEffectiveness() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationSystem_Impl::isShellandCoilIntercoolerEffectivenessDefaulted() const {
      return isEmpty(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness);
    }

    boost::optional<ModelObjectList> RefrigerationSystem_Impl::highStageCompressorList() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_SystemFields::HighStageCompressorListName);
    }

    template <class T>
    void RefrigerationSystem_Impl::removeAllTemplate(boost::optional<ModelObjectList>& modelObjectList) {
      if (modelObjectList) {
        std::vector<ModelObject> modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          boost::optional<T> modelObject = elem.optionalCast<T>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
        }
      }
    }

    template <class T>
    void RefrigerationSystem_Impl::removeTemplate(const T& modelObject, boost::optional<ModelObjectList>& modelObjectList) {
      if (modelObjectList) {
        modelObjectList->removeModelObject(modelObject);
      }
    }

    template <class T>
    bool RefrigerationSystem_Impl::addTemplate(const T& modelObject, boost::optional<ModelObjectList>& modelObjectList) {
      if (modelObjectList) {
        return modelObjectList->addModelObject(modelObject);
      }
      return false;
    }

    bool RefrigerationSystem_Impl::addCase(const RefrigerationCase& refrigerationCase) {
      // From 9.2 IO ref:
      // > This list may contain a combination of case and walk-in names OR a list of air chiller names.
      // > Air chillers may not be included in any list that also includes cases or walk-ins.
      if (!this->airChillers().empty()) {
        LOG(Warn, "You cannot mix RefigerationCase/RefrigerationWalkins with RefrigerationAirChillers, occurred for " << briefDescription());
        return false;
      }

      // Enforce unicity
      if (boost::optional<RefrigerationSystem> currentSystem = refrigerationCase.system()) {
        LOG(Warn, refrigerationCase.briefDescription()
                    << " was removed from its existing RefrigerationSystem named '" << currentSystem->nameString() << "'.");
        currentSystem->removeCase(refrigerationCase);
      }

      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      return addTemplate<RefrigerationCase>(refrigerationCase, modelObjectList);
    }

    void RefrigerationSystem_Impl::removeCase(const RefrigerationCase& refrigerationCase) {
      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      removeTemplate<RefrigerationCase>(refrigerationCase, modelObjectList);
    }

    void RefrigerationSystem_Impl::removeAllCases() {
      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      removeAllTemplate<RefrigerationCase>(modelObjectList);
    }

    bool RefrigerationSystem_Impl::addWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
      // From 9.2 IO ref:
      // > This list may contain a combination of case and walk-in names OR a list of air chiller names.
      // > Air chillers may not be included in any list that also includes cases or walk-ins.
      if (!this->airChillers().empty()) {
        LOG(Warn, "You cannot mix RefigerationCase/RefrigerationWalkins with RefrigerationAirChillers, occurred for " << briefDescription());
        return false;
      }

      // Enforce unicity
      if (boost::optional<RefrigerationSystem> currentSystem = refrigerationWalkin.system()) {
        LOG(Warn, refrigerationWalkin.briefDescription()
                    << " was removed from its existing RefrigerationSystem named '" << currentSystem->nameString() << "'.");
        currentSystem->removeWalkin(refrigerationWalkin);
      }

      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      return addTemplate<RefrigerationWalkIn>(refrigerationWalkin, modelObjectList);
    }

    void RefrigerationSystem_Impl::removeWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      removeTemplate<RefrigerationWalkIn>(refrigerationWalkin, modelObjectList);
    }

    void RefrigerationSystem_Impl::removeAllWalkins() {
      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      removeAllTemplate<RefrigerationWalkIn>(modelObjectList);
    }

    bool RefrigerationSystem_Impl::addAirChiller(const RefrigerationAirChiller& refrigerationAirChiller) {
      // From 9.2 IO ref:
      // > This list may contain a combination of case and walk-in names OR a list of air chiller names.
      // > Air chillers may not be included in any list that also includes cases or walk-ins.
      if (boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList()) {
        std::vector<ModelObject> modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          if (elem.optionalCast<RefrigerationWalkIn>() || elem.optionalCast<RefrigerationCase>()) {
            LOG(Warn, "You cannot mix RefigerationCase/RefrigerationWalkins with RefrigerationAirChillers, occurred for " << briefDescription());
            return false;
          }
        }
      }

      // Enforce unicity
      if (boost::optional<RefrigerationSystem> currentSystem = refrigerationAirChiller.system()) {
        LOG(Warn, refrigerationAirChiller.briefDescription()
                    << " was removed from its existing RefrigerationSystem named '" << currentSystem->nameString() << "'.");
        currentSystem->removeAirChiller(refrigerationAirChiller);
      }

      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      return addTemplate<RefrigerationAirChiller>(refrigerationAirChiller, modelObjectList);
    }

    void RefrigerationSystem_Impl::removeAirChiller(const RefrigerationAirChiller& refrigerationAirChiller) {
      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      removeTemplate<RefrigerationAirChiller>(refrigerationAirChiller, modelObjectList);
    }

    void RefrigerationSystem_Impl::removeAllAirChillers() {
      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      removeAllTemplate<RefrigerationAirChiller>(modelObjectList);
    }

    bool RefrigerationSystem_Impl::addCompressor(const RefrigerationCompressor& refrigerationCompressor) {
      // Enforce unicity
      if (boost::optional<RefrigerationSystem> currentSystem = refrigerationCompressor.system()) {
        LOG(Warn, refrigerationCompressor.briefDescription()
                    << " was removed from its existing RefrigerationSystem (non High Stage) named '" << currentSystem->nameString() << "'.");
        currentSystem->removeCompressor(refrigerationCompressor);
      }

      return compressorList().addModelObject(refrigerationCompressor);
    }

    void RefrigerationSystem_Impl::removeCompressor(const RefrigerationCompressor& refrigerationCompressor) {
      compressorList().removeModelObject(refrigerationCompressor);
    }

    void RefrigerationSystem_Impl::removeAllCompressors() {
      compressorList().removeAllModelObjects();
    }

    bool RefrigerationSystem_Impl::addHighStageCompressor(const RefrigerationCompressor& refrigerationHighStageCompressor) {
      // Enforce unicity
      if (boost::optional<RefrigerationSystem> currentSystem = refrigerationHighStageCompressor.system()) {
        LOG(Warn, refrigerationHighStageCompressor.briefDescription()
                    << " was removed from its existing RefrigerationSystem (High Stage) named '" << currentSystem->nameString() << "'.");
        currentSystem->removeCompressor(refrigerationHighStageCompressor);
      }
      boost::optional<ModelObjectList> modelObjectList = highStageCompressorList();
      return addTemplate<RefrigerationCompressor>(refrigerationHighStageCompressor, modelObjectList);
    }

    void RefrigerationSystem_Impl::removeHighStageCompressor(const RefrigerationCompressor& refrigerationHighStageCompressor) {
      boost::optional<ModelObjectList> modelObjectList = highStageCompressorList();
      removeTemplate<RefrigerationCompressor>(refrigerationHighStageCompressor, modelObjectList);
    }

    void RefrigerationSystem_Impl::removeAllHighStageCompressors() {
      boost::optional<ModelObjectList> modelObjectList = highStageCompressorList();
      removeAllTemplate<RefrigerationCompressor>(modelObjectList);
    }

    bool RefrigerationSystem_Impl::addSecondarySystemLoad(const RefrigerationSecondarySystem& refrigerationSecondarySystem) {
      // Enforce unicity
      if (boost::optional<RefrigerationSystem> currentSystem = refrigerationSecondarySystem.system()) {
        LOG(Warn, refrigerationSecondarySystem.briefDescription()
                    << " was removed from its existing RefrigerationSystem named '" << currentSystem->nameString() << "'.");
        currentSystem->removeSecondarySystemLoad(refrigerationSecondarySystem);
      }
      boost::optional<ModelObjectList> modelObjectList = refrigerationTransferLoadList();
      return addTemplate<RefrigerationSecondarySystem>(refrigerationSecondarySystem, modelObjectList);
    }

    void RefrigerationSystem_Impl::removeSecondarySystemLoad(const RefrigerationSecondarySystem& refrigerationSecondarySystem) {
      boost::optional<ModelObjectList> modelObjectList = refrigerationTransferLoadList();
      removeTemplate<RefrigerationSecondarySystem>(refrigerationSecondarySystem, modelObjectList);
    }

    void RefrigerationSystem_Impl::removeAllSecondarySystemLoads() {
      boost::optional<ModelObjectList> modelObjectList = refrigerationTransferLoadList();
      removeAllTemplate<RefrigerationSecondarySystem>(modelObjectList);
    }

    bool RefrigerationSystem_Impl::addCascadeCondenserLoad(const RefrigerationCondenserCascade& refrigerationCondenserCascade) {
      // Enforce unicity
      if (boost::optional<RefrigerationSystem> currentSystem = refrigerationCondenserCascade.system()) {
        LOG(Warn, refrigerationCondenserCascade.briefDescription()
                    << " was removed from its existing RefrigerationSystem named '" << currentSystem->nameString() << "'.");
        currentSystem->removeCascadeCondenserLoad(refrigerationCondenserCascade);
      }
      boost::optional<ModelObjectList> modelObjectList = refrigerationTransferLoadList();
      return addTemplate<RefrigerationCondenserCascade>(refrigerationCondenserCascade, modelObjectList);
    }

    void RefrigerationSystem_Impl::removeCascadeCondenserLoad(const RefrigerationCondenserCascade& refrigerationCondenserCascade) {
      boost::optional<ModelObjectList> modelObjectList = refrigerationTransferLoadList();
      removeTemplate<RefrigerationCondenserCascade>(refrigerationCondenserCascade, modelObjectList);
    }

    void RefrigerationSystem_Impl::removeAllCascadeCondenserLoads() {
      boost::optional<ModelObjectList> modelObjectList = refrigerationTransferLoadList();
      removeAllTemplate<RefrigerationCondenserCascade>(modelObjectList);
    }

    bool RefrigerationSystem_Impl::setRefrigeratedCaseAndWalkInList(const boost::optional<ModelObjectList>& modelObjectList) {
      bool result(false);
      if (modelObjectList) {
        result = setPointer(OS_Refrigeration_SystemFields::RefrigeratedCaseAndWalkInListName, modelObjectList.get().handle());
      } else {
        resetRefrigeratedCaseAndWalkInList();
        result = true;
      }
      return result;
    }

    void RefrigerationSystem_Impl::resetRefrigeratedCaseAndWalkInList() {
      bool result = setString(OS_Refrigeration_SystemFields::RefrigeratedCaseAndWalkInListName, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSystem_Impl::setRefrigerationTransferLoadList(const boost::optional<ModelObjectList>& modelObjectList) {
      bool result(false);
      if (modelObjectList) {
        result = setPointer(OS_Refrigeration_SystemFields::RefrigerationTransferLoadListName, modelObjectList.get().handle());
      } else {
        resetRefrigerationTransferLoadList();
        result = true;
      }
      return result;
    }

    void RefrigerationSystem_Impl::resetRefrigerationTransferLoadList() {
      bool result = setString(OS_Refrigeration_SystemFields::RefrigerationTransferLoadListName, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSystem_Impl::setRefrigerationCondenser(const ModelObject& refrigerationCondenser) {

      boost::optional<RefrigerationSystem> currentSystem;
      // Enforce unicity
      if (auto refrigerationCondenserAirCooled = refrigerationCondenser.optionalCast<RefrigerationCondenserAirCooled>()) {
        currentSystem = refrigerationCondenserAirCooled->system();
      }

      if (!currentSystem) {
        if (auto refrigerationCondenserWaterCooled = refrigerationCondenser.optionalCast<RefrigerationCondenserWaterCooled>()) {
          currentSystem = refrigerationCondenserWaterCooled->system();
        }
      }

      if (!currentSystem) {
        if (auto refrigerationCondenserEvaporativeCooled = refrigerationCondenser.optionalCast<RefrigerationCondenserEvaporativeCooled>()) {
          currentSystem = refrigerationCondenserEvaporativeCooled->system();
        }
      }

      if (!currentSystem) {
        if (auto refrigerationCondenserCascade = refrigerationCondenser.optionalCast<RefrigerationCondenserCascade>()) {
          currentSystem = refrigerationCondenserCascade->heatRejectingSystem();
        }
      }

      bool result = setPointer(OS_Refrigeration_SystemFields::RefrigerationCondenserName, refrigerationCondenser.handle());
      if (result && currentSystem && (currentSystem->handle() != this->handle())) {
        LOG(Warn, refrigerationCondenser.briefDescription()
                    << " was removed from its existing RefrigerationSystem (Condenser Name) named '" << currentSystem->nameString() << "'.");
        currentSystem->resetRefrigerationCondenser();
      }

      return result;
    }

    void RefrigerationSystem_Impl::resetRefrigerationCondenser() {
      bool result = setString(OS_Refrigeration_SystemFields::RefrigerationCondenserName, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSystem_Impl::setCompressorList(const ModelObjectList& modelObjectList) {
      bool result = setPointer(OS_Refrigeration_SystemFields::CompressorListName, modelObjectList.handle());
      return result;
    }

    bool RefrigerationSystem_Impl::setMinimumCondensingTemperature(double minimumCondensingTemperature) {
      bool result = setDouble(OS_Refrigeration_SystemFields::MinimumCondensingTemperature, minimumCondensingTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationSystem_Impl::setRefrigerationSystemWorkingFluidType(const std::string& refrigerationSystemWorkingFluidType) {
      bool result = setString(OS_Refrigeration_SystemFields::RefrigerationSystemWorkingFluidType, refrigerationSystemWorkingFluidType);
      return result;
    }

    bool RefrigerationSystem_Impl::setSuctionTemperatureControlType(const std::string& suctionTemperatureControlType) {
      bool result = setString(OS_Refrigeration_SystemFields::SuctionTemperatureControlType, suctionTemperatureControlType);
      return result;
    }

    void RefrigerationSystem_Impl::resetSuctionTemperatureControlType() {
      bool result = setString(OS_Refrigeration_SystemFields::SuctionTemperatureControlType, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSystem_Impl::setMechanicalSubcooler(const boost::optional<RefrigerationSubcoolerMechanical>& refrigerationSubcoolerMechanical) {
      bool result(false);
      if (refrigerationSubcoolerMechanical) {
        // Enforce unicity
        if (boost::optional<RefrigerationSystem> currentSystem = refrigerationSubcoolerMechanical->system()) {
          LOG(Warn, refrigerationSubcoolerMechanical->briefDescription()
                      << " was removed from its existing RefrigerationSystem named '" << currentSystem->nameString() << "'.");
          currentSystem->resetMechanicalSubcooler();
        }
        result = setPointer(OS_Refrigeration_SystemFields::MechanicalSubcoolerName, refrigerationSubcoolerMechanical.get().handle());
      } else {
        resetMechanicalSubcooler();
        result = true;
      }
      return result;
    }

    void RefrigerationSystem_Impl::resetMechanicalSubcooler() {
      bool result = setString(OS_Refrigeration_SystemFields::MechanicalSubcoolerName, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSystem_Impl::setLiquidSuctionHeatExchangerSubcooler(
      const boost::optional<RefrigerationSubcoolerLiquidSuction>& refrigerationSubcoolerLiquidSuction) {
      bool result(false);
      if (refrigerationSubcoolerLiquidSuction) {
        // Enforce unicity
        if (boost::optional<RefrigerationSystem> currentSystem = refrigerationSubcoolerLiquidSuction->system()) {
          LOG(Warn, refrigerationSubcoolerLiquidSuction->briefDescription()
                      << " was removed from its existing RefrigerationSystem named '" << currentSystem->nameString() << "'.");
          currentSystem->resetLiquidSuctionHeatExchangerSubcooler();
        }
        result = setPointer(OS_Refrigeration_SystemFields::LiquidSuctionHeatExchangerSubcoolerName, refrigerationSubcoolerLiquidSuction->handle());
      } else {
        resetLiquidSuctionHeatExchangerSubcooler();
        result = true;
      }
      return result;
    }

    void RefrigerationSystem_Impl::resetLiquidSuctionHeatExchangerSubcooler() {
      bool result = setString(OS_Refrigeration_SystemFields::LiquidSuctionHeatExchangerSubcoolerName, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSystem_Impl::setSumUASuctionPiping(double sumUASuctionPiping) {
      bool result = setDouble(OS_Refrigeration_SystemFields::SumUASuctionPiping, sumUASuctionPiping);
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationSystem_Impl::resetSumUASuctionPiping() {
      bool result = setString(OS_Refrigeration_SystemFields::SumUASuctionPiping, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSystem_Impl::setSuctionPipingZone(const boost::optional<ThermalZone>& thermalZone) {
      bool result(false);
      if (thermalZone) {
        result = setPointer(OS_Refrigeration_SystemFields::SuctionPipingZoneName, thermalZone.get().handle());
      } else {
        resetSuctionPipingZone();
        result = true;
      }
      return result;
    }

    void RefrigerationSystem_Impl::resetSuctionPipingZone() {
      bool result = setString(OS_Refrigeration_SystemFields::SuctionPipingZoneName, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSystem_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      bool result = setString(OS_Refrigeration_SystemFields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationSystem_Impl::resetEndUseSubcategory() {
      bool result = setString(OS_Refrigeration_SystemFields::EndUseSubcategory, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSystem_Impl::setIntercoolerType(const std::string& intercoolerType) {
      bool result = setString(OS_Refrigeration_SystemFields::IntercoolerType, intercoolerType);
      return result;
    }

    void RefrigerationSystem_Impl::resetIntercoolerType() {
      bool result = setString(OS_Refrigeration_SystemFields::IntercoolerType, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSystem_Impl::setShellandCoilIntercoolerEffectiveness(double shellandCoilIntercoolerEffectiveness) {
      bool result = setDouble(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness, shellandCoilIntercoolerEffectiveness);
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationSystem_Impl::resetShellandCoilIntercoolerEffectiveness() {
      bool result = setString(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSystem_Impl::setHighStageCompressorList(const boost::optional<ModelObjectList>& modelObjectList) {
      bool result(false);
      if (modelObjectList) {
        result = setPointer(OS_Refrigeration_SystemFields::HighStageCompressorListName, modelObjectList.get().handle());
      } else {
        resetHighStageCompressorList();
        result = true;
      }
      return result;
    }

    void RefrigerationSystem_Impl::resetHighStageCompressorList() {
      bool result = setString(OS_Refrigeration_SystemFields::HighStageCompressorListName, "");
      OS_ASSERT(result);
    }

    boost::optional<ModelObject> RefrigerationSystem_Impl::optionalRefrigerationCondenser() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_Refrigeration_SystemFields::RefrigerationCondenserName);
    }

    boost::optional<ModelObjectList> RefrigerationSystem_Impl::optionalCompressorList() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_SystemFields::CompressorListName);
    }

    std::vector<EMSActuatorNames> RefrigerationSystem_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Refrigeration:System", "Minimum Condensing Temperature"}};
      return actuators;
    }

    std::vector<std::string> RefrigerationSystem_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }

  }  // namespace detail

  RefrigerationSystem::RefrigerationSystem(const Model& model) : ModelObject(RefrigerationSystem::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::RefrigerationSystem_Impl>());

    ModelObjectList caseAndWalkinList = ModelObjectList(model);
    caseAndWalkinList.setName(this->name().get() + " Case and Walkin List");
    bool ok = getImpl<detail::RefrigerationSystem_Impl>()->setRefrigeratedCaseAndWalkInList(caseAndWalkinList);
    OS_ASSERT(ok);
    ModelObjectList transferLoadList = ModelObjectList(model);
    transferLoadList.setName(this->name().get() + " Transfer Load List");
    ok = getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationTransferLoadList(transferLoadList);
    OS_ASSERT(ok);
    ModelObjectList compressorlist = ModelObjectList(model);
    compressorlist.setName(this->name().get() + " Compressor List");
    ok = getImpl<detail::RefrigerationSystem_Impl>()->setCompressorList(compressorlist);
    OS_ASSERT(ok);
    ModelObjectList highStageCompressorlist = ModelObjectList(model);
    highStageCompressorlist.setName(this->name().get() + " High Stage Compressor List");
    ok = getImpl<detail::RefrigerationSystem_Impl>()->setHighStageCompressorList(highStageCompressorlist);
    OS_ASSERT(ok);
    setMinimumCondensingTemperature(21.0);
    ok = setRefrigerationSystemWorkingFluidType("R404a");
    OS_ASSERT(ok);
    ok = setSuctionTemperatureControlType("ConstantSuctionTemperature");
    OS_ASSERT(ok);
  }

  IddObjectType RefrigerationSystem::iddObjectType() {
    return {IddObjectType::OS_Refrigeration_System};
  }

  std::vector<std::string> RefrigerationSystem::refrigerationSystemWorkingFluidTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Refrigeration_SystemFields::RefrigerationSystemWorkingFluidType);
  }

  std::vector<std::string> RefrigerationSystem::suctionTemperatureControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Refrigeration_SystemFields::SuctionTemperatureControlType);
  }

  std::vector<std::string> RefrigerationSystem::intercoolerTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Refrigeration_SystemFields::IntercoolerType);
  }

  std::vector<RefrigerationCase> RefrigerationSystem::cases() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->cases();
  }

  std::vector<RefrigerationWalkIn> RefrigerationSystem::walkins() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->walkins();
  }

  std::vector<RefrigerationCompressor> RefrigerationSystem::compressors() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->compressors();
  }

  std::vector<RefrigerationCompressor> RefrigerationSystem::highStageCompressors() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->highStageCompressors();
  }

  std::vector<RefrigerationSecondarySystem> RefrigerationSystem::secondarySystemLoads() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->secondarySystemLoads();
  }

  std::vector<RefrigerationCondenserCascade> RefrigerationSystem::cascadeCondenserLoads() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->cascadeCondenserLoads();
  }

  std::vector<RefrigerationAirChiller> RefrigerationSystem::airChillers() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->airChillers();
  }

  boost::optional<ModelObject> RefrigerationSystem::refrigerationCondenser() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->refrigerationCondenser();
  }

  double RefrigerationSystem::minimumCondensingTemperature() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->minimumCondensingTemperature();
  }

  std::string RefrigerationSystem::refrigerationSystemWorkingFluidType() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->refrigerationSystemWorkingFluidType();
  }

  std::string RefrigerationSystem::suctionTemperatureControlType() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->suctionTemperatureControlType();
  }

  bool RefrigerationSystem::isSuctionTemperatureControlTypeDefaulted() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->isSuctionTemperatureControlTypeDefaulted();
  }

  boost::optional<RefrigerationSubcoolerMechanical> RefrigerationSystem::mechanicalSubcooler() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->mechanicalSubcooler();
  }

  boost::optional<RefrigerationSubcoolerLiquidSuction> RefrigerationSystem::liquidSuctionHeatExchangerSubcooler() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->liquidSuctionHeatExchangerSubcooler();
  }

  double RefrigerationSystem::sumUASuctionPiping() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->sumUASuctionPiping();
  }

  bool RefrigerationSystem::isSumUASuctionPipingDefaulted() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->isSumUASuctionPipingDefaulted();
  }

  boost::optional<ThermalZone> RefrigerationSystem::suctionPipingZone() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->suctionPipingZone();
  }

  std::string RefrigerationSystem::endUseSubcategory() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->endUseSubcategory();
  }

  bool RefrigerationSystem::isEndUseSubcategoryDefaulted() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->isEndUseSubcategoryDefaulted();
  }

  std::string RefrigerationSystem::numberofCompressorStages() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->numberofCompressorStages();
  }

  std::string RefrigerationSystem::intercoolerType() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->intercoolerType();
  }

  bool RefrigerationSystem::isIntercoolerTypeDefaulted() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->isIntercoolerTypeDefaulted();
  }

  double RefrigerationSystem::shellandCoilIntercoolerEffectiveness() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->shellandCoilIntercoolerEffectiveness();
  }

  bool RefrigerationSystem::isShellandCoilIntercoolerEffectivenessDefaulted() const {
    return getImpl<detail::RefrigerationSystem_Impl>()->isShellandCoilIntercoolerEffectivenessDefaulted();
  }

  bool RefrigerationSystem::addCase(const RefrigerationCase& refrigerationCase) {
    return getImpl<detail::RefrigerationSystem_Impl>()->addCase(refrigerationCase);
  }

  void RefrigerationSystem::removeCase(const RefrigerationCase& refrigerationCase) {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeCase(refrigerationCase);
  }

  void RefrigerationSystem::removeAllCases() {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeAllCases();
  }

  bool RefrigerationSystem::addWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
    return getImpl<detail::RefrigerationSystem_Impl>()->addWalkin(refrigerationWalkin);
  }

  void RefrigerationSystem::removeWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeWalkin(refrigerationWalkin);
  }

  void RefrigerationSystem::removeAllWalkins() {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeAllWalkins();
  }

  bool RefrigerationSystem::addCompressor(const RefrigerationCompressor& refrigerationCompressor) {
    return getImpl<detail::RefrigerationSystem_Impl>()->addCompressor(refrigerationCompressor);
  }

  void RefrigerationSystem::removeCompressor(const RefrigerationCompressor& refrigerationCompressor) {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeCompressor(refrigerationCompressor);
  }

  void RefrigerationSystem::removeAllCompressors() {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeAllCompressors();
  }

  bool RefrigerationSystem::addHighStageCompressor(const RefrigerationCompressor& refrigerationHighStageCompressor) {
    return getImpl<detail::RefrigerationSystem_Impl>()->addHighStageCompressor(refrigerationHighStageCompressor);
  }

  void RefrigerationSystem::removeHighStageCompressor(const RefrigerationCompressor& refrigerationHighStageCompressor) {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeHighStageCompressor(refrigerationHighStageCompressor);
  }

  void RefrigerationSystem::removeAllHighStageCompressors() {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeAllHighStageCompressors();
  }

  bool RefrigerationSystem::addSecondarySystemLoad(const RefrigerationSecondarySystem& refrigerationSecondarySystemLoad) {
    return getImpl<detail::RefrigerationSystem_Impl>()->addSecondarySystemLoad(refrigerationSecondarySystemLoad);
  }

  void RefrigerationSystem::removeSecondarySystemLoad(const RefrigerationSecondarySystem& refrigerationSecondarySystemLoad) {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeSecondarySystemLoad(refrigerationSecondarySystemLoad);
  }

  void RefrigerationSystem::removeAllSecondarySystemLoads() {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeAllSecondarySystemLoads();
  }

  bool RefrigerationSystem::addCascadeCondenserLoad(const RefrigerationCondenserCascade& refrigerationCondenserCascade) {
    return getImpl<detail::RefrigerationSystem_Impl>()->addCascadeCondenserLoad(refrigerationCondenserCascade);
  }

  void RefrigerationSystem::removeCascadeCondenserLoad(const RefrigerationCondenserCascade& refrigerationCondenserCascade) {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeCascadeCondenserLoad(refrigerationCondenserCascade);
  }

  void RefrigerationSystem::removeAllCascadeCondenserLoads() {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeAllCascadeCondenserLoads();
  }

  bool RefrigerationSystem::addAirChiller(const RefrigerationAirChiller& refrigerationAirChiller) {
    return getImpl<detail::RefrigerationSystem_Impl>()->addAirChiller(refrigerationAirChiller);
  }

  void RefrigerationSystem::removeAirChiller(const RefrigerationAirChiller& refrigerationAirChiller) {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeAirChiller(refrigerationAirChiller);
  }

  void RefrigerationSystem::removeAllAirChillers() {
    return getImpl<detail::RefrigerationSystem_Impl>()->removeAllAirChillers();
  }

  bool RefrigerationSystem::setRefrigerationCondenser(const ModelObject& refrigerationCondenser) {
    return getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationCondenser(refrigerationCondenser);
  }

  void RefrigerationSystem::resetRefrigerationCondenser() {
    return getImpl<detail::RefrigerationSystem_Impl>()->resetRefrigerationCondenser();
  }

  bool RefrigerationSystem::setMinimumCondensingTemperature(double minimumCondensingTemperature) {
    return getImpl<detail::RefrigerationSystem_Impl>()->setMinimumCondensingTemperature(minimumCondensingTemperature);
  }

  bool RefrigerationSystem::setRefrigerationSystemWorkingFluidType(const std::string& refrigerationSystemWorkingFluidType) {
    return getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationSystemWorkingFluidType(refrigerationSystemWorkingFluidType);
  }

  bool RefrigerationSystem::setSuctionTemperatureControlType(const std::string& suctionTemperatureControlType) {
    return getImpl<detail::RefrigerationSystem_Impl>()->setSuctionTemperatureControlType(suctionTemperatureControlType);
  }

  void RefrigerationSystem::resetSuctionTemperatureControlType() {
    getImpl<detail::RefrigerationSystem_Impl>()->resetSuctionTemperatureControlType();
  }

  bool RefrigerationSystem::setMechanicalSubcooler(const RefrigerationSubcoolerMechanical& refrigerationSubcoolerMechanical) {
    return getImpl<detail::RefrigerationSystem_Impl>()->setMechanicalSubcooler(refrigerationSubcoolerMechanical);
  }

  void RefrigerationSystem::resetMechanicalSubcooler() {
    getImpl<detail::RefrigerationSystem_Impl>()->resetMechanicalSubcooler();
  }

  bool RefrigerationSystem::setLiquidSuctionHeatExchangerSubcooler(const RefrigerationSubcoolerLiquidSuction& refrigerationSubcoolerLiquidSuction) {
    return getImpl<detail::RefrigerationSystem_Impl>()->setLiquidSuctionHeatExchangerSubcooler(refrigerationSubcoolerLiquidSuction);
  }

  void RefrigerationSystem::resetLiquidSuctionHeatExchangerSubcooler() {
    getImpl<detail::RefrigerationSystem_Impl>()->resetLiquidSuctionHeatExchangerSubcooler();
  }

  bool RefrigerationSystem::setSumUASuctionPiping(double sumUASuctionPiping) {
    return getImpl<detail::RefrigerationSystem_Impl>()->setSumUASuctionPiping(sumUASuctionPiping);
  }

  void RefrigerationSystem::resetSumUASuctionPiping() {
    getImpl<detail::RefrigerationSystem_Impl>()->resetSumUASuctionPiping();
  }

  bool RefrigerationSystem::setSuctionPipingZone(const ThermalZone& thermalZone) {
    return getImpl<detail::RefrigerationSystem_Impl>()->setSuctionPipingZone(thermalZone);
  }

  void RefrigerationSystem::resetSuctionPipingZone() {
    getImpl<detail::RefrigerationSystem_Impl>()->resetSuctionPipingZone();
  }

  bool RefrigerationSystem::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::RefrigerationSystem_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void RefrigerationSystem::resetEndUseSubcategory() {
    getImpl<detail::RefrigerationSystem_Impl>()->resetEndUseSubcategory();
  }

  bool RefrigerationSystem::setIntercoolerType(const std::string& intercoolerType) {
    return getImpl<detail::RefrigerationSystem_Impl>()->setIntercoolerType(intercoolerType);
  }

  void RefrigerationSystem::resetIntercoolerType() {
    getImpl<detail::RefrigerationSystem_Impl>()->resetIntercoolerType();
  }

  bool RefrigerationSystem::setShellandCoilIntercoolerEffectiveness(double shellandCoilIntercoolerEffectiveness) {
    return getImpl<detail::RefrigerationSystem_Impl>()->setShellandCoilIntercoolerEffectiveness(shellandCoilIntercoolerEffectiveness);
  }

  void RefrigerationSystem::resetShellandCoilIntercoolerEffectiveness() {
    getImpl<detail::RefrigerationSystem_Impl>()->resetShellandCoilIntercoolerEffectiveness();
  }

  /// @cond
  RefrigerationSystem::RefrigerationSystem(std::shared_ptr<detail::RefrigerationSystem_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
