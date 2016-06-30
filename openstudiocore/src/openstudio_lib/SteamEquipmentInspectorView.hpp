/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_STEAMEQUIPMENTINSPECTORVIEW_HPP
#define OPENSTUDIO_STEAMEQUIPMENTINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

namespace openstudio {

namespace model {

class SteamEquipmentDefinition;

}

class OSLineEdit2;

class OSQuantityEdit2;

class OSDropZone;

class SteamEquipmentDefinitionInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    SteamEquipmentDefinitionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = nullptr );

    virtual ~SteamEquipmentDefinitionInspectorView() {}

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

  private:

    void attach(openstudio::model::SteamEquipmentDefinition & steamEquipmentDefinition);

    void detach();

    void refresh();

    OSLineEdit2 * m_nameEdit;

    OSQuantityEdit2 * m_designLevelEdit;

    OSQuantityEdit2 * m_wattsPerSpaceFloorAreaEdit;

    OSQuantityEdit2 * m_wattsPerPersonEdit;

    OSQuantityEdit2 * m_fractionLatentEdit;

    OSQuantityEdit2 * m_fractionRadiantEdit;

    OSQuantityEdit2 * m_fractionLostEdit;

    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP) override;
};

} // openstudio

#endif // OPENSTUDIO_STEAMEQUIPMENTINSPECTORVIEW_HPP

