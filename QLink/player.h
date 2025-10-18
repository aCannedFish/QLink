// player.h
// 游戏玩家类头文件
// 定义连连看游戏中的玩家类，包含玩家的位置、分数、状态等属性

#pragma once
#include <QWidget>
#include <QRectF>

// 游戏玩家类
// 继承自QWidget，表示连连看游戏中的一个玩家
// 包含玩家的位置坐标、分数、激活状态等属性
class Player : public QWidget
{
	Q_OBJECT
public:
	// 默认构造函数
	// 创建一个空的玩家对象
	Player();
	
	// 位置和激活状态构造函数
	// x: 玩家的x坐标
	// y: 玩家的y坐标
	// isActive: 玩家的激活状态
	// 创建一个指定位置和激活状态的玩家
	Player(int x, int y, int isActive);
	
	// 获取玩家坐标矩形
	// 返回玩家的坐标矩形引用
	// 返回玩家的QRectF坐标对象，包含位置和尺寸信息
	QRectF& getCord();
	
	// 增加玩家分数
	// exp: 增加的分数值
	// 在玩家当前分数基础上增加指定分数
	void addPoint(int exp);
	
	// 获取玩家分数
	// 返回玩家当前分数
	// 返回玩家的当前累计分数
	int getPoint() const;
	
	// 设置玩家分数
	// newPoint: 新的分数值
	// 直接设置玩家的分数为指定值
	void changePoint(int newPoint);
	
	// 获取玩家在地图中的x坐标
	// 返回地图中的x坐标
	// 返回玩家在游戏地图中的逻辑x坐标
	int getXInMap() const;
	
	// 获取玩家在地图中的y坐标
	// 返回地图中的y坐标
	// 返回玩家在游戏地图中的逻辑y坐标
	int getYInMap() const;
	
	// 设置玩家在地图中的x坐标
	// x: 新的x坐标
	// 更新玩家在游戏地图中的逻辑x坐标
	void setXInMap(int x);
	
	// 设置玩家在地图中的y坐标
	// y: 新的y坐标
	// 更新玩家在游戏地图中的逻辑y坐标
	void setYInMap(int y);
	
	// 获取玩家激活状态
	// 返回玩家是否激活
	// 返回玩家的激活状态，用于控制玩家操作权限
	bool getActive() const;
	
	// 设置玩家激活状态
	// a: 新的激活状态
	// 更新玩家的激活状态，控制玩家是否可以操作
	void setActive(bool a);

private:
	QRectF cord;        // 玩家的坐标矩形，存储像素坐标位置
	int point = 0;      // 玩家当前分数
	int width = 50;     // 玩家宽度（像素）
	int height = 50;    // 玩家高度（像素）
	int active = 0;     // 玩家激活状态：0为未激活，1为激活
	int xInMap = 0;     // 玩家在地图中的逻辑x坐标
	int yInMap = 0;     // 玩家在地图中的逻辑y坐标
};