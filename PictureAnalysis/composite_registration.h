//
//  composite_registration.h
//  pictureAnalysis
//
//  Created by Matthew Tay Han Yang on 4/13/15. from itk software guide.
//
//

#ifndef __pictureAnalysis__composite_registration__
#define __pictureAnalysis__composite_registration__

#include <stdio.h>
#include "itkGradientMagnitudeImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

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
#endif /* defined(__pictureAnalysis__composite_registration__) */
