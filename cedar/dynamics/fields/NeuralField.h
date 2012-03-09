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

    File:        NeuralField.h

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

#ifndef CEDAR_DYN_NEURAL_FIELD_H
#define CEDAR_DYN_NEURAL_FIELD_H

// CEDAR INCLUDES
#include "cedar/dynamics/namespace.h"
#include "cedar/dynamics/Dynamics.h"
#include "cedar/auxiliaries/DoubleParameter.h"
#include "cedar/auxiliaries/UIntParameter.h"
#include "cedar/auxiliaries/DoubleVectorParameter.h"
#include "cedar/auxiliaries/math/namespace.h"
#include "cedar/auxiliaries/kernel/namespace.h"
#include "cedar/auxiliaries/ObjectListParameterTemplate.h"
#include "cedar/auxiliaries/namespace.h"

// SYSTEM INCLUDES


/*!@brief An implementation of Neural Fields for the processing framework.
 *
 * @todo Put a detailed description here, including the equation of the field dynamics and references to the relevant paper(s)
 */
class cedar::dyn::NeuralField : public cedar::dyn::Dynamics
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------
  Q_OBJECT

  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------
  typedef cedar::aux::ObjectListParameterTemplate<cedar::aux::kernel::Kernel> KernelListParameter;
  CEDAR_GENERATE_POINTER_TYPES_INTRUSIVE(KernelListParameter);

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  NeuralField();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief determine if a given Data is a valid input to the field
  cedar::proc::DataSlot::VALIDITY determineInputValidity(cedar::proc::ConstDataSlotPtr, cedar::aux::DataPtr) const;
  void onStart();
  void onStop();

public slots:
  //!@brief handle a change in dimensionality, which leads to creating new matrices
  void dimensionalityChanged();
  //!@brief handle a change in size along dimensions, which leads to creating new matrices
  void dimensionSizeChanged();
  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  /*!@brief compute the euler step of the field equation
   *
   * This is the field equation:
   * \f[
   * \dot{u}(\boldmath{x},t) = \frac{\delta t}{\tau} (-u + h + \int\dots\int w(\boldmath{x}' - \boldmath{x}) \sigma(u\boldmath{x}) d\boldmath{x} + s(\boldmath{x}))
   * \f]
   */
  void eulerStep(const cedar::unit::Time& time);

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  //!@brief update the size and dimensionality of internal matrices
  void updateMatrices();

  //!@brief check if input fits to field in dimension and size
  bool isMatrixCompatibleInput(const cv::Mat& matrix) const;

  //!@brief Resets the field.
  void reset();

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //!@brief this SpaceCode matrix contains the current field activity of the NeuralField
  cedar::dyn::SpaceCodePtr mActivation;
  //!@brief this SpaceCode matrix contains the current field activity, sent through the sigmoid function
  cedar::dyn::SpaceCodePtr mSigmoidalActivation;
  //!@brief this SpaceCode matrix contains the current lateral interactions of the NeuralField, i.e. convolution result
  cedar::dyn::SpaceCodePtr mLateralInteraction;
  //!@brief this MatData contains the input noise
  cedar::aux::MatDataPtr mInputNoise;
  //!@brief this MatData contains the neural noise
  cedar::aux::MatDataPtr mNeuralNoise;
  //!@brief the resting level of a field
  cedar::aux::DoubleParameterPtr mRestingLevel;
  //!@brief the relaxation rate of the field
  cedar::aux::DoubleParameterPtr mTau; //!@todo deal with units, now: milliseconds
  //!@brief the global inhibition of the field, which is not contained in the kernel
  cedar::aux::DoubleParameterPtr mGlobalInhibition;
  //!@brief any sigmoid function
  cedar::aux::math::SigmoidPtr mSigmoid;
  //!@brief the noise correlation kernel
  cedar::aux::kernel::GaussPtr mNoiseCorrelationKernel;
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //!@brief the field dimensionality - may range from 1 to 16 in principle, but more like 6 or 7 in reality
  cedar::aux::UIntParameterPtr _mDimensionality; //!@todo not the only class needing this - think about parent class

  //!@brief the field sizes in each dimension
  cedar::aux::UIntVectorParameterPtr _mSizes;

  //!@brief input noise gain
  cedar::aux::DoubleParameterPtr _mInputNoiseGain;

  //!@brief The list of kernels for this field.
  KernelListParameterPtr _mKernels;

private:
  // none yet

}; // class cedar::dyn::NeuralField

#endif // CEDAR_DYN_NEURAL_FIELD_H
