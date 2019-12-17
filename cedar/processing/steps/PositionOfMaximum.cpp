/*======================================================================================================================

    Copyright 2011, 2012, 2013, 2014, 2015, 2016, 2017 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        PositionOfMaximum.cpp

    Maintainer:  Jean-Stephane Jokeit
    Email:       
    Date:        2017 05 14

    Description: Source file for the class cedar::proc::steps::PositionOfMaximum.

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CLASS HEADER
#include "cedar/processing/steps/PositionOfMaximum.h"

// CEDAR INCLUDES
#include "cedar/processing/typecheck/IsMatrix.h"
#include "cedar/auxiliaries/BoolParameter.h"

// SYSTEM INCLUDES
#include "cedar/processing/ElementDeclaration.h"
#include <limits> 

//----------------------------------------------------------------------------------------------------------------------
// register the class
//----------------------------------------------------------------------------------------------------------------------
namespace
  {
  bool declare()
  {
    using cedar::proc::ElementDeclarationPtr;
    using cedar::proc::ElementDeclarationTemplate;

    ElementDeclarationPtr declaration
    (
      new ElementDeclarationTemplate<cedar::proc::steps::PositionOfMaximum>
      (
        "Programming",
        "cedar.processing.steps.PositionOfMaximum"
      )
    );

    declaration->setIconPath(":/steps/positionofmaximum.svg");
    declaration->setDescription
    (
      "Returns the indices (positions in the array) "
      "of the MAXIMUM (or other, see below) value of the input tensor.\n"
      "Note: The position of the maximum is always the "
      "first (i.e. leftmost, upmost) pixel!\n"
      "You can optionally choose to find the position of the "
      "CENTROID (center of mass). The Centroid method implemented "
      "assumes you have an unimodal distribution and that the values of "
      "the input are in an appropriate range, i.e. most of the pixels are of value zero. Note: If many pixels are not exactly zero, this will affect the result significantly."
    );

    declaration->declare();

    return true;
  }

  bool declared = declare();
}

cedar::aux::EnumType<cedar::proc::steps::PositionOfMaximum::UnitType>
cedar::proc::steps::PositionOfMaximum::UnitType::mType("PositionOfMaximum::");

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::steps::PositionOfMaximum::PositionOfMaximum()
:
// outputs
mOutput(new cedar::aux::MatData(cv::Mat())),
mPositionType
(
  new cedar::aux::EnumParameter
  (
    this,
    "position type",
    cedar::proc::steps::PositionOfMaximum::UnitType::typePtr(),
    cedar::proc::steps::PositionOfMaximum::UnitType::Maximum
  )
),
mNaNIfNoPeak(new cedar::aux::BoolParameter(this, "NaN if no peak", false)),
mPeakThreshold(new cedar::aux::DoubleParameter(this, "Peak detection threshold", 0.1))
{
  // declare all data
  cedar::proc::DataSlotPtr input = this->declareInput("input");
  this->declareOutput("output", mOutput);

  input->setCheck(cedar::proc::typecheck::IsMatrix());

  mPeakThreshold->setConstant(true); 
 
  QObject::connect(this->mPositionType.get(), SIGNAL(valueChanged()), this, SLOT(outputTypeChanged()));
  QObject::connect(this->mNaNIfNoPeak.get(), SIGNAL(valueChanged()), this, SLOT(outputTypeChanged()));
  QObject::connect(this->mPeakThreshold.get(), SIGNAL(valueChanged()), this, SLOT(outputTypeChanged()));
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::steps::PositionOfMaximum::outputTypeChanged()
{
  if (mNaNIfNoPeak->getValue())
  {
    mPeakThreshold->setConstant(false);
  }
  else
  {
    mPeakThreshold->setConstant(true);
  }

  recompute();
}

void cedar::proc::steps::PositionOfMaximum::inputConnectionChanged(const std::string& inputName)
{
  // TODO: you may want to replace this code by using a cedar::proc::InputSlotHelper

  // Again, let's first make sure that this is really the input in case anyone ever changes our interface.
  CEDAR_DEBUG_ASSERT(inputName == "input");

  // Assign the input to the member. This saves us from casting in every computation step.
  this->mInput = boost::dynamic_pointer_cast<const cedar::aux::MatData>(this->getInput(inputName));

  bool output_changed = false;
  if (!this->mInput)
  {
    // no input -> no output
    this->mOutput->setData(cv::Mat());
    output_changed = true;
  }
  else
  {
    // Let's get a reference to the input matrix.
    const cv::Mat& input = this->mInput->getData();

    // check if the input is different from the output
    if (input.type() != this->mOutput->getData().type() || input.size != this->mOutput->getData().size)
    {
      output_changed = true;
    }

    // Make a copy to create a matrix of the same type, dimensions, ...
    this->mOutput->setData( cv::Mat::zeros(2, 1, CV_32F) );

    this->mOutput->copyAnnotationsFrom(this->mInput);
  }

  if (output_changed)
  {
    recompute();
    this->emitOutputPropertiesChangedSignal("output");
  }
}

void cedar::proc::steps::PositionOfMaximum::compute(const cedar::proc::Arguments&)
{
  recompute();
}

void cedar::proc::steps::PositionOfMaximum::recompute()
{
  bool isNaN;

  if (!mInput
     || mInput->getData().empty())
  {
    mOutput->setData( cv::Mat::zeros(2,2,CV_32F) );
    return;
  }

  switch (this->mPositionType->getValue())
  {
    case cedar::proc::steps::PositionOfMaximum::UnitType::Maximum:
      // fall-through
    default:
    {
      double minimum, maximum;
      int minLoc[2]= {-1, -1};
      int maxLoc[2]= {-1, -1};

      cv::minMaxIdx(mInput->getData(), &minimum, &maximum, minLoc, maxLoc);

      mOutput->getData().at<float>(0,0) = maxLoc[0];
      mOutput->getData().at<float>(1,0) = maxLoc[1];
      break;
    }

    case cedar::proc::steps::PositionOfMaximum::UnitType::Centroid:
    {
      // moments:
      // this assumes the input is already went through a sigmoid
      cv::Moments m= cv::moments( mInput->getData(), true);
      cv::Point p( m.m10/m.m00, m.m01/m.m00);
      
      float p1= p.x;
      float p2= p.y;

      if ( cedar::aux::math::isZero( p1 ) 
           || p1 < 0.0 )
      {
        p1= 0.0;
        isNaN= true;
      }
      if ( cedar::aux::math::isZero( p2 ) 
           || p2 < 0.0 )
      {
        p2= 0.0;
        isNaN= true;
      }

      mOutput->getData().at<float>(0,0) = p1;
      mOutput->getData().at<float>(1,0) = p2;
      break;
    }
  }

  if (mNaNIfNoPeak->getValue())
  {
    float val= mInput->getData().at<float>(
                 mOutput->getData().at<float>(0,0),
                 mOutput->getData().at<float>(1,0) );

    if ( isNaN
         || cedar::aux::math::isZero( val ) 
         || val < mPeakThreshold->getValue() ) 
    {
      mOutput->getData().at<float>(0,0) = std::numeric_limits<float>::quiet_NaN();
      mOutput->getData().at<float>(1,0) = std::numeric_limits<float>::quiet_NaN();
    }
    else
    {
    }
  }
}
