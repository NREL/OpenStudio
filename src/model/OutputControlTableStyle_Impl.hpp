/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTCONTROLTABLESTYLE_IMPL_HPP
#define MODEL_OUTPUTCONTROLTABLESTYLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputControlTableStyle_Impl is a ModelObject_Impl that is the implementation class for OutputControlTableStyle.*/
    class MODEL_API OutputControlTableStyle_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      OutputControlTableStyle_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputControlTableStyle_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputControlTableStyle_Impl(const OutputControlTableStyle_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputControlTableStyle_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string columnSeparator() const;

      std::string unitConversion() const;

      //@}
      /** @name Setters */
      //@{

      bool setColumnSeparator(const std::string& columnSeparator);

      bool setUnitConversion(const std::string& unitConversion);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputControlTableStyle");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTCONTROLTABLESTYLE_IMPL_HPP
