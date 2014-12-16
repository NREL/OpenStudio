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

#ifndef OPENSTUDIO_CONSTRUCTIONBASEINSPECTORVIEW_HPP
#define OPENSTUDIO_CONSTRUCTIONBASEINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

#include "../model/StandardsInformationConstruction.hpp"

class QComboBox;

class QGridLayout;

class QLabel;

namespace openstudio {

namespace model {

 class Construction;

}

//class ConstructionObjectVectorController;

//class OSDropZone;

class OSLineEdit;

class OSComboBox2;

class ConstructionBaseInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    ConstructionBaseInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0);

    virtual ~ConstructionBaseInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

    void createLayout();

    void attach(openstudio::model::Construction & construction);

    void detach();

    void refresh();

    void showFenestration();

    void hideFenestration();

    void enableFenestration();

    void disableFenestration();

    QGridLayout * m_mainGridLayout = nullptr;

    OSLineEdit * m_nameEdit;

    OSComboBox2 * m_intendedSurfaceType;

    QComboBox * m_standard = nullptr;

    QComboBox * m_standardSource = nullptr;

    QComboBox * m_fenestration = nullptr;

    QComboBox * m_standardsConstructionType = nullptr;

    QLabel * m_fenestrationLabel = nullptr;

    boost::optional<openstudio::model::StandardsInformationConstruction> m_standardsInformation;

    bool m_isIP;

  protected slots:

    void standardsConstructionTypeChanged(const QString& text);

    void editStandardsConstructionType(const QString& text);

    void populateStandardsConstructionType();

  public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_CONSTRUCTIONBASEINSPECTORVIEW_HPP

