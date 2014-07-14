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

#ifndef OPENSTUDIO_INTERIORPARTITIONSURFACEINSPECTORVIEW_HPP
#define OPENSTUDIO_INTERIORPARTITIONSURFACEINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

namespace openstudio {

class OSDropZone;
class OSLineEdit;
class OSSwitch;
class OSQuantityEdit;

class InteriorPartitionSurfaceConstructionVectorController;
class PlanarSurfaceWidget;

namespace model {
  class InteriorPartitionSurface;
}

class InteriorPartitionSurfaceInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    InteriorPartitionSurfaceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0 );

    virtual ~InteriorPartitionSurfaceInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void attach(openstudio::model::InteriorPartitionSurface& interiorPartitionSurface);

    void detach();

    void refresh();

    OSLineEdit* m_nameEdit;
    InteriorPartitionSurfaceConstructionVectorController* m_constructionVectorController;
    OSDropZone* m_constructionDropZone;
    OSQuantityEdit* m_surfaceAreaEdit;
    OSSwitch* m_convertToInternalMassSwitch;
    PlanarSurfaceWidget* m_planarSurfaceWidget;
    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_INTERIORPARTITIONSURFACEINSPECTORVIEW_HPP

