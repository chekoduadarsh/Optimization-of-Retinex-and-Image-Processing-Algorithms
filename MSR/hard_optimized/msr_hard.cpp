#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
using namespace cv;

int width = 320;
int height = 240;

queue <Mat> frames;
queue <Mat> oframes;

Mat frame;

bool end_prog = false;

int fcount = 0, t_num = 4;

mutex mtx, mtx1;

Size sizer(301, 301);

int counter = 0;

float logg[500];

double *tim;

//double tim = 0;

void ssr_thread(int id)
{
	Mat outputimg(height, width, CV_8UC3);
	
	Mat gauss1(height, width, CV_8UC3);
	Mat gauss2(height, width, CV_8UC3);
	Mat gauss3(height, width, CV_8UC3);

	Mat gaussianB(height, width, CV_8UC1);
	Mat gaussianG(heigh+t, width, CV_8UC1);
	Mat gaussianR(height, width, CV_8UC1);

	Mat R(height, width, CV_8UC1);
	Mat G(height, width, CV_8UC1);
	Mat B(height, width, CV_8UC1);

	Mat img;
	float x = 0, y = 0, z = 0, xx = 0, yy = 0, zz = 0, xxx = 0, yyy = 0, zzz = 0;

	while (true)
	{
		while (frames.size() == 0 && !end_prog)
		{
			this_thread::sleep_for(chrono::milliseconds(10));
		}

		if (end_prog && frames.size() == 0)
			break;

		if (counter % t_num != id)
			continue;

		auto start = std::chrono::high_resolution_clock::now();		
		
		mtx.lock();
		img = frames.front();
		frames.pop();
		mtx.unlock();

		counter++;

		GaussianBlur(img, gauss1, sizer, 300);
		GaussianBlur(img, gauss2, sizer, 80);
		GaussianBlur(img, gauss3, sizer, 250);

		x = y = z = 0;
		
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{				
				x = logg[img.at<Vec3b>(i, j)[2]] / logg[gauss1.at<Vec3b>(i, j)[2]] / 3;
				z = logg[img.at<Vec3b>(i, j)[0]] / logg[gauss1.at<Vec3b>(i, j)[0]] / 3;
				y = logg[img.at<Vec3b>(i, j)[1]] / logg[gauss1.at<Vec3b>(i, j)[1]] / 3;
				
				xx = logg[img.at<Vec3b>(i, j)[2]] / logg[gauss2.at<Vec3b>(i, j)[2]] / 3;
				zz = logg[img.at<Vec3b>(i, j)[0]] / logg[gauss2.at<Vec3b>(i, j)[0]] / 3;
				yy = logg[img.at<Vec3b>(i, j)[1]] / logg[gauss2.at<Vec3b>(i, j)[1]] / 3;
				
				xxx = logg[img.at<Vec3b>(i, j)[2]] / logg[gauss3.at<Vec3b>(i, j)[2]] / 3;
				zzz = logg[img.at<Vec3b>(i, j)[0]] / logg[gauss3.at<Vec3b>(i, j)[0]] / 3;
				yyy = logg[img.at<Vec3b>(i, j)[1]] / logg[gauss3.at<Vec3b>(i, j)[1]] / 3;
				
				
				/*x = log10(img.at<Vec3b>(i, j)[2]) / log10(gauss1.at<Vec3b>(i, j)[2]) / 3;
				z = log10(img.at<Vec3b>(i, j)[0]) / log10(gauss1.at<Vec3b>(i, j)[0]) / 3;
				y = log10(img.at<Vec3b>(i, j)[1]) / log10(gauss1.at<Vec3b>(i, j)[1]) / 3;
				
				xx = log10(img.at<Vec3b>(i, j)[2]) / log10(gauss2.at<Vec3b>(i, j)[2]) / 3;
				zz = log10(img.at<Vec3b>(i, j)[0]) / log10(gauss2.at<Vec3b>(i, j)[0]) / 3;
				yy = log10(img.at<Vec3b>(i, j)[1]) / log10(gauss2.at<Vec3b>(i, j)[1]) / 3;
				
				xxx = log10(img.at<Vec3b>(i, j)[2]) / log10(gauss3.at<Vec3b>(i, j)[2]) / 3;
				zzz = log10(img.at<Vec3b>(i, j)[0]) / log10(gauss3.at<Vec3b>(i, j)[0]) / 3;
				yyy = log10(img.at<Vec3b>(i, j)[1]) / log10(gauss3.at<Vec3b>(i, j)[1]) / 3;*/

				z = (z + zz + zzz) * 210;
				y = (y + yy + yyy) * 210;
				x = (x + xx + xxx) * 210;
				 
				/*x *= 210;
				y *= 210;
				z *= 210;*/

				x = (x < 0)? 0 : x;
				x = (x > 255)? 255 : x;


				y = (y < 0)? 0 : y;
				y = (y > 255)? 255 : y;

				z = (z < 0)? 0 : z;
				z = (z > 255)? 255 : z;

				outputimg.at<Vec3b>(i, j)[0] = z;
				outputimg.at<Vec3b>(i, j)[1] = y;
				outputimg.at<Vec3b>(i, j)[2] = x;
			}
		}
		
		mtx1.lock();	
		oframes.push(outputimg);
		
		auto stop = std::chrono::high_resolution_clock::now();
		
		tim[id] += std::chrono::duration_cast<chrono::milliseconds>(stop - start).count(); 
		mtx1.unlock();

	}
	
	cout << "Exiting Thread " << id << endl;
}


