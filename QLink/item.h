#pragma once
#include <QPixmap>
#include <QRectF>
#include <QPoint>
#include <QWidget>

// 定义游戏中所有可用的道具类型
enum class ItemType {
    AddTime,    // 增加时间道具：+1秒
    Shuffle,    // 洗牌道具：重新排列所有方块
    Hint,       // 提示道具：高亮显示一对可消除的方块
    Flash,      // 闪现道具：允许鼠标点击移动
    Freeze,     // 冻结道具：冻结对手操作
    Dizzy       // 眩晕道具：使对手方向控制颠倒
};

// 游戏道具类
// 表示游戏中的一个道具，包含道具类型、位置、图标等属性
// 提供道具的绘制和效果触发功能
class Item {
public:
    // 默认构造函数
    // 创建一个默认的道具对象，类型为AddTime，不可见
    Item();
    
    // 完整构造函数
    // type: 道具类型
    // mapPos: 道具在地图中的逻辑位置
    // rect: 道具的像素坐标矩形
    // pixmap: 道具的图标
    // 创建一个指定属性的道具对象
    Item(ItemType type, const QPoint& mapPos, const QRectF& rect, const QPixmap& pixmap);
    
    // 虚析构函数
    // 清理道具对象资源
    virtual ~Item();

    // 获取道具类型
    // 返回道具类型枚举值
    // 返回道具的类型，用于判断道具效果
    ItemType getType() const;
    
    // 设置道具类型
    // type: 新的道具类型
    // 更新道具的类型
    void setType(ItemType type);

    // 获取道具在地图中的位置
    // 返回道具的逻辑坐标
    // 返回道具在游戏地图中的逻辑位置
    QPoint getMapPos() const;
    
    // 设置道具在地图中的位置
    // pos: 新的逻辑坐标
    // 更新道具在游戏地图中的逻辑位置
    void setMapPos(const QPoint& pos);

    // 获取道具的像素坐标矩形
    // 返回道具的像素坐标矩形
    // 返回道具在屏幕上的像素坐标和尺寸
    QRectF getRect() const;
    
    // 设置道具的像素坐标矩形
    // rect: 新的像素坐标矩形
    // 更新道具在屏幕上的像素坐标和尺寸
    void setRect(const QRectF& rect);

    // 检查道具是否可见
    // 返回道具是否可见
    // 返回道具的可见性状态
    bool isVisible() const;
    
    // 设置道具可见性
    // visible: 新的可见性状态
    // 更新道具的可见性，控制道具是否显示
    void setVisible(bool visible);

    // 获取道具图标
    // 返回道具的图标
    // 返回道具的图标，用于绘制显示
    QPixmap getPixmap() const;
    
    // 设置道具图标
    // pixmap: 新的图标
    // 更新道具的图标
    void setPixmap(const QPixmap& pixmap);

    // 绘制道具
    // painter: 绘图设备
    // 在指定的绘图设备上绘制道具图标
    // 只有当道具可见且图标有效时才进行绘制
    virtual void draw(QPainter& painter) const;

    // 触发道具效果
    // gameWidget: 游戏窗口指针
    // 触发道具的具体效果，具体实现由子类或主逻辑完成
    virtual void triggerEffect(QWidget* gameWidget);

protected:
    ItemType type;      // 道具类型
    QPoint mapPos;      // 道具在地图中的逻辑坐标
    QRectF rect;        // 道具的像素坐标矩形
    QPixmap pixmap;     // 道具的图标
    bool visible;       // 道具是否可见
};
