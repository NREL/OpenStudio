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

#ifndef OPENSTUDIO_CONSTRUCTIONINTERNALSOURCEINSPECTORVIEW_HPP
#define OPENSTUDIO_CONSTRUCTIONINTERNALSOURCEINSPECTORVIEW_HPP

#include "ConstructionBaseInspectorView.hpp"

#include "../model/StandardsInformationConstruction.hpp"

class QComboBox;

namespace openstudio {

namespace model {

  class ConstructionWithInternalSource;

}

class ConstructionObjectVectorController;

class OSDropZone;

class OSIntegerEdit;

class OSLineEdit;

class OSQuantityEdit;

class OSComboBox2;

class ConstructionInternalSourceInspectorView : public ConstructionBaseInspectorView
{
  Q_OBJECT

  public:

    ConstructionInternalSourceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0);

    virtual ~ConstructionInternalSourceInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

  protected slots:

    void populateStandardsConstructionType();

  private:

    void createLayout();

    void attach(openstudio::model::ConstructionWithInternalSource & constructionWithInternalSource);

    void detach();

    OSDropZone * m_constructionDZ;

    ConstructionObjectVectorController * m_constructionVC;

    OSIntegerEdit * m_sourcePresentAfterLayerNumberEdit;

    OSIntegerEdit * m_temperatureCalculationRequestedAfterLayerNumberEdit;

    OSIntegerEdit * m_dimensionsForTheCTFCalculationEdit;

    OSQuantityEdit * m_tubeSpacingEdit;
};

} // openstudio

#endif // OPENSTUDIO_CONSTRUCTIONINTERNALSOURCEINSPECTORVIEW_H

