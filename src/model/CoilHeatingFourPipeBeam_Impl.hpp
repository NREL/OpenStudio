/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGFOURPIPEBEAM_IMPL_HPP
#define MODEL_COILHEATINGFOURPIPEBEAM_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;
  class Curve;
  class AirTerminalSingleDuctConstantVolumeFourPipeBeam;

  namespace detail {

    /** CoilHeatingFourPipeBeam_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingFourPipeBeam.*/
    class MODEL_API CoilHeatingFourPipeBeam_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingFourPipeBeam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingFourPipeBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingFourPipeBeam_Impl(const CoilHeatingFourPipeBeam_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingFourPipeBeam_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<StraightComponent> containingStraightComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Node> hotWaterInletNode() const;

      boost::optional<Node> hotWaterOutletNode() const;

      double beamRatedHeatingCapacityperBeamLength() const;
      bool isBeamRatedHeatingCapacityperBeamLengthDefaulted() const;

      double beamRatedHeatingRoomAirHotWaterTemperatureDifference() const;
      bool isBeamRatedHeatingRoomAirHotWaterTemperatureDifferenceDefaulted() const;

      double beamRatedHotWaterVolumeFlowRateperBeamLength() const;
      bool isBeamRatedHotWaterVolumeFlowRateperBeamLengthDefaulted() const;

      Curve beamHeatingCapacityTemperatureDifferenceModificationFactorCurve() const;

      Curve beamHeatingCapacityAirFlowModificationFactorCurve() const;

      Curve beamHeatingCapacityHotWaterFlowModificationFactorCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setBeamRatedHeatingCapacityperBeamLength(double beamRatedHeatingCapacityperBeamLength);
      void resetBeamRatedHeatingCapacityperBeamLength();

      bool setBeamRatedHeatingRoomAirHotWaterTemperatureDifference(double beamRatedHeatingRoomAirHotWaterTemperatureDifference);
      void resetBeamRatedHeatingRoomAirHotWaterTemperatureDifference();

      bool setBeamRatedHotWaterVolumeFlowRateperBeamLength(double beamRatedHotWaterVolumeFlowRateperBeamLength);
      void resetBeamRatedHotWaterVolumeFlowRateperBeamLength();

      bool setBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve(const Curve& curve);
      // void resetBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve();

      bool setBeamHeatingCapacityAirFlowModificationFactorCurve(const Curve& curve);
      // void resetBeamHeatingCapacityAirFlowModificationFactorCurve();

      bool setBeamHeatingCapacityHotWaterFlowModificationFactorCurve(const Curve& curve);
      // void resetBeamHeatingCapacityHotWaterFlowModificationFactorCurve();

      //@}
      /** @name Other */
      //@{

      /* Convenience function to return the parent AirTerminal:SingleDuct:ConstantVolume:FourPipeBeam */
      boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeBeam> airTerminalSingleDuctConstantVolumeFourPipeBeam() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingFourPipeBeam");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGFOURPIPEBEAM_IMPL_HPP
