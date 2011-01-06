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

    Institute:   Ruhr-Universitaet Bochum
                 Institut fuer Neuroinformatik

    File:        KinematicChainWidget.cpp

    Maintainer:  Bjoern Weghenkel
    Email:       bjoern.weghenkel@ini.rub.de
    Date:        2011 01 06

    Description: Implementation of the @em cedar::dev::robot::KinematicChainWidget class.

    Credits:

======================================================================================================================*/


// LOCAL INCLUDES

#include "KinematicChainWidget.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES

#include "stdio.h"
#include <iostream>

#include <QtGui/QApplication>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>

using namespace std;
using namespace cedar::dev::robot;


//----------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------

KinematicChainWidget::KinematicChainWidget(const KinematicChainPtr &kinematicChain, QWidget * parent, Qt::WindowFlags f) : QWidget(parent, f)
{
  // store a smart pointer to KinematicChain
  mpKinematicChain = kinematicChain;

  // Set up the model and configure the view...
  setWindowTitle(QApplication::translate("KinematicChainWindow", "KinematicChain"));

  mpGridLayout = new QGridLayout();
  QRadioButton *radioButtonAngle = new QRadioButton(QApplication::translate("KinematicChainWindow", "Angle [rad]"));
  QRadioButton *radioButtonVelocity = new QRadioButton(QApplication::translate("KinematicChainWindow", "Velocity [rad/s]"));
  QRadioButton *radioButtonAcceleration = new QRadioButton(QApplication::translate("KinematicChainWindow", "Acceleration [rad/s]"));
  mpGridLayout->addWidget(radioButtonAngle, 0, 1);
  mpGridLayout->addWidget(radioButtonVelocity, 0, 2);
  mpGridLayout->addWidget(radioButtonAcceleration, 0, 3);
  connect(radioButtonAngle, SIGNAL(clicked()), this, SLOT(radioButtonAngleClicked()));
  connect(radioButtonVelocity, SIGNAL(clicked()), this, SLOT(radioButtonVelocityClicked()));
  connect(radioButtonAcceleration, SIGNAL(clicked()), this, SLOT(radioButtonAccelerationClicked()));

  mpGridLayout->setColumnStretch(0,1);
  mpGridLayout->setColumnStretch(1,2);
  mpGridLayout->setColumnStretch(2,2);
  mpGridLayout->setColumnStretch(3,2);

  for(unsigned int i = 0; i < mpKinematicChain->getNumberOfJoints(); ++i)
  {
    // add label
    char labelText[10];
    sprintf(labelText, "Joint %d", i);
    QLabel *label = new QLabel(QApplication::translate("KinematicChainWindow", labelText));
    mpGridLayout->addWidget(label, i+1, 0);

    // add spinboxes
    for(unsigned int j = 0; j < 3; ++j)
    {
      QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox();
      mpGridLayout->addWidget(doubleSpinBox, i+1, j+1);
      connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateJointValue(double)));
    }
  }

  radioButtonAngle->click();
  setLayout(mpGridLayout);
  setMaximumHeight(0);

  // start a timer to update the interface
  mpTimer = new QTimer();
  connect(mpTimer, SIGNAL(timeout()), this, SLOT(updateSpinBoxes()));
  mpTimer->start(mUpdateInterval);

  return;
}


KinematicChainWidget::~KinematicChainWidget()
{
  mpKinematicChain->stop();
  mpTimer->stop();
  delete mpTimer;
}


//------------------------------------------------------------------------------
// methods
//------------------------------------------------------------------------------

void KinematicChainWidget::setActiveColumn(unsigned int c)
{
  for(unsigned int i = 0; i < 3; ++i)
  {
    bool enabled = (i == c) ? true : false;
    for(unsigned int j = 0; j < mpKinematicChain->getNumberOfJoints(); ++j)
    {
      mpGridLayout->itemAtPosition(j+1, i+1)->widget()->setEnabled(enabled);
    }
  }
}


void KinematicChainWidget::radioButtonAngleClicked()
{
  setActiveColumn(0);
  mpKinematicChain->stop();
  mpKinematicChain->setWorkingMode(KinematicChain::ANGLE);
  mpKinematicChain->start();
  updateSpinBoxes();
}


void KinematicChainWidget::radioButtonVelocityClicked()
{
  setActiveColumn(1);
  mpKinematicChain->stop();
  mpKinematicChain->setWorkingMode(KinematicChain::VELOCITY);
  mpKinematicChain->start();
  updateSpinBoxes();
}


void KinematicChainWidget::radioButtonAccelerationClicked()
{
  setActiveColumn(2);
  mpKinematicChain->stop();
  mpKinematicChain->setWorkingMode(KinematicChain::ACCELERATION);
  mpKinematicChain->start();
  updateSpinBoxes();
}


void KinematicChainWidget::updateSpinBoxes()
{
  for(unsigned i = 0; i < mpKinematicChain->getNumberOfJoints(); ++i)
  {
    QDoubleSpinBox *p_spin_box_angle = static_cast<QDoubleSpinBox*>(mpGridLayout->itemAtPosition(i+1, 1)->widget());
    QDoubleSpinBox *p_spin_box_velocity = static_cast<QDoubleSpinBox*>(mpGridLayout->itemAtPosition(i+1, 2)->widget());
    QDoubleSpinBox *p_spin_box_acceleration = static_cast<QDoubleSpinBox*>(mpGridLayout->itemAtPosition(i+1, 3)->widget());

    p_spin_box_angle->setValue(mpKinematicChain->getJointAngle(i));
    p_spin_box_velocity->setValue(mpKinematicChain->getJointVelocity(i));
    p_spin_box_acceleration->setValue(mpKinematicChain->getJointAcceleration(i));
  }
}


void KinematicChainWidget::updateJointValue(double value)
{
  QWidget *sender = static_cast<QWidget*>(this->sender());
  int index_sender = mpGridLayout->indexOf(sender);

  int row, column;
  int joint, mode;
  int dummy1, dummy2;

  mpGridLayout->getItemPosition(index_sender, &row, &column, &dummy1, &dummy2);
  joint = row - 1;
  mode = column - 1;

  switch(mode)
  {
  case 0:
    mpKinematicChain->setJointAngle(joint, value);
    break;

  case 1:
    mpKinematicChain->setJointVelocity(joint, value);
    break;

  case 2:
    mpKinematicChain->setJointAcceleration(joint, value);
    break;

  default:
    cout << "Error: I was not able to determine the corresponding working mode for this signal. This should never happen!" << endl;
  }

  return;
}
