/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONVERGENCELIMITS_IMPL_HPP
#define MODEL_CONVERGENCELIMITS_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ConvergenceLimits_Impl is a ModelObject_Impl that is the implementation class for ConvergenceLimits.*/
    class MODEL_API ConvergenceLimits_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ConvergenceLimits_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ConvergenceLimits_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ConvergenceLimits_Impl(const ConvergenceLimits_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ConvergenceLimits_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<int> minimumSystemTimestep() const;

      int maximumHVACIterations() const;

      bool isMaximumHVACIterationsDefaulted() const;

      int minimumPlantIterations() const;

      bool isMinimumPlantIterationsDefaulted() const;

      int maximumPlantIterations() const;

      bool isMaximumPlantIterationsDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setMinimumSystemTimestep(boost::optional<int> minimumSystemTimestep);

      void resetMinimumSystemTimestep();

      bool setMaximumHVACIterations(int maximumHVACIterations);

      void resetMaximumHVACIterations();

      bool setMinimumPlantIterations(int minimumPlantIterations);

      void resetMinimumPlantIterations();

      bool setMaximumPlantIterations(int maximumPlantIterations);

      void resetMaximumPlantIterations();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ConvergenceLimits");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONVERGENCELIMITS_IMPL_HPP
