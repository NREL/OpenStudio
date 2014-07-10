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

#ifndef SHAREDGUICOMPONENTS_OSGRIDVIEW_HPP
#define SHAREDGUICOMPONENTS_OSGRIDVIEW_HPP

#include <QWidget>

#include "../model/ModelObject.hpp"

class QGridLayout;
class QLabel;
class QString;

namespace openstudio{

class OSCollapsibleView;
class OSGridController;

class OSGridView : public QWidget
{
  Q_OBJECT

public:

  OSGridView(OSGridController * gridController,  const QString & headerText, const QString & dropZoneText, QWidget * parent = nullptr);

  virtual ~OSGridView() {};

  QGridLayout * m_gridLayout;

signals:

  void cellClicked(int row, int column);

  void rowClicked(int row);

  void columnClicked(int column);

private slots:

  void refresh(int row, int column);

  void deleteAll();

  void refreshAll();

  void addWidget(int row, int column);

  void removeWidget(int row, int column);

  void setHorizontalHeader(std::vector<QWidget *> widgets);

  void setHorizontalHeader(std::vector<QString> names);

  void selectCategory(int index);

private:

  void setGridController(OSGridController * gridController);

  void refreshRow(model::ModelObject modelObject);

  void refreshColumn(int columnId);

  void selectCell(int row, int column);

  void selectRow(int row);

  void selectColumn(int column);

  OSCollapsibleView * m_CollapsibleView;

  OSGridController * m_gridController;

};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSGRIDVIEW_HPP

