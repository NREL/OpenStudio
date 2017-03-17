/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef OPENSTUDIO_MODELOBJECTINSPECTORVIEW_HPP
#define OPENSTUDIO_MODELOBJECTINSPECTORVIEW_HPP

#include "../model/ComponentData.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

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
                             QWidget * parent = nullptr);

    virtual ~ModelObjectInspectorView() {}

    // override if your implementation supports multiple object selection
    // (eg, via GridView)
    virtual bool supportsMultipleObjectSelection() const { return false; }
    virtual std::vector<model::ModelObject> selectedObjects() const;
    void update();

  signals:

    void toggleUnitsClicked(bool displayIP);

    void itemSelected(OSItem * item);

    void selectionCleared();

    void itemsRequested();

    void dropZoneItemClicked(OSItem* item);

    void onChange();

  public slots:

    void selectModelObject(const openstudio::model::ModelObject& modelObject);

  protected:

    virtual void onSelectItem(OSItem *item) override;

    virtual void onClearSelection() override;

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
                         QWidget * parent = nullptr);

    virtual ~DefaultInspectorView() {}

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

  private:
    
    QLabel * m_mainLabel;
};

} // openstudio

#endif // OPENSTUDIO_MODELOBJECTINSPECTORVIEW_HPP

