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

#ifndef OPENSTUDIO_CONSTRUCTIONWINDOWDATAFILEINSPECTORVIEW_HPP
#define OPENSTUDIO_CONSTRUCTIONWINDOWDATAFILEINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

namespace openstudio {

namespace model {

 class WindowDataFile;

}

class OSLineEdit;

class ConstructionWindowDataFileInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    ConstructionWindowDataFileInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0);

    virtual ~ConstructionWindowDataFileInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:
    void createLayout();

    void attach(openstudio::model::WindowDataFile & windowDataFile);

    void detach();

    void refresh();

    OSLineEdit * m_nameEdit;

    OSLineEdit * m_urlEdit;

    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_CONSTRUCTIONWINDOWDATAFILEINSPECTORVIEW_HPP

