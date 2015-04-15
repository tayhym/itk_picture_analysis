//
//  registration_mutual_information.h
//  pictureAnalysis
//
//  Created by Matthew Tay Han Yang on 4/15/15.
//
//


#ifndef __pictureAnalysis__registration_mutual_information__
#define __pictureAnalysis__registration_mutual_information__

#include <stdio.h>
#include "itkImageRegistrationMethodv4.h"
#include "itkTranslationTransform.h"
#include "itkMattesMutualInformationImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
  // Software Guide : EndCodeSnippet
  
  
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
  
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkCheckerBoardImageFilter.h"

#include "itkCommand.h"
  
class CommandIterationUpdate : public itk::Command
  {
  public:
    typedef  CommandIterationUpdate   Self;
    typedef  itk::Command             Superclass;
    typedef itk::SmartPointer<Self>   Pointer;
    itkNewMacro( Self );
    
  protected:
    CommandIterationUpdate() {};
    
  public:
    typedef itk::RegularStepGradientDescentOptimizerv4<double> OptimizerType;
    typedef   const OptimizerType *                            OptimizerPointer;
    
    void Execute(itk::Object *caller, const itk::EventObject & event)
    {
      Execute( (const itk::Object *)caller, event);
    }
    
    void Execute(const itk::Object * object, const itk::EventObject & event)
    {
      OptimizerPointer optimizer = static_cast< OptimizerPointer >( object );
      if( ! itk::IterationEvent().CheckEvent( &event ) )
      {
        return;
      }
      std::cout << optimizer->GetCurrentIteration() << "   ";
      std::cout << optimizer->GetValue() << "   ";
      std::cout << optimizer->GetCurrentPosition() << std::endl;
    }
  };


#endif /* defined(__pictureAnalysis__registration_mutual_information__) */

class MutualInformationRegistration {
  char *fixed_image;
  char *moving_image;
  typedef itk::Image<unsigned char,2> ImageType;
public:
  MutualInformationRegistration(char *image_name_one, char *image_name_two);
  ImageType::Pointer start_registration();
  
};