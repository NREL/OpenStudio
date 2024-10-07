/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MODELOBJECTLIST_HPP
#define MODEL_MODELOBJECTLIST_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ModelObjectList_Impl;

  }  // namespace detail

  /** ModelObjectList is a ModelObject that wraps the OpenStudio IDD object 'OS:ModelObjectList'. */
  class MODEL_API ModelObjectList : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ModelObjectList(const Model& model);

    virtual ~ModelObjectList() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ModelObjectList(const ModelObjectList& other) = default;
    ModelObjectList(ModelObjectList&& other) = default;
    ModelObjectList& operator=(const ModelObjectList&) = default;
    ModelObjectList& operator=(ModelObjectList&&) = default;

    //@}

    static IddObjectType iddObjectType();

    std::vector<IdfObject> remove();

    ModelObject clone(Model model) const;

    unsigned size() const;

    bool addModelObject(const ModelObject& modelObject);

    bool hasModelObject(const ModelObject& modelObject) const;

    void removeModelObject(const ModelObject& modelObject);

    void removeAllModelObjects();

    std::vector<ModelObject> modelObjects() const;

   protected:
    /// @cond
    using ImplType = detail::ModelObjectList_Impl;

    explicit ModelObjectList(std::shared_ptr<detail::ModelObjectList_Impl> impl);

    friend class detail::ModelObjectList_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ModelObjectList");
  };

  /** \relates ModelObjectList*/
  using OptionalModelObjectList = boost::optional<ModelObjectList>;

  /** \relates ModelObjectList*/
  using ModelObjectListVector = std::vector<ModelObjectList>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MODELOBJECTLIST_HPP
