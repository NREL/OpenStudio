/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTCONSTRUCTIONS_IMPL_HPP
#define MODEL_OUTPUTCONSTRUCTIONS_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputConstructions_Impl is a ModelObject_Impl that is the implementation class for OutputConstructions.*/
    class MODEL_API OutputConstructions_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      OutputConstructions_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputConstructions_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputConstructions_Impl(const OutputConstructions_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputConstructions_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      bool reportConstructions() const;

      bool reportMaterials() const;

      //@}
      /** @name Setters */
      //@{

      bool setReportConstructions(bool reportConstructions);

      bool setReportMaterials(bool reportMaterials);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputConstructions");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTCONSTRUCTIONS_IMPL_HPP
