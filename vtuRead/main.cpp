
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <stdio.h>
#include <cmath>
#include <vector>


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
            //int location = line.find(s1);
            //location = location + 6;
            //filename = line.substr(location,15);
            fileCounter++;
            //cout<<filename<<endl;
        }
    }
    infile.close();

    //cout<<fileCounter<<endl;
}

void saveFilename(string file)
{
    string vtuFile[fileCounter];
    //vector<string> vtuFile(fileCounter);

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
            //vtuFile.push_back(line.substr(location,15));
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
    cout<<"The file name is: "<<file<<endl;
    string line;
	ifstream infile;

	ofstream outfile;

	stringstream numTrans;
	int numOfPoints;
	int nn = 0;

    float x_max = 0.0;
    float x_min = 0.0;
    float y_max = 0.0;
    float y_min = 0.0;




	infile.open(file.data());

	outfile.open("/home/csunix/sc17dh/Project/saveCdnt.txt",ios::app);

	if (!infile.is_open())
	{
		cout << "Error,no such file\n";
	}


    string s1 = "Piece NumberOfPoints";
    string s2 = "<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">";
    string s3 = "\"";

    while(getline(infile,line))
    {


	    if(line.find(s1) != string::npos)
        {

           int location1 = line.find(s1) + 22;
           int location2 = line.find(s3,location1);
           int location = location2 - location1;

            numTrans << line.substr(location1,location);
            numTrans >> numOfPoints;

            cout<<"location1 is: "<<location1<<endl;
            cout<<"location2 is: "<<location2<<endl;
            cout<<"Number of Points is: "<<numOfPoints<<endl;


        }


        if (line.find(s2) != string::npos)
            break;

    }

    cout<<endl;

    float cdnt[numOfPoints][2];

    //vector<vector<float> >cdnt(fileCounter,vector<float>(2,0));

    while (getline(infile,line))
    {
        //cout<<line<<endl;

		float first_num, second_num, third_num;
		sscanf(line.c_str(), "%f %f %f", &first_num, &second_num, &third_num);
		//cout << first_num << " " << second_num << " " << third_num << endl;
		cdnt[nn][0] = first_num;
		cdnt[nn][1] = second_num;
		vec_num.push_back(first_num);
		nn++;
/*
		if(cdnt[nn][0] > x_max)
            {
                x_max = cdnt[nn][0];
            }
        if(cdnt[nn][0] < x_min)
            {
                x_min = cdnt[nn][0];
            }
        if(cdnt[nn][1] > y_max)
            {
                y_max = cdnt[nn][1];
            }
        if(cdnt[nn][0] < y_min)
            {
                y_min = cdnt[nn][1];
            }

*/

		if(nn >= numOfPoints)
            break;
	}

    outfile << "The file name is: "<<file<<endl<<"Number of Points is: "<<numOfPoints<<endl;

	for (int i = 0; i < numOfPoints; i++)
	{
	    outfile << cdnt[i][0] << " " << cdnt[i][1] << endl;
		//cout << cdnt[i][0] << " " << cdnt[i][1] << endl;
	}
	outfile << endl;
	cout<<endl;

	cout<<"The max x is :"<<x_max<<endl;
    cout<<"The min x is :"<<x_min<<endl;
    cout<<"The max y is :"<<y_max<<endl;
    cout<<"The min y is :"<<x_min<<endl;

	infile.close();

	outfile.close();
}




int main()
{

	//readTxt("C:\\Users\\munin\\Desktop\\Project\\example_meshpoints_10\\example_meshpoints_10\\worm.pvd");
    //readTxt("/home/csunix/sc17dh/Project/sampeVTUs/worm_000010.vtu");
    //readTxt("/home/csunix/sc17dh/Project/example_meshpoints_10/worm_000000.vtu");
	//readCdnt("/home/csunix/sc17dh/Project/example_meshpoints_10/worm_000000.vtu");
	//readCdnt("/home/csunix/sc17dh/Project/sampeVTUs/worm_000010.vtu");


    readPvd("/home/csunix/sc17dh/Project/example_meshpoints_10/worm.pvd");
    //saveFilename("/home/csunix/sc17dh/Project/example_meshpoints_10/worm.pvd");


    string file="/home/csunix/sc17dh/Project/example_meshpoints_10/worm.pvd";
    string vtuFile[fileCounter];
    //vector<string> vtuFile(fileCounter);

    string line;
    string filename;
	ifstream infile;

	ofstream outfile;

	outfile.open("/home/csunix/sc17dh/Project/saveCdnt.txt",ios::out);
	outfile.close();

	int n = 0;

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
            vtuFile[n] = "/home/csunix/sc17dh/Project/example_meshpoints_10/"+line.substr(location,15);
            //vtuFile.push_back(line.substr(location,15));
            n++;
        }

    }

    cout<<"The number of files is :"<<n<<endl;

    for (int i = 0; i < n; i++)
    {
        //cout<<vtuFile[i]<<endl;
        readCdnt(vtuFile[i]);
    }

    cout<<"File num is:"<<fileCounter<<endl;






	return 0;
}



