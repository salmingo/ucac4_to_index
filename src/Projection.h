/**
 * @file Projection.h
 * @brief 定义投影及接口
 * @version 0.1
 * @date 2020-11-15
 * @author 卢晓猛
 * @note
 * 投影类型:
 * - 正视投影: TAN投影, 天文常用
 */

#ifndef PROJECTION_H_
#define PROJECTION_H_

#include <functional>

class Projection {
public:
	Projection();
	virtual ~Projection();

public:
	/*!
	 * @brief 投影类型
	 */
	enum {
		PRJ_TAN,	///< 正视投影
		PRJ_MAX
	};

protected:
	double l0;	///< 参考点经度, 弧度
	double b0;	///< 参考点纬度, 弧度
	double sl0, cl0;	///< l0的正弦和余弦
	double sb0, cb0;	///< b0的正弦和余弦
	int type;	///< 投影类型
	/*!
	 * @brief 正向投影函数: 球面=>平面
	 * @param _1  经度, 弧度
	 * @param _2  纬度, 弧度
	 * @param _3  X, 弧度
	 * @param _4  Y, 弧度
	 * @return
	 * 0: 成功
	 * 1: 与参考点距离太远
	 * 2: 目标在反向方向
	 * 3: 目标在反向方向且与参考点距离太远
	 */
	std::function<int(double,double,double&,double&)> prjfwd;
	/*!
	 * @brief 反向投影函数: 平面=>球面
	 * @param _1  X, 弧度
	 * @param _2  Y, 弧度
	 * @param _3  经度, 弧度
	 * @param _4  纬度, 弧度
	 */
	std::function<void(double,double,double&,double&)> prjrev;

	/////////////////////////////////////////////////////////////////////////
protected:
	/*!
	 * @brief 正视投影: 正向
	 */
	int fwd_tan(double l, double b, double& xi, double& eta);
	/*!
	 * @brief 正视投影: 反向
	 */
	void rev_tan(double xi, double eta, double& l, double& b);

	/////////////////////////////////////////////////////////////////////////
public:
	/*!
	 * @brief 设置投影类型
	 * @param type  投影类型
	 * @return
	 * 操作结果
	 */
	bool SetType(int type);
	/*!
	 * @brief 设置投影参考点
	 * @param l  参考点经度, 弧度
	 * @param b  参考点纬度, 弧度
	 */
	void SetPoint(double l, double b);
	/*!
	 * @brief 正向投影: 球面=>平面
	 * @param l    经度, 弧度
	 * @param b    纬度, 弧度
	 * @param xi   投影平面X, 弧度
	 * @param eta  投影平面Y, 弧度
	 * @return
	 * 投影结果
	 */
	int Forward(double l, double b, double& xi, double& eta);
	/*!
	 * @brief 反向投影: 平面=>球面
	 * @param xi   投影平面X, 弧度
	 * @param eta  投影平面Y, 弧度
	 * @param l    经度, 弧度
	 * @param b    纬度, 弧度
	 */
	void Reverse(double xi, double eta, double& l, double& b);
};

#endif /* PROJECTION_H_ */
