/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATOR_IMPL_HPP
#define MODEL_GENERATOR_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API Generator_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      Generator_Impl(IddObjectType type, Model_Impl* model);

      Generator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Generator_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Generator_Impl(const Generator_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~Generator_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      /// remove this object from any ElectricLoadCenterDistribution objects then remove from file
      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual std::string generatorObjectType() const = 0;

      virtual boost::optional<double> ratedElectricPowerOutput() const = 0;

      virtual boost::optional<Schedule> availabilitySchedule() const = 0;

      virtual boost::optional<double> ratedThermaltoElectricalPowerRatio() const = 0;

      //virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      //virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

      //@}
      /** @name Setters */
      //@{

      //@}
      /** @name Other */
      //@{

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.Generator");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATOR_IMPL_HPP
