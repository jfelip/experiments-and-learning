#include <CVector3.hpp>
#include <ctime>
#include <iostream>
#include <Eigen/Dense>

void testArrayImplementation(unsigned int iterations)
{
	Vector3::Vector3 v1;
	Vector3::assign(v1,5);

	uint i=0;
	while (i<iterations)
	{
		Vector3::inverse(v1);
		++i;
	}

}

void testClassImplementation(unsigned int iterations)
{
	Vector3::CVector3<double> v1;
	v1 = 5;

	uint i=0;
	while (i<iterations)
	{
		v1.inverse();
		++i;
	}

}

void testEigenImplementation(unsigned int iterations)
{
	Eigen::Vector3d v;
	v << 5 ,5, 5;

	uint i=0;
	while (i<iterations)
	{
		v = -v;
		++i;
	}
}


int main(int argc, char** argv)
{

	unsigned int iterations = 10000000;

	using namespace std;

	clock_t begin = clock();
	testArrayImplementation(iterations);
	clock_t end = clock();

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time using array implementation: "<< elapsed_secs << std::endl;

	begin = clock();
	testClassImplementation(iterations);
	end = clock();

	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time using class implementation: "<< elapsed_secs << std::endl;

	begin = clock();
	testEigenImplementation(iterations);
	end = clock();

	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time using eigen implementation: "<< elapsed_secs;

}


