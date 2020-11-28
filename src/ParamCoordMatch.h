/**
 * @file ParamCoordMatch.h
 * @brief 定义坐标匹配参数及其访问接口
 * @version 0.1
 * @date 2020-11-14
 * @author 卢晓猛
 * @note
 * - 以xml格式记录参数文件
 * - 基于boost实现访问接口
 * @note
 * 坐标匹配参数用途:
 * - 基于坐标和亮度建立用于匹配过程的模型
 * - 匹配过程控制参数
 * @note
 * 坐标系可以在相同框架或者不同框架, 例如:
 * - 相同框架: 图像XY坐标系一致性匹配
 * - 不同框架: 图像XY坐标系--WCS世界坐标系一致性匹配
 */

#ifndef _PARAM_COORD_MATCH_H_
#define _PARAM_COORD_MATCH_H_

#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

/*!
 * @struct ParamCoordMatch
 * @brief 定义坐标匹配参数及其访问接口
 */
struct ParamCoordMatch {
	/* 模型构建参数 */
	/*!
	 * @brief 世界坐标系像元比例尺下限, 角秒/像元
	 */
	double lowScale;
	/*!
	 * @brief 世界坐标系像元比例尺下限, 角秒/像元
	 */
	double highScale;
	/*!
	 * @brief 最小'点-点'距离, 无量纲
	 * @note
	 * 坐标系单位距离的倍数
	 */
	int minRange;
	/*!
	 * @brief 张角, 角度
	 */
	double fieldAngle;
	/*!
	 * @brief 构建模型的最小样本数量
	 * @note
	 * - 正整数
	 * - 不含中心和定向参考点
	 */
	int sampleCount;
	/*!
	 * @brief 'X'坐标的列索引. 索引从0开始
	 */
	int xIndex;
	/*!
	 * @brief 'Y'坐标的列索引. 索引从0开始
	 */
	int yIndex;
	/*!
	 * @brief 亮度的索引. 索引从0开始
	 * @note
	 * - 当索引==-1时, 不执行排序
	 */
	int fluxIndex;
	/*!
	 * @brief 亮度递减排序
	 * @note
	 * 样本按照从亮到暗排序.
	 * - 当亮度的flux对应星等时, 应选择递增排序
	 */
	bool sortDescend;
	/* 模型匹配参数 */
	/*!
	 * @brief 星表路径
	 */
	std::string pathCat;
	/*!
	 * @brief 匹配深度. 选择最亮的depth颗星构建匹配对象
	 */
	int depth;
	/*!
	 * @brief 归算距离偏差的最大阈值, 1/10000归算距离
	 */
	int diffUniRangeMax;
	/*!
	 * @brief 归算角度偏差的最大阈值, 1/100归算角度, 角度
	 */
	int diffUniAngleMax;
	/*!
	 * @brief 坐标系镜像标志
	 * -1: 参与匹配的两个坐标系为相同旋转方向, 即: 同为左手或右手系
	 * +1: 参与匹配的两个坐标系为相反旋转方向, 即: 一为左手系, 一为右手系
	 *  0: 不确定坐标系镜像关系, 分别尝试相同和相反方向
	 */
	int parity;
	/* 匹配结果输出 */
	/*!
	 * @brief 以WCS文件格式输出匹配模型
	 * @note
	 * 文件名格式: <source_file_title>.wcs
	 */
	bool outputWcs;
	/*!
	 * @brief 输出匹配结果
	 * @note
	 * 文件名格式: <source_file_title>.matched
	 */
	bool outputMatched;

protected:
	namespace PT = boost::property_tree;	///< 声明命名空间

public:
	/* 接口 */
	/*!
	 * @brief 构建缺省参数文件
	 * @param filepath  文件路径
	 */
	void InitFile(const std::string& filepath) {
		try {
			PT::ptree pt;

			pt.add("scale.<xmlattr>.low",         8.0);
			pt.add("scale.<xmlattr>.high",        12.0);
			pt.add("range.<xmlattr>.min",         100);
			pt.add("field.<xmlattr>.angle",       60.0);
			pt.add("sampleCount.<xmlattr>.value", 2);
			pt.add("colIndex.<xmlattr>.x",        0);
			pt.add("colIndex.<xmlattr>.y",        1);
			pt.add("colIndex.<xmlattr>.flux",     2);
			pt.add("fluxSort.<xmlattr>.descend", false);

			pt.add("catalogue.<xmlattr>.filePath", "");
			pt.add("depth", "10");
			pt.add("unitary.<xmlattr>.range", "0.01");
			pt.add("unitary.<xmlattr>.angle", "0.5");
			pt.add("parity", 0);

			pt.add("wcs.<xmlattr>.output", "");
			pt.add("matched.<xmlattr>.output", "");

			PT::xml_writer_settings<std::string> settings(' ', 4);
			PT::write_xml(filepath, pt, std::locale(), settings);
		}
		catch(PT::xml_parser_error& ex) {
		}
	}

	/*!
	 * @brief 从文件加载参数
	 * @param filepath  文件路径
	 * @return
	 * true:  加载成功
	 * false: 加载失败; 生成缺省文件
	 */
	bool LoadFile(const std::string& filepath) {
		try {
			PT::ptree pt;
			PT::read_xml(filepath, pt, boost::property_tree::xml_parser::trim_whitespace);

			lowScale  = pt.get("scale.<xmlattr>.low",   8.0);
			highScale = pt.get("scale.<xmlattr>.high", 12.0);
			minRange  = pt.get("range.<xmlattr>.min",  100);
			fieldAngle = pt.get("field.<xmlattr>.angle", 60.0);
			sampleCount = pt.get("sampleCount.<xmlattr>.value", 2);
			xIndex     = pt.get("colIndex.<xmlattr>.x",    0);
			yIndex     = pt.get("colIndex.<xmlattr>.y",    1);
			fluxIndex  = pt.get("colIndex.<xmlattr>.flux", 2);
			sortDescend= pt.get("fluxSort.<xmlattr>.descend", false);

			pathCat         = pt.get("catalogue.<xmlattr>.filePath", "");
			depth           = pt.get("depth", 10);
			diffUniRangeMax = pt.get("unitary.<xmlattr>.range",  "0.01");
			diffUniAngleMax = pt.get("unitary.<xmlattr>.angle",  "0.5");
			parity = pt.get("parity", 0);

			outputWcs     = pt.get("wcs.<xmlattr>.output",     true);
			outputMatched = pt.get("matched.<xmlattr>.output", true);

			if (lowScale < 0.1)  lowScale = 0.1;
			if (highScale > 1.5 * lowScale) highScale = 1.5 * lowScale;
			if (minRange < 50)   minRange = 50;
			if (sampleCount < 2) sampleCount = 2;

			return true;
		}
		catch(PT::xml_parser_error& ex) {
			InitFile(filepath);
			return false;
		}
	}
};

#endif
