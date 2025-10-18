// block.h
// 游戏方块类头文件
// 定义连连看游戏中的方块类，包含方块的位置、状态、形状等属性

#pragma once
#include <QWidget>
#include <QRectF>

// 游戏方块类
// 继承自QWidget，表示连连看游戏中的一个方块
// 包含方块的位置坐标、状态、形状类型等属性
class Block : public QWidget
{
	Q_OBJECT
public:
	// 默认构造函数
	// 创建一个空的方块对象
	Block();
	
	// 位置构造函数
	// x: 方块的x坐标
	// y: 方块的y坐标
	// width: 方块宽度
	// height: 方块高度
	// 创建一个指定位置和尺寸的方块
	Block(int x, int y, int width, int height);
	
	// 位置和形状构造函数
	// x: 方块的x坐标
	// y: 方块的y坐标
	// width: 方块宽度
	// height: 方块高度
	// iniForm: 方块的初始形状类型
	// 创建一个指定位置、尺寸和形状的方块
	Block(int x, int y, int width, int height, int iniForm);
	
	// 完整构造函数
	// x: 方块的x坐标
	// y: 方块的y坐标
	// width: 方块宽度
	// height: 方块高度
	// iniForm: 方块的初始形状类型
	// iniState: 方块的初始状态
	// 创建一个指定所有属性的方块
	Block(int x, int y, int width, int height, int iniForm, int iniState);
	
	// 析构函数
	// 清理方块对象资源
	~Block();
	
	// 交换两个方块的位置
	// b1: 第一个方块
	// b2: 第二个方块
	// 交换两个方块的坐标位置
	void swap(Block& b1, Block& b2);
	
	// 设置方块坐标
	// x: 新的x坐标
	// y: 新的y坐标
	// 更新方块的位置坐标
	void setCord(int x, int y);
	
	// 获取方块状态
	// 返回方块当前状态值
	// 返回方块的状态：0为已消除/空方块，1为未激活方块，2为激活方块
	int getState() const;
	
	// 获取方块形状
	// 返回方块形状类型值
	// 返回方块的形状类型编号
	int getForm() const;
	
	// 设置方块状态
	// newState: 新的状态值
	// 更新方块的状态
	void setState(int newState);
	
	// 设置方块形状
	// form: 新的形状类型
	// 更新方块的形状类型
	void setForm(int form);
	
	// 获取方块坐标矩形
	// 返回方块的坐标矩形引用
	// 返回方块的QRectF坐标对象
	QRectF& getCord();
	
	// 友元函数：交换两个方块
	// b1: 第一个方块
	// b2: 第二个方块
	// 全局函数，用于交换两个方块的位置
	friend void swap(Block& b1, Block& b2);
	
	// 设置方块在地图中的逻辑坐标
	// mx: 地图中的x坐标
	// my: 地图中的y坐标
	// 设置方块在游戏地图中的逻辑位置
	void setMapXY(int mx, int my);
	
	// 获取方块在地图中的x坐标
	// 返回地图中的x坐标
	// 返回方块在游戏地图中的逻辑x坐标
	int getMapX() const;
	
	// 获取方块在地图中的y坐标
	// 返回地图中的y坐标
	// 返回方块在游戏地图中的逻辑y坐标
	int getMapY() const;

private:
	QRectF cord;        // 方块的坐标矩形，存储像素坐标位置
	int state = 1;      // 方块状态：0为已消除/空方块，1为未激活方块，2为激活方块
	int form;           // 方块形状类型，用于匹配消除
	int mapX = -1;      // 方块在地图中的逻辑x坐标
	int mapY = -1;      // 方块在地图中的逻辑y坐标
};