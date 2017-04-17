#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <iomanip>

#define LENGTH 9 //0-8
#define WIDTH 12   //0-11

using namespace std;

void initarr ( int a[][WIDTH]);
void printarr (const int a[][WIDTH]);

vector<vector<pair<int,int> > > loaddata (bool build, int plus[][WIDTH],int neg[][WIDTH], double totals[],const string &arg1);

void printloaded (const vector<vector<pair<int,int> > > &loaded);

void classify(vector<vector<pair<int,int> > > loaded, const int plus[][WIDTH],const int neg[][WIDTH], const double totals[3]);


int main(int argc,  char **argv)
{
  int plus [LENGTH][WIDTH];
  int neg [LENGTH][WIDTH];
  double totals[3]; //plus, negative, & overall totals in corresponding order

  initarr(plus);
  initarr(neg);

  //load train data and build classifier
  vector<vector<pair<int,int> > > trainloaded = loaddata(true, plus,neg,totals,argv[1]);

  //load test data
  vector<vector<pair<int,int> > > testloaded = loaddata(false, plus,neg,totals,argv[2]);

  //cout << "TRAIN DATA \n";
  //printloaded(trainloaded);
  //cout << "TEST DATA \n";
  //printloaded(testloaded);

  //cout << "positive class \n";
  //printarr(plus);

  //cout << "negative class \n";
  //printarr(neg);

  classify(trainloaded,plus,neg,totals);
  classify(testloaded,plus,neg,totals);
}

void classify(vector<vector<pair<int,int> > > loaded, const int plus[][WIDTH],const int neg[][WIDTH], const double totals[3])
{
  long double nprob=1;
  long double pprob=1;

  int truepos=0;
  int trueneg=0;
  int falsepos=0;
  int falseneg=0;

  int clss=0;
  int attribs= 0;

  //cout << setprecision(10) << std::fixed;

  for (vector<pair<int,int> > v: loaded)
  {
    clss = v.back().first;
    attribs = v.back().second;
    v.pop_back();
    for (pair<int,int> p: v)
    {
        pprob *= (plus[p.first][p.second]/totals[0]);
        //cout << "pProb" << pprob << '\n';
        nprob *= (neg[p.first][p.second])/totals[1];
        //cout << "nProb" << nprob << '\n';
    } // end inner loop

    pprob *= (totals[0]/totals[2]);
    //cout << "pProb" << pprob << '\n';
    nprob *= (totals[1]/totals[2]);
    //cout << "nProb" << nprob << '\n';

    if (pprob > nprob && clss == 1)
      truepos++;
    if (pprob > nprob && clss == -1)
      falsepos++;
    if (nprob > pprob && clss == 1)
      falseneg++;
    if (nprob > pprob && clss == -1)
      trueneg++;

      pprob=1;
      nprob=1;

  } // end outer loop

  cout << truepos << " " << falseneg << " " << falsepos  << " " << trueneg << '\n';


}

vector<vector<pair<int,int> > > loaddata (bool build, int plus[][WIDTH],int neg[][WIDTH], double totals[],const string &arg1)
{
  std::ifstream infile(arg1);
  vector< vector< pair<int,int> > > full;
  vector< pair<int,int> > temp;

  int clss=0;
  int index=0;
  int value=0;
  int numattrib=0;

  while (!infile.eof())
  {

    if (infile.peek() != '\n' && !infile.eof())
    {
      infile >> clss;
      //cout << "\n Class: "<< clss << " ";

    }

    int zero [] = {0,0,0,0,0,0,0,0,0};

    while(1)
    {
      if (infile.peek() == '\n' || infile.eof())
      {
        infile.get(); break;
      }

      infile >> index;
      //cout << " " <<index;
      infile.ignore();
      infile >> value;
      //cout << ":" << value;

      zero[index-1] = value;
      numattrib++;

      if (build == true)
      {
      switch (clss)
        {
          case 1: plus[index-1][value]++; totals[0]++; totals[2]++; break;
          case -1: neg[index-1][value]++; totals[1]++; totals[2]++; break;
          default: break;
        }
      }

        temp.push_back(make_pair(index-1,value));
    }



    if (!temp.empty())
    {

      for (int a =0; a < LENGTH; a++)
      {
        if (zero[a] == 0)
          {
            if (clss ==1)
              plus[a][0]++;
            if (clss==-1)
              neg[a][0]++;

              temp.push_back(make_pair(a,0));
          }
      }
        temp.push_back(make_pair(clss,numattrib));
        full.push_back(temp);
        temp.clear();
    }
        numattrib =0;
  }

  //cout << "\n TOTAL: " << totals[2];
  //cout << "\n P Total: " << totals[0];
  //cout << "\n N Total: " << totals[1];
  //cout << "\n";

  return full;
}

void initarr (int a[][WIDTH])
{
  for (int i =0; i < LENGTH; i++)
    for (int j =0; j < WIDTH; j++)
      {
        a[i][j] = 1;
      }
}

void printarr (const int a[][WIDTH])
{
  for (int i =0; i < LENGTH; i++)
  {
    for (int j =0; j < WIDTH; j++)
      {
        cout << a[i][j]<< " ";
      }
      cout << '\n';
    }
}

void printloaded (const vector<vector<pair<int,int> > > &loaded)
{
  for (vector<pair<int,int> > v: loaded)
  {
    cout << "Class: " << v.back().first;
    for (pair<int,int> p: v)
    {
      cout << " ";
      cout << p.first;
      cout << ":";
      cout <<p.second;
      cout << " ";
    }
    cout << '\n';
  }
}
