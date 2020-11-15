/**
 * @file Projection.cpp
 * @brief 定义投影及接口
 * @version 0.1
 * @date 2020-11-15
 * @author 卢晓猛
 */

#include <memory>
#include "ADefine.h"
#include "Projection.h"

using namespace std::placeholders;
using namespace AstroUtil;

Projection::Projection() {
	l0 = b0 = 0.0;
	sl0 = sb0 = 0.0;
	cl0 = cb0 = 1.0;
	type = -1;
	SetType(0);
}

Projection::~Projection() {

}

/////////////////////////////////////////////////////////////////////////
int Projection::fwd_tan(double l, double b, double& xi, double& eta) {
	int rslt(0);
	double sb, cb, sda, cda, fract;

	sb = sin(b);
	cb = cos(b);
	sda = sin(l - l0);
	cda = cos(l - l0);
	fract = sb * sb0 + cb * cb0 * cda;

	if (fract < AEPS) {
		if (fract >= 0.0) {
			rslt = 1;
			fract = AEPS;
		}
		else if (fract > -AEPS) {
			rslt = 2;
			fract = -AEPS;
		}
		else rslt = 3;
	}
	xi = cb * sda / fract;
	eta = (sb * cb0 - cb * sb0 * cda) / fract;
	return rslt;
}

void Projection::rev_tan(double xi, double eta, double& l, double& b) {
	double fract = cb0 - eta * sb0;
	l = cycmod(atan2(xi, fract) + l0, A2PI);
	b = atan2(sb0 + eta * cb0, sqrt(xi * xi + fract * fract));
}

/////////////////////////////////////////////////////////////////////////
bool Projection::SetType(int type) {
	if (type < 0 || type >= PRJ_MAX)
		return false;
	if (this->type != type) {
		this->type = type;
		switch(type) {
		case PRJ_TAN:
			prjfwd = std::bind(&Projection::fwd_tan, this, _1, _2, _3, _4);
			prjrev = std::bind(&Projection::rev_tan, this, _1, _2, _3, _4);
			break;
		default:
			break;
		}
	}

	return true;
}

void Projection::SetPoint(double l, double b) {
	l0 = l;
	b0 = b;
	sl0 = sin(l0);
	cl0 = cos(l0);
	sb0 = sin(b0);
	cb0 = cos(b0);
}

int Projection::Forward(double l, double b, double& xi, double& eta) {
	return prjfwd(l, b, xi, eta);
}

void Projection::Reverse(double xi, double eta, double& l, double& b) {
	prjrev(xi, eta, l, b);
}
