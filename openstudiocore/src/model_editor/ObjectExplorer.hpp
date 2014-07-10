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

#ifndef MODELEDITOR_OBJECTEXPLORER_HPP
#define MODELEDITOR_OBJECTEXPLORER_HPP

#include <QWidget>

#include "../utilities/idf/IdfFile.hpp"

class QLineEdit;
class QListWidget;

class ListWidget;

namespace modeleditor
{

class ObjectExplorer : public QWidget
{
  Q_OBJECT

public:
  ObjectExplorer(openstudio::IddFile& iddFile, QWidget * parent = nullptr);
  virtual ~ObjectExplorer();
  void updateIddFile();

public slots:
  void on_groupTextEdited(const QString&);
  void on_objectTextEdited(const QString&);
  void on_groupItemSelectionChanged();
  void on_objectItemSelectionChanged();

signals:

protected:

private:
  void createWidgets();
  void connectSignalsAndSlots();
  void createLayout();
  void groupTextEdited(const QString& text);
  void objectTextEdited(const QString& text);
  void groupItemSelectionChanged();
  void objectItemSelectionChanged();

  QLineEdit * mGroupEdit;
  QLineEdit * mObjectEdit;
  QListWidget * mGroupList;
  ListWidget * mObjectList;
  openstudio::IddFile mIddFile;
};

} // namespace modeleditor

#endif // MODELEDITOR_OBJECTEXPLORER_HPP
