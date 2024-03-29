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



//int pd1_syncDraw2D(char *infile, int fileId, char *pedefile, int pedeId, double maxPixelSignal, double minPixelSignal, double ip){
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
string num=nameId.substr(headLength,dataLength);//获得从headlength开始，长度为dataLength的字符串
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
double maxPixelSignal;
maxPixelSignal = atof(argv[5]);
double minPixelSignal;
minPixelSignal = atof(argv[6]);
int ip;
ip = atoi(argv[7]);
char *store_file_name;
store_file_name = argv[8];

int Key_input;
char str[50];
int k=0;

pd1Pede pede;
placData_1 pd1;
char infn[200];
char pedefn[200];
char beamfn[200];

//sprintf(infn,"%s_%d.pd1",infile, fileId);
sprintf(pedefn,"%s/xpede_%d.txt", pedefile, pedeId);
// sprintf(beamfn,"../data/runData/%s/", store_file_name);
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
    //int nCh=pd1.adcCha();
    int nCh=1;
    plot pt;
    pt.setupCanvas(nCh,1, ip, 720, 720);//设置canvas上图片的行数，列数, 及 canvas的位置
    pt.setup2D(nCh,1,72,72,"hist2d"); 
    pt.setup2DMinMax(minPixelSignal,maxPixelSignal);
    int ch;

  change_pd1: for(std::vector<int>::size_type j=0;j<idList.size();j++)
  {
    if(fileId <= idList[j])
    {
   		fileId = idList[j];
    	sprintf(infn,"%s/xbeam_%d.pd1",infile, fileId);
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
			pt.h2[0]->SetTitle(str);

			for(int k=0;k<nCh;k++){
			if(k<nCh/2){
				pt.loadData2Hist2D(pd1.frameDat+k*72*72,k+nCh/2);
			}else{
				pt.loadData2Hist2D(pd1.frameDat+k*72*72,nCh-k-1);	
				}
			}

			//pt.loadData2Hist2D(pd1.frameDat+k*72*72,nCh-k-1);
			pt.draw2D(); 

			if(kbhit()) 
            {
 			    ch=getch();
 			    //cout<<"key is "<<int(ch)<<endl;
				if(ch==27) break;// Esc == 27,Spacebar=32,Enter=13&108,
				//LeftArrow=37, RightArrow=39, UpArrow=38,DownArrow=40
				// if(ch==38) break;				
				if(ch==32)
				{
					wait_opera: cout << "请输入正负10000内的数字前后跳帧,-10000跳到上个pd1,10000跳到下个pd1,50000截图,0继续" << endl;
					scanf("%d", &Key_input);
					if( Key_input == 0 )
					{
						cout << "You input zero, continue!" << endl;
					}
					//for change files
					else if(Key_input == -10000)//should big than pd1.nFrame()
					{
						if(j==0)//list number can not less than 0
							{
							fileId = idList[j];
							//cout << "1XXXXXX j=" <<j << endl;
							}
						else
							{
							fileId = idList[j-1];
							//cout << "1XXXXXX j-1=" <<j-1 << endl;
							}
						goto change_pd1;
					}
					else if(Key_input == 10000)
					{
						if(j==idList.size() - 1)//list number can not bigger than idList.size()
							{
							fileId = idList[j];
							//cout << "1XXXXXX j=" <<j << endl;
							}
						else
							{
							fileId = idList[j + 1];
							//cout << "1XXXXXX j-1=" <<j-1 << endl;
							}
						//fileId = idList[j+1];
						//cout << "1XXXXXX j+1=" <<j+1 << endl;
						goto change_pd1;
					}
					//end of change files
					//for saving frame png	
					else if(Key_input == 50000)
					{
						pt.c->SaveAs(TString::Format("%s/cut_%s_xbeam%d_xpede%d_iFrame%d.png", infile,store_file_name,pedeId,fileId,i));
						goto wait_opera;
					}
					//end for saving frame png	
					else
					{	
						change_frame: i = i + Key_input;
						//cout<<i<<endl;						
						pd1.getFrame(i, 0); // 输入帧数和adc通道数，不加adc通道数这个参数，会获取所有通道的数据
						//sprintf(str, "frame %d",i);
						sprintf(str, "xbeam_%d.pd1 frame %d",fileId,i);
						pede.subPede(pd1.frameDat, 0);  //subtract pede 不加第二个参数，会操作所有通道的数据
						pt.h2[0]->SetTitle(str);

						for(int k=0;k<nCh;k++){
						if(k<nCh/2){
							pt.loadData2Hist2D(pd1.frameDat+k*72*72,k+nCh/2);
						}else{
							pt.loadData2Hist2D(pd1.frameDat+k*72*72,nCh-k-1);	
							}
						}

						//pt.loadData2Hist2D(pd1.frameDat+k*72*72,nCh-k-1);
						pt.draw2D();

						scanf("%d", &Key_input);
						if( Key_input == 0 )
						cout << "You input zero, continue!" << endl;
						//for change files
						else if(Key_input == -10000)
						{
						if(j==0)//list number can not less than 0
							{
							fileId = idList[j];
							//cout << "2XXXXXX j=" <<j << endl;
							}
						else
							{
							fileId = idList[j-1];
							//cout << "2XXXXXX j-1=" <<j-1 << endl;
							}
						goto change_pd1;
						}
						else if(Key_input == 10000)
						{
						if(j==idList.size() - 1)//list number can not bigger than idList.size()
							{
							fileId = idList[j];
							//cout << "2XXXXXX j=" <<j << endl;
							}
						else
							{
							fileId = idList[j + 1];
							//cout << "1XXXXXX j-1=" <<j-1 << endl;
							}
						//fileId = idList[j+1];
						//cout << "2XXXXXX j+1=" <<j+1 << endl;
						goto change_pd1;
						}
						//end of change files
						//for saving frame png	
						else if(Key_input == 50000)
						{
						// pt.c->SaveAs(TString::Format("../data/runData/%s/cut_%s_%d_%d.png", store_file_name,store_file_name,fileId,i));
							pt.c->SaveAs(TString::Format("%s/cut_%s_xbeam%d_xpede%d_iFrame%d.png", infile,store_file_name,pedeId,fileId,i));
						goto wait_opera;
						}
						//end for saving frame png		
						else goto change_frame;
						//cout << n <<endl;
					}//if( Key_input != 0 )			    		
				}//if(ch==32)				
			}//kbhit()
			
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

return 0;
}//main()
