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

#ifndef OPENSTUDIO_DAYLIGHTINGCONTROLINSPECTORVIEW_HPP
#define OPENSTUDIO_DAYLIGHTINGCONTROLINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

namespace openstudio {

class OSLineEdit;
class OSComboBox;
class OSQuantityEdit;

namespace model {
  class DaylightingControl;
}

class DaylightingControlInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    DaylightingControlInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0 );

    virtual ~DaylightingControlInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void attach(openstudio::model::DaylightingControl& daylightingControl);

    void detach();

    void refresh();

    OSLineEdit* m_nameEdit;
    OSQuantityEdit* m_xCoordinateEdit;
    OSQuantityEdit* m_yCoordinateEdit;
    OSQuantityEdit* m_zCoordinateEdit;
    OSQuantityEdit* m_psiRotationEdit;
    OSQuantityEdit* m_thetaRotationEdit;
    OSQuantityEdit* m_phiRotationEdit;
    OSQuantityEdit* m_illuminanceSetpointEdit;
    OSComboBox* m_controlTypeComboBox;
    OSQuantityEdit* m_numberOfStepsEdit;
    OSQuantityEdit* m_probabilityResetEdit;
    OSQuantityEdit* m_numberOfViewsEdit;
    OSQuantityEdit* m_maxGlareEdit;
    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};


} // openstudio

#endif // OPENSTUDIO_DAYLIGHTINGCONTROLINSPECTORVIEW_HPP

