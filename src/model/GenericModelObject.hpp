/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERICMODELOBJECT_HPP
#define MODEL_GENERICMODELOBJECT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {
  namespace detail {
    class GenericModelObject_Impl;
  }  // namespace detail

  /** GenericModelObject is a generic interface for \link ModelObject ModelObjects\endlink
 *  that do not yet have a customized interface. */

  /** GenericModelObject derives from ModelObject and is a generic interface to any OpenStudio IDD object..
 *
 *  GenericModelObject is different from all other \link ModelObject ModelObjects \endlink in that
 *  it does not wrap a specific OpenStudio IDD object.  Instead it is a generic wrapper that can hold
 *  OpenStudio IDD objects of any type.  When objects are added to Model, a large switch statement is
 *  used to construct wrappers of the correct type for each new object.  If the type of a newly added
 *  object has not yet been wrapped by a specific ModelObject then a GenericModelObject is used to hold
 *  a reference to the data.  GenericModelObject is planned to be deprecated once all OpenStudio IDD types
 *  are wrapped by specific \link ModelObject ModelObjects \endlink, this class only exists in the interim
 *  so translation to and from EnergyPlus via the implicit translator does not lose data objects.
 *
 */
  class MODEL_API GenericModelObject : public ModelObject
  {
   public:
    virtual ~GenericModelObject() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GenericModelObject(const GenericModelObject& other) = default;
    GenericModelObject(GenericModelObject&& other) = default;
    GenericModelObject& operator=(const GenericModelObject&) = default;
    GenericModelObject& operator=(GenericModelObject&&) = default;

   protected:
    /// @cond

    using ImplType = detail::GenericModelObject_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit GenericModelObject(std::shared_ptr<detail::GenericModelObject_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.GenericModelObject");

    /// @endcond
  };

  // optional GenericModelObject
  using OptionalGenericModelObject = boost::optional<GenericModelObject>;

  // vector of GenericModelObject
  using GenericModelObjectVector = std::vector<GenericModelObject>;

}  // namespace model
}  // namespace openstudio

#endif  //MODEL_GENERICMODELOBJECT_HPP