int main()
{
	VideoCapture cap(0);
	//VideoCapture cap(0);
	
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 480);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 320);
	

VideoWriter outputVideo("./vid4msr.avi", CV_FOURCC('M', 'J', 'P', 'G'),
	8,
	Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),
		cap.get(CV_CAP_PROP_FRAME_HEIGHT)));

	Mat frame, out;
	
	cap.read(frame);
	
	height = frame.rows;
	width = frame.cols;

	for(int k=1; k < 500;k++)
	{
		logg[k] = log10(k);
	}
	
	tim = new double[t_num];
	
	for(int i = 0; i < t_num; i++) {
		tim[i] = 0;
	}
	
	thread th0(ssr_thread, 0);
	thread th1(ssr_thread, 1);
	thread th2(ssr_thread, 2);
	thread th3(ssr_thread, 3);

	//std::chrono::steady_clock::time_point _start(std::chrono::steady_clock::now());
	while (waitKey(40) != 27)
	{
		cap.read(frame);

		if (frame.rows == 0 || frame.cols == 0)
			break;
		
		mtx.lock();	
		frames.push(frame);
		mtx.unlock();

		if(oframes.size() != 0)
		{
			mtx1.lock();
			out = oframes.front();	
			oframes.pop();
			outputVideo.write(out);
			imshow("0", out);
			imshow("1", frame);
			mtx1.unlock();
			
			
		}
		
		fcount++;
		//imshow("1", frame);
		
		//if(fcount == 900)
		//	break;
		//imwrite("./ssroutput",out);
		
	}
	//std::chrono::steady_clock::time_point _end(std::chrono::steady_clock::now());

	//tim = chrono::duration_cast<std::chrono::duration<double>>(_end - _start).count();
	//cout << "time taken for "<<counter<<"   "<<fcount<<"is"<<tim<<endl;
	end_prog = true;

	cout << "Esc key Pressed or Video has Ended\n";
 
	//while(frames.size() != 0)
		//cout << frames.size() << endl;
		
	th0.join();
	th1.join();
	th2.join();
	th3.join();
	
	
	for(int i = 1; i < t_num; i++) 
	{
		tim[0] += tim[i];
	}
	
	cout << "Time taken for " << counter << " " << fcount << " is " << tim[0]/3 << " ms" <<endl;

	cout << "Thread Processed: " << counter << endl;
	cout << "Main Processed: " << fcount << endl;

outputVideo.release();

	waitKey(0);
	
	destroyAllWindows();
}
