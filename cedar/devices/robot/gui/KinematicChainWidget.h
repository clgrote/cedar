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

    File:        KinematicChainWidget.h

    Maintainer:  Bjoern Weghenkel
    Email:       bjoern.weghenkel@ini.rub.de
    Date:        2011 01 06

    Description: Header for the @em cedar::dev::robot::KinematicChainWidget class.

    Credits:

======================================================================================================================*/


#ifndef KINEMATICCHAINWIDGET_H_
#define KINEMATICCHAINWIDGET_H_

// LOCAL INCLUDES

// PROJECT INCLUDES

#include "devices/robot/KinematicChain.h"

// SYSTEM INCLUDES

#include <QtCore/QTimer>
#include <QtGui/QGridLayout>
#include <QtGui/QWidget>

class KinematicChainWidget : public QWidget
{
  //----------------------------------------------------------------------------
  // macros
  //----------------------------------------------------------------------------

  Q_OBJECT;

  //----------------------------------------------------------------------------
  // constructors and destructor
  //----------------------------------------------------------------------------

public:

  KinematicChainWidget(const cedar::dev::robot::KinematicChainPtr &kinematicChain, QWidget *parent = 0, Qt::WindowFlags f = 0);
  ~KinematicChainWidget();

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  void simulateVelocity(bool);
  void simulateAcceleration(bool);

  //----------------------------------------------------------------------------
  // protected methods
  //----------------------------------------------------------------------------

protected:

  // none yet

  //----------------------------------------------------------------------------
  // private methods
  //----------------------------------------------------------------------------

private slots:

  void radioButtonAngleClicked();
  void radioButtonVelocityClicked();
  void radioButtonAccelerationClicked();
  void updateJointValue(double value);
  void updateSpinBoxes();

private:

  void setActiveColumn(unsigned int c);

  //----------------------------------------------------------------------------
  // members
  //----------------------------------------------------------------------------

public:

  // none yet (hopefully never!)

protected:

  // none yet

private:

  static const int mUpdateInterval = 100;

  cedar::dev::robot::KinematicChainPtr mpKinematicChain;
  QGridLayout *mpGridLayout;
  QTimer *mpTimer;
  bool mSimulateVelocity;
  bool mSimulateAcceleration;

  //----------------------------------------------------------------------------
  // parameters
  //----------------------------------------------------------------------------

public:

  // none yet

protected:

  // none yet

private:

  // none yet

};

#endif /* KINEMATICCHAINWIDGET_H_ */
