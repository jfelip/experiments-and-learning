#include <CVector3.hpp>
#include <ctime>
#include <iostream>
//#include <Eigen/Dense>

using namespace vec3;

//void testArrayImplementation(unsigned int iterations)
//{
//	Vector3::Vector3 v1;
//	Vector3::assign(v1,5);
//
//	uint i=0;
//	while (i<iterations)
//	{
//		Vector3::inverse(v1);
//		++i;
//	}
//
//}
//
//void testClassImplementation(unsigned int iterations)
//{
//	Vector3<double> v1;
//	v1 = 5;
//
//	uint i=0;
//	while (i<iterations)
//	{
//		v1.inverse();
//		++i;
//	}
//
//}
//
//void testEigenImplementation(unsigned int iterations)
//{
//	Eigen::Vector3d v;
//	v << 5 ,5, 5;
//
//	uint i=0;
//	while (i<iterations)
//	{
//		v = -v;
//		++i;
//	}
//}

void testQuaternionForInitialization(unsigned int iterations)
{
	double q[4] = {0,1,2,3};
	double q2[4] = {0,0,0,0};


	uint i=0;
	while (i<iterations)
	{
		for (uint j=0; j<4; ++j)
			q2[j] = q[j];
		++i;
	}
}

void testQuaternionSequentialInitialization(unsigned int iterations)
{
	double q[4] = {0,1,2,3};
	double q2[4] = {0,0,0,0};

	uint i=0;
	while (i<iterations)
	{
		q2[0] = q[0];
		q2[1] = q[1];
		q2[2] = q[2];
		q2[3] = q[3];
		++i;
	}
}


int main(int argc, char** argv)
{

	unsigned int iterations = 10000000;

	using namespace std;

    clock_t begin, end;
    double elapsed_secs;

//	begin = clock();
//	testArrayImplementation(iterations);
//	end = clock();
//
//	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//	cout<<"Time using array implementation: "<< elapsed_secs << std::endl;

	begin = clock();
    testQuaternionSequentialInitialization(iterations);
	end = clock();

	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time using Sequential implementation: "<< elapsed_secs << std::endl;

	begin = clock();
	testQuaternionForInitialization(iterations);
	end = clock();

	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time using For-loop implementation: "<< elapsed_secs;

}


