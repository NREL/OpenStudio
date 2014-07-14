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

#ifndef OPENSTUDIO_FACILITYTREEWIDGET_HPP
#define OPENSTUDIO_FACILITYTREEWIDGET_HPP

#include "ModelObjectTreeWidget.hpp"

class QString;
class QTreeWidgetItem;
class QPaintEvent;

namespace openstudio {

class FacilityTreeWidget : public ModelObjectTreeWidget
{
  Q_OBJECT

  public:

    FacilityTreeWidget(const model::Model& model, QWidget * parent = 0);
    virtual ~FacilityTreeWidget() {}

    IddObjectType currentIddObjectType() const;

    boost::optional<openstudio::model::ModelObject> selectedModelObject() const;

  protected:

    virtual void onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

    virtual void onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

    void paintEvent(QPaintEvent* event);

  private slots:
  
    void onSortByChanged(const QString& text);

    void currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

    void initialize();

  private:

    IddObjectType m_sortByType;

    IddObjectType m_iddObjectType;

    boost::optional<openstudio::model::ModelObject> m_selectedObject;

    boost::optional<openstudio::Handle> m_selectedObjectHandle;
};  

} // openstudio

#endif // OPENSTUDIO_FACILITYTREEWIDGET_HPP

