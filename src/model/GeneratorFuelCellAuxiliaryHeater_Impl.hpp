/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLAUXILIARYHEATER_IMPL_HPP
#define MODEL_GENERATORFUELCELLAUXILIARYHEATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class GeneratorFuelCell;

  namespace detail {

    /** GeneratorFuelCellAuxiliaryHeater_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCellAuxiliaryHeater.*/
    class MODEL_API GeneratorFuelCellAuxiliaryHeater_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorFuelCellAuxiliaryHeater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellAuxiliaryHeater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellAuxiliaryHeater_Impl(const GeneratorFuelCellAuxiliaryHeater_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorFuelCellAuxiliaryHeater_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const;

      virtual std::vector<ModelObject> children() const;

      //@}
      /** @name Getters */
      //@{

      double excessAirRatio() const;

      double ancillaryPowerConstantTerm() const;

      double ancillaryPowerLinearTerm() const;

      double skinLossUFactorTimesAreaValue() const;

      std::string skinLossDestination() const;

      boost::optional<ThermalZone> zonetoReceiveSkinLosses() const;

      std::string heatingCapacityUnits() const;

      double maximumHeatingCapacityinWatts() const;

      double minimumHeatingCapacityinWatts() const;

      double maximumHeatingCapacityinKmolperSecond() const;

      double minimumHeatingCapacityinKmolperSecond() const;

      // Return optional parent generator
      boost::optional<GeneratorFuelCell> fuelCell() const;

      //@}
      /** @name Setters */
      //@{

      bool setExcessAirRatio(double excessAirRatio);

      void resetExcessAirRatio();

      bool setAncillaryPowerConstantTerm(double ancillaryPowerConstantTerm);

      void resetAncillaryPowerConstantTerm();

      bool setAncillaryPowerLinearTerm(double ancillaryPowerLinearTerm);

      void resetAncillaryPowerLinearTerm();

      bool setSkinLossUFactorTimesAreaValue(double skinLossUFactorTimesAreaValue);

      void resetSkinLossUFactorTimesAreaValue();

      bool setSkinLossDestination(const std::string& skinLossDestination);

      void resetSkinLossDestination();

      bool setZonetoReceiveSkinLosses(const ThermalZone& zone);

      void resetZonetoReceiveSkinLosses();

      bool setHeatingCapacityUnits(const std::string& heatingCapacityUnits);

      void resetHeatingCapacityUnits();

      bool setMaximumHeatingCapacityinWatts(double maximumHeatingCapacityinWatts);

      void resetMaximumHeatingCapacityinWatts();

      bool setMinimumHeatingCapacityinWatts(double minimumHeatingCapacityinWatts);

      void resetMinimumHeatingCapacityinWatts();

      bool setMaximumHeatingCapacityinKmolperSecond(double maximumHeatingCapacityinKmolperSecond);

      void resetMaximumHeatingCapacityinKmolperSecond();

      bool setMinimumHeatingCapacityinKmolperSecond(double minimumHeatingCapacityinKmolperSecond);

      void resetMinimumHeatingCapacityinKmolperSecond();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorFuelCellAuxiliaryHeater");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLAUXILIARYHEATER_IMPL_HPP
