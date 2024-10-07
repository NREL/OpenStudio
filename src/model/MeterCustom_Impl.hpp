/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_METERCUSTOM_IMPL_HPP
#define MODEL_METERCUSTOM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

class FuelType;

namespace model {

  namespace detail {

    /** MeterCustom_Impl is a ModelObject_Impl that is the implementation class for MeterCustom.*/
    class MODEL_API MeterCustom_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      MeterCustom_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      MeterCustom_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      MeterCustom_Impl(const MeterCustom_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~MeterCustom_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<std::string> fuelType() const;

      // Return a vector of (Key, Var) pairs
      std::vector<std::pair<std::string, std::string>> keyVarGroups();

      // Return the number of (KeyName, OutputVariableorMeterName) groups
      unsigned numKeyVarGroups() const;

      // Lower Level functions
      /** Get the Key Name at index. Indexing starts at 0. */
      boost::optional<std::string> keyName(unsigned index) const;

      /** Get the Output Variable of Meter Name at index. Indexing starts at 0. */
      boost::optional<std::string> outputVariableorMeterName(unsigned index) const;

      //@}
      /** @name Setters */
      //@{

      bool setFuelType(const FuelType& fuelType);

      void resetFuelType();

      // Add a new (Key, Var) group
      bool addKeyVarGroup(const std::string& keyName, const std::string& outputVariableorMeterName);

      // Remove the (Key, Var) group at given index
      bool removeKeyVarGroup(unsigned groupIndex);

      // Remove all the (Key, Var) groups
      void removeAllKeyVarGroups();

      // Lower level functions
      /** Set the Key Name at index. Indexing starts at 0. */
      bool setKeyName(unsigned index, const std::string& str);

      /** Set the Output Variable of Meter Name at index. Indexing starts at 0. */
      bool setOutputVariableorMeterName(unsigned index, const std::string& str);

      //@}
      /** @name Other */
      //@{

      //@}
      /** @name Type Casting */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.MeterCustom");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_METERCUSTOM_IMPL_HPP
