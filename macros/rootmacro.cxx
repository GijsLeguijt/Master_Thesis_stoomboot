void rootmacro()
{	
	//Selecting which plot to make	
	int i;
	cout 	<< "Choose:" 					<< endl
			<< "10: (x,y,z) of primaries"	<< endl
			<< "11: (x,y,z) of hits"		<< endl
			<< "20: (x,y) of primaries"		<< endl
			<< "21: (x,y) of hits"			<< endl
			<< "30: r of primaries"	 		<< endl
			<< "31: r of hits"		 		<< endl
			<< "40: (r,z) of primaries"		<< endl
			<< "41: (r,z) of hits"			<< endl;
	cin		>> i;
	cout	<< endl;

	//Selecting if cuts have to be made (no choices on which cut yet)
	TString input_cut;
	cout	<< "For cut choose 'y', else choose 'n':" << endl;
	cin		>> input_cut;
	cout	<< endl;

	//Setting the cut
	TString cut;
	if (input_cut == 'y')
	{	TString c_depe   = "etot > 0";
		TString c_nodisc = "abs(zp_pri) < 750";
	
		cut  = c_depe + " && " + c_nodisc;
		cout << "Cut: " << cut << endl;
	}
	else
		cut  = "";
	
	//Radial distance of particle and primary
	TString rp 		= "sqrt(xp^2 + yp^2)";
	TString rp_pri 	= "sqrt(xp_pri^2 + yp_pri^2)";
	
	//Plotting	
	switch (i)
	{
		case 10: evt->Draw("xp_pri:yp_pri:zp_pri");				break;
		case 11: evt->Draw("xp:yp:zp");				 			break;
		case 20: evt->Draw("xp_pri:yp_pri",cut); 				break;
		case 21: evt->Draw("xp:yp",cut); 						break;
		case 30: evt->Draw(rp_pri,cut); 						break;
		case 31: evt->Draw(rp,cut);		 						break;
		case 40: evt->Draw("zp_pri:"+rp_pri,"etot > 0","colz");	break;
		case 41: evt->Draw("zp:"+rp,"etot > 0","colz");			break;
	}
}


