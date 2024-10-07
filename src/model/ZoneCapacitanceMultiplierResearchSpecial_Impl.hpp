/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONECAPACITANCEMULTIPLIERRESEARCHSPECIAL_IMPL_HPP
#define MODEL_ZONECAPACITANCEMULTIPLIERRESEARCHSPECIAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ZoneCapacitanceMultiplierResearchSpecial_Impl is a ModelObject_Impl that is the implementation class for ZoneCapacitanceMultiplierResearchSpecial.*/
    class MODEL_API ZoneCapacitanceMultiplierResearchSpecial_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      ZoneCapacitanceMultiplierResearchSpecial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ZoneCapacitanceMultiplierResearchSpecial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      ZoneCapacitanceMultiplierResearchSpecial_Impl(const ZoneCapacitanceMultiplierResearchSpecial_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~ZoneCapacitanceMultiplierResearchSpecial_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double temperatureCapacityMultiplier() const;

      bool isTemperatureCapacityMultiplierDefaulted() const;

      double humidityCapacityMultiplier() const;

      bool isHumidityCapacityMultiplierDefaulted() const;

      double carbonDioxideCapacityMultiplier() const;

      bool isCarbonDioxideCapacityMultiplierDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setTemperatureCapacityMultiplier(double temperatureCapacityMultiplier);

      void resetTemperatureCapacityMultiplier();

      bool setHumidityCapacityMultiplier(double humidityCapacityMultiplier);

      void resetHumidityCapacityMultiplier();

      bool setCarbonDioxideCapacityMultiplier(double carbonDioxideCapacityMultiplier);

      void resetCarbonDioxideCapacityMultiplier();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneCapacitanceMultiplierResearchSpecial");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONECAPACITANCEMULTIPLIERRESEARCHSPECIAL_IMPL_HPP
