#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QPicture>
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QPixmapCache>


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
int global_num = 0;

int sizeOfBmp = 0;

int x_max = 0;
int x_min = 0;
int y_max = 0;
int y_min = 0;



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



void compareSize(string file)
{
        string line;
        ifstream infile;
        stringstream numTrans;
        int numOfPoints;
        int nn = 0;

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

        cout<<endl;

        float cdnt[numOfPoints][2];

        //vector<vector<float> >cdnt(fileCounter,vector<float>(2,0));

        while (getline(infile,line))
        {
            float first_num, second_num, third_num;
            sscanf(line.c_str(), "%f %f %f", &first_num, &second_num, &third_num);
            //cout << first_num << " " << second_num << " " << third_num << endl;
            cdnt[nn][0] = first_num;
            cdnt[nn][1] = second_num;
            vec_num.push_back(first_num);
            nn++;

            if(nn >= numOfPoints)
                break;
        }

        int px[numOfPoints];
        int py[numOfPoints];

        for (int i = 0; i < numOfPoints; i++)
        {
            //outfile << cdnt[i][0] << " " << cdnt[i][1] << endl;
            px[i] = cdnt[i][0] * 500;
            py[i] = cdnt[i][1] * 500;

            if(px[i] > x_max)
                        {
                            x_max = px[i];
                        }
            if(px[i] < x_min)
                        {
                            x_min = px[i];
                        }
            if(py[i] > y_max)
                        {
                            y_max = py[i];
                        }
            if(py[i] < y_min)
                        {
                            y_min = py[i];
                        }

            // cout << cdnt[i][0] << " " << cdnt[i][1] << endl;
        }

        int x_length = x_max - x_min;
        int y_length = y_max - y_min;
        sizeOfBmp = max(x_length,y_length);
/*
        cout<<"Size of bmp is"<<sizeOfBmp<<endl;

        cout<<"The max x is :"<<x_max<<endl;
        cout<<"The min x is :"<<x_min<<endl;
        cout<<"The max y is :"<<y_max<<endl;
        cout<<"The min y is :"<<x_min<<endl;
*/
}

void createBmp(string file)
{
    cout<<"The file name is: "<<file<<endl;
    string line;
  