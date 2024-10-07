/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_WORKSPACEOBJECTORDER_HPP
#define UTILITIES_IDF_WORKSPACEOBJECTORDER_HPP

#include "../UtilitiesAPI.hpp"

#include "Handle.hpp"
#include "WorkspaceObject.hpp"
#include "ObjectOrderBase.hpp"

#include <optional>

namespace openstudio {

struct IddObjectType;

namespace detail {

  class Workspace_Impl;  // forward declaration

  /** Implementation of WorkspaceObjectOrder. */
  class UTILITIES_API WorkspaceObjectOrder_Impl : public ObjectOrderBase
  {
   public:
    using ObjectGetter = std::function<boost::optional<WorkspaceObject>(const Handle&)>;

    WorkspaceObjectOrder_Impl(const ObjectGetter& objectGetter);

    WorkspaceObjectOrder_Impl(const std::vector<Handle>& directOrder, const ObjectGetter& objectGetter);

    WorkspaceObjectOrder_Impl(const std::vector<IddObjectType>& iddOrder, const ObjectGetter& objectGetter);

    virtual ~WorkspaceObjectOrder_Impl() override = default;

    // GETTERS AND SETTERS

    /// Returns whether or not direct ordered. Otherwise, is ordered by IddObjectType.
    bool isDirectOrder() const;
    /// returns the direct order. return value is false if not ordering this way.
    boost::optional<std::vector<Handle>> directOrder() const;
    /// deletes other ordering options and sets direct order
    void setDirectOrder(const std::vector<Handle>& order);
    /// returns false if not ordering directly
    bool push_back(const Handle& handle);
    /// returns false if not ordering directly
    bool insert(const Handle& handle, const Handle& insertBeforeHandle);
    /// returns false if not ordering directly
    bool insert(const Handle& handle, unsigned index);
    /// returns false if not ordering directly, or request is otherwise invalid
    bool move(const Handle& handle, const Handle& insertBeforeHandle);
    /// returns false if not ordering directly, or request is otherwise invalid
    bool move(const Handle& handle, unsigned index);
    /// returns false if not ordering directly, or request is otherwise invalid
    bool swap(const Handle& handle1, const Handle& handle2);
    /// returns false if not ordering directly, or request is otherwise invalid
    bool erase(const Handle& handle);

    /// deletes other ordering options and orders by IddObjectType
    virtual void setOrderByIddEnum() override;
    /// deletes other ordering options and sets IddObjectType order
    virtual void setIddOrder(const std::vector<IddObjectType>& order) override;

    // SORTING

    /** Predicate for external sorters (for instance, std::set). */
    bool less(const Handle& left, const Handle& right) const;

    /** Predicate for external sorters (for instance, std::set). */
    bool less(const WorkspaceObject& left, const WorkspaceObject& right) const;

    /** Predicate for external sorters (for instance, std::set). */
    bool less(IddObjectType left, IddObjectType right) const override;

    std::vector<Handle> sort(const std::vector<Handle>& handles) const;

    std::vector<openstudio::WorkspaceObject> sort(const std::vector<openstudio::WorkspaceObject>& objects) const;

    /// returns empty vector if not all handles can be converted to objects
    std::vector<WorkspaceObject> sortedObjects(const std::vector<Handle>& handles) const;

    /// returns empty vector if not all objects can be converted to handles
    std::vector<Handle> sortedHandles(const std::vector<WorkspaceObject>& objects) const;

    // STATE CHECKING

    /** Returns whether order of handle is directly specified. */
    bool inOrder(const Handle& handle) const;

    /** Returns index of handle in order, if its order is directly specified. */
    boost::optional<unsigned> indexInOrder(const Handle& handle) const;

   protected:
    friend class Workspace_Impl;

    void setObjectGetter(const ObjectGetter& getter);

   private:
    ObjectGetter m_objectGetter;
    boost::optional<std::vector<Handle>> m_directOrder;

    REGISTER_LOGGER("utilities.idf.WorkspaceObjectOrder");

    // HELPER FUNCTIONS

    // only call when m_directOrder == true
    std::vector<Handle>::iterator getIterator(const Handle& handle);
    std::vector<Handle>::iterator getIterator(IddObjectType type);
    std::vector<Handle>::iterator getIterator(const WorkspaceObject& object);

    std::vector<Handle>::const_iterator getIterator(const Handle& handle) const;
    std::vector<Handle>::const_iterator getIterator(IddObjectType type) const;
    std::vector<Handle>::const_iterator getIterator(const WorkspaceObject& object) const;

    boost::optional<IddObjectType> getIddObjectType(const Handle& handle) const;

    // returns empty vector if can't convert all.
    WorkspaceObjectVector getObjects(const std::vector<Handle>& handles) const;
  };

