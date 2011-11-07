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

    File:        NeuralField.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 04

    Description:

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "cedar/dynamics/fields/NeuralField.h"
#include "cedar/dynamics/SpaceCode.h"
#include "cedar/auxiliaries/NumericParameter.h"
#include "cedar/auxiliaries/NumericVectorParameter.h"
#include "cedar/auxiliaries/DataTemplate.h"
#include "cedar/auxiliaries/math/Sigmoid.h"
#include "cedar/auxiliaries/math/AbsSigmoid.h"
#include "cedar/auxiliaries/kernel/Gauss.h"
#include "cedar/auxiliaries/assert.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::dyn::NeuralField::NeuralField()
:
mActivation(new cedar::dyn::SpaceCode(cv::Mat::zeros(10,10,CV_32F))),
mSigmoidalActivation(new cedar::dyn::SpaceCode(cv::Mat::zeros(10,10,CV_32F))),
mLateralInteraction(new cedar::dyn::SpaceCode(cv::Mat::zeros(10,10,CV_32F))),
mRestingLevel(new cedar::aux::DoubleParameter(this, "restingLevel", -5.0, -100, 0)),
mTau(new cedar::aux::DoubleParameter(this, "tau", 100.0, 1.0, 10000.0)),
mGlobalInhibition(new cedar::aux::DoubleParameter(this, "globalInhibition", -0.01, -100.0, 100.0)),
mSigmoid(new cedar::aux::math::AbsSigmoid(0.0, 10.0)),
_mDimensionality(new cedar::aux::UIntParameter(this, "dimensionality", 1, 1000)),
_mSizes(new cedar::aux::UIntVectorParameter(this, "sizes", 2, 10, 1, 1000))
{
  _mDimensionality->setValue(2);
  _mSizes->makeDefault();
  this->declareBuffer("activation");
  this->setBuffer("activation", mActivation);
  this->declareBuffer("lateralInteraction");
  this->setBuffer("lateralInteraction", mLateralInteraction);

  this->declareOutput("sigmoid(activation)");
  this->setOutput("sigmoid(activation)", mSigmoidalActivation);

  this->declareInput("input", false);

  this->addConfigurableChild("sigmoid", this->mSigmoid);

  std::vector<double> sigmas;
  std::vector<double> shifts;
  sigmas.push_back(3.0);
  shifts.push_back(0.0);
  sigmas.push_back(3.0);
  shifts.push_back(0.0);
  mKernel = cedar::aux::kernel::GaussPtr(new cedar::aux::kernel::Gauss(1.0, sigmas, shifts, 5.0, 2));
  this->declareBuffer("kernel");
  this->setBuffer("kernel", mKernel->getKernelRaw());
  this->mKernel->hideDimensionality(true);
  this->addConfigurableChild("lateral kernel", this->mKernel);

  QObject::connect(_mSizes.get(), SIGNAL(valueChanged()), this, SLOT(dimensionSizeChanged()));
  QObject::connect(_mDimensionality.get(), SIGNAL(valueChanged()), this, SLOT(dimensionalityChanged()));

  // now check the dimensionality and sizes of all matrices
  this->updateMatrices();
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------


cedar::proc::DataSlot::VALIDITY cedar::dyn::NeuralField::determineInputValidity
                                                         (
                                                           cedar::proc::ConstDataSlotPtr slot,
                                                           cedar::aux::DataPtr data
                                                         ) const
{
  if (slot->getRole() == cedar::proc::DataRole::INPUT && slot->getName() == "input")
  {
    if (cedar::dyn::SpaceCodePtr input = boost::shared_dynamic_cast<cedar::dyn::SpaceCode>(data))
    {
      if (!this->isMatrixCompatibleInput(input->getData()))
      {
        return cedar::proc::DataSlot::VALIDITY_ERROR;
      }
      else
      {
        return cedar::proc::DataSlot::VALIDITY_VALID;
      }
    }
    else if (cedar::aux::MatDataPtr input = boost::shared_dynamic_cast<cedar::aux::MatData>(data))
    {
      if (!this->isMatrixCompatibleInput(input->getData()))
      {
        return cedar::proc::DataSlot::VALIDITY_ERROR;
      }
      else
      {
        return cedar::proc::DataSlot::VALIDITY_WARNING;
      }
    }
    return cedar::proc::DataSlot::VALIDITY_ERROR;
  }
  return this->cedar::proc::Step::determineInputValidity(slot, data);
}

void cedar::dyn::NeuralField::eulerStep(const cedar::unit::Time& time)
{
  cv::Mat& u = this->mActivation->getData();
  cv::Mat& sigmoid_u = this->mSigmoidalActivation->getData();
  cv::Mat& lateral_interaction = this->mLateralInteraction->getData();
  const double& h = mRestingLevel->getValue();
  const double& tau = mTau->getValue();
  const double& global_inhibition = mGlobalInhibition->getValue();

  sigmoid_u = mSigmoid->compute<float>(u);
  //!@todo Should this not use the data->lock*
  mKernel->getReadWriteLock()->lockForRead();
  lateral_interaction = mKernel->convolveWith(sigmoid_u);
  mKernel->getReadWriteLock()->unlock();

  CEDAR_ASSERT(u.size == sigmoid_u.size);
  CEDAR_ASSERT(u.size == lateral_interaction.size);

  cv::Mat d_u = -u + h + sigmoid_u + lateral_interaction + global_inhibition * cv::sum(sigmoid_u)[0];;
  /*!@todo the following is probably slow -- it'd be better to store a pointer in the neural field class and update
   *       it when the connections change.
   */
  if (cedar::aux::DataPtr input = this->getInput("input"))
  {
    cv::Mat& input_mat = input->getData<cv::Mat>();
    CEDAR_ASSERT(input_mat.size == d_u.size);
    d_u += input_mat;
  }

  //!\todo deal with units, now: milliseconds
  u += cedar::unit::Milliseconds(time) / cedar::unit::Milliseconds(tau) * d_u;
  //std::cout << "field: " << u.at<float>(0,0) << std::endl;

}

bool cedar::dyn::NeuralField::isMatrixCompatibleInput(const cv::Mat& matrix) const
{
  // special case due to opencv's strange handling of 1d-matrices
  if(matrix.dims == 2 && (matrix.rows == 1 || matrix.cols == 1))
  {
    // if this field is set to more dimensions than the input (in this case 1), they are not compatible
    if (this->_mDimensionality->getValue() != 1)
      return false;

    CEDAR_DEBUG_ASSERT(this->_mSizes->getValue().size() == 1);

    // if the dimensions are both 1, rows or cols must be the same as the field size
    if (static_cast<int>(this->_mSizes->at(0)) != matrix.rows
        && static_cast<int>(this->_mSizes->at(0)) != matrix.cols)
      return false;
  }
  else
  {
    if (static_cast<int>(this->_mDimensionality->getValue()) != matrix.dims)
      return false;
    for (unsigned int dim = 0; dim < this->_mSizes->getValue().size(); ++dim)
    {
      if (matrix.size[static_cast<int>(dim)] != static_cast<int>(this->_mSizes->at(dim)))
        return false;
    }
  }
  return true;
}

void cedar::dyn::NeuralField::dimensionalityChanged()
{
  this->_mSizes->resize(_mDimensionality->getValue(), _mSizes->getDefaultValue());
  this->updateMatrices();
}

void cedar::dyn::NeuralField::dimensionSizeChanged()
{
  this->updateMatrices();
}

void cedar::dyn::NeuralField::updateMatrices()
{
  int dimensionality = static_cast<int>(_mDimensionality->getValue());

  std::vector<int> sizes(dimensionality);
  for (int dim = 0; dim < dimensionality; ++dim)
  {
    sizes[dim] = _mSizes->at(dim);
  }
  this->lockAll();
  if (dimensionality == 1)
  {
    this->mActivation->getData() = cv::Mat(sizes[0], 1, CV_32F, cv::Scalar(mRestingLevel->getValue()));
    this->mSigmoidalActivation->getData() = cv::Mat(sizes[0], 1, CV_32F, cv::Scalar(0));
    this->mLateralInteraction->getData() = cv::Mat(sizes[0], 1, CV_32F, cv::Scalar(0));
  }
  else
  {
    this->mActivation->getData() = cv::Mat(dimensionality,&sizes.at(0), CV_32F, cv::Scalar(mRestingLevel->getValue()));
    this->mSigmoidalActivation->getData() = cv::Mat(dimensionality, &sizes.at(0), CV_32F, cv::Scalar(0));
    this->mLateralInteraction->getData() = cv::Mat(dimensionality, &sizes.at(0), CV_32F, cv::Scalar(0));
  }
  this->unlockAll();

  this->mKernel->setDimensionality(dimensionality);
}
