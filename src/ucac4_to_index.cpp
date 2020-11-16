/**
 * @brief 使用UCAC4星表原始数据, 生成用于天文定位和流量测光的索引星表
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "ADefine.h"
#include "ACatUCAC4.h"
#include "AMath.h"

using namespace AstroUtil;

int main(int argc, char **argv) {
	printf ("Usage:\n"
			"\tucac4_to_index field_angle mag_max\n");

	double faMax = argc > 1 ? atof(argv[1]) : 60;
	double faMin = faMax / 6.0;
	double faMaxDeg = faMax / 60.0;
	double faMinDeg = faMin / 60.0;
	double step, t;
	double rac, decc;
	int n0, n1, n2, nmin(100000), i;
	int n30(0), n60(0), n90(0), n120(0);
	short magmax = argc > 2 ? atoi(argv[2]) * 1000 : 10000;
	short mag0, mag;
	std::vector<ucac4_item> found;
	ucac4_item brightest;
	ucac4item_ptr stars;
	ACatUCAC4 ucac4;
	char filename[20];
	FILE *fp;

	sprintf (filename, "sigma_%d-mag%d.txt", int(faMax), magmax / 1000);
	fp = fopen(filename, "w");

	ucac4.SetPathRoot("/Users/lxm/Catalogue/UCAC4");
	for (decc = -90.0, n2 = 0; decc <= 90.0; decc += 10.0/*faMaxDeg*/) {
		printf ("DEC Center = %6.2f\n", decc);
		t = cos(decc * D2R);
		if (t < AEPS) step = 360.0;
		else step = 10.0 / t;
//		else step = faMaxDeg / t;
		for (rac = 0.0; rac < 360.0; rac += step, ++n2) {
			n0 = n1 = 0;
			mag0 = 20000;
			if (ucac4.FindStar(rac, decc, faMax * 2)) {
				stars = ucac4.GetResult(n0);
				found.clear();
				for (i = 0; i < n0; ++i) {
					if ((mag = stars[i].apasm[1]) < magmax) {
						++n1;
						found.push_back(stars[i]);
						if (mag < mag0) {
							mag0 = mag;
							brightest = stars[i];
						}
					}
				}
				// 计算与最亮点之间的距离
				n0 = 0;
				for (i = 0; i < n1; ++i) {
					t = SphereRange(brightest.ra * 0.001 * AS2R, (brightest.spd * 0.001 * AS2D - 90.0) * D2R,
							found[i].ra * 0.001 * AS2R, (found[i].spd * 0.001 * AS2D - 90.0) * D2R);
					t *= R2D;
					if (t >= faMinDeg && t <= faMaxDeg) {
						++n0;
						// 构建模型: 简化
					}
				}

				if (n0 / 12 >= 4) ++n30;
				if (n0 / 6 >= 4) ++n60;
				if (n0 / 4 >= 4) ++n90;
				if (n0 / 3 >= 4) ++ n120;

				printf ("\t%6.2f  %4d\n", rac, n0);
				fprintf (fp, "%6.2f %6.2f  %4d\n", rac, decc, n0);
				if (nmin > n0) nmin = n0;
			}
		}
	}
	printf ("total = %d\n"
			"n30   = %d\n"
			"n60   = %d\n"
			"n90   = %d\n"
			"n120  = %d\n",
			n2,
			n30,
			n60,
			n90,
			n120);

	fprintf (fp, "\n--------------------------------\n");
	fprintf (fp,
			"total = %d\n"
			"n30   = %d\n"
			"n60   = %d\n"
			"n90   = %d\n"
			"n120  = %d\n",
			n2,
			n30,
			n60,
			n90,
			n120);
	fclose(fp);

	return 0;
}
