//Author		: Xiao Tuzi
//Date(Create)	: 09/08/2022
//Description	: this program is declaration of calculating damping coefficient


#pragma once

void calDamp(double& alphad, double& betad, double& Gf, double& GEf, double* E = nullptr, int nMode = 0, double* ksi = nullptr);