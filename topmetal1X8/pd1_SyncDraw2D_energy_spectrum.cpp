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
//#include "rootlib.h"//for linux
#include "rootlibx.h"//only for mac
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

// int pd1_SyncDraw2D_energy_spectrum_root(const char *infile, int fileId, const char *pedefile, int pedeId, double maxPixelSignal, double minPixelSignal, double ip, const char *store_file_name)
int main(int argc, char **argv)
{
  TApplication* rootapp = new TApplication("pd1_SyncDraw2D_energy_spectrum_root2", &argc, argv);
// maxPixelSignal=maxPixelSignal;
// minPixelSignal=minPixelSignal;
// ip=ip;

char *infile;
infile = argv[1];
int fileId;
fileId = atol(argv[2]);
char *pedefile;
pedefile = argv[3];
int pedeId;
pedeId = atol(argv[4]);
double maxPixelSignal;
maxPixelSignal = atof(argv[5]);
double minPixelSignal;
minPixelSignal = atof(argv[6]);
int ip;
ip = atoi(argv[7]);
char *store_file_name;
store_file_name = argv[8];

int Key_input;
char str[30];
int k=0;

pd1Pede pede;
placData_1 pd1;
char infn[100];
char pedefn[100];
char beamfn[100];
char txtfn[100];

//sprintf(infn,"%s_%d.pd1",infile, fileId);
sprintf(pedefn,"%s_%d.txt", pedefile, pedeId);
sprintf(beamfn,"../data/runData/%s/", store_file_name);
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
    // gBenchmark->Start("main");
    TCanvas *c1 = new TCanvas("c1","Dynamic Filling Example",200,10,700,500);
   c1->SetFillColor(42);
   c1->GetFrame()->SetFillColor(21);
   c1->GetFrame()->SetBorderSize(6);
   c1->GetFrame()->SetBorderMode(-1);

    TH1F *hpx = new TH1F("hpx","This is the energy spectrum",200,-1000,150000);
   hpx->SetFillColor(48);
   hpx->GetXaxis()->SetTitle("ADC         ");
   hpx->GetYaxis()->SetTitle("COUNTS      ");

   const Int_t kUPDATE = 100;
   int counts = 0;

  change_pd1: for(std::vector<int>::size_type j=0;j<idList.size();j++)
  {
    if(fileId <= idList[j])
    {
   		fileId = idList[j];
    	sprintf(infn,"%s_%d.pd1",infile, fileId);
    	cout << "begin to plot" << infn << endl;
    	//cout << "begin j=" <<j << endl;
    	pd1.read(infn);
		pede.setup(pedefn);

		for(int i=0;i<pd1.nFrame();i++)
		{
			//cout<<i<<endl;			
			pd1.getFrame(i, 0); // 输入帧数和adc通道数，不加adc通道数这个参数，会获取所有通道的数据
			//sprintf(str, "frame %d", i);
			sprintf(str, "xbeam_%d.pd1 frame %d",fileId,i);
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
					sum_frame = sum_frame + pd1.frameDat[k];
				}
				
			}
				// n2 << fixed << setprecision(0) <<setiosflags(ios::left)<< setw(4) << pd1.frameDat[j];
				//n2 << fixed << setprecision(0) <<setiosflags(ios::left)<< setw(4) << sum_frame;
				//cout << fixed << setprecision(0) <<setiosflags(ios::left)<< setw(4) << sum_frame <<endl;
				// if ( (j+1) % 72 == 0)
				//n2 << endl;

				hpx->Fill(sum_frame);
                counts = counts + 1;

		if (counts && (counts%kUPDATE) == 0) {
         if (counts == kUPDATE) hpx->Draw();
         c1->Modified();
         c1->Update();
         if (gSystem->ProcessEvents())
            break;
      }	
		// cout << sum_frame <<endl;
		} //for(int i=0;i<pd1.nFrame();i++)
		cout<<"end"<<endl;


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
/////end of begin of refresh dir

  }  //if(fileId <= idList[j])

	} //for(int j=0;j<idList.size();j++) 	
//n2.close();
        hpx->SetFillColor(50);//停住后的颜色
        c1->Modified();
        c1->Update();
c1->SaveAs(TString::Format("../data/runData/%s/energy_xbeam_%d_pede_%d.png",store_file_name,fileId,pedeId));
rootapp->Run();
return 0;

}//main()


