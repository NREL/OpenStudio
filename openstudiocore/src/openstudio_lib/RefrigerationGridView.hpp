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
#include <openstudio_lib/OSItem.hpp>

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

class RefrigerationCaseGridController : public OSGridController
{

  Q_OBJECT

public:

  RefrigerationCaseGridController(const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects);

  virtual ~RefrigerationCaseGridController() {}

protected:

  virtual void setCategoriesAndFields();

  virtual void addColumns(const std::vector<QString> & fields);


public slots:

  virtual void onItemDropped(const OSItemId& itemId);

};

class RefrigerationWalkInGridController : public OSGridController
{

  Q_OBJECT

public:

  RefrigerationWalkInGridController(const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects);

  virtual ~RefrigerationWalkInGridController() {}

protected:

  virtual void setCategoriesAndFields();

  virtual void addColumns(const std::vector<QString> & fields);

public slots:

  virtual void onItemDropped(const OSItemId& itemId);

};

} // openstudio

#endif // OPENSTUDIO_REFRIGERATIONGRIDVIEW_H

