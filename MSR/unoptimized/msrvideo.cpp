#include "opencv2/opencv.hpp"
#include <iostream>
#include <math.h>
#include <chrono>

using namespace std::chrono;

using namespace std;

using namespace cv;

int main(int, char**)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	double fcount = 0000;

	VideoCapture cap("vid2320x240.mp4");
/*
  VideoWriter outputVideo("./multiscale900frames.avi", CV_FOURCC('D', 'I', 'V', 'X'),
	  cap.get(CV_CAP_PROP_FPS),
	Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),
		cap.get(CV_CAP_PROP_FRAME_HEIGHT)));*/

	//VideoCapture cap(0);

	double x, y, z, x1, y1, z1, x2, y2, z2;

	namedWindow("output", 1);

	while (1)
	{
		//high_resolution_clock::time_point t1 = high_resolution_clock::now();

		Mat frame;

		Mat dst;

		cap >> frame;

		if (fcount > 900)
		{
			break;
		}

		Mat output(frame.rows, frame.cols, CV_8UC3);

		Mat gaussianB(frame.rows, frame.cols, CV_8UC1);
		Mat gaussianG(frame.rows, frame.cols, CV_8UC1);
		Mat gaussianR(frame.rows, frame.cols, CV_8UC1);

		Mat gaussianB1(frame.rows, frame.cols, CV_8UC1);
		Mat gaussianG2(frame.rows, frame.cols, CV_8UC1);
		Mat gaussianR3(frame.rows, frame.cols, CV_8UC1);

		Mat gaussianB11(frame.rows, frame.cols, CV_8UC1);
		Mat gaussianG22(frame.rows, frame.cols, CV_8UC1);
		Mat gaussianR33(frame.rows, frame.cols, CV_8UC1);

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

		GaussianBlur(B, gaussianB1, Size(301, 301), 80);
		GaussianBlur(G, gaussianG2, Size(301, 301), 80);
		GaussianBlur(R, gaussianR3, Size(301, 301), 80);

		GaussianBlur(B, gaussianB11, Size(301, 301), 250);
		GaussianBlur(G, gaussianG22, Size(301, 301), 250);
		GaussianBlur(R, gaussianR33, Size(301, 301), 250);

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
				z = abs(log10(B.at<uchar>(i, j)) / log10(gaussianB.at<uchar>(i, j)));

				y = abs(log10(G.at<uchar>(i, j)) / log10(gaussianG.at<uchar>(i, j)));

				x = abs(log10(R.at<uchar>(i, j)) / log10(gaussianR.at<uchar>(i, j)));

				z1 = abs(log10(B.at<uchar>(i, j)) / log10(gaussianB1.at<uchar>(i, j)));

				y1 = abs(log10(G.at<uchar>(i, j)) / log10(gaussianG2.at<uchar>(i, j)));

				x1 = abs(log10(R.at<uchar>(i, j)) / log10(gaussianR3.at<uchar>(i, j)));

				z2 = abs(log10(B.at<uchar>(i, j)) / log10(gaussianB11.at<uchar>(i, j)));

				y2 = abs(log10(G.at<uchar>(i, j)) / log10(gaussianG22.at<uchar>(i, j)));

				x2 = abs(log10(R.at<uchar>(i, j)) / log10(gaussianR33.at<uchar>(i, j)));


				x = x / 3;
				y = y / 3;
				z = z / 3;
				x1 = x1 / 3;
				y1 = y1 / 3;
				z1 = z1 / 3;
				x2 = x2 / 3;
				y2 = y2 / 3;
				z2 = z2 / 3;

				//		//cout << x << ' ' << y << ' ' << z << endl;

				double xx = ((z + z1 + z2));
				double yy = ((y + y1 + y2));
				double zz = ((x + x1 + x2));

				xx *= 210;
				yy *= 210;
				zz *= 210;

				if (xx < 0)
					xx = 0;

				if (xx > 255)
					xx = 255;

				if (yy < 0)
					yy = 0;

				if (yy > 255)
					yy = 255;

				if (zz < 0)
					zz = 0;

				if (zz > 255)
					zz = 255;


				output.at<Vec3b>(i, j)[0] = xx;
				output.at<Vec3b>(i, j)[1] = yy;
				output.at<Vec3b>(i, j)[2] = zz;
			}
		}

		fcount++;

		GaussianBlur(output, dst, cv::Size(0, 0), 3);
		addWeighted(output, 1.5, dst, -0.5, 0, dst);

		//Mat f;

		////for (int i = 0; i < frame.rows; i++)
		////{
		//	for (int j = 0; j < frame.cols; j++)
		//	{
		//		 f = output - check;

		//		cout << 
		//	}
		//}



		//imshow("frame", frame);


		//imshow("first output", check);

		//cvtColor(frame, output, CV_BGR2GRAY);

		//cout << "hello error is not here" << endl;

		//GaussianBlur(frame, output, Size(7, 7), 1.5, 1.5);

		//Canny(output, output, 0, 30, 3);

		//output = frame;

		imshow("output", dst);

         /*outputVideo.write(output);*/

		imshow("frame", frame);

		//imshow("output", output);

		//high_resolution_clock::time_point t2 = high_resolution_clock::now();

		//duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

		//cout << "It took me " << time_span.count() << " seconds." << endl;

		//imshow("output", output);

		if (waitKey(30) >= 0) break;
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	cout << "It took me " << time_span.count() << " seconds.";

	waitKey(0);

		/* outputVideo.release();*/


	return 0;

}
