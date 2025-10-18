#include "block.h"
#include <QDebug>

// 默认构造函数
// 创建一个空的方块对象，不设置任何属性
Block::Block()
{

}

// 位置构造函数
// x: 方块的x坐标
// y: 方块的y坐标
// width: 方块宽度
// height: 方块高度
// 创建一个指定位置和尺寸的方块，使用QRectF存储坐标信息
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
// 创建一个指定位置、尺寸和形状的方块，默认状态为未激活(1)
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
// 创建一个指定所有属性的方块，包括位置、尺寸、形状和状态
Block::Block(int x, int y, int width, int height, int iniForm, int iniState)
{
	cord = QRectF(x, y, width, height); // 设置坐标和尺寸
	state = iniState; // 设置指定状态
	form = iniForm; // 设置形状类型
}

// 析构函数
// 清理方块对象资源，由于没有动态分配的内存，此函数为空
Block::~Block()
{

}

// 设置方块坐标
// x: 新的x坐标
// y: 新的y坐标
// 更新方块的位置坐标，保持原有的宽度和高度不变
void Block::setCord(int x, int y)
{
	cord.setX(x); // 设置x坐标
	cord.setY(y); // 设置y坐标
}

// 获取方块状态
// 返回方块当前状态值
// 返回方块的状态：0: 已消除/空方块，1: 未激活方块，2: 激活方块
int Block::getState() const  
{  
	return state; // 返回当前状态
}

// 获取方块形状
// 返回方块形状类型值
// 返回方块的形状类型编号，用于匹配消除判断
int Block::getForm() const
{
	return form; // 返回形状类型
}

// 设置方块状态
// newState: 新的状态值
// 更新方块的状态，用于游戏逻辑控制
void Block::setState(int newState)
{
	state = newState; // 更新状态值
}

// 设置方块形状
// newForm: 新的形状类型
// 更新方块的形状类型，通常用于洗牌功能
void Block::setForm(int newForm)
{
	form = newForm; // 更新形状类型
}

// 获取方块坐标矩形
// 返回方块的坐标矩形引用
// 返回方块的QRectF坐标对象，包含位置和尺寸信息
QRectF& Block::getCord()
{
	return cord; // 返回坐标矩形的引用
}

// 设置方块在地图中的逻辑坐标
// mx: 地图中的x坐标
// my: 地图中的y坐标
// 设置方块在游戏地图中的逻辑位置，用于游戏逻辑计算
void Block::setMapXY(int mx, int my)
{
	mapX = mx; // 设置地图x坐标
	mapY = my; // 设置地图y坐标
}

// 获取方块在地图中的x坐标
// 返回地图中的x坐标
// 返回方块在游戏地图中的逻辑x坐标，用于路径查找算法
int Block::getMapX() const 
{
	return mapX; // 返回地图x坐标
}

// 获取方块在地图中的y坐标
// 返回地图中的y坐标
// 返回方块在游戏地图中的逻辑y坐标，用于路径查找算法
int Block::getMapY() const
{
	return mapY; // 返回地图y坐标
}