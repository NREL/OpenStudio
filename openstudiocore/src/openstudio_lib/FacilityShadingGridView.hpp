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

#ifndef OPENSTUDIO_FACILITYSHADINGGRIDVIEW_HPP
#define OPENSTUDIO_FACILITYSHADINGGRIDVIEW_HPP

#include "../shared_gui_components/OSGridController.hpp"

#include "GridViewSubTab.hpp"
#include "OSItem.hpp"

#include "../model/Model.hpp"

class QComboBox;
class QLineEdit;

namespace openstudio{

  class OSQuantityEdit2;

  class FacilityShadingGridController;

  class FacilityShadingGridView : public GridViewSubTab
  {
    Q_OBJECT

  public:

    FacilityShadingGridView(bool isIP, const model::Model & model, QWidget * parent = 0);

    virtual ~FacilityShadingGridView() {}

    QLineEdit *  m_nameFilter = nullptr;

    QLineEdit *  m_tiltGreaterThanFilter = nullptr;

    QLineEdit *  m_tiltLessThanFilter = nullptr;

    QComboBox *  m_typeFilter = nullptr;

    QLineEdit *  m_orientationGreaterThanFilter = nullptr;

    QLineEdit *  m_orientationLessThanFilter = nullptr;

  private:

    REGISTER_LOGGER("openstudio.FacilityShadingGridView");

    virtual void addObject(const openstudio::IddObjectType& iddObjectType) override;

    virtual void purgeObjects(const openstudio::IddObjectType& iddObjectType) override;

    void filterChanged();

    std::set<openstudio::model::ModelObject> m_objectsFilteredByName;

    std::set<openstudio::model::ModelObject> m_objectsFilteredByTilt;

    std::set<openstudio::model::ModelObject> m_objectsFilterdByType;

    std::set<openstudio::model::ModelObject> m_objectsFilteredByOrientation;

  protected slots :

    virtual void onSelectItem() override;

    virtual void onClearSelection() override;
    
  private slots :

    void tiltFilterChanged();

    void orientationFilterChanged();

    void nameFilterChanged();

    void typeFilterChanged(const QString & text);

  };

  class FacilityShadingGridController : public OSGridController
  {

    Q_OBJECT

  public:

    FacilityShadingGridController(bool isIP,
      const QString & headerText,
      IddObjectType iddObjectType,
      model::Model model,
      std::vector<model::ModelObject> modelObjects);

    virtual ~FacilityShadingGridController() {}

    virtual void refreshModelObjects();

    virtual void categorySelected(int index);

  protected:

    virtual void setCategoriesAndFields();

    virtual void addColumns(const QString &category, std::vector<QString> & fields);

    virtual void checkSelectedFields();

    virtual QString getColor(const model::ModelObject & modelObject);

  public slots:

    virtual void onItemDropped(const OSItemId& itemId);

  };

} // openstudio

#endif // OPENSTUDIO_FACILITYSHADINGGRIDVIEW_HPP

