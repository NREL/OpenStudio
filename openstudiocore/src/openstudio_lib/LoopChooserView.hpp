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

#ifndef OPENSTUDIO_LOOPCHOOSERVIEW_HPP
#define OPENSTUDIO_LOOPCHOOSERVIEW_HPP

#include <QWidget>
#include "../model/ModelObject.hpp"
#include "../model/WaterToAirComponent.hpp"
#include "../model/HVACComponent.hpp"
#include "../model/Loop.hpp"
#include "../utilities/core/String.hpp"

class QCheckBox;

class QVBoxLayout;

namespace openstudio {

class LoopChooserItem;

class LoopChooserView : public QWidget
{
  Q_OBJECT;

  public:

  LoopChooserView(QWidget* parent = nullptr);

  virtual ~LoopChooserView() {}

  LoopChooserItem * loopChooserItemForLoop(std::string loopName);

  void layoutView();

  public slots:

  void layoutModelObject(boost::optional<model::ModelObject> & modelObject);

  void onAddToLoopClicked(model::Loop &);

  void onRemoveFromLoopClicked(model::Loop &);

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  protected:

  void paintEvent ( QPaintEvent * event ) override;

 private:

  boost::optional<model::HVACComponent> m_component;

  QVBoxLayout * m_vLayout;

  std::vector<LoopChooserItem *> m_loopChooserItems;
};

class LoopChooserItem : public QWidget
{
  Q_OBJECT;

  public:

  LoopChooserItem(model::Loop &, LoopChooserView * parent = nullptr);

  virtual ~LoopChooserItem() {}

  std::string loopName();

  signals:

  void addToLoopClicked(model::Loop &);

  void removeFromLoopClicked(model::Loop &);

  public slots:

  void setChecked( bool checked );

  private slots:

  void sendClickedSignal( bool checked );

  private:

  QCheckBox * m_checkBox;

  LoopChooserView * m_loopChooserView;

  model::Loop m_loop;
};

} // openstudio

#endif // OPENSTUDIO_LOOPCHOOSERVIEW_HPP

