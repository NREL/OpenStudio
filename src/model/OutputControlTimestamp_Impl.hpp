/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTCONTROLTIMESTAMP_IMPL_HPP
#define MODEL_OUTPUTCONTROLTIMESTAMP_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputControlTimestamp_Impl is a ModelObject_Impl that is the implementation class for OutputControlTimestamp.*/
    class MODEL_API OutputControlTimestamp_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      OutputControlTimestamp_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputControlTimestamp_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputControlTimestamp_Impl(const OutputControlTimestamp_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputControlTimestamp_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      bool iso8601Format() const;

      bool timestampAtBeginningOfInterval() const;

      //@}
      /** @name Setters */
      //@{

      bool setISO8601Format(bool iso8601Format);

      bool setTimestampAtBeginningOfInterval(bool timestampAtBeginningOfInterval);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputControlTimestamp");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTCONTROLTIMESTAMP_IMPL_HPP
