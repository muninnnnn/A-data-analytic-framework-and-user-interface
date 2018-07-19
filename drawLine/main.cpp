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

    vector<vector<float> >cdnt;

        for (int i = 0; i < numOfPoints; i++)
        {
            cdnt.push_back(vector<float>(2));
        }

    while (getline(infile,line))
    {
        //cout<<line<<endl;

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


    vector<int>px(numOfPoints);
    vector<int>py(numOfPoints);


    for (int i = 0; i < numOfPoints; i++)
    {

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

        //cout<<"px is"<<px[i]<<endl;
        //cout<<"py is"<<py[i]<<endl;
    }

    int x_length = x_max - x_min;
    int y_length = y_max - y_min;
    sizeOfBmp = max (x_length,y_length);
/*
    cout<<"The size of BMP is "<<sizeOfBmp<<endl;
    cout<<"x_max is "<<x_max<<endl;
    cout<<"x_min is "<<x_min<<endl;
    cout<<"y_max is "<<y_max<<endl;
    cout<<"y_min is "<<y_min<<endl;
*/

}



void createBmp(string file)
{
    cout<<"The file name is: "<<file<<endl;
    string line;
    ifstream infile;
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

    vector<vector<float> >cdnt;

        for (int i = 0; i < numOfPoints; i++)
        {
            cdnt.push_back(vector<float>(2));
        }

    while (getline(infile,line))
    {
        //cout<<line<<endl;

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


    vector<int>px(numOfPoints);
    vector<int>py(numOfPoints);


    for (int i = 0; i < 10; i++)
    {
        //outfile << cdnt[i][0] << " " << cdnt[i][1] << endl;
        px[i] = cdnt[i][0] * sizeOfBmp;
        py[i] = sizeOfBmp - cdnt[i][1] * sizeOfBmp;

       // cout << cdnt[i][0] << " " << cdnt[i][1] << endl;
    }


    //outfile << endl;
    //cout<<endl;

    QLabel l;
    QPicture pi;
   // QPainter p(&pi);
    QPixmap pix(sizeOfBmp+200, sizeOfBmp+200);
    QPainter p(&pix);

    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
    p.translate(200,-150);
    pix.fill(Qt::white);

    //p.begin(&pix);

    for (int i = 0; i < 9; i++ )
    {
       p.drawLine(px[i],py[i],px[i+1],py[i+1]);
    }

    string bmpName = "wormNo.";
    string bmpFormat = ".bmp";
    ostringstream ss;

    ss<<bmpName<<global_num+1<<bmpFormat;
    global_num++;

    QString qbmpName=QString::fromStdString(ss.str());
    pix.save(qbmpName);
    p.end();

    infile.close();

    //outfile.close();

}


int main(int argc, char *argv[])
{

    string file="/home/csunix/sc17dh/Project/example_meshpoints_10/worm.pvd";
    readPvd(file);

    QApplication a(argc, argv);

    vector<string> vtuFile(fileCounter);
    string line;
    string filename;
    ifstream infile;
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


           for (int i = 0; i < fileCounter; i++)
               {
                   //cout<<vtuFile[i]<<endl;
                  compareSize(vtuFile[i]);
                  //createBmp(vtuFile[i]);
               }


           for (int i = 0; i < fileCounter; i++)
               {
                   //cout<<vtuFile[i]<<endl;
                   createBmp(vtuFile[i]);
               }






   return a.exec();
}
