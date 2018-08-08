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
#include <stdlib.h>


using namespace std;

int fileCounter = 0;
int global_num = 0;
int numOfPoints = 0;
int init_flag = 0;


vector<string> vtuFile;


vector<float> vtuTime;

vector<vector<float> > cdnt;

vector<float> mid_curve;
int curve_flag = 0;

float x_max, x_min, y_max, y_min;

float speed;
float frequency;



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
    string filename;
    float time_step;

    infile.open(file.data());

    if (!infile.is_open())
    {
        cout << "Error,no such file\n";
    }

    string s1 = "file=\"";
    string s2 = "DataSet timestep";
    string s3 = "\"";


    while(getline(infile,line))
    {
        if (line.find(s1) != string::npos)
        {
            int location_name = line.find(s1);
            location_name = location_name + 6;
            filename="/home/csunix/sc17dh/Project/sampeVTUs/"+line.substr(location_name,15);
            //filename= "/home/csunix/sc17dh/Project/example_meshpoints_10/"+line.substr(location_name,15);
            vtuFile.push_back(filename);
            fileCounter++;
        }        
        if (line.find(s2) != string::npos)
       {
                   int location1 = line.find(s2) + 18;
                   int location2 = line.find(s3,location1);
                   int location_time = location2-location1;

                   string stime = line.substr(location1,location_time);

                   time_step = atof(stime.c_str());
                   vtuTime.push_back(time_step);
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

void readCurve(string file)
{
    string line;
    ifstream infile;
    int file_flag = 0;
    int curve_flag = 0;
    float temp_curve;
    vector<float> curve;


    infile.open(file.data());

    if (!infile.is_open())
    {
        cout << "Error,no such file\n";
    }

    string s = "<DataArray type=\"Float32\" Name=\"P\" NumberOfComponents=\"1\">";

    while(getline(infile,line))
    {
        if (line.find(s) != string::npos)
            break;
    }

    getline(infile,line);
    stringstream curve_read(line);

              while(curve_read >> temp_curve)
            {
                curve.push_back(temp_curve);
            }
              int midpoint = (numOfPoints+1) / 2;
              mid_curve.push_back(curve[midpoint]);
}

float calculateFrequency()
{
    vector<float> temp_curve;
    vector<float> tempnext_curve;
    for(int i = 0; i < fileCounter-1; i++)
    {
        temp_curve.push_back(mid_curve[i]);
    }
    for(int i = 1; i < fileCounter; i++)
    {
        tempnext_curve.push_back(mid_curve[i]);
    }

    vector<int> ind;

    for (int i = 0; i < fileCounter -1; i++)
    {
        if((temp_curve[i]*tempnext_curve[i] < 0.0)&&(tempnext_curve[i] > 0.0))
            ind.push_back(i);
    }

    vector<float> time_period;

    for(int i = 0; i < ind.size();i++)
    {
        time_period.push_back(vtuTime[ind[i]]);
    }

    vector<float> time_diff;
    for(int i = 0; i < time_period.size()-1; i++)
    {
        float diff_temp = time_period[i+1] = time_period[i];
        time_diff.push_back(diff_temp);
    }

    int average_point = time_diff.size();
    float sum = 0.0;
    for(int i = 0; i < average_point; i++)
    {
        sum += time_diff[i];
    }

    float period = sum/average_point;


    if(period == 0)
    {
        return 0.0;
    }
        else
    {
        return 1/period;
    }



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
    string frequencyTxt = "The frequency is :";
    ostringstream ss_1;
    ostringstream ss_2;
    ss_1<<speedTxt<<speed;
    ss_2<<frequencyTxt<<frequency;
    QString bmpSpeedTxt = QString::fromStdString(ss_1.str()) ;
    QString bmpFrequencyTxt = QString::fromStdString(ss_2.str());
    p.drawText(300,50,bmpSpeedTxt);
    p.drawText(300,70,bmpFrequencyTxt);

    //p.begin(&pix);

    for (int i = 0; i < numOfPoints - 1; i++ )
    {
       p.drawLine(pixel_cdnt[i][0],pixel_cdnt[i][1],pixel_cdnt[i+1][0],pixel_cdnt[i+1][1]);
    }

    string bmpName = "Image";
    string bmpFormat = ".bmp";
    ostringstream ss_name;

    ss_name<<bmpName<<global_num+1<<bmpFormat;
    global_num++;

    QString qbmpName=QString::fromStdString(ss_name.str());
    pix.save(qbmpName);
    p.end();

    cout<<"Finish no."<<global_num-1<<endl;

}


int main(int argc, char *argv[])
{

    string file="/home/csunix/sc17dh/Project/sampeVTUs/worm.pvd";
    //string file="/home/csunix/sc17dh/Project/example_meshpoints_10/worm.pvd";
    readPvd(file);

    QApplication a(argc, argv);

           for (int i = 0; i < fileCounter; i++) //Find max/min, read numOfPoints
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

      for (int i = 0; i < fileCounter; i++) //Compute Speed to create bitmap
      {

          readCurve(vtuFile[i]);
          readCDNT(vtuFile[i]);

          if(i == 0)
          {
              calculateCoM(cdnt);
              CoM1 = CoM;
          }

          if(i == fileCounter-1)
          {
              calculateCoM(cdnt);
              CoM2 = CoM;
          }

          speed = calculateSpeed(CoM1,CoM2);

      }
          frequency = calculateFrequency();



      for (int i = 0; i < fileCounter; i++)             //Create the Bmp
      {
          //readCurve(vtuFile[i]);

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
