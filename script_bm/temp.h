void beamPosFit_pd1(char *pedeFile, int ich, int iframe, double upLimit, double lowLimit)
   {//for 2d map fit
    pd1Pede pede;
    pede.setup(pedeFile); 
 
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1);

	int nTopMetalChips=pd1.adcCha();
	int nPixelsOnChip=pd1.nPix();        
    int nFrames=pd1.nFrame();
    double arry2dmap[72][72]; 
    int location_Num[72];  
    int Del_width =5;  

	double rowSum;
	double pixelValue;
	double pixelMean;
	double pixelSigma2;
	double th2DupLimit = 10;
	double th2DlowLimit = -10;
	double pitch = 83;
	double b;
	double k;

	double di;
	double pointX, pointY; 
	double diMean=0;
	double diRMS=0;
	double resol=0;
	double resolMean=0;
	double resolRMS=0;


        pd1.getChaDat(ich);
        double adc_value;
        int binX;
        int binY;

	TString hist2dName;
        hist2dName = "hAdc2dMap_iframe"; //hist2dName += iframe;
        TH2D *hAdc2dMap= new TH2D(hist2dName, "pd1 ADC 2d Map;pixel row index;pixel column index", 72, 0, 72, 72, 0, 72);


        for(int ipixel=0;ipixel<pd1.nPix();ipixel++){        
            binX = ipixel/72;//取整数
            binY = ipixel%72;//取余数
           adc_value = 0;
            for(int isample=0;isample<pd1.samPerPix();isample++){
                adc_value+=pd1.chaDat[iframe*pd1.samPerPix()*pd1.nPix()+ipixel*pd1.samPerPix()+isample];
            }
            adc_value/=(double)pd1.samPerPix();
            adc_value-=pede.meanPed[ich*pd1.nPix()+ipixel];  
            // hAdc2dMap->SetBinContent(binX+1,binY+1,adc_value); 
            arry2dmap[binY][binX] = adc_value;
        }

        //resort the hAdc2dMap
        //get the largest adc_value of every row
          for(int i=0;i<72;i++){
          //max_element(v.begin(), v.end());
          //distance(s.begin(), i); i 是vector的下标，也就是max_element
          location_Num[i] = distance(arry2dmap[i], max_element(arry2dmap[i], arry2dmap[i] + sizeof(arry2dmap[i])/sizeof(arry2dmap[i][0]))) ;

         //keep the largest Num +- width

        for(int m = 0; m < location_Num[i] - Del_width; m++)
        {
         arry2dmap[i][m]=0;
        }

        for(int n = location_Num[i] + Del_width + 1; n < 72; n++)
        {
         arry2dmap[i][n]=0;
        }

      	}

      	//give to the hAdc2dMap
      	for (int k = 0; k < 72; k++)
    {
      for (int l = 0; l < 72; l++)
      {
		hAdc2dMap->SetBinContent(l+1,k+1,arry2dmap[k][l]); 
      }
   }


	TGraphErrors ge;
	TGraphErrors geRota;	
	int iPoint=0;
        for(int irow = 1; irow < 71; irow++){
        	rowSum = 0;
		pixelMean = 0;
		pixelSigma2 =0;
         for(int icol = 0; icol < 72; icol++){
		pixelValue = hAdc2dMap->GetBinContent(icol+1, irow+1);
	      if(pixelValue<upLimit & pixelValue>lowLimit){
		rowSum = rowSum + pixelValue;		
		}			
		}

	    for(int icol = 0; icol<72; icol++){
		pixelValue = hAdc2dMap->GetBinContent(icol+1, irow+1);
	     if(pixelValue<upLimit & pixelValue>lowLimit){
		pixelMean = pixelMean+pixelValue/rowSum*icol*pitch;
		}

		}

	    for(int icol = 0; icol<72; icol++){
		pixelValue = hAdc2dMap->GetBinContent(icol+1, irow+1);
	     if(pixelValue<upLimit & pixelValue>lowLimit){
		pixelSigma2 = pixelSigma2 + (pixelValue/rowSum)*((icol+1)*pitch-pixelMean)*((icol+1)*pitch-pixelMean);

		}

		}
		//Set x, y values for ipoint
		ge.SetPoint(iPoint, (irow+1)*pitch, pixelMean );
		//Set ex, ey values for ipoint
		ge.SetPointError(iPoint, 0, sqrt(pixelSigma2));
		geRota.SetPoint(iPoint, pixelMean, (irow+1)*pitch );
		//geRota.SetPointError(iPoint, sqrt(pixelSigma2), 0);
		iPoint++;

	}

	TF1 *fit1 = new TF1("fit1", "pol1", 2*pitch, 71*pitch);
    ge.Fit("fit1", "QR");//fit ge and geRota
	b = fit1->GetParameter(0);//intercept of fit line
	k = fit1->GetParameter(1);//slope of fit line

	diMean = 0;
	diRMS = 0;
	iPoint = 0;
	
    for(int irow = 1; irow < 71; irow++){
      
	ge.GetPoint(iPoint,pointX,pointY);

	di = (pointY-(k*(irow+1)*pitch+b))*cos(atan(k));
	
	diMean = diMean + di;
	diRMS = diRMS + di*di;
	iPoint++; 
    }


	diRMS = sqrt(abs(diRMS-diMean*diMean/70)/(70-1));
	diMean = diMean/70;
	resol = diRMS/sqrt(70);


	cout<<"resol: "<<resol<<endl;

	TString hist2dNameRotat;
        hist2dNameRotat = "hAdc2dMap_iframe"; //hist2dNameRotat += iframe;

        TH2D *hAdc2dMapRotat= new TH2D(hist2dNameRotat, "", 72, 0*pitch, 72*pitch, 72, 0*pitch, 72*pitch);

            for(int ipixel=0;ipixel<pd1.nPix();ipixel++){        
            binX = ipixel/72;
            binY = ipixel%72;
            adc_value = 0;
            for(int isample=0;isample<pd1.samPerPix();isample++){
                adc_value+=pd1.chaDat[iframe*pd1.samPerPix()*pd1.nPix()+ipixel*pd1.samPerPix()+isample];
            }

            adc_value/=(double)pd1.samPerPix();
            adc_value-=pede.meanPed[ich*pd1.nPix()+ipixel];   
            arry2dmap[binY][binX] = adc_value;
        }

        //resort the hAdc2dMap
        //get the largest adc_value of every row
          for(int i=0;i<72;i++){
          //cout << "Max element: " << *max_element(arry2dmap[i], arry2dmap[i]+ sizeof(arry2dmap[i])/sizeof(arry2dmap[i][0])) << "\n";
          //cout << "Max element location: " << distance(arry2dmap[i], max_element(arry2dmap[i], arry2dmap[i] + sizeof(arry2dmap[i])/sizeof(arry2dmap[i][0]))) << "\n";
          location_Num[i] = distance(arry2dmap[i], max_element(arry2dmap[i], arry2dmap[i] + sizeof(arry2dmap[i])/sizeof(arry2dmap[i][0]))) ;

         //keep the largest Num +- width
        for(int m = 0; m < location_Num[i] - Del_width; m++)
        {
         arry2dmap[i][m]=0;
        }

        for(int n = location_Num[i] + Del_width + 1; n < 72; n++)
        {
         arry2dmap[i][n]=0;
        }

      	}

      	//give to the hAdc2dMap
      	for (int k = 0; k < 72; k++)
    {
      for (int l = 0; l < 72; l++)
      {
		hAdc2dMapRotat->SetBinContent(l+1,k+1,arry2dmap[k][l]); 
      }
   }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// TString hist2dNameRotat90;
 //        hist2dNameRotat90 = "hAdc2dMaRotat90";
 //        TH2D *hAdc2dMapRotat90= new TH2D(hist2dNameRotat90, "", 72, 0*pitch, 72*pitch, 72, 0*pitch, 72*pitch);

 //       for(int irow = 0; irow < 72; irow++){
 //            for(int icol = 0; icol < 72; icol++){
	// 	hAdc2dMapRotat90->SetBinContent(irow+1, icol+1, hAdc2dMapRotat->GetBinContent(icol+1, irow+1));    	
	// 	}
	// }
 
	// hAdc2dMapRotat90->GetXaxis()->SetTitle("#mum");
	// hAdc2dMapRotat90->GetXaxis()->CenterTitle();
	// hAdc2dMapRotat90->GetYaxis()->SetTitleOffset(1.6);
	// hAdc2dMapRotat90->GetYaxis()->SetTitle("#mum");	
	// hAdc2dMapRotat90->GetYaxis()->CenterTitle();	
	// hAdc2dMapRotat90->SetMaximum(th2DupLimit);	
	// hAdc2dMapRotat90->SetMinimum(th2DlowLimit);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	hAdc2dMapRotat->GetXaxis()->SetTitle("#mum");
	hAdc2dMapRotat->GetXaxis()->CenterTitle();
	hAdc2dMapRotat->GetYaxis()->SetTitleOffset(1.6);
	hAdc2dMapRotat->GetYaxis()->SetTitle("#mum");	
	hAdc2dMapRotat->GetYaxis()->CenterTitle();	
	hAdc2dMapRotat->SetMaximum(th2DupLimit);
	hAdc2dMapRotat->SetMinimum(th2DlowLimit);	

    TCanvas *cBeamPosFit = new TCanvas("cBeamPosFit", "Beam Position Fit", 800 ,800);	
	cBeamPosFit->cd();
    cBeamPosFit->SetCanvasSize(750, 750);
	//hAdc2dMapRotat90->Draw("COLZ");
    hAdc2dMapRotat->Draw("COLZ");//draw 72*72 2d map

	geRota.Draw("same p*");//draw center of gravity point
	//ge.Draw("same p*");//draw center of gravity point that turn 90 degree

     /*   TF1 *fit1Rota = new TF1("fit1Rota", "1/[0]*x-[1]/[0]", pitch, 73*pitch);
	fit1Rota->SetParameter(0,k);
	fit1Rota->SetParameter(1,b);
	fit1Rota->Draw("same");*/

	TGraph gPoint;
	iPoint = 0;

    for(int irow = 1; irow < 71; irow++){
    gPoint.SetPoint(iPoint, (irow+1)*pitch*k+b, (irow+1)*pitch);
	iPoint++;
	}

	gPoint.Draw("same c");//draw fitting line
	// gPoint.SetLineColor(2);
	// gPoint.SetLineWidth(4);

	char *beamPosFitNamePng =Form("pd1png/beamPositionFitChip%dFrame%d.png",ich, iframe);
        cBeamPosFit->SaveAs(beamPosFitNamePng);

	// char *beamPosFitNamePdf =Form("pd1png/beamPositionFitChip%dFrame%d.pdf",ich, iframe);
 //        cBeamPosFit->SaveAs(beamPosFitNamePdf);
	delete cBeamPosFit;

} 
