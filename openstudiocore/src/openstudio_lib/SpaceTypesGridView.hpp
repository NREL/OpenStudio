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

#ifndef OPENSTUDIO_SPACETYPESGRIDVIEW_HPP
#define OPENSTUDIO_SPACETYPESGRIDVIEW_HPP

#include "../shared_gui_components/OSGridController.hpp"

#include "OSItem.hpp"

#include "../model/Model.hpp"

#include <QWidget>

class QComboBox;
class QLabel;

namespace openstudio{

  class ModelSubTabView;
  class SpaceTypesGridController;

  class SpaceTypesGridView : public QWidget
  {
    Q_OBJECT

  public:

  SpaceTypesGridView(bool isIP, const model::Model & model, QWidget * parent = nullptr);

    virtual ~SpaceTypesGridView() {}

    std::vector<model::ModelObject> selectedObjects() const;

    void enableFilter();

    void disableFilter();

    QComboBox * m_filters = nullptr;

  private:

    REGISTER_LOGGER("openstudio.SpaceTypesGridView");

    bool m_isIP;

    SpaceTypesGridController * m_gridController = nullptr;

    QLabel * m_filterLabel = nullptr;

  signals:

    void toggleUnitsClicked(bool displayIP);

    void dropZoneItemClicked(OSItem* item);

    void selectionCleared();

  };

  class SpaceTypesGridController : public OSGridController
  {

    Q_OBJECT

  public:

    SpaceTypesGridController(bool isIP,
      const QString & headerText,
      IddObjectType iddObjectType,
      model::Model model,
      std::vector<model::ModelObject> modelObjects);

    virtual ~SpaceTypesGridController() {}

    virtual void refreshModelObjects() override;

    virtual void categorySelected(int index) override;

  protected:

    virtual void setCategoriesAndFields() override;

    virtual void addColumns(const QString &category, std::vector<QString> & fields) override;

    virtual void checkSelectedFields() override;

    virtual QString getColor(const model::ModelObject & modelObject) override;

  private:

    SpaceTypesGridView * spaceTypesGridView(); 

  public slots:

    virtual void onItemDropped(const OSItemId& itemId) override;

    virtual void onComboBoxIndexChanged(int index) override;

    void filterChanged(const QString & text);

  };

} // openstudio

#endif // OPENSTUDIO_SPACETYPESGRIDVIEW_HPP

