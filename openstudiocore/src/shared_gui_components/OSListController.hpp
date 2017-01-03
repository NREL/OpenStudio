/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef SHAREDGUICOMPONENTS_OSLISTCONTROLLER_HPP
#define SHAREDGUICOMPONENTS_OSLISTCONTROLLER_HPP

#include <QObject>
#include <QSharedPointer>
#include <QPointer>
#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement
#include <vector>

class QWidget;
class QGraphicsObject;

namespace openstudio {

class OSListItem;
class OSItemSelectionController;

/** The purpose of OSListController is to provide an ordered list of items and signal when items have been added and removed.
 *  The OSListController class can also manage item selection, however this capability requires that OSListItem objects are 
 *  constructed with a pointer to the OSListController they are associated with -or- the item's controller has been set after creation.
 */
class OSListController : public QObject, public Nano::Observer
{
  Q_OBJECT

  public:

  OSListController();

  virtual ~OSListController();

  virtual QSharedPointer<OSListItem> itemAt(int i) = 0;

  virtual int count() = 0;

  // The OSListController is initialized with a default OSItemSelectionController class.  
  // If several lists are intended to share selections, for example if only one selection is allowed
  // among several lists, use setSelectionController to share one selection controller among many list controllers.
  QSharedPointer<OSItemSelectionController> selectionController() const;

  void setSelectionController(QSharedPointer<OSItemSelectionController> controller);

  signals:

  // Emit this signal when an item has been added to the underlying model or data structure.
  void itemInserted(int index); 

  // Emit this signal when an item has been removed from the underlying model or data structure.
  void itemRemoved(int index);

  // If the model or data structure beneath a single item has changed, 
  // emit this signal to update the view attached to the item at this index.
  void itemChanged(int index);

  // If the underlying model or data structure has been invalidated,
  // emit this signal to trigger a complete update of the attached views.
  void modelReset();

  private:

  friend class OSListItem;
  friend class OSItemSelectionController;

  // OSListItem classes use these methods to add and remove themselves from the list controller's internal vector, m_items.
  // These methods are used when a new item is constructed with a list controller argument, or when OSItem::setController is called.
  // The order of items in m_items is inconsequential.  Derived classes should not need to interact with these methods in any way.
  // The internal list m_items and the associated accessor methods are used to support internal logic related to item selection.
  // There is possibly a better choice for internal storage then std::vector.
  void registerItem(QPointer<OSListItem> item);

  void unregisterItem(QPointer<OSListItem> item);

  QSharedPointer<OSItemSelectionController> m_selectionController;

  std::vector<QPointer<OSListItem> > m_registeredItems;
};

/** The purpose of OSListItem is to encapsulate data and if necessary notify when the data changes.
 * The data encapsulated by OSListItem will often come from a separate domain model rather than from directly within the OSListItem.
 * OSListItem will commonly be subclassed and it is considered part of the controller hierarchy.
 */
class OSListItem : public QObject
{
  Q_OBJECT

  public:

  OSListItem(OSListController * listController = nullptr);

  virtual ~OSListItem();

  void setController(OSListController * controller);
  
  OSListController * controller() const { return m_listController; }

  bool isSelected() const;

  public slots:

  virtual void setSelected(bool isSelected);

  void toggleSelected();

  signals:

  void selectedChanged(bool isSelected);

  protected:

  QPointer<OSListController> m_listController;
};

// The purpose of OSItemSelectionController is to manage item selections.  You shouldn't often need to subclass.
// OSListController classes get initialized with one of these.
class OSItemSelectionController : public QObject, public Nano::Observer
{
  Q_OBJECT

  public:

  OSItemSelectionController();

  virtual ~OSItemSelectionController() {}

  void setAllowMultipleSelections(bool multipleSelections);

  bool allowMultipleSelections() const;

  std::vector<QPointer<OSListItem> > selectedItems() const;

  public slots:

  void unselectAllItems();

  void selectAllItems();

  void emitSelectionChanged();

  signals:

  void selectionChanged(std::vector<QPointer<OSListItem> > selectedItems);

  private:

  friend class OSListItem;
  friend class OSListController;

  // Accessor methods for m_listControllers. This class keeps a record of the list controllers it is associated with.
  void registerListController(OSListController * listController);

  void unregisterListController(OSListController * listController);

  // Accessor methods for m_selectedItems.  These are used by OSListItem to select and deselect items.
  // These methods will assert if item is NULL.
  //
  // This method is unintelligent so it will add items multiple times.  Thus avoiding the need to search.  Use carefully.
  void addSelectedItem(OSListItem * item);

  // This method will do nothing if the item is not selected.
  void removeSelectedItem(OSListItem * item);

  std::vector<QPointer<OSListItem> > m_selectedItems;

  // The list controllers that this selection controller is associated with
  // This member supports select all.
  std::vector<QPointer<OSListController> > m_listControllers;

  bool m_allowMultipleSelections;
};

/** The purpose of OSItemDelegate is to create a visual representation of an OSListItem and to connect the data provided by an 
 * OSListItem to the view.  OSItemDelegate should be subclassed, and the view() method should be reimplemented to provide a
 * QWidget that is not empty.  This class will be commonly subclassed and is a member of the controller logic.  It is particular
 * to the view and the data source.  If the view provided by OSItemDelegate is used often or if the view has a significant amount 
 * of detail, a separate view class should be defined outside of the OSItemDelegate and merely instantiated here.  On the other hand 
 * if the view very specific with little opportunity for reuse, and if the design is simple, it is acceptable for OSItemDelegate::view()
 * to build up a widget conglomeration on the fly from primitive widget types like QLabel, QWidget, etc.
 */
class OSItemDelegate : public QObject, public Nano::Observer
{
  Q_OBJECT

  public:

  virtual ~OSItemDelegate() {}

  virtual QWidget * view(QSharedPointer<OSListItem> dataSource);
};

/**  The purpose of OSGraphicsItemDelegate is the same as OSItemDelegate except it is used with QGraphicsObject instances instead of QWidget instances.
 */
class OSGraphicsItemDelegate : public QObject, public Nano::Observer
{
  Q_OBJECT;

  public:

  virtual ~OSGraphicsItemDelegate() {}

  virtual QGraphicsObject * view(QSharedPointer<OSListItem> dataSource);
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSLISTCONTROLLER_HPP

