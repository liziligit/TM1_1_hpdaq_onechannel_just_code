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
//#include "rootlib.h"
#include "rootlibx.h"//only for mac


int pd1_analysis_test(const char *fn, const char *pedeFile, int ich, int iframe, double upLimit, double lowLimit)
{

    char dataInput[200];
    char pedeInput[200];
    sprintf(dataInput,"%s.pd1",fn);
    sprintf(pedeInput,"%s.txt",pedeFile);

    pd1Debug pd1d;
	  pd1d.read(dataInput);

    // pd3d.noiseRange();
//       pd3d.drawRow(ich, iframe);
//	pd3d.fitBeam(ich, iframe);
//   pd3d.fitBeamError(ich);
//    pd3d.beamPosResol(ich, upLimit, lowLimit);
//     pd3d.beamPosResolCheck(ich, upLimit, lowLimit);
pd1d.beamPosFit_show_all_pixel_pd1(pedeInput, ich, iframe, upLimit, lowLimit);//for 2d map max pixel fit show all pixel
// pd1d.beamPosFit_pd1(pedeInput, ich, iframe, upLimit, lowLimit);//for 2d map max pixel fit show 11 pixel
// pd1d.beamProfile_pd1(pedeInput, ich, iframe, upLimit, lowLimit);//align straight and sum 72 row to fit
// pd1d.beamPosGausrowFit_pd1(pedeInput, ich, iframe, upLimit, lowLimit);//for single row fit
// pd1d.beamPosgausFit_pd1(pedeInput, ich, iframe, upLimit, lowLimit);//for 72 row fit
pd1d.beamAngleResol_pd1(pedeInput, ich, iframe, upLimit, lowLimit);//for angle fit
//pd3d.framePosResol(ich, iframe, upLimit, lowLimit);
//pd3d.beamIntensityResol(ich, upLimit, lowLimit);
//pd3d.beamSize(ich, upLimit, lowLimit);
//pd3d.beamBadPixel(ich, upLimit, lowLimit);
//pd3d.beamPresentation(ich, upLimit, lowLimit);
//pd3d.beamAngleResol(ich, upLimit, lowLimit);
//pd3d.beamTime(ich, upLimit, lowLimit);
//     pd3d.beamPara(ich);
//	pd3d.beamPara_correction(ich);
//     pd3d.beamFrame(ich);
//     pd3d.beamFrameAveFit(ich);
 //      pd3d.beamFrameAveFitRota(ich);
//	pd3d.beamParaAcu(ich);
//    pd3d.displayInterfaceUpdate(maxPixelSignal);
   // pd3d.displayInterface_copy(maxPixelSignal);
   // pd3d.drawFrameUpdate(0,10);

   // for(int i=300;i<400;i++){
   // pd3d.drawFrame(4,frame);
    // int a;
// scanf("%d",&a);	
   // }
	return 0;
}
