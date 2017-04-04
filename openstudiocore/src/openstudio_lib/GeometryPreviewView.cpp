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

#include "GeometryPreviewView.hpp"

#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QCamera>
#include <Qt3DCore/QCameraLens>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QFrameGraph>
#include <Qt3DRender/QForwardRenderer>
#include <Qt3DRender/QPhongMaterial>
#include <Qt3DRender/QCylinderMesh>
#include <Qt3DRender/QSphereMesh>
#include <Qt3DRender/QTorusMesh>
#include <Qt3DRender/QWindow>

#include <QPropertyAnimation>

namespace openstudio {

GeometryPreviewView::GeometryPreviewView(bool isIP,
                                         const openstudio::model::Model& model,
                                         QWidget * parent)
: QWidget(parent)
{
  // TODO: DLM impliment units switching
  //connect(this, &GeometryPreviewView::toggleUnitsClicked, modelObjectInspectorView(), &ModelObjectInspectorView::toggleUnitsClicked);

  QVBoxLayout *layout = new QVBoxLayout;

  PreviewView* view = new PreviewView(this);
  layout->addWidget(view);

  setLayout(layout);
}

PreviewView::PreviewView(QWidget *t_parent)
  : QWidget(t_parent),
    m_isIP(true)
{
  auto mainLayout = new QVBoxLayout;
  setLayout(mainLayout);

  m_view = new Qt3DRender::QWindow();
  QWidget *container = QWidget::createWindowContainer(m_view);
  mainLayout->addWidget(container);

  Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
  m_view->registerAspect(input);

  // Root entity
  Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

  // Camera
  Qt3DCore::QCamera *cameraEntity = m_view->defaultCamera();

  cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
  cameraEntity->setPosition(QVector3D(0, 0, -40.0f));
  cameraEntity->setUpVector(QVector3D(0, 1, 0));
  cameraEntity->setViewCenter(QVector3D(0, 0, 0));
  input->setCamera(cameraEntity);

  // Material
  Qt3DRender::QMaterial *material = new Qt3DRender::QPhongMaterial(rootEntity);

  // Torus
  Qt3DCore::QEntity *torusEntity = new Qt3DCore::QEntity(rootEntity);
  Qt3DRender::QTorusMesh *torusMesh = new Qt3DRender::QTorusMesh;
  torusMesh->setRadius(5);
  torusMesh->setMinorRadius(1);
  torusMesh->setRings(100);
  torusMesh->setSlices(20);

  Qt3DCore::QTransform *torusTransform = new Qt3DCore::QTransform;
  torusTransform->setScale3D(QVector3D(1.5, 1, 0.5));
  torusTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 45.0f));

  torusEntity->addComponent(torusMesh);
  torusEntity->addComponent(torusTransform);
  torusEntity->addComponent(material);

  // Sphere
  Qt3DCore::QEntity *sphereEntity = new Qt3DCore::QEntity(rootEntity);
  Qt3DRender::QSphereMesh *sphereMesh = new Qt3DRender::QSphereMesh;
  sphereMesh->setRadius(3);

  Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform;
  //OrbitTransformController *controller = new OrbitTransformController(sphereTransform);
  //controller->setTarget(sphereTransform);
  //controller->setRadius(20.0f);

  QPropertyAnimation *sphereRotateTransformAnimation = new QPropertyAnimation(sphereTransform);
  //sphereRotateTransformAnimation->setTargetObject(controller);
  sphereRotateTransformAnimation->setTargetObject(sphereEntity);
  sphereRotateTransformAnimation->setPropertyName("angle");
  sphereRotateTransformAnimation->setStartValue(QVariant::fromValue(0));
  sphereRotateTransformAnimation->setEndValue(QVariant::fromValue(360));
  sphereRotateTransformAnimation->setDuration(10000);
  sphereRotateTransformAnimation->setLoopCount(-1);
  sphereRotateTransformAnimation->start();

  sphereEntity->addComponent(sphereMesh);
  sphereEntity->addComponent(sphereTransform);
  sphereEntity->addComponent(material);

  m_view->setRootEntity(rootEntity);
  m_view->show();
}

PreviewView::~PreviewView()
{
}

void PreviewView::onUnitSystemChange(bool t_isIP) 
{
  LOG(Debug, "onUnitSystemChange " << t_isIP << " reloading results");
  m_isIP = t_isIP;
}

} // openstudio
