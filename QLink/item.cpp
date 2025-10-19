#include "item.h"
#include <QPainter>

// 默认构造函数
Item::Item()
    : type(ItemType::AddTime), mapPos(0, 0), rect(0, 0, 0, 0), pixmap(), visible(false)
{}

// 完整构造函数
// type: 道具类型
// mapPos: 道具在地图中的逻辑位置
// rect: 道具的像素坐标矩形
// pixmap: 道具的图标
Item::Item(ItemType type, const QPoint& mapPos, const QRectF& rect, const QPixmap& pixmap)
    : type(type), mapPos(mapPos), rect(rect), pixmap(pixmap), visible(true)
{}

// 虚析构函数
Item::~Item() {}

// 获取道具类型
ItemType Item::getType() const { return type; }

// 设置道具类型
// t: 新的道具类型
void Item::setType(ItemType t) { type = t; }

// 获取道具在地图中的位置
QPoint Item::getMapPos() const { return mapPos; }

// 设置道具在地图中的位置
void Item::setMapPos(const QPoint& pos) { mapPos = pos; }

// 获取道具的像素坐标
QRectF Item::getRect() const { return rect; }

// 设置道具的像素坐标
void Item::setRect(const QRectF& r) { rect = r; }

// 检查道具是否可见
bool Item::isVisible() const { return visible; }

// 设置道具可见性
void Item::setVisible(bool v) { visible = v; }

// 获取道具图标
QPixmap Item::getPixmap() const { return pixmap; }

// 设置道具图标
void Item::setPixmap(const QPixmap& p) { pixmap = p; }

// 绘制道具
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
void Item::triggerEffect(QWidget* /*gameWidget*/) {
    // 具体效果由主逻辑或子类实现
}
