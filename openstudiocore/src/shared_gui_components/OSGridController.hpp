/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_OSGRIDCONTROLLER_H
#define OPENSTUDIO_OSGRIDCONTROLLER_H

#include <QObject>
#include <QPointer>
#include <QSharedPointer>

#include <vector>

class QGraphicsObject;
class QWidget;

namespace openstudio {

class OSGridItem;
class OSGridItemSelectionController;

/** The purpose of OSGridController is to provide an ordered grid of items and signal when items have been added and removed.
 *  The OSGridController class can also manage item selection, however this capability requires that OSGridItem objects are 
 *  constructed with a pointer to the OSGridController they are associated with -or- the item's controller has been set after creation.
 */
class OSGridController : public QObject
{
  Q_OBJECT

  public:

  OSGridController();

  virtual ~OSGridController();

  virtual QSharedPointer<OSGridItem> itemAt(int row, int column) = 0;

  virtual int rowCount() = 0;
  
  virtual int columnCount() = 0;

  // The OSGridController is initialized with a default OSGridItemSelectionController class.  
  // If several grids are intended to share selections, for example if only one selection is allowed
  // among several grids, use setSelectionController to share one selection controller among many grid constrollers.
  QSharedPointer<OSGridItemSelectionController> selectionController() const;

  void setSelectionController(QSharedPointer<OSGridItemSelectionController> controller);

  signals:

  // Emit this signal when an item has been added to the underlying model or data structure.
  void itemInserted(int row, int column); 

  // Emit this signal when an item has been removed from the underlying model or data structure.
  void itemRemoved(int row, int column);

  // If the model or data structure beneath a single item has changed, 
  // emit this signal to update the view attached to the item at this index.
  void itemChanged(int row, int column);

  // If the underlying model or data structure has been invalidated,
  // emit this signal to trigger a complete update of the attached views.
  void modelReset();

  private:

  friend class OSGridItem;
  friend class OSGridItemSelectionController;

  // OSGridItem classes use these methods to add and remove themselves from the grid controller's internal vector, m_items.
  // These methods are used when a new item is constructed with a grid controller argument, or when OSItem::setController is called.
  // The order of items in m_items is inconsequential.  Derived classes should not need to interact with these methods in any way.
  // The internal grid m_items and the associated accessor methods are used to support internal logic related to item selection.
  // There is possibly a better choice for internal storage then std::vector.
  void registerItem(QPointer<OSGridItem> item);

  void unregisterItem(QPointer<OSGridItem> item);

  QSharedPointer<OSGridItemSelectionController> m_selectionController;

  std::vector<QPointer<OSGridItem> > m_registeredItems;
};

/** The purpose of OSGridItem is to encapsulate data and if necessary notify when the data changes.
 * The data encapsulated by OSGridItem will often come from a separate domain model rather than from directly within the OSGridItem.
 * OSGridItem will commonly be subclassed and it is considered part of the controller hierarchy.
 */
class OSGridItem : public QObject
{
  Q_OBJECT

  public:

  OSGridItem(OSGridController * gridController = 0);

  virtual ~OSGridItem();

  void setController(OSGridController * controller);
  
  OSGridController * controller() const { return m_gridController; }

  bool isSelected() const;

  public slots:

  virtual void setSelected(bool isSelected);

  void toggleSelected();

  signals:

  void selectedChanged(bool isSelected);

  protected:

  QPointer<OSGridController> m_gridController;
};

// The purpose of OSGridItemSelectionController is to manage item selections.  You shouldn't often need to subclass.
// OSGridController classes get initialized with one of these.
class OSGridItemSelectionController : public QObject
{
  Q_OBJECT

  public:

  OSGridItemSelectionController();

  virtual ~OSGridItemSelectionController() {}

  void setAllowMultipleSelections(bool multipleSelections);

  bool allowMultipleSelections() const;

  std::vector<QPointer<OSGridItem> > selectedItems() const;

  public slots:

  void unselectAllItems();

  void selectAllItems();

  void emitSelectionChanged();

  signals:

  void selectionChanged(std::vector<QPointer<OSGridItem> > selectedItems);

  private:

  friend class OSGridItem;
  friend class OSGridController;

  // Accessor methods for m_gridControllers. This class keeps a record of the grid controllers it is associated with.
  void registerGridController(OSGridController * gridController);

  void unregisterGridController(OSGridController * gridController);

  // Accessor methods for m_selectedItems.  These are used by OSGridItem to select and unselect items.
  // These methods will assert if item is NULL.
  //
  // This method is unintelligent so it will add items multiple times.  Thus avoiding the need to search.  Use carefully.
  void addSelectedItem(OSGridItem * item);

  // This method will do nothing if the item is not selected.
  void removeSelectedItem(OSGridItem * item);

  std::vector<QPointer<OSGridItem> > m_selectedItems;

  // The grid controllers that this selection controller is associated with
  // This member supports select all.
  std::vector<QPointer<OSGridController> > m_gridControllers;

  bool m_allowMultipleSelections;
};

/** The purpose of OSGridItemDelegate is to create a visual representation of an OSGridItem and to connect the data provided by an 
 * OSGridItem to the view.  OSGridItemDelegate should be subclassed, and the view() method should be reimplemented to provide a
 * QWidget that is not empty.  This class will be commonly subclassed and is a member of the controller logic.  It is particular
 * to the view and the data source.  If the view proivded by OSGridItemDelegate is used often or if the view has a signficant amount 
 * of detail, a seperate view class should be defined outside of the OSGridItemDelegate and merely instantiated here.  On the other hand 
 * if the view very specific with little opportunity for reuse, and if the design is simple, it is acceptable for OSGridItemDelegate::view()
 * to build up a widget conglomeration on the fly from primitive widget types like QLabel, QWidget, etc.
 */
class OSGridItemDelegate : public QObject
{
  Q_OBJECT

  public:

  virtual ~OSGridItemDelegate() {}

  virtual QWidget * view(QSharedPointer<OSGridItem> dataSource);
};

} // openstudio

#endif // OPENSTUDIO_OSGRIDCONTROLLER_H

