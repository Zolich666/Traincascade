#include<opencv2/highgui.hpp>
#include<opencv2/core.hpp>
#include<opencv2/ml.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect.hpp>

#include<iostream>
#include<fstream>

#define CombNum 3

using namespace cv;
using namespace std;
using namespace ml;

void CalculateScore(vector<Rect>goals_all, vector<Rect>goals_final, int* Scores);
bool isOverlap(Rect r1, Rect r2);

int main()
{
	int k = 1;
	Mat TestImage;
	Mat CopyTest;
	vector<Rect>goals_all,goals_final;
	CascadeClassifier cascade;
	cascade.load("F:\\Adaboost\\0123MLTest\\cascade1.xml");
	IplImage* pImg;
	int Scores[20];
	ofstream out_txt("output.txt");
	out_txt.close();
	for (k = 1; k < 140; k++)
	{
		TestImage = imread("F:\\Adaboost\\Test Set\\960_540\\DayTest(" + to_string(k) + ").jpg");
		CopyTest = TestImage.clone();

		ofstream out_txt("output.txt", ios::app);
		cascade.detectMultiScale(CopyTest, goals_all, 1.2, 0, 0 | CASCADE_SCALE_IMAGE);//识别图，所有框存入goals_all
		cascade.detectMultiScale(CopyTest, goals_final, 1.2, CombNum, 0 | CASCADE_SCALE_IMAGE);//识别图，存入容器goals_final，每次矩形框放大倍数，几个矩形框合并
		cout << "DayTest(" << k << ")" << endl << goals_final.size() << endl;
		out_txt << "test/DayTest(" << k <<")" << endl << goals_final.size() << endl;
		out_txt.close();
		//if (goals_final.size() == 0) break;
		//goals_final.x   .y    .width    .height
		CalculateScore(goals_all, goals_final, Scores);
		for (int i = 0; i < goals_final.size(); i++)
		{
			rectangle(CopyTest, goals_final[i], Scalar(0, 255, 0), 2);//测试图，goals_final[i]所含的矩形框，颜色，线宽
			cout << goals_final[i].x << " " << goals_final[i].y << " " << goals_final[i].width << " " << goals_final[i].height << " " << Scores[i] << endl;
			ofstream out_txt("output.txt", ios::app);
			out_txt << goals_final[i].x << " " << goals_final[i].y << " " << goals_final[i].width << " " << goals_final[i].height << " " << Scores[i] << endl;
			out_txt.close();
		}
		imshow("DayTest " + to_string(k), CopyTest);
		waitKey(1000);
		destroyWindow("DayTest " + to_string(k));
	}


}

void CalculateScore(vector<Rect>goals_all, vector<Rect>goals_final,int* Scores)
{
	int i = 0;
	for (vector<Rect>::const_iterator r = goals_final.begin(); r != goals_final.end(); r++, i++)
	{
	
		int score = 0;
		for (vector<Rect>::const_iterator s = goals_all.begin(); s != goals_all.end(); s++)
		{
			if (isOverlap(*r, *s))//如果重复就说明这个子矩形框是正确的，可以加分
			{
				score++;
			}
		}
		Scores[i] = score;
	}
}

bool isOverlap(Rect r1, Rect r2)//判断矩形是否重叠
{

	int x0 = r1.x > r2.x ? r1.x : r2.x;
	int y0 = r1.y > r2.y ? r1.y : r2.y;
	int x1 = r1.x + r1.width;
	int y1 = r1.y + r1.height;
	int x2 = r2.x + r2.width;
	int y2 = r2.y + r2.height;
	int x3 = x1 < x2 ? x1 : x2;
	int y3 = y1 < y2 ? y1 : y2;
	if (x3 < x0 || y3 < y0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
