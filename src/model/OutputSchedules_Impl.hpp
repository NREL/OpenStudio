/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTSCHEDULES_IMPL_HPP
#define MODEL_OUTPUTSCHEDULES_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputSchedules_Impl is a ModelObject_Impl that is the implementation class for OutputSchedules.*/
    class MODEL_API OutputSchedules_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      OutputSchedules_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputSchedules_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputSchedules_Impl(const OutputSchedules_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputSchedules_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string keyField() const;

      //@}
      /** @name Setters */
      //@{

      bool setKeyField(const std::string& keyField);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputSchedules");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTSCHEDULES_IMPL_HPP
