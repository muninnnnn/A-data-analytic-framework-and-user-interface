
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <stdio.h>
#include <cmath>


using namespace std;
vector<float> vec_num;

void readTxt(string file)
{
	ifstream infile;
    infile.open(file.data());
	assert(infile.is_open());

	string s;

	while (getline(infile, s))
	{

		cout << s << endl;
	}
	infile.close();
}

float delta = 1e-5;
int isEqual(float x, float y)
{
	if ((x - y) > delta && (x-y)<-delta )
		return 0;
	else
		return 1;

}


int main()
{

	//readTxt("C:\\Users\\munin\\Desktop\\Project\\example_meshpoints_10\\example_meshpoints_10\\worm.pvd");
	//readTxt("/home/csunix/sc17dh/Project/example_meshpoints_10/worm_000000.vtu");

	string filename, line;
	ifstream file;
	filename = "/home/csunix/sc17dh/Project/example_meshpoints_10/worm_000000.vtu";

	float a[50][2];
	float cdnt[10][2];
	int n = 0;
	int m;

	file.open(filename.data());

	if (!file.is_open())
	{
		cout << "Error,no such file\n";
	}

	while (getline(file, line))
	{

		float first_num, second_num, third_num;
		sscanf(line.c_str(), "%f %f %d", &first_num, &second_num, &third_num);//格式化提取
		//cout << first_num << " " << second_num << " " << third_num << endl;
		a[n][0] = first_num;
		a[n][1] = second_num;
		vec_num.push_back(first_num);
		n++;
	}

	//cout<< n <<endl;
	m = 19;
	for (int i = 0; i < 10; i++)
	{
		cdnt[i][0] = a[m + i][0];
		cdnt[i][1] = a[m + i][1];
	}
	for (int i = 0; i < 10; i++)
	{
		cout << cdnt[i][0] << " " << cdnt[i][1] << endl;
	}




	return 0;


}



