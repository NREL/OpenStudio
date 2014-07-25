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

#ifndef OPENSTUDIO_MODELSUBTABVIEW_HPP
#define OPENSTUDIO_MODELSUBTABVIEW_HPP

#include "MainTabView.hpp"
#include "SubTabView.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include <QWidget>

class QPushButton;
class QScrollArea;

namespace openstudio {

class OSItemSelector;
class OSItemSelectorButtons;
class OSItem;
class OSItemId;
class ModelObjectInspectorView;


class ModelSubTabView : public SubTabView
{
  Q_OBJECT

  public:

    ModelSubTabView(OSItemSelector* itemSelector,
               ModelObjectInspectorView* modelObjectInspectorView,
               QWidget* parent = 0);

    virtual ~ModelSubTabView() {}

    ModelObjectInspectorView* modelObjectInspectorView();

  signals:

    void modelObjectSelected(model::OptionalModelObject & modelObject, bool readOnly);

    void toggleUnitsClicked(bool displayIP);

  protected:

    bool m_isIP;

  protected slots:

    void dropZoneItemClicked(OSItem* item);

    void onItemSelected();

  private:

    ModelObjectInspectorView* m_modelObjectInspectorView;
};


} // openstudio

#endif // OPENSTUDIO_MODELSUBTABVIEW_HPP

