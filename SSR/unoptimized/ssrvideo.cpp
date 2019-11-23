#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>


using namespace std::chrono;

using namespace std;

using namespace cv;

float tim;

int main()
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	VideoCapture cap("vid2320x240.mp4"); 
	
	//VideoCapture cap(0);

	double fcount = 0000;

	//namedWindow("output", 1);

	/*VideoWriter outputVideo("./smallscale900frames.avi", CV_FOURCC('M', 'J', 'P', 'G'),
		cap.get(CV_CAP_PROP_FPS),
		Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),
		cap.get(CV_CAP_PROP_FRAME_HEIGHT)));*/
auto start = std::chrono::high_resolution_clock::now();
	while (1)
	{

		Mat frame;

	//	cap >> frame; 

  		if (!cap.read(frame)) // if not success, break loop
    		    {		cout<<"here"<<endl;
        		   auto stop = std::chrono::high_resolution_clock::now();
				cout<<"here"<<endl;
				tim=std::chrono::duration_cast<chrono::milliseconds>(stop - start).count();
			cout << "It took me " << tim << " seconds.";
			cout<<"here"<<endl;
       		     break;
     		   }
	

		Mat output(frame.rows, frame.cols ,CV_8UC3);

		Mat gaussianB(frame.rows, frame.cols, CV_8UC1);
		Mat gaussianG(frame.rows, frame.cols, CV_8UC1);
		Mat gaussianR(frame.rows, frame.cols, CV_8UC1);

		//

		Mat B(frame.rows, frame.cols, CV_8UC1);
		Mat G(frame.rows, frame.cols, CV_8UC1);
		Mat R(frame.rows, frame.cols, CV_8UC1);


		//
		///*for (int i = 0; i < frame.rows; i++)
		//{
		//for (int j = 0; j < frame.cols; j++)
		//{
		//B.at<uchar>(i, j) = frame.at<Vec3b>(i, j)[0];
		//G.at<uchar>(i, j) = frame.at<Vec3b>(i, j)[1];
		//R.at<uchar>(i, j) = frame.at<Vec3b>(i, j)[2];

		//cout  << "B : "  << int(B.at<uchar>(i, j)) << "\t" << "G : "  << int(G.at<uchar>(i, j)) << "\t" << "R: " << int(R.at<uchar>(i, j)) << endl ;
		//Sleep(50);
		//}
		//}*/

		//

		for (int i = 0; i < frame.rows; i++)
		{
			for (int j = 0; j < frame.cols; j++)
			{
				B.at<uchar>(i, j) = (frame.at<Vec3b>(i, j)[0] < 4) ? 4 : frame.at<Vec3b>(i, j)[0];
				G.at<uchar>(i, j) = (frame.at<Vec3b>(i, j)[1] < 4) ? 4 : frame.at<Vec3b>(i, j)[1];
				R.at<uchar>(i, j) = (frame.at<Vec3b>(i, j)[2] < 4) ? 4 : frame.at<Vec3b>(i, j)[2];
			}
		}
		
		//

		///*
		//imshow("R", R);
		//imshow("G", G);
		//imshow("B", B);
		//waitKey(0);
		//destroyAllWindows();*/

		GaussianBlur(B, gaussianB, Size(301, 301), 300);
		GaussianBlur(G, gaussianG, Size(301, 301), 300);
		GaussianBlur(R, gaussianR, Size(301, 301), 300);

		//

		///*imshow("R", gaussianR);
		//imshow("G", gaussianG);
		//imshow("B", gaussianB);
		//waitKey(0);
		//destroyAllWindows();*/

		for (int i = 0; i < frame.rows; i++)
		{
			for (int j = 0; j < frame.cols; j++)
			{
				double x = fabs(log10(frame.at<Vec3b>(i, j)[2]) -log10(gaussianB.at<Vec3b>(i, j)[2]));

				double z = fabs(log10(frame.at<Vec3b>(i, j)[0]) -log10(gaussianG.at<Vec3b>(i, j)[0]));
				double y = fabs(log10(frame.at<Vec3b>(i, j)[1]) -log10(gaussianR.at<Vec3b>(i, j)[1]));

			//		//cout << x << ' ' << y << ' ' << z << endl;

				x *= 200;
				y *= 200;
				z *= 200;

				if (x < 0)
					x = 0;

				if (x > 255)
				x = 255;

				if (y < 0)
					y = 0;

				if (y > 255)
					y = 255;

				if (z < 0)
					z = 0;

				if (z > 255)
					z = 255;
			
		
			output.at<Vec3b>(i, j)[0] = 255-z;
			output.at<Vec3b>(i, j)[1] = 255-y;
			output.at<Vec3b>(i, j)[2] = 255-x;
			}
		}
		imshow("output", output);
		fcount++;
		//if (fcount == 938)
		{
		//	break;
		}

		if (waitKey(30) >= 0) break;
	}
	
	

	auto stop = std::chrono::high_resolution_clock::now();

	cout << "It took me " << std::chrono::duration_cast<chrono::milliseconds>(stop - start).count()<< " seconds.";

	waitKey(0);

	/*outputVideo.release();*/

	return 0;

}
