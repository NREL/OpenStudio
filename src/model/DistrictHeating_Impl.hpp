/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DISTRICTHEATING_IMPL_HPP
#define MODEL_DISTRICTHEATING_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    /** DistrictHeating_Impl is a StraightComponent_Impl that is the implementation class for DistrictHeating.*/

    class MODEL_API DistrictHeating_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      DistrictHeating_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DistrictHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DistrictHeating_Impl(const DistrictHeating_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DistrictHeating_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;
      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> nominalCapacity() const;

      bool isNominalCapacityAutosized() const;

      boost::optional<double> autosizedNominalCapacity() const;

      //@}
      /** @name Setters */
      //@{

      bool setNominalCapacity(boost::optional<double> nominalCapacity);

      void autosizeNominalCapacity();

      bool addToNode(Node& node) override;

      //@}
      /** @name Other */
      //@{

      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DistrictHeating");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_DISTRICTHEATING_IMPL_HPP
