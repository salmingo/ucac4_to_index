/**
 * @brief 使用UCAC4星表原始数据, 生成用于天文定位和流量测光的索引星表
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "ADefine.h"
#include "ACatUCAC4.h"

using namespace AstroUtil;

int main(int argc, char **argv) {
	//...选项
	//...
	double faMin = atof(argv[1]);	// 角分
	double faMax = faMin * 1.414;
	double faMinDeg = faMin / 60.0;
	double step, t;
	double rac, decc;
	int n;
	ACatUCAC4 ucac4;
	ucac4item_ptr stars;

	ucac4.SetPathRoot("");
	for (decc = -90.0; decc <= 90.0; decc += faMinDeg) {
		t = cos(decc * D2R);
		if (t < AEPS) step = 360.0;
		else step = faMinDeg / cos(decc * D2R);
		for (rac = 0.0; rac < 360.0; rac += step) {
			n = 0;
			if (ucac4.FindStar(rac, decc, faMin))
				stars = ucac4.GetResult(n);

			printf ("%6.2f  %6.2f  %d\n", rac, decc, n);
		}
	}
	return 0;
}
