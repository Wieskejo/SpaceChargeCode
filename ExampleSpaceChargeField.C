#ifndef __CLING__
#include "../build/include/AtPatternY.h"
#include "../build/include/AtRadialChargeModel.h"
#endif

double EField2(double rho, double z)
{
	double lambda = 2.35e-8;               // SI
	constexpr double rBeam = 2.0/100;            // in *m*
	constexpr double eps = 8.85418782E-12; // SI
	constexpr double pi = 3.14159265358979;
	constexpr double eps2pi = 2 * pi * eps;
	rho /= 100.; // Convert units from cm to m
	z /= 100.; // Convert units from cm to m

	double field;
	if (rho > rBeam)
	  field = lambda / eps2pi / rho * (z/1.); // v/m
	else
	  //field = lambda / eps2pi / rBeam / rBeam * rho * (z/1.); // v/m
	  field = 0;
	return field / 100.;                              // V/cm
	//return field;
}

void CorrectFissionEvent()
{
	auto model = new AtRadialChargeModel(&EField2);
	model->SetStepSize(0.1);	
	
	std::vector<AtHit> correctedHits;
	std::cout << "Correcting space charge" << std::endl;
	for (const auto &hit : tracks[0].GetHitArray()) { 
	 auto correctedHit = hit;
	 auto oldHitX = hit.GetPosition().X(); auto oldHitY = hit.GetPosition().Y(); auto oldHitZ = hit.GetPosition().Z();
	 double offsetX = 10.0*oldHitZ/1000;
	 double offsetY = -6.0*(1 - oldHitZ/1000);
	 ROOT::Math::XYZPoint oldOffsetHitPos(oldHitX - offsetX, oldHitY - offsetY, oldHitZ);
	 correctedHit.SetPosition(model->CorrectSpaceCharge(oldOffsetHitPos));
	 auto newHitX = correctedHit.GetPosition().X(); auto newHitY = correctedHit.GetPosition().Y(); auto newHitZ = correctedHit.GetPosition().Z();
	 ROOT::Math::XYZPoint newOffsetHitPos(newHitX + offsetX, newHitY + offsetY, newHitZ);
	 correctedHit.SetPosition(newOffsetHitPos);
	 correctedHits.push_back(correctedHit);
	}	
}