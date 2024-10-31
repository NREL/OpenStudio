/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SIZINGPARAMETERS_IMPL_HPP
#define MODEL_SIZINGPARAMETERS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** SizingParameters_Impl is a ModelObject_Impl that is the implementation class for SizingParameters.*/
    class MODEL_API SizingParameters_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SizingParameters_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SizingParameters_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SizingParameters_Impl(const SizingParameters_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SizingParameters_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual boost::optional<ParentObject> parent() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double heatingSizingFactor() const;

      bool isHeatingSizingFactorDefaulted() const;

      double coolingSizingFactor() const;

      bool isCoolingSizingFactorDefaulted() const;

      boost::optional<int> timestepsinAveragingWindow() const;

      //@}
      /** @name Setters */
      //@{

      bool setHeatingSizingFactor(double heatingSizingFactor);

      void resetHeatingSizingFactor();

      bool setCoolingSizingFactor(double coolingSizingFactor);

      void resetCoolingSizingFactor();

      bool setTimestepsinAveragingWindow(boost::optional<int> timestepsinAveragingWindow);

      void resetTimestepsinAveragingWindow();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SizingParameters");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SIZINGPARAMETERS_IMPL_HPP
