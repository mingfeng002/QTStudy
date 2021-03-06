#ifndef IMAGEGRAPHICSVIEW_H
#define IMAGEGRAPHICSVIEW_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include <OpenCVTool.h>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QString>
#include <QPointF>
#include <QPoint>
#include <QCursor>
#include <QColor>
#include <QPixmap>
#include <QStack>
#include <QScrollBar>
#include <QDebug>

using namespace std;
using namespace cv;

class ImageGraphicsview : public QGraphicsView
{
     Q_OBJECT

public:
    enum ActionName{Pencil, Eraser, BigGlasses, SmallGlasses, OpenHand, ClosedHand, RectSelect, FreeSelect, SelectMove, Default, Forbidden};
    Mat maskUnion;//处理过的图片的maskMat的并集。

    ImageGraphicsview(QWidget *parent);
    ~ImageGraphicsview();
    void reset();//重置变量状态
    Mat saveCurrentMat(string filename);//保存当前图片到指定的路径
    bool isModified();
    void setModified(bool modified);

    bool isSaved();
    void setSaved(bool saved);

    void makeMaskUnion();


signals:
    void mouseMovetriggerSignal(QString location);
    void zoomUpPressed();
    void zoomDownPressed();
    void actionNameChanged(ActionName);
    void glassesChanged(bool isZoomUp);//放大镜工具改变信号

public slots:
    void zoomUp();
    void zoomDown();
    void setActionName(ActionName actionName);
    void setGlasses(bool flag);//改变放大镜工具状态
    void setPencilColor(QColor color);//设置铅笔工具颜色
    void setEraserColor(QColor color);//设置橡皮工具颜色
    void setWidth(int width);//设置线宽
    void setPixmapItem(QGraphicsPixmapItem* item);//设置当前scene中的图片项
    void setCurrentMat(Mat& m);//设置当前的Mat
    void undo();//撤销
    void redo();//恢复

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void actionHandDrag(QMouseEvent *event,QPointF point);//抓手移动实现
    void selectMoving(QMouseEvent *event,QPointF point);//拖动选中区域
    bool isInsideOfRoi(QPointF point);//判断鼠标是否在所选区域内
    inline void updatePixmapItem();//更新currentMat到PixmapItem
    inline void updateMaskItem();//更新maskMat到maskItem
    inline void updateSelcetItem();//更新selectMat到selectItem
    inline void initItem();//初始化所有透明Item
    void roiToMaskMat();//将选择的区域合成到图层中
    void roiCancellSelect();//取消选择的区域
    inline void clearUndoStack();//清空撤销区
    inline void clearRedoStack();//清空恢复区
    Rect minRect(vector<Point> movePoints);//计算点集的最外面矩形边界

private:
    ActionName currentActionName = Default;//记录当前选中的工具
    ActionName preAction;//前一次的action
    QPointF startPoint;//鼠标点击起始点
    QPointF endPoint;//鼠标释放点
    bool isZoomUp;//标记放大缩小的状态，使用的原因是QT的键盘响应事件存在问题，具体见：http://z632922970z.blog.163.com/blog/static/16316610320112245372844/
    double zoomUpRate;//放大倍率
    double zoomDownRate;//缩小倍率
    bool isPressed;//标记鼠标左键是否被按下

    bool modified; // 图片是否被编辑过（判断是否保存的依据）
    bool saved; // 图片是否已保存

    QCursor bigCursor;//放大镜鼠标样式
    QCursor smallCursor;//缩小镜鼠标样式
    QCursor pencilCursor;//铅笔鼠标样式
    QCursor eraserCursor;//橡皮鼠标样式
    QCursor forbiddenCursor;//禁止鼠标样式
    QPixmap pixmap;//当前的图片Pixmap
    QGraphicsPixmapItem *pixmapItem;//当前scene中的图片项

    Mat currentMat;//当前的图片Mat
    int thickness;//画笔粗细

    QPixmap roiPixmap;//选择工具所选的区域Pixmap
    Mat roiMat;//选择工具所选的区域Mat
    QGraphicsPixmapItem *roiItem;//选择工具所选的区域Item
    bool isRoiMoved;//所选区域在选中后是否被移动过

    vector<Point> movePoints;//自由选择工具在移动时经过的点的集合
    QPointF prePoint;//移动的前一个点
    Mat binaryMat;//掩膜二值图像
    QPoint oriStartPoint;//最初始的点击点

    QPixmap selectPixmap;//选择工具绘制图层
    Mat selectMat;//选择工具绘制Mat
    QGraphicsPixmapItem *selectItem;//选择工具绘制Item

    QPixmap maskPixmap;//图层Pixmap
    Mat maskMat;//图层Mat
    QGraphicsPixmapItem *maskItem;//图层Item


    Scalar pencilColor;//铅笔颜色（前景色）
    Scalar backColor;//背景色
    Scalar eraserColor;//橡皮颜色

    QStack<Mat> undoStack;//撤销
    QStack<Mat> redoStack;//恢复

    OpenCVTool opencvTool;//Opencv工具类
};

#endif // IMAGEGRAPHICSVIEW_H
