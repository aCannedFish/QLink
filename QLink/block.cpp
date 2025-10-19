#include "block.h"
#include <QDebug>

// 默认构造函数
Block::Block()
{

}

// 位置构造函数
// x: 方块的x坐标
// y: 方块的y坐标
// width: 方块宽度
// height: 方块高度
Block::Block(int x, int y, int width, int height)
{
	cord = QRectF(x, y, width, height); // 使用QRectF存储方块的坐标和尺寸
}

// 位置和形状构造函数
// x: 方块的x坐标
// y: 方块的y坐标
// width: 方块宽度
// height: 方块高度
// iniForm: 方块的初始形状类型
Block::Block(int x, int y, int width, int height, int iniForm)
{
	cord = QRectF(x, y, width, height); // 设置坐标和尺寸
	state = 1; // 设置默认状态为未激活
	form = iniForm; // 设置形状类型
}

// 完整构造函数
// x: 方块的x坐标
// y: 方块的y坐标
// width: 方块宽度
// height: 方块高度
// iniForm: 方块的初始形状类型
// iniState: 方块的初始状态
Block::Block(int x, int y, int width, int height, int iniForm, int iniState)
{
	cord = QRectF(x, y, width, height); // 设置坐标和尺寸
	state = iniState; // 设置指定状态
	form = iniForm; // 设置形状类型
}

// 析构函数
Block::~Block()
{

}

// 设置方块坐标
// x: 新的x坐标
// y: 新的y坐标
void Block::setCord(int x, int y)
{
	cord.setX(x); // 设置x坐标
	cord.setY(y); // 设置y坐标
}

// 获取方块状态
int Block::getState() const  
{  
	return state; // 返回当前状态
}

// 获取方块形状
int Block::getForm() const
{
	return form; // 返回形状类型
}

// 设置方块状态
void Block::setState(int newState)
{
	state = newState; // 更新状态值
}

// 设置方块形状
void Block::setForm(int newForm)
{
	form = newForm; // 更新形状类型
}

// 获取方块坐标
QRectF& Block::getCord()
{
	return cord; // 返回坐标矩形的引用
}

// 设置方块在地图中的逻辑坐标
// mx: 地图中的x坐标
// my: 地图中的y坐标
void Block::setMapXY(int mx, int my)
{
	mapX = mx; // 设置地图x坐标
	mapY = my; // 设置地图y坐标
}

// 获取方块在地图中的x坐标
int Block::getMapX() const 
{
	return mapX; // 返回地图x坐标
}

// 获取方块在地图中的y坐标
int Block::getMapY() const
{
	return mapY; // 返回地图y坐标
}
