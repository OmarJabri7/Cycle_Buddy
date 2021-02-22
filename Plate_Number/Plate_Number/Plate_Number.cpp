#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc.hpp>
#include "opencv2/shape.hpp"

using namespace cv;
using namespace std;
Mat ReSize(Mat imgArr, int maxWidth, int maxHeight);
Mat ImgProcess(Mat reSizeImg);
//vector<vector<Point> > FindContour(Mat processImg);

//resize the picture
Mat ReSize(Mat imgArr, int maxWidth, int maxHeight) {
	Mat imgReSize;
	double changeXRate = 0.0;
	double changeYRate = 0.0;
	Size dsize = Size(maxWidth, maxHeight);

	int width = imgArr.cols;
	int height = imgArr.rows;
	if (width > maxWidth) {
		changeXRate = maxWidth / width;
	}
	resize(imgArr, imgReSize, dsize, changeXRate, changeYRate);

	return imgReSize;
}

Mat ImgProcess(Mat reSizeImg) {
	Mat  imgPro;
	Size kSize = Size(5, 5);
	Mat contourImg;
	Mat thresholdImg;
	Mat edgesImg;
	Mat openingImg;
	Mat closingImg;

	//Gaussian filter
	Mat imgGauss;
	GaussianBlur(reSizeImg, imgGauss, kSize, 0, 0, BORDER_DEFAULT);
	Size mSize = Size(23, 23);
	Point mPoint = Point(-1, -1);
	Mat kernel = getStructuringElement(MORPH_RECT, mSize, mPoint);
	Mat imgOpening;
	morphologyEx(imgGauss, imgOpening, MORPH_OPEN, kernel, mPoint, 2, BORDER_DEFAULT, morphologyDefaultBorderValue());
	addWeighted(reSizeImg, 1, imgOpening, -1, 0, imgOpening, -1);
	imshow("imgOpening", imgOpening);

	//  find the edge of the picture
	threshold(imgOpening, thresholdImg, 0, 255, THRESH_BINARY + THRESH_OTSU);
	imshow("thresholdImg", thresholdImg);
	Canny(thresholdImg, edgesImg, 100, 200, 3, false);
	imshow("edgesImg", edgesImg);

	// connect the edge of the picture
	Size mSize1 = Size(7, 7);
	Point mPoint1 = Point(-1, -1);
	Mat kernel1 = getStructuringElement(MORPH_RECT, mSize1, mPoint1);
	morphologyEx(edgesImg, closingImg, MORPH_CLOSE, kernel1, mPoint1, 2, BORDER_DEFAULT, morphologyDefaultBorderValue());
	imshow("closingImg", closingImg);
	Size mSize2 = Size(7, 7);
	Mat kernel2 = getStructuringElement(MORPH_RECT, mSize2, mPoint1);
	morphologyEx(closingImg, openingImg, MORPH_OPEN, kernel2, mPoint1, 2, BORDER_DEFAULT, morphologyDefaultBorderValue());
	imshow("openingImg", openingImg);
	//Find rectangele area, which can be the plate number area

	vector<vector<Point> > contoursArrs;
	vector<Vec4i> hierarchy;
	findContours(openingImg, contoursArrs, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	vector<vector<Point> > tempContoursArrs(contoursArrs.size());

	//contourArea(contoursArr, false);
	int k = 0;
	for (size_t i = 0; i < contoursArrs.size(); i++) {

		if (contourArea(contoursArrs[i], false) > 2000) {
			tempContoursArrs[k] = contoursArrs[i];
			k++;
			cout << contourArea(contoursArrs[i], false) << endl;

		}
	}
	vector<RotatedRect> minRotatedRect(k);
	vector<Rect>  minRect(k);
	for (size_t i = 0; i < k; i++) {
		minRotatedRect[i] = minAreaRect(tempContoursArrs[i]);

		cout << minRotatedRect[i].size << endl;
		double ratio = minRotatedRect[i].size.height / minRotatedRect[i].size.width;
		int m = 0;
		//Mat drawing = Mat::zeros(openingImg.size(), CV_8UC3);
		vector<vector<Point> > PlateContoursArrs(k);
		if ((ratio > 2.8 && ratio < 2.9)) {
			RNG rng(12345);
			PlateContoursArrs[m] = tempContoursArrs[i];
			vector<RotatedRect>	plateRotatedRect(k);
			plateRotatedRect[m] = minAreaRect(PlateContoursArrs[m]);

			Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
			// contour
			drawContours(reSizeImg, PlateContoursArrs, (int)0, color);
			// ellipse
			//ellipse(drawing, minEllipse[i], color, 2);
			// rotated rectangle
			Point2f rect_points[4];
			minRotatedRect[i].points(rect_points);
			for (int j = 0; j < 4; j++)
			{
				line(reSizeImg, rect_points[j], rect_points[(j + 1) % 4], color);
			}
			imshow("ContoursLine", reSizeImg);
			
			//get the pictures
			Rect brect = plateRotatedRect[m].boundingRect(); //返回包含旋转矩形的最小矩形  
			//rectangle(reSizeImg, brect, color, 1, 8, 0);
			rectangle(reSizeImg, brect, Scalar(0, 0, 255));
			imshow("RectImg'", reSizeImg);
			m++;
		}
	}

	imgPro = imgOpening;

	return imgPro;

}

int main() {

	//namedWindow("Car orignal");
	//namedWindow("Car gray");
	Mat grayImg;

	// put the image here
	Mat img = imread("D:\\C++_Project\\Plate_Number\\car_test.jpg");
	int maxWindowWidth = 400;
	int maxWindowHeight = 300;
	cvtColor(img, grayImg, COLOR_BGR2GRAY);

	//imshow("Car original", img);	
	//imshow("Car gray", grayImg);
	//waitKey(60000);
	//destroyAllWindows();

	Mat  grayImgResize = ReSize(grayImg, maxWindowWidth, maxWindowHeight);

	imshow("CarReSize", grayImgResize);
	Mat imgPro = ImgProcess(grayImgResize);
	imshow("morphological processing", imgPro);

	//vector<vector<Point> > contourImg;
	//contourImg = FindContour(imgPro);
	//imshow("feature", contourImg);
	// wait for 60000 ms, the window will close automatically  

	waitKey(60000);
	destroyAllWindows();
}