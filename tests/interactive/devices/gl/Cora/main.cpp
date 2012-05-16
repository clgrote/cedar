/*======================================================================================================================

    Copyright 2011 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany

    This file is part of cedar.

    cedar is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    cedar is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with cedar. If not, see <http://www.gnu.org/licenses/>.

========================================================================================================================

 ----- Institute:   Ruhr-Universitaet Bochum
                    Institut fuer Neuroinformatik

 ----- File:        main.cpp

 ----- Maintainer:  Hendrik Reimann
 ----- Email:       hendrik.reimann@ini.rub.de
 ----- Date:        2010 11 17

 ----- Description: interactive test for the kinematic chain simulation

 ----- Credits:
 -----------------------------------------------------------------------------*/

// CEDAR INCLUDES
#include "cedar/devices/robot/SimulatedKinematicChain.h"
#include "cedar/devices/robot/gl/CoraArm.h"
#include "cedar/devices/robot/gl/CoraHead.h"
#include "cedar/devices/robot/gui/KinematicChainWidget.h"
#include "cedar/auxiliaries/System.h"
#include "cedar/auxiliaries/gl/Scene.h"
#include "cedar/auxiliaries/gui/Viewer.h"
#include "cedar/auxiliaries/gui/SceneWidget.h"
#include "cedar/auxiliaries/sleepFunctions.h"

// SYSTEM INCLUDES
#include <QApplication>


int main(int argc, char **argv)
{
  std::string arm_configuration_file_old = cedar::aux::System::locateResource("configs/cora_arm.conf");
  std::string head_configuration_file_old = cedar::aux::System::locateResource("configs/cora_head.conf");
  std::string arm_configuration_file = cedar::aux::System::locateResource("configs/cora_arm.json");
  std::string head_configuration_file = cedar::aux::System::locateResource("configs/cora_head.json");

  QApplication a(argc, argv);

  // create simulated kinematic chains
  cedar::dev::robot::KinematicChainPtr p_cora_arm
  (
    new cedar::dev::robot::SimulatedKinematicChain(arm_configuration_file_old)
  );
  p_cora_arm->readJson(arm_configuration_file);
  cedar::dev::robot::KinematicChainPtr p_cora_head
  (
    new cedar::dev::robot::SimulatedKinematicChain(head_configuration_file_old)
  );
  p_cora_head->readJson(head_configuration_file);

  // create gl visualization objects
  cedar::dev::robot::gl::KinematicChainPtr p_cora_arm_visualization(new cedar::dev::robot::gl::CoraArm(p_cora_arm));
  cedar::dev::robot::gl::KinematicChainPtr p_cora_head_visualization(new cedar::dev::robot::gl::CoraHead(p_cora_head));

  // create scene and viewer to display the arm
  cedar::aux::gl::ScenePtr p_scene(new cedar::aux::gl::Scene());
  p_scene->setSceneLimit(2);
  p_scene->drawFloor(true);
  cedar::aux::gui::Viewer viewer(p_scene);
  viewer.show();
  viewer.setSceneRadius(p_scene->getSceneLimit());

  // create visualization objects for the head and arm
  cedar::aux::gl::ObjectVisualizationPtr p_object;
  p_object = p_cora_arm_visualization;
  p_scene->addObjectVisualization(p_object);
  p_object = p_cora_head_visualization;
  p_scene->addObjectVisualization(p_object);

  // create control widgets for the scene and the arm
  cedar::aux::gui::SceneWidgetPtr p_scene_widget(new cedar::aux::gui::SceneWidget(p_scene));
  cedar::dev::robot::gui::KinematicChainWidget widget_arm(p_cora_arm);
  cedar::dev::robot::gui::KinematicChainWidget widget_head(p_cora_head);

  p_scene_widget->show();
  widget_arm.show();
  widget_head.show();

  p_cora_arm->startTimer(50.0);
  p_cora_head->startTimer(50.0);
  viewer.startTimer(50);
  a.exec();

  p_cora_arm->stop();
  p_cora_head->stop();
  cedar::aux::sleep(cedar::unit::Seconds(1));

  return 0;
}
