//
//  CompositeFilter.cpp
//  pictureAnalysis
//
//  Created by Matthew Tay Han Yang on 4/14/15. from itk software guide.
//
//


#include "CompositeFilter.h"

// code for composite filter - definitions
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
