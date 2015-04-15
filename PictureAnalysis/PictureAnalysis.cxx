/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

// the header files for core itk modules
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include <iostream>

// secondary header files for viewing images, visualization etc
#include "QuickView.h"


// tiertary header files from additional modifications
#include "itkDemonsRegistrationFilter.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkWarpImageFilter.h"

/********************************************************/


#include "itkImageRegistrationMethodv4.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
// Software Guide : EndCodeSnippet


#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkSubtractImageFilter.h"

#include "CompositeFilter/CompositeFilter.cpp"

//#include "registration_one.cpp"
//#include "registration_two.cpp"
#include "registration_mutual_information.cpp"

/********************************************************/
int main(int argc, char ** argv)
{
 
  std::cout << "Starting analysis of image..." << std::endl;
  
  std::cout << "Reading in images from local disk..." << std::endl;
  
  typedef uint32_t PixelType;
  const unsigned int Dimension = 2;
  
  typedef itk::Image<PixelType, Dimension> ImageType;
  typedef itk::ImageFileReader< ImageType > ReaderType;
  
  ReaderType::Pointer reader = ReaderType::New();
  QuickView viewer;

  //  cast the image from unsigned to float
  typedef itk::Image<float,2> FloatImageType;
  typedef itk::CastImageFilter<ImageType, FloatImageType> CastFilterType;
  
  
  // read in images, and convert from unsigned to float
  FloatImageType::Pointer image_one = FloatImageType::New();
  FloatImageType::Pointer image_two = FloatImageType::New();
  FloatImageType::Pointer image_three = FloatImageType::New();
  
  CastFilterType::Pointer castFilter = CastFilterType::New();
  for (int i = 1; i<4 ; i++ ) {
    char *inputImage = argv[i];
    reader->SetFileName( inputImage );
    reader->Update();
    
    
    castFilter->SetInput(reader->GetOutput());
    if (i==1) {
      image_one = castFilter->GetOutput();
      viewer.AddImage<FloatImageType>(image_one);

    } else if (i==2) {
      image_two = castFilter->GetOutput();
      viewer.AddImage<FloatImageType>(image_two);

    } else {
      image_three = castFilter->GetOutput();
      viewer.AddImage<FloatImageType>(image_three);
    }
  }
  
  viewer.Visualize();
  
  // TODO: Registration of the 3 images together - getting the first and third images registered to the second image
  
  
  
//  typedef itk::CompositeRegistrationFilter<FloatImageType> FilterType;
//  FilterType::Pointer filter = FilterType::New();
//
//  filter->SetInput( image_one );
//  
//  viewer.AddImage( filter->GetOutput() );
//  viewer.Visualize();
  
  std::cout << "done" <<std::endl;

  
  
  // registration code
  char *fixed_image = argv[2];
  char *moving_image = argv[1];
  
//  Registration_LinearTranslation *lin_reg = new Registration_LinearTranslation(fixed_image,moving_image);
//  lin_reg->start_registration();
  
//  ContoursRegistration *cont_reg = new ContoursRegistration(fixed_image,moving_image);
//  cont_reg->start_registration();

  QuickView viewer2;

  CastFilterType::Pointer castFilter2 = CastFilterType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
//  reader2->SetFileName( "demons_registration.jpg");
//  castFilter2->SetInput(reader2->GetOutput());
//  FloatImageType::Pointer inputImage = castFilter2->GetOutput();
//  viewer2.AddImage<FloatImageType>(inputImage); // deformed
//
//  
//  
//  reader2->SetFileName( argv[1] );
//  castFilter2->SetInput( reader2->GetOutput() );
//  viewer2.AddImage<FloatImageType>(castFilter2->GetOutput() ); // orig
//
//  reader2->SetFileName( argv[2] );
//  castFilter2->SetInput( reader2->GetOutput() );
//  viewer2.AddImage<FloatImageType>( castFilter2->GetOutput() );  // target
  
  

  MutualInformationRegistration *mi_reg = new MutualInformationRegistration(argv[2], argv[1]);
  typedef itk::Image<unsigned char,2> CharImageType;
  CharImageType::Pointer deformedImage = mi_reg->start_registration();

  reader2->SetFileName( argv[1] );
  castFilter2->SetInput( reader2->GetOutput() );
  viewer2.AddImage<FloatImageType>(castFilter2->GetOutput() ); // orig
  
  viewer2.AddImage<CharImageType>(deformedImage); // deformed

  reader2->SetFileName( argv[2] );
  castFilter2->SetInput( reader2->GetOutput() );
  viewer2.AddImage<FloatImageType>( castFilter2->GetOutput() );  // target


  viewer2.Visualize();

  
  return 0;
}

