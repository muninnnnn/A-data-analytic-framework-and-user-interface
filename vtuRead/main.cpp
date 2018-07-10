
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
	//readTxt("/home/csunix/sc17dh/Project/sampeVTUs/worm_000010.vtu");

	string filename, line;
	ifstream file;
	filename = "/home/csunix/sc17dh/Project/sampeVTUs/worm_000010.vtu";

	float origin[500][2];
	float cdnt[128][2];
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
		origin[n][0] = first_num;
		origin[n][1] = second_num;
		vec_num.push_back(first_num);
		n++;
	}

	//cout<< n <<endl;
	m = 139;
	for (int i = 0; i < 128; i++)
	{
		cdnt[i][0] = origin[m + i][0];
		cdnt[i][1] = origin[m + i][1];
	}
	for (int i = 0; i < 128; i++)
	{
		cout << cdnt[i][0] << " " << cdnt[i][1] << endl;
	}




	return 0;


}



