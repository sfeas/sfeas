//Author		: Xiao Tuzi
//Date(Create)	: 09/08/2022
//Description	: this program is calculating time integral coefficient


#include "Solu/Tools/getTimeIntegral.h"
#include "Global/GlobalVar.h"
#include "Keyword/KTAINTM.h"
#include "Keyword/KTAINTP.h"

void getTimeIntegral(double dt, double* A, double* B) {
	double alpha = TAINTP->alpha;
	double delta = TAINTP->delta;
	double alphaF = TAINTP->alphaF;
	double alphaM = TAINTP->alphaM;
	double theta = TAINTP->theta;

	// Newmark integral method
	if (TAINTM->IntegralMethod == 1) {
		A[0] = 1.0 / (alpha * dt * dt);
		A[1] = delta / (alpha * dt);
		A[2] = 1.0 / (alpha * dt);
		A[3] = 0.5 / alpha - 1.0;
		A[4] = delta / alpha - 1.0;
		A[5] = (0.5 * delta / alpha - 1.0) * dt;
		A[6] = 1.0;
		A[7] = 1.0;
		A[8] = 0.0;
		A[9] = 0.0;

		B[0] = A[0];
		B[1] = -A[2];
		B[2] = -A[3];
		B[3] = A[1];
		B[4] = -A[4];
		B[5] = -A[5];
		B[6] = 1.0;
		B[7] = 0.0;
		B[8] = 0.0;

	}
	// Wilson integral method
	else if (TAINTM->IntegralMethod == 2) {
		A[0] = 6.0 / (theta * theta * dt * dt);
		A[1] = 3.0 / (theta * dt);
		A[2] = 6.0 / (theta * dt);
		A[3] = 2.0;
		A[4] = 2.0;
		A[5] = 0.5 * theta * dt;
		A[6] = 1.0;
		A[7] = theta;
		A[8] = 1.0 - theta;
		A[9] = 0.0;

		B[0] = A[0] / theta;
		B[1] = -A[2] / theta;
		B[2] = 1.0 - 3.0 / theta;
		B[3] = 3.0 / (theta * theta *theta *dt);
		B[4] = 1.0 - 3.0 / (theta * theta);
		B[5] = 0.5 * dt * (2.0 - 3.0 / theta);
		B[6] = 1.0 / (theta * theta * theta);
		B[7] = (1.0 - 1.0 / (theta * theta)) * dt;
		B[8] = 0.5 * dt * dt * (1.0 - 1.0 / theta);
	}
	// HHT integral method
	else if (TAINTM->IntegralMethod == 3){
		A[0] = (1.0 - alphaM) / (alpha * dt * dt);
		A[1] = delta * (1.0 - alphaF) / (alpha * dt);
		A[2] = (1.0 - alphaM) / (alpha * dt);
		A[3] = 0.5 * (1.0 - alphaM) / alpha - 1.0;
		A[4] = delta * (1.0 - alphaF) / alpha - 1.0;
		A[5] = (1.0 - alphaF) * (0.5 * delta / alpha - 1.0) * dt;
		A[6] = 1.0 - alphaF;
		A[7] = 1.0 - alphaF;
		A[8] = alphaF;
		A[9] = -alphaF;

		B[0] = A[0];
		B[1] = -A[2];
		B[2] = -A[3];
		B[3] = A[1];
		B[4] = -A[4];
		B[5] = -A[5];
		B[6] = 1.0;
		B[7] = 0.0;
		B[8] = 0.0;
	}

}