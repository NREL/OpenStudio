/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef UTILITIES_IDF_WORKSPACEOBJECTORDER_HPP
#define UTILITIES_IDF_WORKSPACEOBJECTORDER_HPP

#include "../UtilitiesAPI.hpp"

#include "Handle.hpp"
#include "WorkspaceObject.hpp"
#include "ObjectOrderBase.hpp"

namespace openstudio {

struct IddObjectType;

namespace detail {

  class Workspace_Impl; // forward declaration

  /** Implementation of WorkspaceObjectOrder. */
  class UTILITIES_API WorkspaceObjectOrder_Impl : public ObjectOrderBase {
   public:
    typedef std::function<boost::optional<WorkspaceObject> (const Handle&)> ObjectGetter;

    WorkspaceObjectOrder_Impl(const ObjectGetter& objectGetter);

    WorkspaceObjectOrder_Impl(const std::vector<Handle>& directOrder,
                              const ObjectGetter& objectGetter);

    WorkspaceObjectOrder_Impl(const std::vector<IddObjectType>& iddOrder,
                              const ObjectGetter& objectGetter);

    virtual ~WorkspaceObjectOrder_Impl() {}

    // GETTERS AND SETTERS

    /// Returns whether or not direct ordered. Otherwise, is ordered by IddObjectType.
    bool isDirectOrder() const;
    /// returns the direct order. return value is false if not ordering this way.
    boost::optional< std::vector<Handle> > directOrder() const;
    /// deletes other ordering options and sets direct order
    void setDirectOrder(const std::vector<Handle>& order);
    /// returns false if not ordering directly
    bool push_back(const Handle& handle);
    /// returns false if not ordering directly
    bool insert(const Handle& handle, const Handle& insertBeforeHandle);
    /// returns false if not ordering directly
    bool insert(const Handle& handle,unsigned index);
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
    bool less(const Handle& left,const Handle& right) const;

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
    boost::optional< std::vector<Handle> > m_directOrder;

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

    // ETH@20100409 boost::bind seems to need non-overloaded functions
    // These are (ugly) wrappers to accommodate.
    bool less_Handle(const Handle& left,const Handle& right) const;
    bool less_WorkspaceObject(const WorkspaceObject& left,const WorkspaceObject& right) const;
    virtual bool less_IddObjectType(IddObjectType left, IddObjectType right) const;

  };

  typedef std::shared_ptr<WorkspaceObjectOrder_Impl> WorkspaceObjectOrder_ImplPtr;
} // detail

class Workspace; // forward declaration

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
class UTILITIES_API WorkspaceObjectOrder {
 public:

  virtual ~WorkspaceObjectOrder() {}

  // GETTERS AND SETTERS

  /// returns whether or not currently ordering by IddObjectType
  bool orderByIddEnum() const;
  /// deletes other ordering options and orders by IddObjectType
  void setOrderByIddEnum();

  /** returns the user-specified order of IddObjectTypes. return value is false if
   *  not ordering this way. */
  boost::optional< std::vector<IddObjectType> > iddOrder() const;
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
  boost::optional< std::vector<Handle> > directOrder() const;
  /// deletes other ordering options and sets direct order
  void setDirectOrder(const std::vector<Handle>& order);
  /// returns false if not ordering directly
  bool push_back(const Handle& handle);
  /// returns false if not ordering directly
  bool insert(const Handle& handle, const Handle& insertBeforeHandle);
  /// returns false if not ordering directly
  bool insert(const Handle& handle,unsigned index);
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

} // openstudio

#endif // UTILITIES_IDF_WORKSPACEOBJECTORDER_HPP
