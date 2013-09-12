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

#ifndef OPENSTUDIO_MODELOBJECTINSPECTORVIEW_H
#define OPENSTUDIO_MODELOBJECTINSPECTORVIEW_H

#include <model/ComponentData.hpp>

#include <model/Model.hpp>
#include <model/ModelObject.hpp>

#include <boost/smart_ptr.hpp>
#include <boost/optional.hpp>
#include <QWidget>
#include "OSInspectorView.hpp"

class QLabel;
class QStackedWidget;

namespace openstudio {

class OSItem;

class ModelObjectInspectorView : public OSInspectorView
{
  Q_OBJECT

  public:

    ModelObjectInspectorView(const openstudio::model::Model& model,
                             bool addScrollArea,
                             QWidget * parent = 0);

    virtual ~ModelObjectInspectorView() {}

  signals:

    void toggleUnitsClicked(bool displayIP);

  public slots:

    void selectModelObject(const openstudio::model::ModelObject& modelObject);

  protected:
    virtual void onSelectItem(OSItem *item);
    virtual void onClearSelection();
    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) = 0;

    boost::optional<openstudio::model::ModelObject> modelObject() const;

    model::Model m_model;

  protected slots:

    virtual void toggleUnits(bool displayIP);

  private:

    boost::optional<openstudio::model::ModelObject> m_modelObject;
};

class DefaultInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    DefaultInspectorView(const model::Model& model,
                         QWidget * parent = 0);

    virtual ~DefaultInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:
    
    QLabel * m_mainLabel;
};

} // openstudio

#endif // OPENSTUDIO_MODELOBJECTINSPECTORVIEW_H

