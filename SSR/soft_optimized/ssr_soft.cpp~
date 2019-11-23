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

Size sizer(101, 101);

int counter = 0;

bool qcount = false;

double *tim;

float logg[500];

	
void ssr_thread(int id)
{

	Mat outputimg(height, width, CV_8UC3);
	Mat gauss(height, width, CV_8UC3);

	Mat gaussianB(height, width, CV_8UC1);
	Mat gaussianG(height, width, CV_8UC1);
	Mat gaussianR(height, width, CV_8UC1);

	Mat R(height, width, CV_8UC1);
	Mat G(height, width, CV_8UC1);
	Mat B(height, width, CV_8UC1);

	Mat img;
	float y,z,x;
	x = y = z = 0;
	int sig=200;
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

		
		//start2 = clock();
		
		auto start = std::chrono::high_resolution_clock::now();
		
		mtx.lock();
		img = frames.front();
		frames.pop();
		mtx.unlock();
		//img=imread("clg2.jpg");
		counter++;
			
		GaussianBlur(img, gauss, sizer, 100);
		
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				
			x = (log10(img.at<Vec3b>(i, j)[2]) /log10(gauss.at<Vec3b>(i, j)[2]));
			
			z = (log10(img.at<Vec3b>(i, j)[0]) /log10(gauss.at<Vec3b>(i, j)[0]));
			
			y = (log10(img.at<Vec3b>(i, j)[1]) /log10(gauss.at<Vec3b>(i, j)[1]));
			
			
			


			x *= sig;
			
			y *= sig;
			
			z *= sig;
	

	
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
	//VideoCapture cap(0);
	VideoCapture cap("vid2320x240.mp4");
	
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 480);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 320);

	/*VideoWriter outputVideo("./vid2ssr.avi", CV_FOURCC('M', 'J', 'P', 'G'),
	cap.get(CV_CAP_PROP_FPS),
	Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),
	cap.get(CV_CAP_PROP_FRAME_HEIGHT)));*/

	Mat frame, out;
	
	cap.read(frame);
	
	height = frame.rows;
	width = frame.cols;

	tim = new double[t_num];
	
	for(int i = 0; i < t_num; i++) {
		tim[i] = 0;
	}

	for(int k=1; k < 500;k++)
	{
		logg[k] = log10(k);
	}

	thread th0(ssr_thread, 0);
	thread th1(ssr_thread, 1);
	thread th2(ssr_thread, 2);
	thread th3(ssr_thread, 3);
	
	//_start(std::chrono::steady_clock::now());

	while (waitKey(40) != 27)
	{
		cap.read(frame);

		if (frame.rows == 0 || frame.cols == 0)
			break;
	//	if(!qcount)
		{
			mtx.lock();	
			frames.push(frame);
			//cout<<"Load in Q1  ::"<<frames.size()<<endl;
			mtx.unlock();
		}
		if(oframes.size() > 0)
		{
			mtx1.lock();
			out = oframes.front();	
			oframes.pop();
			//cout<<"Load in Q2  ::"<<oframes.size()<<endl;
			imshow("0", out);
			//imshow("1", frame);
			/*outputVideo.write(out);*/
			//imwrite("./ssroutput.mp4",out);
			mtx1.unlock();
		}
		
		fcount++;
	}
	
	end_prog = true;

	
	cout << "Esc key Pressed or Video has Ended\n";
 	cout << "Thread Processed: " << counter << endl;
	cout << "Main Processed: " << fcount << endl;

	

	th0.join();
	th1.join();
	th2.join();
	th3.join();
	
	for(int i = 1; i < t_num; i++) 
	{
		tim[0] += tim[i];
	}
	
	cout << "Time taken for " << counter << " " << fcount << " is " << tim[0]/t_num << " ms" <<endl;
	cout << "Thread Processed: " << counter << endl;
	cout << "Main Processed: " << fcount << endl;
	
	
	waitKey(0);
	/*outputVideo.release();*/
	destroyAllWindows();
}
