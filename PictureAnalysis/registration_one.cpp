//
//  registration_one.cpp
//  pictureAnalysis
//
//  Created by Matthew Tay Han Yang on 4/14/15. based on itk software guide but refactored slightly.
//
//

#include "registration_one.h"



class CommandIterationUpdate : public itk::Command
{
public:
  typedef CommandIterationUpdate   Self;
  typedef itk::Command             Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  itkNewMacro( Self );
  
protected:
  CommandIterationUpdate() {};
  
public:
  
  typedef itk::RegularStepGradientDescentOptimizerv4<double> OptimizerType;
  typedef const OptimizerType*                               OptimizerPointer;
  
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
    
    std::cout << optimizer->GetCurrentIteration() << " = ";
    std::cout << optimizer->GetValue() << " : ";
    std::cout << optimizer->GetCurrentPosition() << std::endl;
  }
};


Registration_LinearTranslation::Registration_LinearTranslation(char * file_name_one, char * file_name_two) {
  fixed_image = file_name_one;
  moving_image = file_name_two;
}

void Registration_LinearTranslation::start_registration() {
  std::cout<< "hello world" << std::endl;
  const unsigned int Dimension  = 2;
  typedef  float           RegistrationPixelType;
  typedef itk::Image< RegistrationPixelType, Dimension >  FixedImageType;
  typedef itk::Image< RegistrationPixelType, Dimension >  MovingImageType;
  typedef itk::TranslationTransform< double, Dimension > TransformType;
  typedef itk::RegularStepGradientDescentOptimizerv4<double> OptimizerType;
  typedef itk::MeanSquaresImageToImageMetricv4<
  FixedImageType,
  MovingImageType >    MetricType;
  typedef itk::ImageRegistrationMethodv4<
  FixedImageType,
  MovingImageType,
  TransformType   >    RegistrationType;
  MetricType::Pointer         metric        = MetricType::New();
  OptimizerType::Pointer      optimizer     = OptimizerType::New();
  RegistrationType::Pointer   registration  = RegistrationType::New();
  registration->SetMetric(        metric        );
  registration->SetOptimizer(     optimizer     );
  
  typedef itk::LinearInterpolateImageFunction<
  FixedImageType,
  double > FixedLinearInterpolatorType;
  typedef itk::LinearInterpolateImageFunction<
  MovingImageType,
  double > MovingLinearInterpolatorType;
  
  FixedLinearInterpolatorType::Pointer fixedInterpolator =
  FixedLinearInterpolatorType::New();
  MovingLinearInterpolatorType::Pointer movingInterpolator =
  MovingLinearInterpolatorType::New();
  
  metric->SetFixedInterpolator(  fixedInterpolator  );
  metric->SetMovingInterpolator(  movingInterpolator  );
  
  typedef itk::ImageFileReader< FixedImageType  >   FixedImageReaderType;
  typedef itk::ImageFileReader< MovingImageType >   MovingImageReaderType;
  FixedImageReaderType::Pointer   fixedImageReader     = FixedImageReaderType::New();
  MovingImageReaderType::Pointer  movingImageReader    = MovingImageReaderType::New();
  
  fixedImageReader->SetFileName( fixed_image ); // second image is the fixed
  movingImageReader->SetFileName( moving_image );
  
  registration->SetFixedImage(    fixedImageReader->GetOutput()    );
  registration->SetMovingImage(   movingImageReader->GetOutput()   );
  
  TransformType::Pointer movingInitialTransform = TransformType::New();
  
  TransformType::ParametersType initialParameters(
                                                  movingInitialTransform->GetNumberOfParameters() );
  initialParameters[0] = 0.0;  // Initial offset in mm along X
  initialParameters[1] = 0.0;  // Initial offset in mm along Y
  
  movingInitialTransform->SetParameters( initialParameters );
  
  registration->SetMovingInitialTransform( movingInitialTransform );
  
  TransformType::Pointer   identityTransform = TransformType::New();
  identityTransform->SetIdentity();
  
  registration->SetFixedInitialTransform( identityTransform );
  
  
  optimizer->SetLearningRate( 4 );
  optimizer->SetMinimumStepLength( 0.001 );
  optimizer->SetRelaxationFactor( 0.5 );
  
  optimizer->SetNumberOfIterations( 10 );
  
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  optimizer->AddObserver( itk::IterationEvent(), observer );
  
  const unsigned int numberOfLevels = 1;
  
  RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
  shrinkFactorsPerLevel.SetSize( 1 );
  shrinkFactorsPerLevel[0] = 1;
  
  RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
  smoothingSigmasPerLevel.SetSize( 1 );
  smoothingSigmasPerLevel[0] = 0;
  
  registration->SetNumberOfLevels ( numberOfLevels );
  registration->SetSmoothingSigmasPerLevel( smoothingSigmasPerLevel );
  registration->SetShrinkFactorsPerLevel( shrinkFactorsPerLevel );
  
  try
  {
    registration->Update();
    std::cout << "Optimizer stop condition: "
    << registration->GetOptimizer()->GetStopConditionDescription()
    << std::endl;
  }
  catch( itk::ExceptionObject & err )
  {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
  }
  
  
  TransformType::ConstPointer transform = registration->GetTransform();
  TransformType::ParametersType finalParameters = transform->GetParameters();
  const double TranslationAlongX = finalParameters[0];
  const double TranslationAlongY = finalParameters[1];
  
  
  const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
  
  const double bestValue = optimizer->GetValue();
  
  
  std::cout << "Result = " << std::endl;
  std::cout << " Translation X = " << TranslationAlongX  << std::endl;
  std::cout << " Translation Y = " << TranslationAlongY  << std::endl;
  std::cout << " Iterations    = " << numberOfIterations << std::endl;
  std::cout << " Metric value  = " << bestValue          << std::endl;
  
  typedef itk::CompositeTransform<
  double,
  Dimension > CompositeTransformType;
  CompositeTransformType::Pointer outputCompositeTransform =
  CompositeTransformType::New();
  outputCompositeTransform->AddTransform( movingInitialTransform );
  outputCompositeTransform->AddTransform(
                                         registration->GetModifiableTransform() );
  
  
  typedef itk::ResampleImageFilter<
  MovingImageType,
  FixedImageType >    ResampleFilterType;
  ResampleFilterType::Pointer resampler = ResampleFilterType::New();
  resampler->SetInput( movingImageReader->GetOutput() );
  
  resampler->SetTransform( outputCompositeTransform );
  
  FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
  resampler->SetSize( fixedImage->GetLargestPossibleRegion().GetSize() );
  resampler->SetOutputOrigin(  fixedImage->GetOrigin() );
  resampler->SetOutputSpacing( fixedImage->GetSpacing() );
  resampler->SetOutputDirection( fixedImage->GetDirection() );
  resampler->SetDefaultPixelValue( 100 );
  
  typedef unsigned char OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
  typedef itk::CastImageFilter<
  FixedImageType,
  OutputImageType > CastFilterType2;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  
  WriterType::Pointer writer = WriterType::New();
  CastFilterType2::Pointer caster2 = CastFilterType2::New();
  
  writer->SetFileName( "test.jpg" );
  caster2->SetInput( resampler->GetOutput() );
  writer->SetInput( caster2->GetOutput()   );
  writer->Update();
  
  
  typedef itk::SubtractImageFilter<
  FixedImageType,
  FixedImageType,
  FixedImageType > DifferenceFilterType;
  
  DifferenceFilterType::Pointer difference = DifferenceFilterType::New();
  
  difference->SetInput1( fixedImageReader->GetOutput() );
  difference->SetInput2( resampler->GetOutput() );
  
  typedef itk::RescaleIntensityImageFilter<
  FixedImageType,
  OutputImageType >   RescalerType;
  
  RescalerType::Pointer intensityRescaler = RescalerType::New();
  
  intensityRescaler->SetInput( difference->GetOutput() );
  intensityRescaler->SetOutputMinimum(   0 );
  intensityRescaler->SetOutputMaximum( 255 );
  
  resampler->SetDefaultPixelValue( 1 );
  
  
  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput( intensityRescaler->GetOutput() );
  writer2->SetFileName( "test_differences.jpg" );
  writer2->Update();
  
  resampler->SetTransform( identityTransform );

  }









