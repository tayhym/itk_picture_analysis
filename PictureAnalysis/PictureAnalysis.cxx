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
//  Software Guide : BeginLatex
//
//  The following code is an implementation of a small ITK
//  program. It tests including header files and linking with ITK
//  libraries.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet

// the header files for core itk modules
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include <iostream>

// secondary header files for viewing images, visualization etc
#include "QuickView.h"


// tiertary header files from additional modifications


// additional code for registration filters
/*************** additional code for registration filter and its corollary filters *****************/

#include "itkGradientMagnitudeImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
//  Software Guide : EndCodeSnippet

#include "itkNumericTraits.h"


namespace itk {               // namespace
  template <class TImageType>   // template class
  class CompositeRegistrationFilter:  // class definition
    public ImageToImageFilter<TImageType, TImageType>
  {
  public: // standard variables for object factory
    typedef CompositeRegistrationFilter Self;
    typedef ImageToImageFilter<TImageType,TImageType> Superclass;
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    typedef typename TImageType::PixelType PixelType;
    itkNewMacro( Self )
    itkTypeMacro(CompositeRegistrationFilter, ImageToImageFilter);

    itkGetMacro( Threshold, PixelType);
    itkSetMacro( Threshold, PixelType);
    
  protected:
    typedef ThresholdImageFilter< TImageType > ThresholdType;
    typedef GradientMagnitudeImageFilter< TImageType, TImageType > GradientType;
    typedef RescaleIntensityImageFilter< TImageType, TImageType > RescalerType;
    
    typename GradientType::Pointer m_GradientFilter;
    typename ThresholdType::Pointer m_ThresholdFilter;
    typename RescalerType::Pointer m_RescaleFilter;
    PixelType m_Threshold;
    
    CompositeRegistrationFilter();
    
    void GenerateData();
    
    void PrintSelf( std::ostream& os, Indent indent ) const;
    
  };
}

//
//  composite_registration.cpp
//  pictureAnalysis
//
//  Created by Matthew Tay Han Yang on 4/13/15. from itk software guide.
//
//


namespace itk {
  
  template <class TImageType>
  CompositeRegistrationFilter<TImageType>
  ::CompositeRegistrationFilter()
  {
    m_Threshold = 1;
    m_GradientFilter = GradientType::New();
    m_ThresholdFilter = ThresholdType::New();
    m_ThresholdFilter->SetInput( m_GradientFilter->GetOutput() );
    m_RescaleFilter = RescalerType::New();
    m_RescaleFilter->SetInput( m_ThresholdFilter->GetOutput() );
    m_RescaleFilter->SetOutputMinimum(
                                      NumericTraits<PixelType>::NonpositiveMin());
    m_RescaleFilter->SetOutputMaximum(NumericTraits<PixelType>::max());
  }
  
  template <class TImageType>
  void
  CompositeRegistrationFilter<TImageType>::
  GenerateData()
  {
    m_GradientFilter->SetInput( this->GetInput() );
    m_ThresholdFilter->ThresholdBelow( this->m_Threshold );
    m_RescaleFilter->GraftOutput( this->GetOutput() );
    m_RescaleFilter->Update();
    this->GraftOutput( m_RescaleFilter->GetOutput() );
    std::cout <<  " composite filter ran " << std::endl;
  }
  
  template <class TImageType>
  void
  CompositeRegistrationFilter<TImageType>::
  PrintSelf( std::ostream& os, Indent indent ) const
  {
    Superclass::PrintSelf(os,indent);
    os
    << indent << "Threshold:" << this->m_Threshold
    << std::endl;
  }
  
  
}

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
  
  for (int i = 1; i<4 ; i++ ) {
    char *inputImage = argv[i];
    reader->SetFileName( inputImage );
    reader->Update();
    
    CastFilterType::Pointer castFilter = CastFilterType::New();
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
  
//  Corollary: the testing of the images using bluring would work
//  typedef itk::CompositeRegistrationFilter<FloatImageType> CompositeFilterType;
  
//  CompositeFilterType::Pointer compFilt = CompositeFilterType::New();
  
  //typedef itk::CompositeExampleImageFilter<FloatImageType> FilterType;
  typedef itk::CompositeRegistrationFilter<FloatImageType> FilterType;
  FilterType::Pointer filter = FilterType::New();

  filter->SetInput( image_one );
  
  viewer.AddImage( filter->GetOutput() );
  viewer.Visualize();
  
  std::cout << "done" <<std::endl;

//  QuickView viewer;
//  viewer.AddImage<ImageType>(reader->GetOutput());
//  viewer.AddImage<FloatImageType>(castFilter->GetOutput());
//  viewer.Visualize();
//  

//  
//  
//  std::cout << "Display the image read from local disk " << std::endl;
//  
//  
//  std::cout << "Writing processed images to local disk..." << std::endl;
//  typedef itk::ImageFileWriter< ImageType > WriterType;
//  WriterType::Pointer writer = WriterType::New();
//  writer->SetFileName( "test.tif" );
//  writer->SetInput( reader->GetOutput() );
//  writer->Update();
  
  

  

  return 0;
}

