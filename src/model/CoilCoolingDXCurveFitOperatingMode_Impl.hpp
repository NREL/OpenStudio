/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXCURVEFITOPERATINGMODE_IMPL_HPP
#define MODEL_COILCOOLINGDXCURVEFITOPERATINGMODE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class CoilCoolingDXCurveFitPerformance;
  class CoilCoolingDXCurveFitSpeed;

  namespace detail {

    /** CoilCoolingDXCurveFitOperatingMode_Impl is a ResourceObject_Impl that is the implementation class for CoilCoolingDXCurveFitOperatingMode.*/
    class MODEL_API CoilCoolingDXCurveFitOperatingMode_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXCurveFitOperatingMode_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXCurveFitOperatingMode_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDXCurveFitOperatingMode_Impl(const CoilCoolingDXCurveFitOperatingMode_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXCurveFitOperatingMode_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      // If this object is still used by at least one CoilCoolingDXCurveFitPerformance as a BaseOperatingMode, refuse to remove to avoid putting
      // the CoilCoolingDXCurveFitPerformance(s) in a broken state
      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> ratedGrossTotalCoolingCapacity() const;
      bool isRatedGrossTotalCoolingCapacityAutosized() const;

      boost::optional<double> ratedEvaporatorAirFlowRate() const;
      bool isRatedEvaporatorAirFlowRateAutosized() const;

      boost::optional<double> ratedCondenserAirFlowRate() const;
      bool isRatedCondenserAirFlowRateAutosized() const;

      double maximumCyclingRate() const;

      double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

      double latentCapacityTimeConstant() const;

      double nominalTimeforCondensateRemovaltoBegin() const;

      bool applyLatentDegradationtoSpeedsGreaterthan1() const;

      std::string condenserType() const;

      boost::optional<double> nominalEvaporativeCondenserPumpPower() const;
      bool isNominalEvaporativeCondenserPumpPowerAutosized() const;

      unsigned nominalSpeedNumber() const;
      bool isNominalSpeedNumberDefaulted() const;

      std::vector<CoilCoolingDXCurveFitPerformance> coilCoolingDXCurveFitPerformances() const;

      std::vector<CoilCoolingDXCurveFitSpeed> speeds() const;

      // Extensible: Speeds
      boost::optional<unsigned> speedIndex(const CoilCoolingDXCurveFitSpeed& speed) const;
      unsigned numberOfSpeeds() const;

      //@}
      /** @name Setters */
      //@{

      bool setRatedGrossTotalCoolingCapacity(double ratedGrossTotalCoolingCapacity);
      void autosizeRatedGrossTotalCoolingCapacity();

      bool setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate);
      void autosizeRatedEvaporatorAirFlowRate();

      bool setRatedCondenserAirFlowRate(double ratedCondenserAirFlowRate);
      void autosizeRatedCondenserAirFlowRate();

      bool setMaximumCyclingRate(double maximumCyclingRate);

      bool setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(
        double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

      bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

      bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

      bool setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1);

      bool setCondenserType(const std::string& condenserType);

      bool setNominalEvaporativeCondenserPumpPower(double nominalEvaporativeCondenserPumpPower);

      void autosizeNominalEvaporativeCondenserPumpPower();

      bool setNominalSpeedNumber(unsigned nominalSpeedNumber);
      void resetNominalSpeedNumber();

      // Extensible: Speeds
      bool addSpeed(const CoilCoolingDXCurveFitSpeed& speed);
      bool addSpeed(const CoilCoolingDXCurveFitSpeed& speed, unsigned index);
      bool setSpeedIndex(const CoilCoolingDXCurveFitSpeed& speed, unsigned index);
      bool setSpeeds(const std::vector<CoilCoolingDXCurveFitSpeed>& speeds);
      void removeAllSpeeds();
      bool removeSpeed(const CoilCoolingDXCurveFitSpeed& speed);
      bool removeSpeed(unsigned index);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedRatedGrossTotalCoolingCapacity();

      boost::optional<double> autosizedRatedEvaporatorAirFlowRate();

      boost::optional<double> autosizedRatedCondenserAirFlowRate();

      boost::optional<double> autosizedNominalEvaporativeCondenserPumpPower();

      void autosize();

      void applySizingValues();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXCurveFitOperatingMode");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXCURVEFITOPERATINGMODE_IMPL_HPP
