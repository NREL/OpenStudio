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

#ifndef OPENSTUDIO_REFRIGERATIONGRIDVIEW_H
#define OPENSTUDIO_REFRIGERATIONGRIDVIEW_H

#include <shared_gui_components/OSGridController.hpp>
#include <openstudio_lib/ModelObjectVectorController.hpp>
#include <openstudio_lib/OSDropZone.hpp>

#include <model/Model.hpp>

#include <QWidget>

namespace openstudio{

class RefrigerationGridView : public QWidget
{
  Q_OBJECT

public:

  RefrigerationGridView(const model::Model & model, QWidget * parent = 0);

  virtual ~RefrigerationGridView() {}

};

class CaseThermalZoneVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~CaseThermalZoneVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class RefrigerationCaseGridController : public OSGridController
{

public:

  RefrigerationCaseGridController(const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects);

  virtual ~RefrigerationCaseGridController() {}

protected:

  virtual void setCategoriesAndFields();

  virtual void addColumns(const std::vector<QString> & fields);

private:

  CaseThermalZoneVectorController * m_thermalZoneVectorController;
  OSDropZone * m_thermalZoneDropZone;

};

class RefrigerationWalkInGridController : public OSGridController
{

public:

  RefrigerationWalkInGridController(const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects);

  virtual ~RefrigerationWalkInGridController() {}

protected:

  virtual void setCategoriesAndFields();

  virtual void addColumns(const std::vector<QString> & fields);

};

} // openstudio

#endif // OPENSTUDIO_REFRIGERATIONGRIDVIEW_H

