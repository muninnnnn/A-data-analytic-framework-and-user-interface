
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

	while (getline(infile,s))
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
    //readTxt("/home/csunix/sc17dh/Project/example_meshpoints_10/worm_000000.vtu");
	/* 128-points
	string filename, line;
	ifstream infile;
	filename = "/home/csunix/sc17dh/Project/sampeVTUs/worm_000010.vtu";

	float cdnt[128][2];
	int n = 0;

	infile.open(filename.data());

	if (!infile.is_open())
	{
		cout << "Error,no such file\n";
	}

    string s1 = "<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">";

    while(getline(infile,line))
    {
	    int location = line.find(s1);
	    //cout<< location<<endl;
        if (location > 0)
            break;

    }
    while (getline(infile,line))
    {
        //cout<<line<<endl;

		float first_num, second_num, third_num;
		sscanf(line.c_str(), "%f %f %d", &first_num, &second_num, &third_num);
		//cout << first_num << " " << second_num << " " << third_num << endl;
		cdnt[n][0] = first_num;
		cdnt[n][1] = second_num;
		vec_num.push_back(first_num);
		n++;

		if(n >= 128)
            break;
	}

	for (int i = 0; i < 128; i++)
	{
		cout << cdnt[i][0] << " " << cdnt[i][1] << endl;
	}
	*/

	// 10-points
	string filename, line;
	ifstream infile;
	filename = "/home/csunix/sc17dh/Project/example_meshpoints_10/worm_000000.vtu";

	float cdnt[10][2];
	int n = 0;

	infile.open(filename.data());

	if (!infile.is_open())
	{
		cout << "Error,no such file\n";
	}

    string s1 = "<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">";

    while(getline(infile,line))
    {
	    int location = line.find(s1);
	    //cout<< location<<endl;
        if (location > 0)
            break;

    }
    while (getline(infile,line))
    {
        //cout<<line<<endl;

		float first_num, second_num, third_num;
		sscanf(line.c_str(), "%f %f %d", &first_num, &second_num, &third_num);
		//cout << first_num << " " << second_num << " " << third_num << endl;
		cdnt[n][0] = first_num;
		cdnt[n][1] = second_num;
		vec_num.push_back(first_num);
		n++;

		if(n >= 10)
            break;
	}

	for (int i = 0; i < 10; i++)
	{
		cout << cdnt[i][0] << " " << cdnt[i][1] << endl;
	}















	return 0;
}



