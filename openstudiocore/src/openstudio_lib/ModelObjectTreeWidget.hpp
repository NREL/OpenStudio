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

#ifndef OPENSTUDIO_MODELOBJECTTREEWIDGET_HPP
#define OPENSTUDIO_MODELOBJECTTREEWIDGET_HPP

#include "OSItemSelector.hpp"

#include "../model/Model.hpp"

class QTreeWidget;

class QVBoxLayout;

namespace openstudio {

class ModelObjectTreeWidget : public OSItemSelector
{
  Q_OBJECT

  public:

    ModelObjectTreeWidget(const model::Model& model, QWidget * parent = 0);

    virtual ~ModelObjectTreeWidget() {}

    virtual OSItem* selectedItem() const;

    QTreeWidget* treeWidget() const;

    QVBoxLayout* vLayout() const;

    openstudio::model::Model model() const;

  protected:

    virtual void onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle) = 0;

    virtual void onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle) = 0;

    void refresh();

  private slots:

    void objectAdded(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

    void objectRemoved(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

  private:

    QTreeWidget* m_treeWidget;

    QVBoxLayout* m_vLayout;

    openstudio::model::Model m_model;
};

} // openstudio

#endif // OPENSTUDIO_MODELOBJECTTREEWIDGET_HPP

