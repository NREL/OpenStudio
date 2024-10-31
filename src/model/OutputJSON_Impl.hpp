/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTJSON_IMPL_HPP
#define MODEL_OUTPUTJSON_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputJSON_Impl is a ModelObject_Impl that is the implementation class for OutputJSON.*/
    class MODEL_API OutputJSON_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      OutputJSON_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputJSON_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputJSON_Impl(const OutputJSON_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputJSON_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string optionType() const;

      bool outputJSON() const;

      bool outputCBOR() const;

      bool outputMessagePack() const;

      //@}
      /** @name Setters */
      //@{

      bool setOptionType(const std::string& optionType);

      bool setOutputJSON(bool outputJSON);

      bool setOutputCBOR(bool outputCBOR);

      bool setOutputMessagePack(bool outputMessagePack);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputJSON");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTJSON_IMPL_HPP
