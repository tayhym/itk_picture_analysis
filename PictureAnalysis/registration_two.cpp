//
//  registration_two.cpp
//  pictureAnalysis
//
//  Created by Matthew Tay Han Yang on 4/14/15. from itk software guide.
//
//

#include "registration_two.h"

class CommandIterationUpdate : public itk::Command
{
public:
  typedef  CommandIterationUpdate                     Self;
  typedef  itk::Command                               Superclass;
  typedef  itk::SmartPointer<CommandIterationUpdate>  Pointer;
  itkNewMacro( CommandIterationUpdate );
protected:
  CommandIterationUpdate() {};
  
  typedef itk::Image< float, 2 >            InternalImageType;
  typedef itk::Vector< float, 2 >           VectorPixelType;
  typedef itk::Image<  VectorPixelType, 2 > DisplacementFieldType;
  
  typedef itk::DemonsRegistrationFilter<
  InternalImageType,
  InternalImageType,
  DisplacementFieldType>   RegistrationFilterType;
  
public:
  
  void Execute(itk::Object *caller, const itk::EventObject & event)
  {
    Execute( (const itk::Object *)caller, event);
  }
  
  void Execute(const itk::Object * object, const itk::EventObject & event)
  {
    const RegistrationFilterType * filter = static_cast< const RegistrationFilterType * >( object );
    if( !(itk::IterationEvent().CheckEvent( &event )) )
    {
      return;
    }
    std::cout << filter->GetMetric() << std::endl;
  }
};


//class CommandIterationUpdate : public itk::Command
//{
//public:
//  typedef CommandIterationUpdate   Self;
//  typedef itk::Command             Superclass;
//  typedef itk::SmartPointer<Self>  Pointer;
//  itkNewMacro( Self );
//  
//protected:
//  CommandIterationUpdate() {};
//  
//public:
//  
//  typedef itk::RegularStepGradientDescentOptimizerv4<double> OptimizerType;
//  typedef const OptimizerType*                               OptimizerPointer;
//  
//  void Execute(itk::Object *caller, const itk::EventObject & event)
//  {
//    Execute( (const itk::Object *)caller, event);
//  }
//  
//  void Execute(const itk::Object * object, const itk::EventObject & event)
//  {
//    OptimizerPointer optimizer = static_cast< OptimizerPointer >( object );
//    
//    if( ! itk::IterationEvent().CheckEvent( &event ) )
//    {
//      return;
//    }
//    
//    std::cout << optimizer->GetCurrentIteration() << " = ";
//    std::cout << optimizer->GetValue() << " : ";
//    std::cout << optimizer->GetCurrentPosition() << std::endl;
//  }
//};

using namespace itk;
ContoursRegistration::ContoursRegistration(char *filename_image_one, char *filename_image_two) {
  fixed_image = filename_image_one;
  moving_image = filename_image_two;
}

