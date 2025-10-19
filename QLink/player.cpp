#include "player.h"

// 默认构造函数
Player::Player()
{

}

// 位置和激活状态构造函数
// x: 玩家的x坐标
// y: 玩家的y坐标
// isActive: 玩家的激活状态
Player::Player(int x, int y, int isActive)
	: cord(x, y, width, height), active(isActive)
{

}

// 获取玩家坐标
QRectF& Player::getCord()
{
	return cord;
}

// 增加玩家分数
void Player::addPoint(int exp)
{
	point += exp;
}

// 获取玩家分数
int Player::getPoint() const
{
	return point;
}

// 设置玩家分数
// newPoint: 新的分数值
void Player::changePoint(int newPoint)
{
	point = newPoint;
}

// 获取玩家在地图中的x坐标
int Player::getXInMap() const
{
	return xInMap;
}

// 获取玩家在地图中的y坐标
int Player::getYInMap() const
{
	return yInMap;
}

// 设置玩家在地图中的x坐标
void Player::setXInMap(int x)
{
	xInMap = x;
}

// 设置玩家在地图中的y坐标
void Player::setYInMap(int y)
{
	yInMap = y;
}

// 获取玩家激活状态
bool Player::getActive() const
{
	return active;
}

// 设置玩家激活状态
void Player::setActive(bool a)
{
	active = a;
}


