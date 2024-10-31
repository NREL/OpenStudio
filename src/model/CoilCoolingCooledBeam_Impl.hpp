/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGCOOLEDBEAM_IMPL_HPP
#define MODEL_COILCOOLINGCOOLEDBEAM_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CoilCoolingCooledBeam_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingCooledBeam.*/
    class MODEL_API CoilCoolingCooledBeam_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingCooledBeam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingCooledBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingCooledBeam_Impl(const CoilCoolingCooledBeam_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingCooledBeam_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<StraightComponent> containingStraightComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      double coilSurfaceAreaperCoilLength() const;

      bool isCoilSurfaceAreaperCoilLengthDefaulted() const;

      double modelParametera() const;

      bool isModelParameteraDefaulted() const;

      double modelParametern1() const;

      bool isModelParametern1Defaulted() const;

      double modelParametern2() const;

      bool isModelParametern2Defaulted() const;

      double modelParametern3() const;

      bool isModelParametern3Defaulted() const;

      double modelParametera0() const;

      bool isModelParametera0Defaulted() const;

      double modelParameterK1() const;

      bool isModelParameterK1Defaulted() const;

      double modelParametern() const;

      bool isModelParameternDefaulted() const;

      double leavingPipeInsideDiameter() const;

      bool isLeavingPipeInsideDiameterDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setCoilSurfaceAreaperCoilLength(double coilSurfaceAreaperCoilLength);

      void resetCoilSurfaceAreaperCoilLength();

      bool setModelParametera(double modelParametera);

      void resetModelParametera();

      bool setModelParametern1(double modelParametern1);

      void resetModelParametern1();

      bool setModelParametern2(double modelParametern2);

      void resetModelParametern2();

      bool setModelParametern3(double modelParametern3);

      void resetModelParametern3();

      bool setModelParametera0(double modelParametera0);

      void resetModelParametera0();

      bool setModelParameterK1(double modelParameterK1);

      void resetModelParameterK1();

      bool setModelParametern(double modelParametern);

      void resetModelParametern();

      bool setLeavingPipeInsideDiameter(double leavingPipeInsideDiameter);

      void resetLeavingPipeInsideDiameter();

      //@}
      /** @name Other */
      //@{

      bool addToNode(Node& node) override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingCooledBeam");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGCOOLEDBEAM_IMPL_HPP
