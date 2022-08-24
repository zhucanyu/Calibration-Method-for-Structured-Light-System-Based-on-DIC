#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector> 
#include <fstream>  
#include <sstream> 
#include <stdio.h> 

//Read 'centers*.csv' and 'centersnew*.csv' and 'referencecenters.csv' to calibrate parameters.  The calibrated parameters are written to the root.

using namespace cv;
using namespace std;
int main()
{
	//��ȡcsv���鲢�洢��centers  (Read the csv array and store it to 'centers')
	ifstream inFile;
	string lineStr;
	string number;
	double num , errrr;
	vector< vector< Point2d> >centersL;
	vector< vector< Point2d> >centersR;
	vector< vector< Point3d> >objpoint;
	vector<double> linecenter;
	vector<string> str;
	

	vector< Point2d> centers1;
	vector< Point2d> centersnew1;

	for (int i = 0; i < 5; i++) //Number of calibrated pictures
	{
		inFile.open("E:/opencv_test/centers"+to_string(i+1)+".csv", ios::in); //Change it to"Your File Directory/centers..."
		while (getline(inFile, lineStr)) //��ȡ��һ��
		{
			istringstream ss(lineStr); //��istringstream��ȡ����������string ss
			linecenter.clear();;  //������ʱ����linecenter��ű�������
			while (getline(ss, number, ',')) //���ŷָ��stringת��Ϊdouble
			{
				num = atof(number.c_str());
				linecenter.push_back(num);
			}
			Point2d poi;
			poi.x = linecenter[0]; poi.y = linecenter[1];//��double�������ݴ洢����ʱ��poi
			centers1.push_back(poi);//centers1���һ��poi
		}
		cout << "centers"<< i+1<<".csv��ȡ����" << endl;
		inFile.close();
		centersL.push_back(centers1);
		centers1.clear();

	}

	for (int i = 0; i < 5; i++) //Number of calibrated pictures 
	{
		inFile.open("E:/opencv_test/centersnew"+to_string(i+1)+".csv", ios::in); //Change it to"Your File Directory/centers..."
		
		while (getline(inFile, lineStr)) //��ȡ��һ��
		{
			//cout << lineStr << endl;
			istringstream ss(lineStr); //��istringstream��ȡ����������string ss
			linecenter.clear();;  //������ʱ����linecenter��ű�������
			while (getline(ss, number, ',')) //���ŷָ��stringת��Ϊdouble
			{
				num = atof(number.c_str());
				linecenter.push_back(num);
			}
			Point2d poi;
			poi.x = linecenter[0]; poi.y = linecenter[1];//��double�������ݴ洢����ʱ��poi
			centersnew1.push_back(poi);//centersnew1���һ��poi
		}
		cout << "centersnew"<<i+1<<".csv��ȡ����" <<"����"<<centersnew1.size() << endl;
		inFile.close();
		centersR.push_back(centersnew1);
		centersnew1.clear();
	}
	

	

	//��ȡcsv���鲢�洢��referencecenters
	inFile.open("E:/opencv_test/referencecenters.csv", ios::in); //Change it to"Your File Directory/centers..."
	vector< Point3d> referencecenters;
	while (getline(inFile, lineStr)) //��ȡ��һ��
	{
		//cout << lineStr << endl;
		istringstream ss(lineStr); //��istringstream��ȡ����������string ss
		linecenter.clear();;  //������ʱ����linecenter��ű�������
		while (getline(ss, number, ',')) //���ŷָ��stringת��Ϊdouble
		{
			num = atof(number.c_str());
			linecenter.push_back(num);
		}
		Point3d poi;
		poi.x = linecenter[0]; poi.y = linecenter[1]; poi.z = linecenter[2];//��double�������ݴ洢����ʱ��poi
		referencecenters.push_back(poi);//referencecenters���һ��poi
	}
	cout << "referencenters.csv��ȡ����" << endl;
	objpoint.push_back(referencecenters);
	objpoint.push_back(referencecenters);
	objpoint.push_back(referencecenters);
	objpoint.push_back(referencecenters);
	objpoint.push_back(referencecenters);

	Mat image = imread("E:/opencv_test/1.png");
	cout << "ͼƬ��" << image.cols << " ͼƬ�ߣ�" << image.rows << endl;
	Size sz(image.cols, image.rows); //��ȡ�ο�ͼƬ���õ�����
	//cout << " width:" << sz.width << " height:" << sz.height<<endl;
	Mat mtxl = Mat::eye(3, 3, CV_64F);
	Mat mtxr = Mat::eye(3, 3, CV_64F);
	Mat  distl,distr,R, T,rvecs, tvecs;
	TermCriteria stereo_criteria = TermCriteria(TermCriteria::MAX_ITER |TermCriteria::EPS, 1000, 1e-12);
	//�궨

	int N = centersL.size();
	vector< vector< Point2f> >centersLf(N);
	N = centersR.size();
	vector< vector< Point2f> >centersRf(N);
	N = objpoint.size();
	vector< vector< Point3f> >objpointf(N);//�ѵ���float��ʽ

	for (int i = 0; i < centersL.size(); i++)
	{
		centersLf[i].resize(centersL[i].size());
		for (int j = 0; j < centersL[i].size(); j++)
		{	
			centersLf[i][j].x = centersL[i][j].x;
			centersLf[i][j].y = centersL[i][j].y;
		}
		
	}
	for (int i = 0; i < centersR.size(); i++)
	{
		centersRf[i].resize(centersR[i].size());
		for (int j = 0; j < centersR[i].size(); j++)
		{
			centersRf[i][j].x = centersR[i][j].x;
			centersRf[i][j].y = centersR[i][j].y;
		}
	}
	for (int i = 0; i < objpoint.size(); i++)
	{
		objpointf[i].resize(objpoint[i].size());
		for (int j = 0; j < objpoint[i].size(); j++)
		{
			objpointf[i][j].x = objpoint[i][j].x;
			objpointf[i][j].y = objpoint[i][j].y;
			objpointf[i][j].z = objpoint[i][j].z;
		}
	}
	cout << "�궨��ʼ" << endl;
	calibrateCamera(objpointf, centersLf, sz, mtxl, distl, rvecs, tvecs, 0 , stereo_criteria);
	
	cout << "������궨����" << endl;
	cout << "mtxl:" <<endl<< mtxl.at<double>(0, 0)<< " " << mtxl.at<double>(0, 1)<< " " << mtxl.at<double>(0, 2)<< endl;
	cout << mtxl.at<double>(1, 0) <<" "<< mtxl.at<double>(1, 1) << " " << mtxl.at<double>(1, 2) << endl;
	cout << mtxl.at<double>(2, 0) << " " << mtxl.at<double>(2, 1) << " " << mtxl.at<double>(2, 2) << endl;
	cout << "distl:" << endl << distl.at<double>(0) << " " << distl.at<double>(1) << " " << distl.at<double>(2) << " " << distl.at<double>(3) << " " << distl.at<double>(4) << endl;
	calibrateCamera(objpointf, centersRf, sz, mtxr, distr, noArray(), noArray(), 0,stereo_criteria);
	cout << "������궨����" << endl;
	cout << "mtxr:" << endl << mtxr.at<double>(0, 0) << " " << mtxr.at<double>(0, 1) << " " << mtxr.at<double>(0, 2) << endl;
	cout << mtxr.at<double>(1, 0) << " " << mtxr.at<double>(1, 1) << " " << mtxr.at<double>(1, 2) << endl;
	cout << mtxr.at<double>(2, 0) << " " << mtxr.at<double>(2, 1) << " " << mtxr.at<double>(2, 2) << endl;
	cout << "distr:" << endl << distr.at<double>(0) <<" "<< distr.at<double>(1) <<" "<< distr.at<double>(2) <<" "<< distr.at<double>(3) <<" "<< distr.at<double>(4) << " "<<endl;
	errrr=stereoCalibrate (objpointf,centersLf, centersRf, mtxl, distl, mtxr, distr, sz, R, T, noArray(),noArray(),CALIB_USE_INTRINSIC_GUESS|CALIB_FIX_INTRINSIC, stereo_criteria);
	cout << "��ͶӰ���" << errrr << endl;
	//stereoCalibrate (objpointf,centersLf, centersRf, mtxl, distl, mtxr, distr, sz, R, T, noArray(),noArray(),CALIB_USE_INTRINSIC_GUESS, stereo_criteria);
	cout << "R:" << endl << R.at<double>(0, 0) << " " << R.at<double>(0, 1) << " "<<R.at<double>(0, 2) << endl;
	cout << R.at<double>(1, 0) << " "<<R.at<double>(1, 1) << " "<<R.at<double>(1, 2) << endl;
	cout << R.at<double>(2, 0) << " "<<R.at<double>(2, 1) << " "<<R.at<double>(2, 2) << endl;
	cout << "T:" << endl << T.at<double>(0) << " " << T.at<double>(1) << " " << T.at<double>( 2) << endl;
	cout << "�궨����" << endl;

	ofstream of;
	of.open("T.csv", ios::out);
	of << setprecision(10)<<T.at<double>(0) << "," << T.at<double>(1) << "," << T.at<double>(2) ;
	of.close();
	of.open("R.csv", ios::out);
	of << setprecision(10)<< R.at<double>(0, 0) << "," << R.at<double>(0, 1) << "," << R.at<double>(0, 2) << endl;
	of << setprecision(10)<< R.at<double>(1, 0) << "," << R.at<double>(1, 1) << "," << R.at<double>(1, 2) << endl;
	of << setprecision(10)<< R.at<double>(2, 0) << "," << R.at<double>(2, 1) << "," << R.at<double>(2, 2) << endl;
	of.close();
	of.open("mtxl.csv", ios::out);
	of << setprecision(10)<< mtxl.at<double>(0, 0) << "," << mtxl.at<double>(0, 1) << "," << mtxl.at<double>(0, 2) << endl;
	of << setprecision(10)<< mtxl.at<double>(1, 0) << "," << mtxl.at<double>(1, 1) << "," << mtxl.at<double>(1, 2) << endl;
	of << setprecision(10)<< mtxl.at<double>(2, 0) << "," << mtxl.at<double>(2, 1) << "," << mtxl.at<double>(2, 2) << endl;
	of.close();
	of.open("distl.csv", ios::out);
	of << setprecision(10) << distl.at<double>(0) << "," << distl.at<double>(1) << "," << distl.at<double>(2) << "," << distl.at<double>(3) << "," << distl.at<double>(4);
	of.close();
	of.open("mtxr.csv", ios::out);
	of << setprecision(10) << mtxr.at<double>(0, 0) << "," << mtxr.at<double>(0, 1) << "," << mtxr.at<double>(0, 2) << endl;
	of << setprecision(10) << mtxr.at<double>(1, 0) << "," << mtxr.at<double>(1, 1) << "," << mtxr.at<double>(1, 2) << endl;
	of << setprecision(10) << mtxr.at<double>(2, 0) << "," << mtxr.at<double>(2, 1) << "," << mtxr.at<double>(2, 2) << endl;
	of.close();
	of.open("distr.csv", ios::out);
	of << setprecision(10) << distr.at<double>(0) << "," << distr.at<double>(1) << "," << distr.at<double>(2) << "," << distr.at<double>(3) << "," << distr.at<double>(4);
	of.close();

	cout << "��д���ļ�" << endl;
	getchar();
	waitKey(0);
	return 0;
}