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

       int px[10];
       int py[10];
       for (int i = 0; i < 10; i++)
       {
           px[i] = cdnt[i][0] * 300;
           py[i] = cdnt[i][1] * 300;
       }
       for (int i = 0; i < 10; i++)
       {
           qDebug()<<px[i]<< " "<<py[i];
       }


       QApplication a(argc, argv);
       QLabel l;
       QPicture pi;
       QPainter p(&pi);

       //MainWindow w;

       p.setRenderHint(QPainter::Antialiasing);
       p.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));

       for (int i = 0; i < 9; i++ )
       {
          p.drawLine(px[i],py[i],px[i+1],py[i+1]);

       }
       p.end();


       l.setPicture(pi);
       //w.show();
       l.show();












   return a.exec();
}
