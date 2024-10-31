/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PHOTOVOLTAICPERFORMANCE_IMPL_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCE_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API PhotovoltaicPerformance_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PhotovoltaicPerformance_Impl(IddObjectType type, Model_Impl* model);

      PhotovoltaicPerformance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PhotovoltaicPerformance_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PhotovoltaicPerformance_Impl(const PhotovoltaicPerformance_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~PhotovoltaicPerformance_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      /// do not allow this object to be removed if it is referenced by a PhotovoltaicGenerator
      virtual std::vector<openstudio::IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      // DLM: really don't want to implement this for this object, feels too kludgy
      //virtual boost::optional<ParentObject> parent() override;

      //@}
      /** @name Setters */
      //@{

      // DLM: really don't want to implement this for this object, feels too kludgy
      //virtual bool setParent(ParentObject& newParent) override;

      //@}
      /** @name Other */
      //@{

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.PhotovoltaicPerformance");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PHOTOVOLTAICPERFORMANCE_IMPL_HPP
