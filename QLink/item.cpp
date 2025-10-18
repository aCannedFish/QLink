// 游戏道具类实现文件
// 实现连连看游戏中道具类的各种功能，包括构造、属性管理和绘制

#include "item.h"
#include <QPainter>

// 默认构造函数
// 创建一个默认的道具对象：
// - 类型设置为AddTime
// - 位置设置为(0,0)
// - 矩形设置为空
// - 图标为空
// - 可见性设置为false
Item::Item()
    : type(ItemType::AddTime), mapPos(0, 0), rect(0, 0, 0, 0), pixmap(), visible(false)
{}

// 完整构造函数
// type: 道具类型
// mapPos: 道具在地图中的逻辑位置
// rect: 道具的像素坐标矩形
// pixmap: 道具的图标
// 创建一个指定属性的道具对象，可见性默认为true
Item::Item(ItemType type, const QPoint& mapPos, const QRectF& rect, const QPixmap& pixmap)
    : type(type), mapPos(mapPos), rect(rect), pixmap(pixmap), visible(true)
{}

// 虚析构函数
// 清理道具对象资源，由于没有动态分配的内存，此函数为空
Item::~Item() {}

// 获取道具类型
// 返回道具类型枚举值
// 返回道具的类型，用于判断道具效果
ItemType Item::getType() const { return type; }

// 设置道具类型
// t: 新的道具类型
// 更新道具的类型
void Item::setType(ItemType t) { type = t; }

// 获取道具在地图中的位置
// 返回道具的逻辑坐标
// 返回道具在游戏地图中的逻辑位置
QPoint Item::getMapPos() const { return mapPos; }

// 设置道具在地图中的位置
// pos: 新的逻辑坐标
// 更新道具在游戏地图中的逻辑位置
void Item::setMapPos(const QPoint& pos) { mapPos = pos; }

// 获取道具的像素坐标矩形
// 返回道具的像素坐标矩形
// 返回道具在屏幕上的像素坐标和尺寸
QRectF Item::getRect() const { return rect; }

// 设置道具的像素坐标矩形
// r: 新的像素坐标矩形
// 更新道具在屏幕上的像素坐标和尺寸
void Item::setRect(const QRectF& r) { rect = r; }

// 检查道具是否可见
// 返回道具是否可见
// 返回道具的可见性状态
bool Item::isVisible() const { return visible; }

// 设置道具可见性
// v: 新的可见性状态
// 更新道具的可见性，控制道具是否显示
void Item::setVisible(bool v) { visible = v; }

// 获取道具图标
// 返回道具的图标
// 返回道具的图标，用于绘制显示
QPixmap Item::getPixmap() const { return pixmap; }

// 设置道具图标
// p: 新的图标
// 更新道具的图标
void Item::setPixmap(const QPixmap& p) { pixmap = p; }

// 绘制道具
// painter: 绘图设备
// 在指定的绘图设备上绘制道具图标：
// 1. 检查道具是否可见且图标有效
// 2. 计算道具在屏幕上的绘制位置（居中显示）
// 3. 将图标缩放到指定尺寸并绘制
// 4. 使用平滑变换确保图标质量
void Item::draw(QPainter& painter) const {
    if (visible && !pixmap.isNull()) {
        int w = 46, h = 46; // 道具图标的固定尺寸
        int cx = rect.x() + rect.width() / 2;  // 计算道具矩形的中心x坐标
        int cy = rect.y() + rect.height() / 2; // 计算道具矩形的中心y坐标
        QRect targetRect(cx - w/2, cy - h/2, w, h); // 创建目标绘制矩形（居中）
        painter.drawPixmap(targetRect, pixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

// 触发道具效果
// gameWidget: 游戏窗口指针（未使用）
// 触发道具的具体效果，具体实现由子类或主逻辑完成
// 这是一个虚函数，基类中为空实现
void Item::triggerEffect(QWidget* /*gameWidget*/) {
    // 具体效果由主逻辑或子类实现
}