  using WorkspaceObjectOrder_ImplPtr = std::shared_ptr<WorkspaceObjectOrder_Impl>;
}  // namespace detail

class Workspace;  // forward declaration

/** Public interface of WorkspaceObjectOrder. Accessed through Workspace as in:
 *
 *  \code
 *  Workspace ws;
 *  WorkspaceObjectOrder orderer = workspace.order();
 *  \endcode
 *
 *  This interface is provided to let users specify the order in which objects and
 *  handles are returned by Workspace. Sorting is accessed through the workspace itself.
 *  For instance,
 *
 *  \code
 *  ws.objects(true)
 *  \endcode */
class UTILITIES_API WorkspaceObjectOrder
{
 public:
  virtual ~WorkspaceObjectOrder() = default;

  // GETTERS AND SETTERS

  /// returns whether or not currently ordering by IddObjectType
  bool orderByIddEnum() const;
  /// deletes other ordering options and orders by IddObjectType
  void setOrderByIddEnum();

  /** returns the user-specified order of IddObjectTypes. return value is false if
   *  not ordering this way. */
  boost::optional<std::vector<IddObjectType>> iddOrder() const;
  /// deletes other ordering options and sets IddObjectType order
  void setIddOrder(const std::vector<IddObjectType>& order);
  /** returns false if not ordering by user-specified IddObjectType order, or request is
   *  otherwise invalid. */
  bool push_back(IddObjectType type);
  /** returns false if not ordering by user-specified IddObjectType order. */
  bool insert(IddObjectType type, IddObjectType insertBeforeType);
  /** returns false if not ordering by user-specified IddObjectType order. */
  bool insert(IddObjectType type, unsigned index);
  /** move type from current location and place before 'insertBeforeType'. returns
   *  false if not ordering by user-specified IddObjectType order, or request is otherwise
   *  invalid. type must already be in the order. if insertBeforeType is not in the ordering,
   *  places type at the end. */
  bool move(IddObjectType type, IddObjectType insertBeforeType);
  /** move type from current location to index. returns false if not ordering by
   *  user-specified IddObjectType order, or request is otherwise invalid. type must already
   *  be in the order. if index is invalid, places type at the end. */
  bool move(IddObjectType type, unsigned index);
  /** returns false if not ordering by user-specified IddObjectType, or request is
   *  otherwise invalid. */
  bool swap(IddObjectType type1, IddObjectType type2);
  /** returns false if not ordering by user-specified IddObjectType, or request is
   *  otherwise invalid. */
  bool erase(IddObjectType type);

  /// Returns whether or not direct ordered. Otherwise, is ordered by IddObjectType.
  bool isDirectOrder() const;
  /// returns the direct order. return value is false if not ordering this way.
  boost::optional<std::vector<Handle>> directOrder() const;
  /// deletes other ordering options and sets direct order
  void setDirectOrder(const std::vector<Handle>& order);
  /// returns false if not ordering directly
  bool push_back(const Handle& handle);
  /// returns false if not ordering directly
  bool insert(const Handle& handle, const Handle& insertBeforeHandle);
  /// returns false if not ordering directly
  bool insert(const Handle& handle, unsigned index);
  /// returns false if not ordering directly, or request is otherwise invalid
  bool move(const Handle& handle, const Handle& insertBeforeHandle);
  /// returns false if not ordering directly, or request is otherwise invalid
  bool move(const Handle& handle, unsigned index);
  /// returns false if not ordering directly, or request is otherwise invalid
  bool swap(const Handle& handle1, const Handle& handle2);
  /// returns false if not ordering directly, or request is otherwise invalid
  bool erase(const Handle& handle);

  // SORTING

  std::vector<Handle> sort(const std::vector<Handle>& handles) const;

  std::vector<openstudio::WorkspaceObject> sort(const std::vector<openstudio::WorkspaceObject>& objects) const;

  // STATE CHECKING

  /** Returns whether order of handle is directly specified. */
  bool inOrder(const Handle& handle) const;

  /** Returns index of handle in order, if its order is directly specified. */
  boost::optional<unsigned> indexInOrder(const Handle& handle) const;

 protected:
  friend class Workspace;
  friend class detail::Workspace_Impl;

  // Workspace constructs the public interface of the orderer when it is requested.
  WorkspaceObjectOrder(const std::shared_ptr<detail::WorkspaceObjectOrder_Impl>& impl);

 private:
  std::shared_ptr<detail::WorkspaceObjectOrder_Impl> m_impl;

  REGISTER_LOGGER("utilities.idf.WorkspaceObjectOrder");
};

}  // namespace openstudio

#endif  // UTILITIES_IDF_WORKSPACEOBJECTORDER_HPP
