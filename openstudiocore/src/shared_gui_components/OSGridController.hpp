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

class QWidget;

namespace openstudio {

class OSGridController : public QObject
{
  Q_OBJECT

  public:

  // This form requires clients to subclass OSGridController and
  // reimplement rowCount(), and itemAt()
  OSGridController();

  // This form utilizes the default implementations of 
  // rowCount() and itemAt(), showing one row for each object 
  // in the model that is iddObjectType
  OSGridController(IddObjectType iddObjectType, model::Model model);

  virtual ~OSGridController() {};

  bool addComboBoxColumn(QString title, string property, QString label, QSharedPointer<OSListController> choicesController)

  bool addDoubleColumn(QString property,QString ipunits, QString siunits);

  bool addIntegerColumn(std::string property,QString ipunits, QString siunits);

  bool addLineEditColumn(std::string property);

  bool addCheckBoxColumn(std::string property);

  bool addUnsignedColumn(std::string property,QString ipunits, QString siunits);

  // Maybe
  bool addColumn(QString title, QSharedPointer<OSListController> columnController, QSharedPointer<OSItemDelegate> columnDelegate);

  void setVerticalHeader(bool visible, QString title);

  void setHorizontalHeader(std::vector<QWidget> widgets);

  void setHorizontalHeader(std::vector<QString> names);

  virtual int rowCount() const;

  virtual int columnCount() const;

  virtual QSharedPointer<OSListItem> itemAt(int i, int j);

  signals:

  // These signals will be used by the GridView to update
  // An update will involve calls to itemAt() and maybe rowCount and columnCount()

  // Emit this signal when an item has been added to the underlying model or data structure.
  void itemInserted(int row, int column); 

  // Emit this signal when an item has been removed from the underlying model or data structure.
  void itemRemoved(int row, int column);

  // If the model or data structure beneath a single item has changed, 
  // emit this signal to update the view attached to the item at this index.
  void itemChanged(int row, int column);

  // Nuclear reset of everything
  void modelReset();
};

  m_choicesController = new OSListControllerDerivedClass(); 

  ModelObjectGridInspector * mogi = new ModelObjectGridInspector();

  mogi.addRows(someVectorOfModelObjects);

  mogi.addRows(someOtherVectorOfModelObjects);

  mogi.addComboBoxColumn("Bar","Bar Label");

  mogi.addCheckBoxColumn("Foo","Foo Label");

  // OR // 

  ModelObjectGridInspector * mogi = new ModelObjectGridInspector();

  mogi.addComboBoxColumn<OSListControllerDerivedClass>("Bar","Bar Label");

  mogi.addCheckBoxColumn("Foo","Foo Label");

  mogi.addRows(someOtherVectorOfModelObjects);

} // openstudio

#endif // OPENSTUDIO_OSGRIDCONTROLLER_H

