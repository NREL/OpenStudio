/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_BOILERSTEAM_IMPL_HPP
#define MODEL_BOILERSTEAM_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** BoilerSteam_Impl is a StraightComponent_Impl that is the implementation class for BoilerSteam.*/
    class MODEL_API BoilerSteam_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      BoilerSteam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      BoilerSteam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      BoilerSteam_Impl(const BoilerSteam_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~BoilerSteam_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      std::string fuelType() const;

      boost::optional<double> maximumOperatingPressure() const;

      boost::optional<double> theoreticalEfficiency() const;

      boost::optional<double> designOutletSteamTemperature() const;

      boost::optional<double> nominalCapacity() const;

      bool isNominalCapacityAutosized() const;

      boost::optional<double> minimumPartLoadRatio() const;

      boost::optional<double> maximumPartLoadRatio() const;

      boost::optional<double> optimumPartLoadRatio() const;

      boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve() const;

      boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve() const;

      boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve() const;

      double sizingFactor() const;

      bool isSizingFactorDefaulted() const;

      boost::optional<double> autosizedNominalCapacity() const;

      std::string endUseSubcategory() const;

      //@}
      /** @name Setters */
      //@{

      bool setFuelType(const std::string& fuelType);

      bool setMaximumOperatingPressure(boost::optional<double> maximumOperatingPressure);

      void resetMaximumOperatingPressure();

      bool setTheoreticalEfficiency(boost::optional<double> theoreticalEfficiency);

      void resetTheoreticalEfficiency();

      bool setDesignOutletSteamTemperature(boost::optional<double> designOutletSteamTemperature);

      void resetDesignOutletSteamTemperature();

      bool setNominalCapacity(boost::optional<double> nominalCapacity);

      void resetNominalCapacity();

      void autosizeNominalCapacity();

      bool setMinimumPartLoadRatio(boost::optional<double> minimumPartLoadRatio);

      void resetMinimumPartLoadRatio();

      bool setMaximumPartLoadRatio(boost::optional<double> maximumPartLoadRatio);

      void resetMaximumPartLoadRatio();

      bool setOptimumPartLoadRatio(boost::optional<double> optimumPartLoadRatio);

      void resetOptimumPartLoadRatio();

      bool setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve);

      void resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();

      bool setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve);

      void resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();

      bool setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve);

      void resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();

      bool setSizingFactor(double sizingFactor);

      void resetSizingFactor();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.BoilerSteam");

      std::vector<std::string> fuelTypeValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_BOILERSTEAM_IMPL_HPP
