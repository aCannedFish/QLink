#include "player.h"

// 默认构造函数
// 创建一个空的玩家对象，不设置任何属性
Player::Player()
{

}

// 位置和激活状态构造函数
// x: 玩家的x坐标
// y: 玩家的y坐标
// isActive: 玩家的激活状态
// 创建一个指定位置和激活状态的玩家，使用成员初始化列表设置坐标和激活状态
Player::Player(int x, int y, int isActive)
	: cord(x, y, width, height), active(isActive)
{

}

// 获取玩家坐标矩形
// 返回玩家的坐标矩形引用
// 返回玩家的QRectF坐标对象，包含位置和尺寸信息
QRectF& Player::getCord()
{
	return cord;
}

// 增加玩家分数
// exp: 增加的分数值
// 在玩家当前分数基础上增加指定分数，用于游戏计分
void Player::addPoint(int exp)
{
	point += exp;
}

// 获取玩家分数
// 返回玩家当前分数
// 返回玩家的当前累计分数，用于显示和游戏逻辑判断
int Player::getPoint() const
{
	return point;
}

// 设置玩家分数
// newPoint: 新的分数值
// 直接设置玩家的分数为指定值，通常用于重置分数或加载存档
void Player::changePoint(int newPoint)
{
	point = newPoint;
}

// 获取玩家在地图中的x坐标
// 返回地图中的x坐标
// 返回玩家在游戏地图中的逻辑x坐标，用于游戏逻辑计算
int Player::getXInMap() const
{
	return xInMap;
}

// 获取玩家在地图中的y坐标
// 返回地图中的y坐标
// 返回玩家在游戏地图中的逻辑y坐标，用于游戏逻辑计算
int Player::getYInMap() const
{
	return yInMap;
}

// 设置玩家在地图中的x坐标
// x: 新的x坐标
// 更新玩家在游戏地图中的逻辑x坐标，用于玩家移动
void Player::setXInMap(int x)
{
	xInMap = x;
}

// 设置玩家在地图中的y坐标
// y: 新的y坐标
// 更新玩家在游戏地图中的逻辑y坐标，用于玩家移动
void Player::setYInMap(int y)
{
	yInMap = y;
}

// 获取玩家激活状态
// 返回玩家是否激活
// 返回玩家的激活状态，用于控制玩家操作权限
// 在双人模式中，只有激活的玩家才能进行操作
bool Player::getActive() const
{
	return active;
}

// 设置玩家激活状态
// a: 新的激活状态
// 更新玩家的激活状态，控制玩家是否可以操作
// 在双人模式中，用于切换当前可操作的玩家
void Player::setActive(bool a)
{
	active = a;
}


