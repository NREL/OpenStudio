/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_IMPL_HPP
#define MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class CoilHeatingDXMultiSpeed;

  namespace detail {

    /** CoilHeatingDXMultiSpeedStageData_Impl is a ParentObject_Impl that is the implementation class for CoilHeatingDXMultiSpeedStageData.*/
    class MODEL_API CoilHeatingDXMultiSpeedStageData_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingDXMultiSpeedStageData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingDXMultiSpeedStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingDXMultiSpeedStageData_Impl(const CoilHeatingDXMultiSpeedStageData_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingDXMultiSpeedStageData_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      // If this object is used by any CoilHeatingDXMultiSpeed, remove the corresponding extensible group to avoid having 'blanks'
      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> grossRatedHeatingCapacity() const;

      bool isGrossRatedHeatingCapacityAutosized() const;

      double grossRatedHeatingCOP() const;

      boost::optional<double> ratedAirFlowRate() const;

      bool isRatedAirFlowRateAutosized() const;

      double ratedSupplyAirFanPowerPerVolumeFlowRate2017() const;

      double ratedSupplyAirFanPowerPerVolumeFlowRate2023() const;

      Curve heatingCapacityFunctionofTemperatureCurve() const;

      Curve heatingCapacityFunctionofFlowFractionCurve() const;

      Curve energyInputRatioFunctionofTemperatureCurve() const;

      Curve energyInputRatioFunctionofFlowFractionCurve() const;

      Curve partLoadFractionCorrelationCurve() const;

      double ratedWasteHeatFractionofPowerInput() const;

      Curve wasteHeatFunctionofTemperatureCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setGrossRatedHeatingCapacity(boost::optional<double> grossRatedHeatingCapacity);

      void autosizeGrossRatedHeatingCapacity();

      bool setGrossRatedHeatingCOP(double grossRatedHeatingCOP);

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      void autosizeRatedAirFlowRate();

      bool setRatedSupplyAirFanPowerPerVolumeFlowRate2017(double ratedSupplyAirFanPowerPerVolumeFlowRate2017);

      bool setRatedSupplyAirFanPowerPerVolumeFlowRate2023(double ratedSupplyAirFanPowerPerVolumeFlowRate2023);

      bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

      bool setHeatingCapacityFunctionofFlowFractionCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

      bool setPartLoadFractionCorrelationCurve(const Curve& curve);

      bool setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput);

      bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedGrossRatedHeatingCapacity() const;

      boost::optional<double> autosizedRatedAirFlowRate() const;

      void autosize();

      void applySizingValues();

      // Returns the CoilHeatingDXMultiSpeed that references it if any
      boost::optional<CoilHeatingDXMultiSpeed> parentCoil() const;

      // Used to determine the index of this performance data in the
      // list of stages in the parent object.
      boost::optional<std::tuple<int, CoilHeatingDXMultiSpeed>> stageIndexAndParentCoil() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingDXMultiSpeedStageData");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Curve> optionalHeatingCapacityFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalHeatingCapacityFunctionofFlowFractionCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionofFlowFractionCurve() const;
      boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
      boost::optional<Curve> optionalWasteHeatFunctionofTemperatureCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_IMPL_HPP
