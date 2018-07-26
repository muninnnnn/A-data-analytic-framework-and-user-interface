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
float speed;

vector<vector<float> >cdnt;

pair<int,int>pixel_coordinate;
pair<float,float> CoM;

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

void readCDNT(string file)
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


void calculateCoordinate(float wx, float wy, float xmax, float xmin, float ymax, float ymin, int res, int margin)
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

void calculateCoM(vector<vector <float> >wormworld_cdnt)
{
    float x_sum = 0.0;
    float y_sum = 0.0;

    for (int i = 0; i < numOfPoints;i++)
    {
        x_sum+=wormworld_cdnt[i][0];
        y_sum+=wormworld_cdnt[i][1];
    }

    CoM = make_pair(x_sum/numOfPoints,y_sum/numOfPoints);

}


float calculateDistance(float x1, float y1, float x2, float y2)
{
    return sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
}

float calculateSpeed(pair<float,float>CoM1,pair<float,float>CoM2,float T1 = 0.0, float T2 = 5.0)
{
    float distance;
    if(CoM1.first > CoM2.first)
    {
        distance = 0 - calculateDistance(CoM1.first, CoM1.second, CoM2.first, CoM2.second);
    }
    else
    {
        distance = calculateDistance(CoM1.first, CoM1.second, CoM2.first, CoM2.second);
    }

    return distance/(T2-T1);

}


void createBMP(vector<vector<int> >pixel_cdnt, int res)
{
    QLabel l;
    QPicture pi;
   // QPainter p(&pi);
    QPixmap pix(res,res);
    QPainter p(&pix);
    QFont font = p.font();

    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
    font.setPixelSize(18);
    p.setFont(font);

    pix.fill(Qt::white);

    string speedTxt = "The speed is :";
    ostringstream ss_1;
    ss_1<<speedTxt<<speed;
    QString bmpSpeedTxt = QString::fromStdString(ss_1.str()) ;

    p.drawText(300,50,bmpSpeedTxt);

    //p.begin(&pix);

    for (int i = 0; i < numOfPoints - 1; i++ )
    {
       p.drawLine(pixel_cdnt[i][0],pixel_cdnt[i][1],pixel_cdnt[i+1][0],pixel_cdnt[i+1][1]);
    }

    string bmpName = "Image";
    string bmpFormat = ".bmp";
    ostringstream ss_2;

    ss_2<<bmpName<<global_num+1<<bmpFormat;
    global_num++;

    QString qbmpName=QString::fromStdString(ss_2.str());
    pix.save(qbmpName);
    p.end();

    cout<<"Finish no."<<global_num-1<<endl;

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

           for (int i = 0; i < n; i++) //Find max/min, read numOfPoints
           {
               findMax(vtuFile[i]);
           }

           for (int i = 0; i < numOfPoints; i++)
           {
               cdnt.push_back(vector<float>(2));
           }



      float wx,wy;

      vector<vector <int> > bmp_point;
      for(int i = 0; i < numOfPoints; i++)
      {
          bmp_point.push_back(vector<int>(2));
      }


     pair<float,float> CoM1;
     pair<float,float> CoM2;

      for (int i = 0; i < n; i++) //Compute Speed to create bitmap
      {
          readCDNT(vtuFile[i]);

          if(i == 0)
          {
              calculateCoM(cdnt);
              CoM1 = CoM;
          }

          if(i == n-1)
          {
              calculateCoM(cdnt);
              CoM2 = CoM;
          }

          speed = calculateSpeed(CoM1,CoM2);

      }


      for (int i = 0; i < n; i++)             //Create the Bmp
      {
          readCDNT(vtuFile[i]);
          for (int j = 0; j < numOfPoints; j++)
          {
              wx = cdnt[j][0];
              wy = cdnt[j][1];

             calculateCoordinate(wx,wy,x_max,x_min,y_max,y_min,500,20);
             bmp_point[j][0] = pixel_coordinate.first;
             bmp_point[j][1] = pixel_coordinate.second;
          }
          createBMP(bmp_point,500);
      }




return 0;

   return a.exec();
}
