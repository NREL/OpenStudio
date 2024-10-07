/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PIPEADIABATIC_IMPL_HPP
#define MODEL_PIPEADIABATIC_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class MODEL_API PipeAdiabatic_Impl : public StraightComponent_Impl
    {
     public:
      // constructor
      PipeAdiabatic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      PipeAdiabatic_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      PipeAdiabatic_Impl(const PipeAdiabatic_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~PipeAdiabatic_Impl() override = default;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

     private:
      REGISTER_LOGGER("openstudio.model.PipeAdiabatic");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_PIPEADIABATIC_IMPL_HPP
