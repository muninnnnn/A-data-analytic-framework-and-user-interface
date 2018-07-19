
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

float x_max,x_min,y_max,y_min;

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



void compare_size(string file)
{

    string line;
	ifstream infile;

	ofstream outfile;

	stringstream numTrans;
	int numOfPoints;
	int num_flag = 0;


	infile.open(file.data());
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

        }


        if (line.find(s2) != string::npos)
            break;

    }

    while (getline(infile,line))
    {
      	float first_num, second_num, third_num;
		sscanf(line.c_str(), "%f %f %f", &first_num, &second_num, &third_num);

		if (num_flag == 0)
		{
		    x_max = x_min =first_num;
		    y_max = y_min =second_num;

		}

		x_max = max(first_num, x_max);
		x_min = min(first_num, x_min);
		y_max = max(second_num, y_max);
		y_min = min(second_num, y_min);
		vec_num.push_back(first_num);
		num_flag++;

		if(num_flag >= numOfPoints)
            break;

	}
}


void readCdnt(string file)
{
    cout<<"The file name is: "<<file<<endl;
    string line;
	ifstream infile;

	ofstream outfile;

	stringstream numTrans;
	int numOfPoints;
	int num_flag = 0;


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

            //cout<<"location1 is: "<<location1<<endl;
            //cout<<"location2 is: "<<location2<<endl;
            cout<<"Number of Points is: "<<numOfPoints<<endl;


        }


        if (line.find(s2) != string::npos)
            break;

    }

    //cout<<endl;

    //float cdnt[numOfPoints][2];

    vector<vector<float> >cdnt;

    for(int i = 0; i < numOfPoints; i++)
    {
        cdnt.push_back(vector<float>(2));
    }

    while (getline(infile,line))
    {
      	float first_num, second_num, third_num;
		sscanf(line.c_str(), "%f %f %f", &first_num, &second_num, &third_num);
		//cout << first_num << " " << second_num << " " << third_num << endl;
		cdnt[num_flag][0] = first_num;
		cdnt[num_flag][1] = second_num;
		vec_num.push_back(first_num);
		num_flag++;

		if(num_flag >= numOfPoints)
            break;

	}


    outfile << "The file name is: "<<file<<endl<<"Number of Points is: "<<numOfPoints<<endl;

	for (int i = 0; i < numOfPoints; i++)
	{
	    outfile << cdnt[i][0] << " " << cdnt[i][1] << endl;
		//cout << cdnt[i][0] << " " << cdnt[i][1] << endl;
	}
	outfile << endl;

	 float x_width = x_max - x_min;
     float y_width = y_max - y_min;

     float size_wormworld = max(x_width, y_width);

     //cout<<"The width of x is "<<x_width<<endl;
     //cout<<"The width of y is "<<y_width<<endl;

     //cout<<"The size of wormworld is "<<size_wormworld<<endl;

     float x_centre = x_min + size_wormworld/2;
     float y_centre = y_min + size_wormworld/2;

     //cout<<"The centre of x is "<<x_centre<<endl;
     //cout<<"THe centre of y is "<<y_centre<<endl;

    float size_scale = 480/size_wormworld;

    int x_move = 250-(size_scale * x_centre);
    int y_move = 250-(size_scale * y_centre);

    //cout<<"The move in x is :"<<x_move<<endl;
    //cout<<"The move in y is :"<<y_move<<endl;

    vector<int>pixel_coordinate_x(numOfPoints);
    vector<int>pixel_coordinate_y(numOfPoints);

    for(int i = 0; i < numOfPoints; i++)
    {
        pixel_coordinate_x[i] = cdnt[i][0] * size_scale + x_move;
        pixel_coordinate_y[i] = cdnt[i][0] * size_scale + y_move;

        cout<<"The x is:"<<pixel_coordinate_x[i]<<endl;
        cout<<"The y is:"<<pixel_coordinate_y[i]<<endl;
        cout<<endl;
    }



	infile.close();

	outfile.close();
}




int main()
{

    string file="/home/csunix/sc17dh/Project/example_meshpoints_10/worm.pvd";
    readPvd(file);

    vector<string> vtuFile(fileCounter);

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
        compare_size(vtuFile[i]);
    }

    for (int i = 0; i < n; i++)
    {
        //cout<<vtuFile[i]<<endl;
        readCdnt(vtuFile[i]);
    }

    //cout<<"File num is:"<<fileCounter<<endl;







	return 0;
}


