
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <stdio.h>
#include <cmath>

using namespace std;
vector<float> vec_num;
int fileCounter = 0;

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



void readPvd(string file)
{
    string line;
	ifstream infile;

	infile.open(file.data());

	if (!infile.is_open())
	{
		cout << "Error,no such file\n";
	}

    string s1 = "file=\"";
    string filename;
    while(getline(infile,line))
    {
        if (line.find(s1) != string::npos)
        {
            int location = line.find(s1);
            location = location + 6;
            filename = line.substr(location,15);
            fileCounter++;
            //cout<<filename<<endl;
        }
    }
    infile.close();
}

void saveFilename(string file)
{
    string vtuFile[fileCounter];
    int n = 0;
    string line;
    string filename;
	ifstream infile;

	infile.open(file.data());

	if (!infile.is_open())
	{
		cout << "Error,no such file\n";
	}


    string s1 = "file=\"";
    while(getline(infile,line))
    {
        if (line.find(s1) != string::npos)
        {
            int location = line.find(s1);
            location = location + 6;
            vtuFile[n] = line.substr(location,15);
            n++;

        }

    }
    for(int i = 0; i <fileCounter; i++)
    {

        cout<<vtuFile[i]<<endl;
    }
    infile.close();
}


void readCdnt(string file)
{
    string line;
	ifstream infile;
	stringstream numTrans;
	int numOfPoints;
	int n = 0;

	infile.open(file.data());
	if (!infile.is_open())
	{
		cout << "Error,no such file\n";
	}


    string s1 = "Piece NumberOfPoints";
    string s2 = "<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">";

    while(getline(infile,line))
    {


	    if(line.find(s1) != string::npos)
        {
            int location = line.find(s1);
            location = location + 22;
            numTrans << line.substr(location,2);
            numTrans >> numOfPoints;
            cout<<numOfPoints<<endl;

        }


        if (line.find(s2) != string::npos)
            break;

    }

    float cdnt[numOfPoints][2];

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

		if(n >= numOfPoints)
            break;
	}

	for (int i = 0; i < 10; i++)
	{
		cout << cdnt[i][0] << " " << cdnt[i][1] << endl;
	}

	infile.close();
}




int main()
{

	//readTxt("C:\\Users\\munin\\Desktop\\Project\\example_meshpoints_10\\example_meshpoints_10\\worm.pvd");
	//readTxt("/home/csunix/sc17dh/Project/sampeVTUs/worm_000010.vtu");
    //readTxt("/home/csunix/sc17dh/Project/example_meshpoints_10/worm_000000.vtu");
	//readCdnt("/home/csunix/sc17dh/Project/example_meshpoints_10/worm_000000.vtu");


    readPvd("/home/csunix/sc17dh/Project/example_meshpoints_10/worm.pvd");
    saveFilename("/home/csunix/sc17dh/Project/example_meshpoints_10/worm.pvd");

    cout<<"The number of files is :"<<fileCounter<<endl;





	return 0;
}



