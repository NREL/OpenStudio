/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
 *  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef SHAREDGUICOMPONENTS_OSLISTCONTROLLER_HPP
#define SHAREDGUICOMPONENTS_OSLISTCONTROLLER_HPP

#include <QObject>
#include <QSharedPointer>
#include <QPointer>
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
class OSListController : public QObject
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
class OSItemSelectionController : public QObject
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
class OSItemDelegate : public QObject
{
  Q_OBJECT

  public:

  virtual ~OSItemDelegate() {}

  virtual QWidget * view(QSharedPointer<OSListItem> dataSource);
};

/**  The purpose of OSGraphicsItemDelegate is the same as OSItemDelegate except it is used with QGraphicsObject instances instead of QWidget instances.
 */
class OSGraphicsItemDelegate : public QObject
{
  Q_OBJECT;

  public:

  virtual ~OSGraphicsItemDelegate() {}

  virtual QGraphicsObject * view(QSharedPointer<OSListItem> dataSource);
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSLISTCONTROLLER_HPP

