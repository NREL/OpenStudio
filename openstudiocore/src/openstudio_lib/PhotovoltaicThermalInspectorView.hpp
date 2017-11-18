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

#ifndef OPENSTUDIO_PHOTOVOLTAICTHERMALINSPECTORVIEW_HPP
#define OPENSTUDIO_PHOTOVOLTAICTHERMALINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "../model/PhotovoltaicThermal.hpp"
class QComboBox;

namespace openstudio {

//namespace model {

class PhotovoltaicThermal;

//}

class OSComboBox2;

class OSLineEdit2;

class OSQuantityEdit2;

class OSDropZone;

class PhotovoltaicThermalInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    PhotovoltaicThermalInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0 );

    virtual ~PhotovoltaicThermalInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void attach(openstudio::model::PhotovoltaicThermal & PhotovoltaicThermal);

    void detach();

    void refresh();

    OSLineEdit2* m_nameEdit = nullptr;

    OSQuantityEdit2 * m_surfaceAreaEdit = nullptr;

    OSQuantityEdit2 * m_factionActiveEdit = nullptr;

    OSQuantityEdit2 * m_azimuthAngleEdit = nullptr;

    OSQuantityEdit2 * m_inclinationAngleEdit = nullptr;

    OSQuantityEdit2 * m_cellEfficiencyEdit = nullptr;

    OSQuantityEdit2 * m_gtEfficiencyEdit = nullptr;

    OSQuantityEdit2 * m_systemEfficiencyEdit = nullptr;

    OSQuantityEdit2 * m_collectorEfficiencyEdit = nullptr;
	
    OSQuantityEdit2 * m_boilerEfficiencyEdit = nullptr;

    boost::optional<model::PhotovoltaicThermal> m_photovoltaicThermal;

	  bool m_isIP;

    public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_PHOTOVOLTAICTHERMALINSPECTORVIEW_HPP