void ContoursRegistration::start_registration() {
  std::cout << "starting demons registration" << std::endl;
  const unsigned int Dimension = 2;
  typedef unsigned short PixelType;
  
  typedef itk::Image< PixelType, Dimension >  FixedImageType;
  typedef itk::Image< PixelType, Dimension >  MovingImageType;
  
  // Set up the file readers
  typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
  typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;
  
  FixedImageReaderType::Pointer fixedImageReader   = FixedImageReaderType::New();
  MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();
  
  fixedImageReader->SetFileName( fixed_image );
  movingImageReader->SetFileName( moving_image );

  
  // Software Guide : BeginCodeSnippet
  typedef float                                      InternalPixelType;
  typedef itk::Image< InternalPixelType, Dimension > InternalImageType;
  typedef itk::CastImageFilter< FixedImageType,
  InternalImageType >  FixedImageCasterType;
  typedef itk::CastImageFilter< MovingImageType,
  InternalImageType >  MovingImageCasterType;
  
  FixedImageCasterType::Pointer fixedImageCaster = FixedImageCasterType::New();
  MovingImageCasterType::Pointer movingImageCaster
  = MovingImageCasterType::New();
  
  fixedImageCaster->SetInput( fixedImageReader->GetOutput() );
  movingImageCaster->SetInput( movingImageReader->GetOutput() );
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  //
  // The demons algorithm relies on the assumption that pixels representing the
  // same homologous point on an object have the same intensity on both the
  // fixed and moving images to be registered. In this example, we will
  // preprocess the moving image to match the intensity between the images
  // using the \doxygen{HistogramMatchingImageFilter}.
  //
  // \index{itk::HistogramMatchingImageFilter}
  //
  // The basic idea is to match the histograms of the two images at a
  // user-specified number of quantile values. For robustness, the histograms
  // are matched so that the background pixels are excluded from both
  // histograms.  For MR images, a simple procedure is to exclude all gray
  // values that are smaller than the mean gray value of the image.   ///THIS ASSUMPTION NOT TRUE FOR SAR IMAGES?
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef itk::HistogramMatchingImageFilter<
  InternalImageType,
  InternalImageType >   MatchingFilterType;
  MatchingFilterType::Pointer matcher = MatchingFilterType::New();
  // Software Guide : EndCodeSnippet
  
  
  // Software Guide : BeginLatex
  //
  // For this example, we set the moving image as the source or input image and
  // the fixed image as the reference image.
  //
  // \index{itk::HistogramMatchingImageFilter!SetInput()}
  // \index{itk::HistogramMatchingImageFilter!SetSourceImage()}
  // \index{itk::HistogramMatchingImageFilter!SetReferenceImage()}
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  matcher->SetInput( movingImageCaster->GetOutput() );
  matcher->SetReferenceImage( fixedImageCaster->GetOutput() );
  // Software Guide : EndCodeSnippet
  
  
  // Software Guide : BeginLatex
  //
  // We then select the number of bins to represent the histograms and the
  // number of points or quantile values where the histogram is to be
  // matched.
  //
  // \index{itk::HistogramMatchingImageFilter!SetNumberOfHistogramLevels()}
  // \index{itk::HistogramMatchingImageFilter!SetNumberOfMatchPoints()}
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  matcher->SetNumberOfHistogramLevels( 1024 );
  matcher->SetNumberOfMatchPoints( 7 );
  // Software Guide : EndCodeSnippet
  
  
  // Software Guide : BeginLatex
  //
  // Simple background extraction is done by thresholding at the mean
  // intensity.
  //
  // \index{itk::HistogramMatchingImageFilter!ThresholdAtMeanIntensityOn()}
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  matcher->ThresholdAtMeanIntensityOn();
  // Software Guide : EndCodeSnippet
  
  
  // Software Guide : BeginLatex
  //
  // In the \doxygen{DemonsRegistrationFilter}, the deformation field is
  // represented as an image whose pixels are floating point vectors.
  //
  // \index{itk::DemonsRegistrationFilter}
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef itk::Vector< float, Dimension >           VectorPixelType;
  typedef itk::Image<  VectorPixelType, Dimension > DisplacementFieldType;
  typedef itk::DemonsRegistrationFilter<
  InternalImageType,
  InternalImageType,
  DisplacementFieldType> RegistrationFilterType;
  RegistrationFilterType::Pointer filter = RegistrationFilterType::New();
  // Software Guide : EndCodeSnippet
  
  
  // Create the Command observer and register it with the registration filter.
  //
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  filter->AddObserver( itk::IterationEvent(), observer );
  
  
  // Software Guide : BeginLatex
  //
  // The input fixed image is simply the output of the fixed image casting
  // filter.  The input moving image is the output of the histogram matching
  // filter.
  //
  // \index{itk::DemonsRegistrationFilter!SetFixedImage()}
  // \index{itk::DemonsRegistrationFilter!SetMovingImage()}
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  filter->SetFixedImage( fixedImageCaster->GetOutput() );
  filter->SetMovingImage( matcher->GetOutput() );
  // Software Guide : EndCodeSnippet
  
  
  // Software Guide : BeginLatex
  //
  // The demons registration filter has two parameters: the number of
  // iterations to be performed and the standard deviation of the Gaussian
  // smoothing kernel to be applied to the deformation field after each
  // iteration.
  // \index{itk::DemonsRegistrationFilter!SetNumberOfIterations()}
  // \index{itk::DemonsRegistrationFilter!SetStandardDeviations()}
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  filter->SetNumberOfIterations( 100 );
  filter->SetStandardDeviations( 1.0 );
  // Software Guide : EndCodeSnippet
  
  
  // Software Guide : BeginLatex
  //
  // The registration algorithm is triggered by updating the filter. The
  // filter output is the computed deformation field.
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  filter->Update();
  // Software Guide : EndCodeSnippet
  
  
  // Software Guide : BeginLatex
  //
  // The \doxygen{WarpImageFilter} can be used to warp the moving image with
  // the output deformation field. Like the \doxygen{ResampleImageFilter},
  // the WarpImageFilter requires the specification of the input image to be
  // resampled, an input image interpolator, and the output image spacing and
  // origin.
  //
  // \index{itk::WarpImageFilter}
  // \index{itk::WarpImageFilter!SetInput()}
  // \index{itk::WarpImageFilter!SetInterpolator()}
  // \index{itk::WarpImageFilter!SetOutputSpacing()}
  // \index{itk::WarpImageFilter!SetOutputOrigin()}
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef itk::WarpImageFilter<
  MovingImageType,
  MovingImageType,
  DisplacementFieldType  >     WarperType;
  typedef itk::LinearInterpolateImageFunction<
  MovingImageType,
  double          >  InterpolatorType;
  WarperType::Pointer warper = WarperType::New();
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
  
  warper->SetInput( movingImageReader->GetOutput() );
  warper->SetInterpolator( interpolator );
  warper->SetOutputSpacing( fixedImage->GetSpacing() );
  warper->SetOutputOrigin( fixedImage->GetOrigin() );
  warper->SetOutputDirection( fixedImage->GetDirection() );
  
  warper->SetDisplacementField( filter->GetOutput() );
  
  // Write warped image out to file
  typedef  unsigned char                           OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
  typedef itk::CastImageFilter<
  MovingImageType,
  OutputImageType >          CastFilterType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;
  
  WriterType::Pointer      writer =  WriterType::New();
  CastFilterType::Pointer  caster =  CastFilterType::New();
  
  writer->SetFileName( "demons_registration.jpg" );
  
  caster->SetInput( warper->GetOutput() );
  writer->SetInput( caster->GetOutput()   );
  writer->Update();
  
}



  
  
