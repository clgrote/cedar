/*======================================================================================================================

    Copyright 2011, 2012 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        Convolution.cpp

    Maintainer:  Stephan Zibner
    Email:       stephan.zibner@ini.rub.de
    Date:        2011 11 28

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/convolution/Convolution.h"
#include "cedar/auxiliaries/convolution/BorderType.h"
#include "cedar/auxiliaries/convolution/Mode.h"
#include "cedar/auxiliaries/convolution/OpenCV.h"
#include "cedar/auxiliaries/kernel/Kernel.h"
#include "cedar/auxiliaries/math/tools.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::conv::Convolution::Convolution()
:
mCombinedKernel(new cedar::aux::MatData(cv::Mat::zeros(1, 1, CV_32F))),
_mBorderType
(
  new cedar::aux::EnumParameter
  (
    this,
    "borderType",
    cedar::aux::conv::BorderType::typePtr(),
    cedar::aux::conv::BorderType::Zero
  )
),
_mMode
(
  new cedar::aux::EnumParameter
  (
    this,
    "mode",
    cedar::aux::conv::Mode::typePtr(),
    cedar::aux::conv::Mode::Same
  )
),
mKernelList(new cedar::aux::conv::KernelList()),
_mEngine
(
  new cedar::aux::conv::EngineParameter(this, "engine", cedar::aux::conv::EnginePtr(new cedar::aux::conv::OpenCV()))
)
{
  this->selectedEngineChanged();

  QObject::connect(this->_mEngine.get(), SIGNAL(valueChanged()), this, SLOT(selectedEngineChanged()));
  QObject::connect(this->_mEngine.get(), SIGNAL(valueChanged()), this, SIGNAL(configurationChanged()));
  QObject::connect(this->_mMode.get(), SIGNAL(valueChanged()), this, SIGNAL(configurationChanged()));
  QObject::connect(this->_mBorderType.get(), SIGNAL(valueChanged()), this, SIGNAL(configurationChanged()));
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::aux::conv::Convolution::setAllowedModes(const std::set<cedar::aux::conv::Mode::Id>& modes)
{
  this->mAllowedModes = modes;

  this->updateEngineCapabilities();
}

void cedar::aux::conv::Convolution::slotKernelAdded(size_t index)
{
  cedar::aux::kernel::ConstKernelPtr kernel = this->getKernelList()->getKernel(index);
  QObject::connect(kernel.get(), SIGNAL(kernelUpdated()), this, SLOT(updateCombinedKernel()));
  this->updateCombinedKernel();
}

void cedar::aux::conv::Convolution::slotKernelChanged(size_t)
{
  this->updateCombinedKernel();
}

void cedar::aux::conv::Convolution::slotKernelRemoved(size_t)
{
  this->updateCombinedKernel();
  //!@todo disconnect kernelUpdated slot!
}

void cedar::aux::conv::Convolution::updateCombinedKernel()
{
  cv::Mat new_combined_kernel = this->getKernelList()->getCombinedKernel();
  this->mCombinedKernel->lockForWrite();
  this->mCombinedKernel->setData(new_combined_kernel);
  this->mCombinedKernel->unlock();
}

void cedar::aux::conv::Convolution::selectedEngineChanged()
{
  this->getEngine()->setKernelList(this->getKernelList());
  mKernelAddedConnection.disconnect();
  mKernelChangedConnection.disconnect();
  mKernelRemovedConnection.disconnect();

  // connect to the new kernel list.
  mKernelAddedConnection = this->getKernelList()->connectToKernelAddedSignal
                           (
                             boost::bind(&cedar::aux::conv::Convolution::slotKernelAdded, this, _1)
                           );

  mKernelChangedConnection = this->getKernelList()->connectToKernelChangedSignal
                             (
                               boost::bind(&cedar::aux::conv::Convolution::slotKernelChanged, this, _1)
                             );

  mKernelRemovedConnection = this->getKernelList()->connectToKernelRemovedSignal
                             (
                               boost::bind(&cedar::aux::conv::Convolution::slotKernelRemoved, this, _1)
                             );

  this->updateEngineCapabilities();

  this->updateCombinedKernel();
}

void cedar::aux::conv::Convolution::updateEngineCapabilities()
{
  if (this->mAllowedModes.empty())
  {
    this->_mMode->enableAll();
  }
  else
  {
    this->_mMode->disableAll();
  }

  const std::vector<cedar::aux::Enum>& modes = cedar::aux::conv::Mode::type().list();
  for (size_t i = 0; i < modes.size(); ++i)
  {
    const cedar::aux::Enum& enum_value = modes.at(i);

    if (this->mAllowedModes.empty() || this->mAllowedModes.find(enum_value) != this->mAllowedModes.end())
    {
      this->_mMode->setEnabled(enum_value, this->getEngine()->checkModeCapability(enum_value));
    }
  }

  this->_mBorderType->enableAll();
  const std::vector<cedar::aux::Enum>& border_types = cedar::aux::conv::BorderType::type().list();
  for (size_t i = 0; i < border_types.size(); ++i)
  {
    const cedar::aux::Enum& enum_value = border_types.at(i);
    this->_mBorderType->setEnabled(enum_value, this->getEngine()->checkBorderTypeCapability(enum_value));
  }
}
