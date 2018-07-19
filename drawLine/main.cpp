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
    ifstream infile;

    ofstream outfile;

    stringstream numTrans;
    int numOfPoints;
    int nn = 0;

    infile.open(file.data());

    //outfile.open("/home/csunix/sc17dh/Project/saveCdnt.txt",ios::app);

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


        if(nn >= numOfPoints)
            break;
    }

    //outfile << "The file name is: "<<file<<endl<<"Number of Points is: "<<numOfPoints<<endl;


    int px[numOfPoints];
    int py[numOfPoints];


    for (int i = 0; i < numOfPoints; i++)
    {
        //outfile << cdnt[i][0] << " " << cdnt[i][1] << endl;
        px[i] = cdnt[i][0] * sizeOfBmp;
        py[i] = sizeOfBmp-(cdnt[i][1] * sizeOfBmp);


    }


    QLabel l;
    QPicture pi;
   // QPainter p(&pi);
    QPixmap pix(sizeOfBmp, sizeOfBmp);
    QPainter p(&pix);

    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));
   // p.translate(100,300);
    pix.fill(Qt::black);

    //p.begin(&pix);

    for (int i = 0; i < numOfPoints -1; i++ )
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

/*
void Bmp(int num)
{

    int px[10];
    int py[10];
    for (int i = 0; i < 10; i++)
    {
        px[i] = cdnt[i][0] * 500;
        py[i] = cdnt[i][1] * 500;
    }


    QLabel l;
    QPicture pi;
   // QPainter p(&pi);
    QPixmap pix(500, 500);
    QPainter p(&pix);

    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));
    //pix.fill(Qt::transparent);

    //p.begin(&pix);

    for (int i = 0; i < 9; i++ )
    {
       p.drawLine(px[i],py[i],px[i+1],py[i+1]);
    }

    string bmpName = "wormNo.";
    string bmpFormat = ".bmp";
    ostringstream ss;

    ss<<bmpName<<num+1<<bmpFormat;

    QString qbmpName=QString::fromStdString(ss.str());
    pix.save(qbmpName);
    p.end();
    // l.setPicture(pi);
    // l.show();
}

*/
int main(int argc, char *argv[])
{

    string file="/home/csunix/sc17dh/Project/example_meshpoints_10/worm.pvd";
    readPvd(file);

    QApplication a(argc, argv);

    string vtuFile[fileCounter];
           //vector<string> vtuFile(fileCounter);
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
