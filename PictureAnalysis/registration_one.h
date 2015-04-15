//
//  registration_one.h
//  pictureAnalysis
//
//  Created by Matthew Tay Han Yang on 4/14/15. from itk software guide.
//
//

#ifndef __pictureAnalysis__registration_one__
#define __pictureAnalysis__registration_one__

#include <stdio.h>
// Software Guide : BeginCodeSnippet
#include "itkImageRegistrationMethodv4.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
// Software Guide : EndCodeSnippet


#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkSubtractImageFilter.h"

class Registration_LinearTranslation {
  char *fixed_image;
  char *moving_image;
  
public:
  // constructor
  Registration_LinearTranslation(char * file_name_one, char * file_name_two);
  void start_registration();
  
};

#endif /* defined(__pictureAnalysis__registration_one__) */
