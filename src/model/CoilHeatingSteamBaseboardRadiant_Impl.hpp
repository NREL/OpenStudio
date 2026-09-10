/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGSTEAMBASEBOARDRADIANT_IMPL_HPP
#define MODEL_COILHEATINGSTEAMBASEBOARDRADIANT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CoilHeatingSteamBaseboardRadiant_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingSteamBaseboardRadiant.*/
    class MODEL_API CoilHeatingSteamBaseboardRadiant_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingSteamBaseboardRadiant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingSteamBaseboardRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingSteamBaseboardRadiant_Impl(const CoilHeatingSteamBaseboardRadiant_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingSteamBaseboardRadiant_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      std::string heatingDesignCapacityMethod() const;

      boost::optional<double> heatingDesignCapacity() const;

      bool isHeatingDesignCapacityAutosized() const;

      double heatingDesignCapacityPerFloorArea() const;

      double fractionofAutosizedHeatingDesignCapacity() const;

      boost::optional<double> maximumSteamFlowRate() const;

      bool isMaximumSteamFlowRateAutosized() const;

      double degreeofSubCooling() const;

      double convergenceTolerance() const;

      boost::optional<double> autosizedHeatingDesignCapacity() const;

      boost::optional<double> autosizedMaximumSteamFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

      bool setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity);

      void autosizeHeatingDesignCapacity();

      bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

      bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

      bool setMaximumSteamFlowRate(boost::optional<double> maximumSteamFlowRate);

      void autosizeMaximumSteamFlowRate();

      bool setDegreeofSubCooling(double degreeofSubCooling);

      bool setConvergenceTolerance(double convergenceTolerance);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingSteamBaseboardRadiant");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGSTEAMBASEBOARDRADIANT_IMPL_HPP
