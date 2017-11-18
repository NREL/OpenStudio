/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_PHOTOVOLTAICINSPECTORVIEW_HPP
#define OPENSTUDIO_PHOTOVOLTAICINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "../model/Photovoltaic.hpp"

class QComboBox;

namespace openstudio {

//namespace model {

class Photovoltaic;

//}

class OSComboBox2;

class OSLineEdit2;

class OSQuantityEdit2;

class OSDropZone;

class PhotovoltaicInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    PhotovoltaicInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0 );

    virtual ~PhotovoltaicInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void attach(openstudio::model::Photovoltaic & Photovoltaic);

    void detach();

    void refresh();

    OSLineEdit2 * m_nameEdit = nullptr;

	  OSQuantityEdit2 * m_surfaceAreaEdit = nullptr;

    OSQuantityEdit2 * m_factionActiveEdit = nullptr;

	  OSQuantityEdit2 * m_inverterEfficiencyEdit = nullptr;
  
    OSQuantityEdit2 * m_azimuthAngleEdit = nullptr;

    OSQuantityEdit2 * m_inclinationAngleEdit = nullptr;

    OSQuantityEdit2 * m_cellEfficiencyEdit = nullptr;

    OSQuantityEdit2 * m_gtEfficiencyEdit = nullptr;

    OSQuantityEdit2 * m_systemEfficiencyEdit = nullptr;

	  OSComboBox2 * m_PVTypeComboBox = nullptr;

    boost::optional<model::Photovoltaic> m_Photovoltaic;

	  bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_PHOTOVOLTAICINSPECTORVIEW_HPP

