#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TROOT.h>
#include <TMath.h>
#include <TParameter.h>
#include <TGraph.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TSystem.h>
//#include "rootlib.h"//for linux, old file
#include "rootlibx.h"//new file for mac or linux
#include "kbhit.h"
//the follow #include is for count all the beam file
#include <sys/types.h>
#include <dirent.h> 
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <unistd.h> 
#include <vector>

#include <TFrame.h>
#include <stdio.h>
#include <pthread.h>
#include <TH1.h>
#include <TApplication.h>
#include <TTimer.h>
#include <TThread.h>
using namespace std;
///////define the "get the nameId function"
int extractId(string & nameId, string head,string tail)
{
//nameId like beam_1.pd1, get lenghth of all parts
int total=nameId.length();
int tailLength=tail.length();//for .pd1
int headLength=head.length();//for beam_
if(nameId.substr(0,headLength)!=head) return -1;

int dataLength=total-tailLength-headLength;
string num=nameId.substr(headLength,dataLength);
return atoi(num.c_str());
} 
///////end of the define

int main(int argc, char **argv)
{

char *infile;
infile = argv[1];
int fileId;
fileId = atol(argv[2]);
char *pedefile;
pedefile = argv[3];
int pedeId;
pedeId = atol(argv[4]);
// double maxPixelSignal;
// maxPixelSignal = atof(argv[5]);
// double minPixelSignal;
// minPixelSignal = atof(argv[6]);
int ip;
ip = atoi(argv[5]);
char *store_file_name;
store_file_name = argv[6];

int Key_input;
char str[50];
int k=0;

pd1Pede pede;
placData_1 pd1;
char infn[200];
char pedefn[200];
char beamfn[200];
char txtfn[100];

//sprintf(infn,"%s_%d.pd1",infile, fileId);
sprintf(pedefn,"%s/xpede_%d.txt", pedefile, pedeId);

sprintf(beamfn,"%s/", infile);
/////get beamId in to list
vector<string> name_id;

DIR * dir;
struct dirent * ptr;

dir = opendir(beamfn); //打开一个目录

cout << pedefn << endl;
cout << beamfn << "xbeam_*.pd1"<<endl;

    while((ptr = readdir(dir)) != NULL) //循环读取目录数据
    {
    name_id.push_back(ptr->d_name);
    }
closedir(dir);//关闭目录指针

//extract id number
vector<int> idList;
//vector<int> point2name_id;

    for(std::vector<int>::size_type j=0;j<name_id.size();j++)
  {
    //cout << name_id[j] << endl;
    int eId=extractId(name_id[j],"xbeam_",".pd1");

    if(eId>=0)
    {
    idList.push_back(eId);
    //point2name_id.push_back(j);
    }
  }

    //sort number into list point to name_id 
    //sort is not necessary if id is already  in order
    sort(idList.begin(), idList.end());
    for(std::vector<int>::size_type j=0;j<idList.size();j++)
    {
    //cout << idList[j] << endl;
    cout << idList[j] << ' ';
    }
    cout << "" <<endl;
/////end of "get beamId in to list"

    //sprintf(txtfn,"../data/runData/%s/cut_%s.txt", store_file_name,store_file_name);
  //ofstream n2(txtfn,ios::trunc);//在向文件写之前擦掉所有之前数据
  //assert(n2); //是否打开成功
    TCanvas *c1 = new TCanvas("c1","Dynamic Filling Example",10,10,2400,600);
    c1->Divide(1,2);
   c1->SetGrid();
   c1->SetFillColor(42);

    

    // gr->Draw("ACP");
   const Int_t kUPDATE = 1;
   int counts = 0;

  // change_pd1: for(std::vector<int>::size_type j=0;j<idList.size();j++)
  // {
    // if(fileId <= idList[j])
    // {
      // fileId = idList[j];
      sprintf(infn,"%s/xbeam_%d.pd1",infile, fileId);
      cout << "begin to plot" << infn << endl;
      pd1.read(infn);
      pede.setup(pedefn);

    const int n = pd1.nFrame();
    double x[n], y[n];
    
    for(int i=0;i<pd1.nFrame();i++)
    {
      pd1.getFrame(i, 0); // 输入帧数和adc通道数，不加adc通道数这个参数，会获取所有通道的数据
      pede.subPede(pd1.frameDat, 0);  //subtract pede 不加第二个参数，会操作所有通道的数据
 
      int sum_frame = 0;
      for(int k=0;k<5184;k++)
      { 
        if(pd1.frameDat[k] > 10000 || pd1.frameDat[k] < -10000)
        {
          cout << "DAQ system error !" << endl;
          scanf("%d", &Key_input);
          break;
          }
        else
        {
          // sum_frame = sum_frame + pd1.frameDat[k];
          x[i] = i;
          y[i] = pd1.frameDat[ip];
        }
      }
    } //for(int i=0;i<pd1.nFrame();i++)

      vector<double> a;   
      
      int index_max_n = 0;//index_max用于接收最大值下标
      int index_min_n = 0;//index_max用于接收最小值下标

      for(int i = 0; i < n; ++i)//找最大束才值及坐标
        {
            if(y[i] > y[index_max_n])
            {
                index_max_n=i;//用下标存储最大值下标，不仅可以找到最大值，也可以找到它的位置
            }
            if(y[i] < y[index_min_n])
            {
                index_min_n=i;//用下标存储最大值下标，不仅可以找到最大值，也可以找到它的位置
            }
        }

      for(int i = 0; i < n; i++)
      {
        if(y[i] > 0.9 * y[index_max_n])
        {
          a.push_back(y[i]);
        }
      }

      double x2[a.size()], y2[a.size()];

      int index_max_n2 = 0;//index_max用于接收最大值下标
      int index_min_n2 = 0;//index_max用于接收最小值下标

      for(int i = 0; i < a.size(); i++)
      {
          x2[i] = i;
          y2[i] = a[i];
          if(a[i] > a[index_max_n2])
            {
                index_max_n2=i;//用下标存储最大值下标，不仅可以找到最大值，也可以找到它的位置
            }
            if(a[i] < a[index_min_n2])
            {
                index_min_n2=i;//用下标存储最大值下标，不仅可以找到最大值，也可以找到它的位置
            }
      }

    cout<<"end"<<endl;

    TGraph *gr = new TGraph(n,x,y);
    gr->SetLineColor(2);
    gr->SetLineWidth(2);
    gr->SetMarkerColor(4);
    gr->SetMarkerSize(1);
    gr->SetMarkerStyle(20);
    gr->SetTitle(Form("ADC value of pixel %i",ip));
    gr->GetXaxis()->SetTitle("Frame No");
    gr->GetYaxis()->SetTitle("ADC Value");
    gr->GetYaxis()->SetRangeUser(y[index_min_n]*2,y[index_max_n]*1.1);
    c1->cd(1);
    gr->Draw("ACP");

    TGraph *gr2 = new TGraph(a.size(),x2,y2);
    gr2->SetLineColor(2);
    gr2->SetLineWidth(2);
    gr2->SetMarkerColor(4);
    gr2->SetMarkerSize(1);
    gr2->SetMarkerStyle(20);
    gr2->SetTitle(Form("ADC value of pixel %i",ip));
    gr2->GetXaxis()->SetTitle("Frame No");
    gr2->GetYaxis()->SetTitle("ADC Value");
    gr2->GetYaxis()->SetRangeUser(a[index_min_n2],a[index_max_n2]);
    c1->cd(2);
    gr2->Draw("APL");

    c1->Update();
    c1->GetFrame()->SetFillColor(21);
    c1->GetFrame()->SetBorderSize(12);
    c1->Modified();


/////begin of refresh dir
name_id.clear();
idList.clear();
dir = opendir(beamfn); //打开一个目录

cout << pedefn << endl;
cout << beamfn << "xbeam_*.pd1"<<endl;

    while((ptr = readdir(dir)) != NULL) //循环读取目录数据
    {
    name_id.push_back(ptr->d_name);
    }
closedir(dir);//关闭目录指针

    for(std::vector<int>::size_type j=0;j<name_id.size();j++)
  {
    int eId=extractId(name_id[j],"xbeam_",".pd1");

    if(eId>=0)
    {
    idList.push_back(eId);
    }
  }

    //sort number into list point to name_id 
    //sort is not necessary if id is already  in order
    sort(idList.begin(), idList.end());
    for(std::vector<int>::size_type j=0;j<idList.size();j++)
    {
    //cout << idList[j] << endl;
    cout << idList[j] << ' ';
    }
    cout << "" <<endl;
/////end of begin of refresh dir
c1->SaveAs(TString::Format("%s/cut_%s_xbeam%d_xpede%d_pixel%d.png", infile,store_file_name,fileId,pedeId,ip));
// rootapp->Run();
return 0;

}//main()


