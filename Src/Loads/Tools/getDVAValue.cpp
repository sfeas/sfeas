//Author		: Xiao Tuzi
//Date(Create)	: 11/24/2022
//Description	: this program is calculating DVA for Enforced Motion in TASolu


void getDVAValue(const int label, const double* B, double cur, double* DVA) {
	// input:	 DVA is last step
	// output:	 DVA is current step
	// label = 1, cur = disp; label = 2, cur = velo; label = 3, cur = acce;

	double disp, velo, acce;
	if (label == 1) {
		disp = cur;
		acce = B[0] * (disp - DVA[0]) + B[1] * DVA[1] + B[2] * DVA[2];
		velo = B[3] * (disp - DVA[0]) + B[4] * DVA[1] + B[5] * DVA[2];

		DVA[0] = disp;
		DVA[1] = velo;
		DVA[2] = acce;
	}
	else if (label == 2) {
		velo = cur;
		disp = (velo - B[4] * DVA[1] - B[5] * DVA[2]) / B[3] + DVA[0];
		acce = B[0] * (disp - DVA[0]) + B[1] * DVA[1] + B[2] * DVA[2];

		DVA[0] = disp;
		DVA[1] = velo;
		DVA[2] = acce;
	}
	else if (label == 3) {
		acce = cur;
		disp = (acce - B[1] * DVA[1] - B[2] * DVA[2]) / B[0] + DVA[0];
		velo = B[3] * (disp - DVA[0]) + B[4] * DVA[1] + B[5] * DVA[2];

		DVA[0] = disp;
		DVA[1] = velo;
		DVA[2] = acce;
	}
}