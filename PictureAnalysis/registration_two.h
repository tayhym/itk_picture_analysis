//
//  registration_two.h
//  pictureAnalysis
//
//  Created by Matthew Tay Han Yang on 4/14/15.
//
//

#ifndef __pictureAnalysis__registration_two__
#define __pictureAnalysis__registration_two__

#include <stdio.h>
#include "itkImageRegionIterator.h"

#include "itkDemonsRegistrationFilter.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkWarpImageFilter.h"

using namespace itk;


class ContoursRegistration {
  char *fixed_image;
  char *moving_image;
  
public:
  ContoursRegistration(char *filename_image_one, char *filename_image_two);
  void start_registration();
  
};
#endif /* defined(__pictureAnalysis__registration_two__) */
