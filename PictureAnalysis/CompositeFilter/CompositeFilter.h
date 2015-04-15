//
//  CompositeFilter.h
//  pictureAnalysis
//
//  Created by Matthew Tay Han Yang on 4/14/15.
//
//

#ifndef pictureAnalysis_CompositeRegistration_h
#define pictureAnalysis_CompositeRegistration_h

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include <iostream>


#include "itkGradientMagnitudeImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkNumericTraits.h"

#include <stdio.h>

#endif
// code for composite filter - function and method prototypes
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
