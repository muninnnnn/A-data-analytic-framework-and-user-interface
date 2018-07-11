#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QPicture>
#include <QPainter>
#include <QDebug>
#include <QPixmap>



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
float cdnt[10][2];

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


void readCdnt(string file)
{
    /* 128-points
    string line;
    ifstream infile;
    float cdnt[128][2];
    int n = 0;
    infile.open(file.data());
    if (!infile.is_open())
    {
        cout << "Error,no such file\n";
    }
    string s1 = "<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">";
    while(getline(infile,line))
    {
        int location = line.find(s1);
        //cout<< location<<endl;
        if (location > 0)
            break;
    }
    while (getline(infile,line))
    {
        //cout<<line<<endl;
        float first_num, second_num, third_num;
        sscanf(line.c_str(), "%f %f %d", &first_num, &second_num, &third_num);
        //cout << first_num << " " << second_num << " " << third_num << endl;
        cdnt[n][0] = first_num;
        cdnt[n][1] = second_num;
        vec_num.push_back(first_num);
        n++;
        if(n >= 128)
            break;
    }
    for (int i = 0; i < 128; i++)
    {
        cout << cdnt[i][0] << " " << cdnt[i][1] << endl;
    }
    */

    // 10-points
    string line;
    ifstream infile;
    //float cdnt[10][2];
    int n = 0;

    infile.open(file.data());

    if (!infile.is_open())
    {
        cout << "Error,no such file\n";
    }

    string s1 = "<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">";

    while(getline(infile,line))
    {
        int location = line.find(s1);
        //cout<< location<<endl;
        if (location > 0)
            break;

    }
    while (getline(infile,line))
    {
        //cout<<line<<endl;

        float first_num, second_num, third_num;
        sscanf(line.c_str(), "%f %f %d", &first_num, &second_num, &third_num);
        //cout << first_num << " " << second_num << " " << third_num << endl;
        cdnt[n][0] = first_num;
        cdnt[n][1] = second_num;
        vec_num.push_back(first_num);
        n++;

        if(n >= 10)
            break;
    }

    for (int i = 0; i < 10; i++)
    {
        qDebug() << cdnt[i][0] << " " << cdnt[i][1];
    }

}


void createBmp()
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


    pix.save("a.bmp");
    p.end();
    // l.setPicture(pi);
    // l.show();
}


int main(int argc, char *argv[])
{

       readCdnt("/home/csunix/sc17dh/Project/example_meshpoints_10/worm_000080.vtu");

       QApplication a(argc, argv);
       createBmp();

   return a.exec();
}
