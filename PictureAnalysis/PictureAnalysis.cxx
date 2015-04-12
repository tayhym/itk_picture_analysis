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


#include <stdio.h>
#include <unistd.h>
#define GetCurrentDir getcwd
int main(int argc, char ** argv)
{

//  char cCurrentPath[FILENAME_MAX];
//  
//  if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
//  {
//    return errno;
//  }
//  
//  cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
//  
//  printf ("The current working directory is %s", cCurrentPath);
  
  
  const char * inputImageOne = argv[1];
  const char * inputImageTwo = argv[2];
  const char * inputImageThree = argv[3];
  
  std::cout << "Starting analysis of image..." << std::endl;
  
  std::cout << "Reading in images from local disk..." << std::endl;
  
  typedef uint32_t PixelType;
  const unsigned int Dimension = 2;
  
  typedef itk::Image<PixelType, Dimension> ImageType;
  typedef itk::ImageFileReader< ImageType > ReaderType;
  
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( inputImageOne );
  reader->Update();
  
  QuickView viewer;
  viewer.AddImage<ImageType>(reader->GetOutput());
  
//  cast the image from unsigned to float
  typedef itk::Image<float,2> FloatImageType;
  typedef itk::CastImageFilter<ImageType, FloatImageType> CastFilterType;
  CastFilterType::Pointer castFilter = CastFilterType::New();
  castFilter->SetInput(reader->GetOutput());
  viewer.AddImage<FloatImageType>(castFilter->GetOutput());
  viewer.Visualize();
  
  std::cout << "done" <<std::endl;

  
//  QuickView viewer;
//  viewer.AddImage<ImageType>(reader->GetOutput());
//  viewer.AddImage<FloatImageType>(castFilter->GetOutput());
//  viewer.Visualize();
//  
//  ImageType::Pointer image_one = ImageType::New();
////  ImageType::Pointer image_two = ImageType::New();
////  ImageType::Pointer image_three = ImageType::New();
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
  
  
  
//  try
//  {
//    writer->Update();
//  }
//  catch( itk::ExceptionObject & err )
//  {
//    std::cerr << "ExceptionObject caught !" << std::endl;
//    std::cerr << err << std::endl;
//    return EXIT_FAILURE;
//  }
  
//  typedef itk::Image<float,2> FloatImage2DType;
//  itk::ImageFileWriter<FloatImage2DType>::Pointer writer = itk::ImageFileWriter::New();
//  writer->SetInput (last_filter->GetOutput());
//  writer->SetFileName( "test.raw" );
//  writer->Update();
  
  

  return 0;
}
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  This code instantiates a $3D$ image\footnote{Also known as a
//  \emph{volume}.} whose pixels are represented with type \code{unsigned
//  short}. The image is then constructed and assigned to a
//  \doxygen{SmartPointer}. Although later in the text we will discuss
//  \code{SmartPointer}s in detail, for now think of it as a handle on an
//  instance of an object (see section \ref{sec:SmartPointers} for more
//  information). The \doxygen{Image} class will be described in
//  Section~\ref{sec:ImageSection}.
//
//  Software Guide : EndLatex
