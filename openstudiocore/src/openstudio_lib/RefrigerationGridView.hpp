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

#ifndef OPENSTUDIO_REFRIGERATIONGRIDVIEW_HPP
#define OPENSTUDIO_REFRIGERATIONGRIDVIEW_HPP

#include "../shared_gui_components/OSGridController.hpp"
#include "OSItem.hpp"

#include "../model/Model.hpp"

#include <QWidget>

namespace openstudio{

class RefrigerationGridView : public QWidget
{
  Q_OBJECT

public:

  RefrigerationGridView(bool isIP, const model::Model & model, QWidget * parent = 0);

  virtual ~RefrigerationGridView() {}

private:

  bool m_isIP;

signals:

  void toggleUnitsClicked(bool displayIP);

};

class RefrigerationCaseGridController : public OSGridController
{

  Q_OBJECT

public:

  RefrigerationCaseGridController(bool isIP,
    const QString & headerText,
    model::Model model,
    std::vector<model::ModelObject> modelObjects);

  virtual ~RefrigerationCaseGridController() {}

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

class RefrigerationWalkInGridController : public OSGridController
{

  Q_OBJECT

public:

  RefrigerationWalkInGridController(bool isIP,
    const QString & headerText,
    model::Model model,
    std::vector<model::ModelObject> modelObjects);

  virtual ~RefrigerationWalkInGridController() {}

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

#endif // OPENSTUDIO_REFRIGERATIONGRIDVIEW_HPP

