using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Kinect;
using Microsoft.Speech.Recognition;
using System.Threading;
using System.IO;
using Microsoft.Speech.AudioFormat;
using System.Diagnostics;
using System.Windows.Threading;

// Add
using System.IO.Ports;

namespace KinectPowerPointControl
{
    public partial class MainWindow : Window
    {
        KinectSensor sensor;
        SpeechRecognitionEngine speechRecognizer;

        DispatcherTimer readyTimer;

        byte[] colorBytes;
        Skeleton[] skeletons;

        bool isCirclesVisible = true;
        bool isTurnRight = false;
        bool isTurnLeft = false;
        bool isStop = false;
        bool isForwardGestureActive = false;
        bool isBackGestureActive = false;

        SolidColorBrush activeBrush = new SolidColorBrush(Colors.Green);
        SolidColorBrush inactiveBrush = new SolidColorBrush(Colors.Red);

        // Add COMPort
        SerialPort COMPort = new SerialPort("COM19", 9600, Parity.None, 8, StopBits.One);

        public MainWindow()
        {
            InitializeComponent();

            //Runtime initialization is handled when the window is opened. When the window
            //is closed, the runtime MUST be unitialized.
            this.Loaded += new RoutedEventHandler(MainWindow_Loaded);
            //Handle the content obtained from the video camera, once received.

            this.KeyDown += new KeyEventHandler(MainWindow_KeyDown);
        }

        void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            sensor = KinectSensor.KinectSensors.FirstOrDefault();

            if (sensor == null)
            {
                MessageBox.Show("This application requires a Kinect sensor.");
                this.Close();
            }

            sensor.Start();

            sensor.ColorStream.Enable(ColorImageFormat.RgbResolution640x480Fps30);
            sensor.ColorFrameReady += new EventHandler<ColorImageFrameReadyEventArgs>(sensor_ColorFrameReady);

            sensor.DepthStream.Enable(DepthImageFormat.Resolution320x240Fps30);

            sensor.SkeletonStream.Enable();
            sensor.SkeletonFrameReady += new EventHandler<SkeletonFrameReadyEventArgs>(sensor_SkeletonFrameReady);

            //sensor.ElevationAngle = 10;

            Application.Current.Exit += new ExitEventHandler(Current_Exit);

            InitializeSpeechRecognition();

            // Open COMPort
            if (COMPort.IsOpen) COMPort.Close();
            else COMPort.Open();
        }

        void Current_Exit(object sender, ExitEventArgs e)
        {
            if (speechRecognizer != null)
            {
                speechRecognizer.RecognizeAsyncCancel();
                speechRecognizer.RecognizeAsyncStop();
            }
            if (sensor != null)
            {
                sensor.AudioSource.Stop();
                sensor.Stop();
                sensor.Dispose();
                sensor = null;
            }
        }

