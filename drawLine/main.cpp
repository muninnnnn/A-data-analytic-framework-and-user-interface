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

int fileCounter = 0;
int global_num = 0;
int numOfPoints = 0;
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
            fileCounter++;
        }
    }
    infile.close();
}

void findMax(string file)
{
    string line;
    ifstream infile;
    stringstream numTrans;
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

        if((line.find(s1) != string::npos) && (init_flag ==0))
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

void readCDNT(string file, vector<vector<float> >cdnt)
{
    //cout<<"The file name is: "<<file<<endl;
    string line;
    ifstream infile;
    stringstream numTrans;
    int num_flag = 0;

    infile.open(file.data());

    if (!infile.is_open())
    {
        cout << "Error,no such file\n";
    }

    string s = "<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">";

    while(getline(infile,line))
    {
        if (line.find(s) != string::npos)
            break;
    }

    while (getline(infile,line))
    {
        float first_num, second_num, third_num;
        sscanf(line.c_str(), "%f %f %f", &first_num, &second_num, &third_num);
        //cout << first_num << " " << second_num << " " << third_num << endl;
        cdnt[num_flag][0] = first_num;
        cdnt[num_flag][1] = second_num;
        num_flag++;

        if(num_flag >= numOfPoints)
            break;
    }
/*
    for(int i = 0; i < numOfPoints; i++)
    {
        cout<<cdnt[i][0]<<"  "<<cdnt[i][1]<<endl;
    }
*/
}


void calculateCoordinate(pair<int,int>pixel_coordinate, float wx, float wy, float xmax, float xmin, float ymax, float ymin, int res, int margin)
{
    float x_width = xmax - xmin;
    float y_width = ymax - ymin;
    float size_wormworld = max(x_width, y_width);
    float x_centre = xmin + size_wormworld/2;
    float y_centre = ymin + size_wormworld/2;
    float size_scale = (res-2*margin)/size_wormworld;
    int x_move = res/2 - (size_scale * x_centre);
    int y_move = res/2 - (size_scale * y_centre);

    pixel_coordinate.first = wx * size_scale + x_move;
    pixel_coordinate.second = res - (wy * size_scale + y_move);

}

void createBMP(double a, double b, double c, double d)
{
    QLabel l;
    QPicture pi;
   // QPainter p(&pi);
    QPixmap pix(500,500);
    QPainter p(&pix);
    QFont font = p.font();

    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
    font.setPixelSize(18);
    p.setFont(font);

    pix.fill(Qt::white);

    p.drawText(350,50,"Hello World!");

    //p.begin(&pix);

    for (int i = 0; i < numOfPoints - 1; i++ )
    {
       p.drawLine(a,b,c,d);
    }

    string bmpName = "Image";
    string bmpFormat = ".bmp";
    ostringstream ss;

    ss<<bmpName<<global_num+1<<bmpFormat;
    global_num++;

    QString qbmpName=QString::fromStdString(ss.str());
    pix.save(qbmpName);
    p.end();

}


int main(int argc, char *argv[])
{

    string file="/home/csunix/sc17dh/Project/sampeVTUs/worm.pvd";
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
                   vtuFile[n] = "/home/csunix/sc17dh/Project/sampeVTUs/"+line.substr(location,15);
                   //cout<<vtuFile[n]<<endl;
                   n++;

               }
           }





           for (int i = 0; i < n; i++)
           {
               findMax(vtuFile[i]);
           }
       //cout<<"NUM OF POINT IS "<<numOfPoints<<endl;

      vector<vector<float> >cdnts;
      pair<int,int>p_coordinate;
      float wx,wy;

      for (int i = 0; i < numOfPoints; i++)
      {
          cdnts.push_back(vector<float>(2));
      }

      for (int i = 0; i < n; i++)
      {
          readCDNT(vtuFile[i], cdnts);
          cout<<cdnts[0][0]<<endl;

/*
          for (int j = 0; j < numOfPoints; j++)
          {
              //wx = cdnt[j][0];
              //wy = cdnt[j][1];
              //cout<<cdnt[j][0]<<"  "<<cdnt[j][1];
             calculateCoordinate(p_coordinate,0.2,0.8,x_max,x_min,y_max,y_min,500,20);
             cout<<p_coordinate.first<<"     "<<p_coordinate.second<<endl;

          }

          cout<<endl;
      }
*/
return 0;

   return a.exec();
}
