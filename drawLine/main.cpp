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
int init_flag = 0;


float x_max, x_min, y_max, y_min;



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

void findMax(string file)
{
    string line;
    ifstream infile;
    stringstream numTrans;
    int numOfPoints;
    int num_flag = 0;


    infile.open(file.data());


    if (!infile.is_open())
    {
        cout << "Error,no such file\n";    }

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

                if(init_flag == 0)
                {

                    x_max = x_min = first_num;
                    y_max = y_min = second_num;

                    init_flag++;
                }

                x_max = max(first_num, x_max);
                x_min = min(first_num, x_min);
                y_max = max(second_num, y_max);
                y_min = min(second_num, y_min);

                num_flag++;

                if(num_flag >= numOfPoints)
                    break;

            }
}


void createBmp_1(string file)
{
    cout<<"The file name is: "<< file<<endl;
    string line;
    ifstream infile;
    stringstream numTrans;
    int numOfPoints;
    int num_flag = 0;
    int init_flag1 = 0;

    float x_max1,x_min1,y_max1,y_min1;

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
        //vec_num.push_back(first_num);

        if(init_flag1 == 0)
        {
            x_max1 = x_min1 = first_num;
            y_max1 = y_min1 = second_num;
            init_flag1++;
        }

            x_max1 = max(first_num, x_max1);
            x_min1 = min(first_num, x_min1);
            y_max1 = max(second_num, y_max1);
            y_min1 = min(second_num, y_min1);


        num_flag++;

        if(num_flag >= numOfPoints)
            break;
    }

         float x_width = x_max1 - x_min1;
         float y_width = y_max1 - y_min1;

         float size_wormworld = max(x_width, y_width);
/*
         cout<<"X MAX IS "<<x_max1<<endl;
         cout<<"X MIN IS "<<x_min1<<endl;
         cout<<"Y MAX IS "<<y_max1<<endl;
         cout<<"Y MIN IS "<<y_min1<<endl;


         cout<<"The width of x is "<<x_width<<endl;
         cout<<"The width of y is "<<y_width<<endl;
*/
        // cout<<"The size of wormworld is "<<size_wormworld<<endl;

         float x_centre = (x_min1 + x_max1)/2;
         float y_centre = (y_min1 + y_max1)/2;

//         cout<<"The centre of x is "<<x_centre<<endl;
//         cout<<"THe centre of y is "<<y_centre<<endl;

        float size_scale = 440/size_wormworld;


         //cout<<"Scale size is "<<size_scale<<endl;

        int x_move = 250-(size_scale * x_centre);
        int y_move = 250-(size_scale * y_centre);

       // cout<<"The move in x is :"<<x_move<<endl;
        //cout<<"The move in y is :"<<y_move<<endl;

        vector<int>pixel_coordinate_x(numOfPoints);
        vector<int>pixel_coordinate_y(numOfPoints);

        for(int i = 0; i < numOfPoints; i++)
        {
            pixel_coordinate_x[i] = cdnt[i][0] * size_scale + x_move;
            pixel_coordinate_y[i] = 500 - (cdnt[i][1] * size_scale + y_move);

            //cout<<"The x is:"<<pixel_coordinate_x[i]<<endl;
            //cout<<"The y is:"<<pixel_coordinate_y[i]<<endl;
            //cout<<endl;
        }





    QLabel l;
    QPicture pi;
   // QPainter p(&pi);
    QPixmap pix(500,500);
    QPainter p(&pix);

    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));

    pix.fill(Qt::white);

    //p.begin(&pix);

    for (int i = 0; i < numOfPoints - 1; i++ )
    {
       p.drawLine(pixel_coordinate_x[i],pixel_coordinate_y[i],pixel_coordinate_x[i+1],pixel_coordinate_y[i+1]);
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


void createBmp_2(string file)
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
        //vec_num.push_back(first_num);
        num_flag++;

        if(num_flag >= numOfPoints)
            break;
    }

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

        float size_scale = 440/size_wormworld;

       // cout<<"Scale size is "<<size_scale<<endl;

        int x_move = 250-(size_scale * x_centre);
        int y_move = 250-(size_scale * y_centre);

        //cout<<"The move in x is :"<<x_move<<endl;
        //cout<<"The move in y is :"<<y_move<<endl;

        vector<int>pixel_coordinate_x(numOfPoints);
        vector<int>pixel_coordinate_y(numOfPoints);

        for(int i = 0; i < numOfPoints; i++)
        {
            pixel_coordinate_x[i] = cdnt[i][0] * size_scale + x_move;
            pixel_coordinate_y[i] = 500 - (cdnt[i][1] * size_scale + y_move);

            //cout<<"The x is:"<<pixel_coordinate_x[i]<<endl;
            //cout<<"The y is:"<<pixel_coordinate_y[i]<<endl;
            //cout<<endl;
        }

    QLabel l;
    QPicture pi;
   // QPainter p(&pi);
    QPixmap pix(500,500);
    QPainter p(&pix);

    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));

    pix.fill(Qt::white);

    //p.begin(&pix);

    for (int i = 0; i < numOfPoints - 1; i++ )
    {
       p.drawLine(pixel_coordinate_x[i],pixel_coordinate_y[i],pixel_coordinate_x[i+1],pixel_coordinate_y[i+1]);
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
                   //cout<<vtuFile[n]<<endl;
                   n++;

               }
           }

        //  createBmp_1(vtuFile[950]);




           for (int i = 0; i < fileCounter; i++)
               {
                   createBmp_1(vtuFile[i]);
               }
/*         for (int i = 0; i < fileCounter; i++)
               {
                  findMax(vtuFile[i]);
               }


           for (int i = 0; i < fileCounter; i++)
               {
                   createBmp_2(vtuFile[i]);
               }
*/


   return a.exec();
}
