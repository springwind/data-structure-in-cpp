#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <iomanip>
#include <chrono>

using namespace std;

struct Point2D
{
	double x;
	double y;

	Point2D(double x = 0, double y = 0)
		:x(x), y(y)
	{
	}
};

double dist(const Point2D & pt1, const Point2D & pt2)
{
	return sqrt(pow(pt1.x - pt2.x, 2) + pow(pt1.y - pt2.y, 2));
}

class CmpX
{
public:
	bool operator() (const Point2D & p1, const Point2D &p2)
	{
		return p1.x  < p2.x;
	}
};

class CmpY
{
public:
	bool operator() (const Point2D &p1, const Point2D &p2)
	{
		return p1.y < p2.y;
	}
};

class ClosestPoints
{
public:
	ClosestPoints(const vector<Point2D> & points)
		:P(points)
	{
	}

	
	double bruteforce()
	{

		if(P.size() < 2)
			return INFINITY;

		double delta = numeric_limits<double>::max();
		auto i1 = P.begin();
		auto i2 = P.begin();
		for(auto itr1 = P.begin(); itr1 != P.end(); itr1++)
			for(auto itr2  = itr1 + 1; itr2 != P.end(); itr2++)
			{
				double d = dist(*itr1, *itr2);
				if(d < delta)
				{
					delta = d;
					i1 = itr1;
					i2 = itr2;
				}
			}

		cout<<"compute the closest points in a plane by brute force"<<endl;
		cout<<"the closest points are:\t("<<i1->x<<","<<i1->y<<")\t("<<i2->x<<","<<i2->y<<")\td = "<<delta<<endl;

		return delta;
	}
	
	double compute()
	{
		if(P.size() < 2)	//less than 2 points
			return INFINITY;

		_delta = INFINITY;	//the distance of closest points
		_pt1.x = INFINITY;
		_pt1.y = INFINITY;
		_pt2.x = INFINITY;
		_pt2.y = INFINITY;
		
		sort(P.begin(), P.end(), CmpX());

		vector<Point2D> Q(P);
		sort(Q.begin(), Q.end(), CmpY());

		closest(P, 0, P.size() - 1, Q);
		if(_delta < INFINITY)
		{
			cout<<"compute the closest points in a plane using divide and conquer"<<endl;
			cout<<"the closest points are:\t("<<_pt1.x<<","<<_pt1.y<<")\t("<<_pt2.x<<","<<_pt2.y<<")\td = "<<_delta<<endl;
			return _delta;

		}

		return INFINITY;
	}


	void showPoints()
	{
		cout<<endl<<"------------------points ordered by x:------------------"<<endl;
		int n = 0;
		for(auto pt : P)
		{
			cout<<setw(8)<<pt.x<<","<<setw(8)<<pt.y<<"\t";
			if(++n== 5)
			{
				cout<<endl;
				n = 0;
			}
		}
		cout<<"------------------------------------------------------------------"<<endl;
	}
private:
	vector<Point2D> P; //points ordered by x
	//vector<Point2D> Q; //points ordered by y


	Point2D _pt1;
	Point2D _pt2;


	double _delta;

	//P has already sorted by x, Q has already sorted by y
	double closest(const vector<Point2D> & P, int left, int right, vector<Point2D> & Q)
	{
		if(left >= right) //less than 2 points
			return INFINITY;
		else if(left + 2 >= right) //less than 4 points
		{
			return closestInStrip(Q);
		}
		else if(P[right].x - P[left].x  < 1e-10) //P[left, right] are on the same line
		{
			return closestInStrip(Q);
		}


		double xmid = (P[left].x + P[right].x)/2; //the mid x position of the leftest and rightest points


		//partition Q by vertical line x = xmid
		vector<Point2D> QLeft, QRight;
		for(auto pt:Q)
		{
			if(pt.x < xmid)
				QLeft.push_back(pt);
			else
				QRight.push_back(pt);
		}
		
		auto itrMid = lower_bound(P.begin() + left, P.begin() + right + 1, Point2D(xmid, 0), CmpX());
		int mid = itrMid - P.begin();
		double dL = closest(P, left, mid - 1, QLeft); //P[left, mid-1] are to the left of the line x = xmid
		double dR = closest(P, mid, right, QRight);	//P[mid, right] are to the right of the line x = xmid, or on the line

		//to get point that are in the strip, discard points that are not in the strip
		/*for(auto itr = Q.begin(); itr != Q.end(); )
		{
			if(fabs(itr->x - xmid) > _delta)
				itr = Q.erase(itr);
			else
				itr ++;
		}
		double dC = closestInStrip(Q);
		*/
		vector<Point2D> QCenter;
		for(auto itr = Q.begin(); itr != Q.end(); itr++)
		{
			if(fabs(itr->x - xmid) < _delta)
				QCenter.push_back(*itr);
		}
		double dC = closestInStrip(QCenter);
	
		return _delta;
	}
	

	//all points in v are in the strip
	double closestInStrip(const vector<Point2D> & v)
	{
		
		for(auto itr1 = v.begin(); itr1<v.end(); itr1++)
			for(auto itr2 = itr1 + 1; itr2 < v.end(); itr2++)
			{
				if(itr2->y - itr1->y > _delta)
					break; 
				else
				{
					double d = dist(*itr1, *itr2);
					if (d < _delta) //always compare with the current smallest distance
					{
						_delta = d;
						_pt1 = *itr1;
						_pt2 = *itr2;
					}
				}
			}
		return _delta;

	}
 
};



void test1()
{
	cout<<endl<<"===================test 1============================="<<endl;
	vector<Point2D> v(2);
	v[0].x = -100 + rand()%100;
	v[0].y = -100 + rand() %100;

	v[1].x = -100 + rand() %100;
	v[1].y = -100 + rand() %100;


	ClosestPoints cp(v);
	cp.showPoints();
	cp.bruteforce();
	cp.compute();
}

void test2()
{
	cout<<endl<<"===================test 2============================="<<endl;
	vector<Point2D> v(100);

	for(int i = 0 ; i<100; i++)
	{
		v[i].x = -1000 + rand() %2000;
		v[i].y = -1000 + rand() %2000;
	}


	ClosestPoints cp(v);
	cp.showPoints();
	cp.bruteforce();
	cp.compute();
}

void test3()
{
	cout<<endl<<"===================test 3============================="<<endl;
	int M = numeric_limits<int>::max()/10000;
	int N = M / 2;
	cout<<M<<"\t"<<N<<endl;
	vector<Point2D> v(N);

	for(int i = 0 ; i<N; i++)
	{
		v[i].x = -N + rand() %M;
		v[i].y = -N + rand() %M;
	}


	ClosestPoints cp(v);
	//cp.showPoints();
	auto s = chrono::system_clock::now();
	cp.bruteforce();
	auto e1 = chrono::system_clock::now();
	cp.compute();
	auto e2 = chrono::system_clock::now();
	chrono::duration<double> d1 = e1 - s;
	chrono::duration<double> d2 = e2 - e1;

	cout<<"time: "<<d1.count()<<"s\t"<<d2.count()<<"s"<<endl;
}

int main()
{
	test1();

	test2();

	test3();
}
