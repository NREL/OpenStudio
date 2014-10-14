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

#ifndef OPENSTUDIO_SPACETYPESGRIDVIEW_H
#define OPENSTUDIO_SPACETYPESGRIDVIEW_H

#include <shared_gui_components/OSGridController.hpp>
#include <openstudio_lib/OSItem.hpp>

#include <model/Model.hpp>

#include <QWidget>

namespace openstudio{

class ModelSubTabView;

class SpaceTypesGridView : public QWidget
{
  Q_OBJECT

public:

  SpaceTypesGridView(bool isIP, const model::Model & model, QWidget * parent = 0);

  virtual ~SpaceTypesGridView() {}

private:

  bool m_isIP;

signals:

  void toggleUnitsClicked(bool displayIP);

  void dropZoneItemClicked(OSItem* item);

  void itemSelected(OSItem * item);

  void selectionCleared();

  void gridRowSelected(OSItem*);

private slots:

  void onDropZoneItemClicked(OSItem* item);

};

class SpaceTypesGridController : public OSGridController
{

  Q_OBJECT

public:

  SpaceTypesGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects);

  virtual ~SpaceTypesGridController() {}

  virtual void refreshModelObjects();

protected:

  virtual void setCategoriesAndFields();

  virtual void addColumns(std::vector<QString> & fields);

  virtual void checkSelectedFields();

  virtual QString getColor(const model::ModelObject & modelObject);

public slots:

  virtual void onItemDropped(const OSItemId& itemId);

  virtual void onComboBoxIndexChanged(int index);

};

} // openstudio

#endif // OPENSTUDIO_SPACETYPESGRIDVIEW_H