        void MainWindow_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.C)
            {
                ToggleCircles();
            }
        }

        void sensor_ColorFrameReady(object sender, ColorImageFrameReadyEventArgs e)
        {
            using (var image = e.OpenColorImageFrame())
            {
                if (image == null)
                    return;

                if (colorBytes == null ||
                    colorBytes.Length != image.PixelDataLength)
                {
                    colorBytes = new byte[image.PixelDataLength];
                }

                image.CopyPixelDataTo(colorBytes);

                //You could use PixelFormats.Bgr32 below to ignore the alpha,
                //or if you need to set the alpha you would loop through the bytes 
                //as in this loop below
                int length = colorBytes.Length;
                for (int i = 0; i < length; i += 4)
                {
                    colorBytes[i + 3] = 255;
                }

                BitmapSource source = BitmapSource.Create(image.Width,
                    image.Height,
                    96,
                    96,
                    PixelFormats.Bgra32,
                    null,
                    colorBytes,
                    image.Width * image.BytesPerPixel);
                videoImage.Source = source;
            }
        }

        void sensor_SkeletonFrameReady(object sender, SkeletonFrameReadyEventArgs e)
        {
            using (var skeletonFrame = e.OpenSkeletonFrame())
            {
                if (skeletonFrame == null)
                    return;

                if (skeletons == null ||
                    skeletons.Length != skeletonFrame.SkeletonArrayLength)
                {
                    skeletons = new Skeleton[skeletonFrame.SkeletonArrayLength];
                }

                skeletonFrame.CopySkeletonDataTo(skeletons);
            }

            Skeleton closestSkeleton = skeletons.Where(s => s.TrackingState == SkeletonTrackingState.Tracked)
                                                .OrderBy(s => s.Position.Z * Math.Abs(s.Position.X))
                                                .FirstOrDefault();

            if (closestSkeleton == null)
                return;

            var rightElbow = closestSkeleton.Joints[JointType.ElbowRight];
            var leftKnee = closestSkeleton.Joints[JointType.KneeLeft];
            var rightKnee = closestSkeleton.Joints[JointType.KneeRight];
            var rightLeg = closestSkeleton.Joints[JointType.FootRight];
            var leftLeg = closestSkeleton.Joints[JointType.FootLeft];
            var head = closestSkeleton.Joints[JointType.Head];
            var rightHand = closestSkeleton.Joints[JointType.HandRight];
            var leftHand = closestSkeleton.Joints[JointType.HandLeft];
            var hip = closestSkeleton.Joints[JointType.HipCenter];

            if (head.TrackingState == JointTrackingState.NotTracked ||
                rightHand.TrackingState == JointTrackingState.NotTracked ||
                leftHand.TrackingState == JointTrackingState.NotTracked)
            {
                ////// Action robot STOP
                //Don't have a good read on the joints so we cannot process gestures
                return;
            }

            SetEllipsePosition(ellipseRightLeg, rightLeg, isStop);
            // SetEllipsePosition(ellipseRightElbow, rightElbow, isStop);
            SetEllipsePosition(ellipseHip, hip, false);
            SetEllipsePosition(ellipseLeftKnee, leftKnee, isBackGestureActive);
            SetEllipsePosition(ellipseRightKnee, rightKnee, isForwardGestureActive);
            SetEllipsePosition(ellipseHead, head, false);
            SetEllipsePosition(ellipseLeftHand, leftHand, isTurnLeft);
            SetEllipsePosition(ellipseRightHand, rightHand, isTurnRight);


            /*
             * 
             *   bool isTurnRight = false;
        bool isTurnLeft = false;
        bool isStop = false;
        bool isForwardGestureActive = false;
        bool isBackGestureActive = false;
             * */
            ProcessForwardBackGesture(head, rightHand, leftHand, hip, rightKnee, leftKnee, rightElbow, rightLeg);
        }

        //This method is used to position the ellipses on the canvas
        //according to correct movements of the tracked joints.
        private void SetEllipsePosition(Ellipse ellipse, Joint joint, bool isHighlighted)
        {
            if (isHighlighted)
            {
                ellipse.Width = 60;
                ellipse.Height = 60;
                ellipse.Fill = activeBrush;
            }
            else
            {
                ellipse.Width = 20;
                ellipse.Height = 20;
                ellipse.Fill = inactiveBrush;
            }

            CoordinateMapper mapper = sensor.CoordinateMapper;

            var point = mapper.MapSkeletonPointToColorPoint(joint.Position, sensor.ColorStream.Format);

            Canvas.SetLeft(ellipse, point.X - ellipse.ActualWidth / 2);
            Canvas.SetTop(ellipse, point.Y - ellipse.ActualHeight / 2);
        }

        private void ProcessForwardBackGesture(Joint head, Joint rightHand, Joint leftHand, Joint hip, Joint rightKnee, Joint leftKnee, Joint rightElbow, Joint rightLeg)
        {

            /// turn right
            if (rightHand.Position.X > head.Position.X + 0.45)
            {
                if (!isTurnRight)
                {
                    isTurnRight = true;
                    ///control robot turn right (action)
                    Turn_Right();

                    System.Windows.Forms.SendKeys.SendWait("{Right}");
                }
            }
            else
                isTurnRight = false;



            /// Forward
            if ((rightKnee.Position.Y > hip.Position.Y - 0.30))
            {
                if (!isForwardGestureActive)
                {
                    isForwardGestureActive = true;
                    ///control robot to forward (action)
                    Run_Forward();

                    System.Windows.Forms.SendKeys.SendWait("{Up}");
                }
            }
            else
                isForwardGestureActive = false;

            ///
            /// Backward

            if ((leftKnee.Position.Y > hip.Position.Y - 0.30))
            {
                if (!isBackGestureActive)
                {
                    isBackGestureActive = true;
                    ///control robot turn to DOWN (BACKWARD) (action)
                    Run_Backward();

                    System.Windows.Forms.SendKeys.SendWait("{Down}");
                }
            }
            else
                isBackGestureActive = false;

            /////// turn left
            if (leftHand.Position.X < head.Position.X - 0.45)
            {
                if (!isTurnLeft)
                {
                    isTurnLeft = true;
                    System.Windows.Forms.SendKeys.SendWait("{Left}");
                    ///control robot turn left (action)
                    Turn_Left();
                }
            }
            else
                isTurnLeft = false;




            ////////// STOP
            /*
                                      if (rightElbow.Position.X> (hip.Position.X + 0.20))
                                            {
                                                if (!isStop)
                                                {
                                                    isStop = true;
                                                    System.Windows.Forms.SendKeys.SendWait("{Right}");
                                                    //control robot backward (action)
                                                }
                                            }
                                      else
                                            {
                                                isStop = false;
                                            }

            */
            ///STOP
            if (rightLeg.Position.X > (hip.Position.X + 0.40))
            {
                if (!isStop)
                {
                    isStop = true;
                    System.Windows.Forms.SendKeys.SendWait("{Right}");
                    //control robot to STOP(action)
                    Robot_Stop();
                }
            }
            else
            {
                isStop = false;
            }
        }


        void ToggleCircles()
        {
            if (isCirclesVisible)
                HideCircles();
            else
                ShowCircles();
        }

        void HideCircles()
        {
            isCirclesVisible = false;
            ellipseHead.Visibility = System.Windows.Visibility.Collapsed;
            ellipseLeftHand.Visibility = System.Windows.Visibility.Collapsed;
            ellipseRightHand.Visibility = System.Windows.Visibility.Collapsed;
        }

        void ShowCircles()
        {
            isCirclesVisible = true;
            ellipseHead.Visibility = System.Windows.Visibility.Visible;
            ellipseLeftHand.Visibility = System.Windows.Visibility.Visible;
            ellipseRightHand.Visibility = System.Windows.Visibility.Visible;
        }

        private void ShowWindow()
        {
            this.Topmost = true;
            this.WindowState = System.Windows.WindowState.Maximized;
        }

        private void HideWindow()
        {
            this.Topmost = false;
            this.WindowState = System.Windows.WindowState.Minimized;
        }

        #region Speech Recognition Methods

        private static RecognizerInfo GetKinectRecognizer()
        {
            Func<RecognizerInfo, bool> matchingFunc = r =>
            {
                string value;
                r.AdditionalInfo.TryGetValue("Kinect", out value);
                return "True".Equals(value, StringComparison.InvariantCultureIgnoreCase) && "en-US".Equals(r.Culture.Name, StringComparison.InvariantCultureIgnoreCase);
            };
            return SpeechRecognitionEngine.InstalledRecognizers().Where(matchingFunc).FirstOrDefault();
        }

        private void InitializeSpeechRecognition()
        {
            RecognizerInfo ri = GetKinectRecognizer();
            if (ri == null)
            {
                MessageBox.Show(
                    @"There was a problem initializing Speech Recognition.
Ensure you have the Microsoft Speech SDK installed.",
                    "Failed to load Speech SDK",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error);
                return;
            }

            try
            {
                speechRecognizer = new SpeechRecognitionEngine(ri.Id);
            }
            catch
            {
                MessageBox.Show(
                    @"There was a problem initializing Speech Recognition.
Ensure you have the Microsoft Speech SDK installed and configured.",
                    "Failed to load Speech SDK",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error);
            }

            var phrases = new Choices();
            phrases.Add("computer show window");
            phrases.Add("computer hide window");
            phrases.Add("computer show circles");
            phrases.Add("computer hide circles");

            var gb = new GrammarBuilder();
            //Specify the culture to match the recognizer in case we are running in a different culture.                                 
            gb.Culture = ri.Culture;
            gb.Append(phrases);

            // Create the actual Grammar instance, and then load it into the speech recognizer.
            var g = new Grammar(gb);

            speechRecognizer.LoadGrammar(g);
            speechRecognizer.SpeechRecognized += SreSpeechRecognized;
            speechRecognizer.SpeechHypothesized += SreSpeechHypothesized;
            speechRecognizer.SpeechRecognitionRejected += SreSpeechRecognitionRejected;

            this.readyTimer = new DispatcherTimer();
            this.readyTimer.Tick += this.ReadyTimerTick;
            this.readyTimer.Interval = new TimeSpan(0, 0, 4);
            this.readyTimer.Start();

        }

        private void ReadyTimerTick(object sender, EventArgs e)
        {
            this.StartSpeechRecognition();
            this.readyTimer.Stop();
            this.readyTimer.Tick -= ReadyTimerTick;
            this.readyTimer = null;
        }

        private void StartSpeechRecognition()
        {
            if (sensor == null || speechRecognizer == null)
                return;

            var audioSource = this.sensor.AudioSource;
            audioSource.BeamAngleMode = BeamAngleMode.Adaptive;
            var kinectStream = audioSource.Start();

            speechRecognizer.SetInputToAudioStream(
                    kinectStream, new SpeechAudioFormatInfo(EncodingFormat.Pcm, 16000, 16, 1, 32000, 2, null));
            speechRecognizer.RecognizeAsync(RecognizeMode.Multiple);

        }

        void SreSpeechRecognitionRejected(object sender, SpeechRecognitionRejectedEventArgs e)
        {
            Trace.WriteLine("\nSpeech Rejected, confidence: " + e.Result.Confidence);
        }

        void SreSpeechHypothesized(object sender, SpeechHypothesizedEventArgs e)
        {
            Trace.Write("\rSpeech Hypothesized: \t{0}", e.Result.Text);
        }

        void SreSpeechRecognized(object sender, SpeechRecognizedEventArgs e)
        {
            //This first release of the Kinect language pack doesn't have a reliable confidence model, so 
            //we don't use e.Result.Confidence here.
            if (e.Result.Confidence < 0.70)
            {
                Trace.WriteLine("\nSpeech Rejected filtered, confidence: " + e.Result.Confidence);
                return;
            }

            Trace.WriteLine("\nSpeech Recognized, confidence: " + e.Result.Confidence + ": \t{0}", e.Result.Text);

            if (e.Result.Text == "computer show window")
            {
                this.Dispatcher.BeginInvoke((Action)delegate
                    {
                        ShowWindow();
                    });
            }
            else if (e.Result.Text == "computer hide window")
            {
                this.Dispatcher.BeginInvoke((Action)delegate
                {
                    HideWindow();
                });
            }
            else if (e.Result.Text == "computer hide circles")
            {
                this.Dispatcher.BeginInvoke((Action)delegate
                {
                    this.HideCircles();
                });
            }
            else if (e.Result.Text == "computer show circles")
            {
                this.Dispatcher.BeginInvoke((Action)delegate
                {
                    this.ShowCircles();
                });
            }
        }

        #endregion



        #region ROBOT Control
        // Variables
        const byte stx = 0x02;
        const byte etx = 0x03;
        const byte BufferLength = 20;

        private void btTest_Click(object sender, RoutedEventArgs e)
        {
            Robot_Stop();
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            Run_Forward();
        }

        private void button3_Click(object sender, RoutedEventArgs e)
        {
            Run_Backward();
        }

        private void button4_Click(object sender, RoutedEventArgs e)
        {
            Turn_Right();
        }

        private void button2_Click(object sender, RoutedEventArgs e)
        {
            Turn_Left();
        }

        // Run Forward and Trackline
        private void Run_Forward()
        {
            Go_N_Trackline(25);
        }

        // Go Backward
        private void Run_Backward()
        {
            Send_Vel(-20, -20);
        }

        // Turn Left
        private void Turn_Left()
        {
            Send_Vel(28, -30);
        }

        // Turn Right
        private void Turn_Right()
        {
            Send_Vel(-30, 28);
        }

        private void Robot_Stop()
        {
            Send_Vel(0, 0);
        }

        // Send velocity
        private void Send_Vel(Int16 left_speed, Int16 right_speed)
        {
            if (COMPort.IsOpen)
            {
                byte[] buf = new byte[BufferLength];

                buf[0] = stx;
                buf[1] = 12;
                buf[2] = 1;     // Mode Manual control
                buf[3] = (byte)(left_speed >> 24);
                buf[4] = (byte)(left_speed >> 16);
                buf[5] = (byte)(left_speed >> 8);
                buf[6] = (byte)(left_speed);
                buf[7] = (byte)(right_speed >> 24);
                buf[8] = (byte)(right_speed >> 16);
                buf[9] = (byte)(right_speed >> 8);
                buf[10] = (byte)(right_speed);
                buf[11] = etx;

                // Send data
                COMPort.Write(buf, 0, buf[1]);
            }
            else
            {
                MessageBox.Show("COM Connection failed!", "Warning!");
            }
        }

        // Send Tracline Command
        private void Go_N_Trackline(Int16 v_track)  // v_track is percent
        {
            if (COMPort.IsOpen)
            {
                byte[] buf = new byte[BufferLength];

                buf[0] = stx;
                buf[1] = 8;
                buf[2] = 2;     // Mode Auto
                buf[3] = (byte)(v_track >> 24);
                buf[4] = (byte)(v_track >> 16);
                buf[5] = (byte)(v_track >> 8);
                buf[6] = (byte)(v_track);
                buf[7] = etx;

                // Send data
                COMPort.Write(buf, 0, buf[1]);
            }
            else
            {
                MessageBox.Show("COM Connection failed!", "Warning!");
            }
        }

        #endregion
    }
}
