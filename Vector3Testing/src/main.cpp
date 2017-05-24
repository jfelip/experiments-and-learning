#include <CVector3.hpp>
#include <ctime>
#include <iostream>
#include <Eigen/Dense>

void testArrayImplementaiton(unsigned int iterations)
{
	Vector3::Vector3 v1;
	Vector3::assign(v1,5);

	for (unsigned int i=0; i<iterations; ++i)
	{
		Vector3::inverse(v1);
	}

}

void testClassImplementaiton(unsigned int iterations)
{
	Vector3::CVector3<double> v1;
	v1 = 5;

	for (unsigned int i=0; i<iterations; ++i)
	{
		v1.inverse();
	}

}

void testEigenImplementaiton(unsigned int iterations)
{
	Eigen::Vector3d v;
	v << 5 ,5, 5;

	for (unsigned int i=0; i<iterations; ++i)
	{
		v = -v;
	}
}


int main(int argc, char** argv)
{

	unsigned int iterations = 1000000000;

	using namespace std;

	clock_t begin = clock();
	testArrayImplementaiton(iterations);
	clock_t end = clock();

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time using array implementation: "<< elapsed_secs << std::endl;

	begin = clock();
	testClassImplementaiton(iterations);
	end = clock();

	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time using class implementation: "<< elapsed_secs << std::endl;

	begin = clock();
	testEigenImplementaiton(iterations);
	end = clock();

	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time using eigen implementation: "<< elapsed_secs;

}


