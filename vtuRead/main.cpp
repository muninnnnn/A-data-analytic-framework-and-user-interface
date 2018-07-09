#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>


using namespace std;

void readTxt(string file)
{
    ifstream infile;
    infile.open(file.data());
    assert(infile.is_open());

    string s;
    string location("<DataArray type="Float32" NumberOfComponents="3" format="ascii">");

    while(getline(infile,s))
    {


        while (!s.compare(location))
        cout<<s<<endl;
    }
    infile.close();
}


int main()
{

    readTxt("/home/csunix/sc17dh/Project/example_meshpoints_10/worm_000000.vtu");


















    return 0;
}
