#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QPicture>
#include <QPainter>
#include <QDebug>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <stdio.h>
#include <cmath>

using namespace std;
vector<float> vec_num;

int main(int argc, char *argv[])
{
/*
   string filename, line;
   ifstream file;
       filename = "/home/csunix/sc17dh/Project/example_meshpoints_10/worm_000080.vtu";

       float origin[50][2];
       float cdnt[10][2];
       int n = 0;
       int m;

       file.open(filename);

       if (!file.is_open())
       {
           cout << "Error,no such file\n";
       }

       while (getline(file, line))
       {

           float first_num, second_num, third_num;
           sscanf(line.c_str(), "%f %f %d", &first_num, &second_num, &third_num);
           //cout << first_num << " " << second_num << " " << third_num << endl;
           origin[n][0] = first_num;
           origin[n][1] = second_num;
           vec_num.push_back(first_num);
           n++;
       }


       m = 19;
       for (int i = 0; i < 10; i++)
       {
           cdnt[i][0] = origin[m + i][0];
           cdnt[i][1] = origin[m + i][1];
       }
       for (int i = 0; i < 10; i++)
       {
           qDebug()<<cdnt[i][0]<< " "<<cdnt[i][1];
       }
*/
    string filename, line;
        ifstream file;
        filename = "/home/csunix/sc17dh/Project/sampeVTUs/worm_000010.vtu";

        float origin[500][2];
        float cdnt[128][2];
        int n = 0;
        int m;

        file.open(filename.data());

        if (!file.is_open())
        {
            cout << "Error,no such file\n";
        }

        while (getline(file, line))
        {

            float first_num, second_num, third_num;
            sscanf(line.c_str(), "%f %f %d", &first_num, &second_num, &third_num);//格式化提取
            //cout << first_num << " " << second_num << " " << third_num << endl;
            origin[n][0] = first_num;
            origin[n][1] = second_num;
            vec_num.push_back(first_num);
            n++;
        }

        //cout<< n <<endl;
        m = 139;
        for (int i = 0; i < 128; i++)
        {
            cdnt[i][0] = origin[m + i][0];
            cdnt[i][1] = origin[m + i][1];
        }
        for (int i = 0; i < 128; i++)
        {
             qDebug()<<cdnt[i][0]<< " "<<cdnt[i][1];
        }


       int px[128];
       int py[128];
       for (int i = 0; i < 128; i++)
       {
           px[i] = cdnt[i][0] * 800;
           py[i] = cdnt[i][1] * 8000;
       }


       QApplication a(argc, argv);
       QLabel l;
       QPicture pi;
       QPainter p(&pi);

       //MainWindow w;

       p.setRenderHint(QPainter::Antialiasing);
       p.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));

       for (int i = 0; i < 127; i++ )
       {
          p.drawLine(px[i],py[i],px[i+1],py[i+1]);

       }
       p.end();


       l.setPicture(pi);
       //w.show();
       l.show();












   return a.exec();
}
