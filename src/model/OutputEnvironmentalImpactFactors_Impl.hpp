/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTENVIRONMENTALIMPACTFACTORS_IMPL_HPP
#define MODEL_OUTPUTENVIRONMENTALIMPACTFACTORS_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputEnvironmentalImpactFactors_Impl is a ModelObject_Impl that is the implementation class for OutputEnvironmentalImpactFactors.*/
    class MODEL_API OutputEnvironmentalImpactFactors_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      OutputEnvironmentalImpactFactors_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputEnvironmentalImpactFactors_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputEnvironmentalImpactFactors_Impl(const OutputEnvironmentalImpactFactors_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputEnvironmentalImpactFactors_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string reportingFrequency() const;

      //@}
      /** @name Setters */
      //@{

      bool setReportingFrequency(const std::string& reportingFrequency);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputEnvironmentalImpactFactors");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTENVIRONMENTALIMPACTFACTORS_IMPL_HPP
