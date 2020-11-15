/**
 * @file FITSHandler.hpp 基于cfitsio封装FITS文件基本操作
 */

#ifndef FITS_HANDLER_H_
#define FITS_HANDLER_H_

#include <stdlib.h>
#include <cfitsio/longnam.h>
#include <cfitsio/fitsio.h>

struct FITSHandler {
	fitsfile *fitsptr;	//< 基于cfitsio接口的文件操作接口
	int errcode;		//< 错误代码
	char errmsg[100];	//< 错误提示

public:
	FITSHandler() {
		fitsptr = NULL;
		errcode = 0;
	}

	virtual ~FITSHandler() {
		Close();
	}

	bool Close() {
		errcode = 0;
		if (fitsptr)  fits_close_file(fitsptr, &errcode);
		if (!errcode) fitsptr = NULL;
		return !errcode;
	}

	const char *GetError() {
		fits_get_errstatus(errcode, errmsg);
		return &errmsg[0];
	}

	/*!
	 * @brief 获得基于cfitsio的FITS文件访问接口
	 * @return
	 * FITS文件访问接口
	 */
	fitsfile* operator()() {
		return fitsptr;
	}

	/*!
	 * @brief 打开或创建FITS文件并获得基于cfitsio的FITS文件访问接口
	 * @param pathname  文件路径
	 * @param mode      0: 只读; 1: 读写; 2: 新文件
	 * @return
	 * 操作结果
	 */
	bool operator()(const char *filepath, const int mode = 0) {
		if (!Close()) return false;
		errcode = 0;
		if (mode == 0 || mode == 1) {
			fits_open_file(&fitsptr, filepath, mode, &errcode);
		}
		else {
			fits_create_file(&fitsptr, filepath, &errcode);
		}
		return !errcode;
	}
};
typedef FITSHandler HFITS;
typedef FITSHandler* HFITSPtr;

#endif
