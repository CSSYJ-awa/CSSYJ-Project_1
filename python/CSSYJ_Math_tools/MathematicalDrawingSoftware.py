# 数学作图软件

import sys
import json
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QFileDialog, QPushButton, QVBoxLayout, QWidget, QTextEdit, QLabel
)
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.patches import Circle
import numpy as np

class MathViewer(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("数学作图软件原型")
        self.figure = Figure()
        self.canvas = FigureCanvas(self.figure)
        self.text = QTextEdit()
        self.text.setReadOnly(True)
        self.step_label = QLabel("当前步骤：0")
        self.load_btn = QPushButton("加载资源文件")
        self.next_btn = QPushButton("下一步")
        self.prev_btn = QPushButton("上一步")
        self.load_btn.clicked.connect(self.load_file)
        self.next_btn.clicked.connect(self.next_step)
        self.prev_btn.clicked.connect(self.prev_step)

        layout = QVBoxLayout()
        layout.addWidget(self.canvas)
        layout.addWidget(self.step_label)
        layout.addWidget(self.text)
        layout.addWidget(self.load_btn)
        layout.addWidget(self.prev_btn)
        layout.addWidget(self.next_btn)

        container = QWidget()
        container.setLayout(layout)
        self.setCentralWidget(container)

        self.data = []
        self.current_step = 0
        self._zoom = 1.0
        self.canvas.mpl_connect('scroll_event', self.on_scroll)
    def on_scroll(self, event):
        # 鼠标滚轮缩放
        if event.button == 'up':
            self._zoom *= 1.2
        elif event.button == 'down':
            self._zoom /= 1.2
        self.show_step()

    def load_file(self):
        fname, _ = QFileDialog.getOpenFileName(self, "选择资源文件", "", "JSON Files (*.json)")
        if fname:
            with open(fname, "r", encoding="utf-8") as f:
                self.data = json.load(f)["steps"]
            self.current_step = 0
            self.show_step()

    def show_step(self):
        self.figure.clear()
        ax = self.figure.add_subplot(111)
        xlim, ylim = [0, 0], [0, 0]
        if self.data:
            step = self.data[self.current_step]
            for item in step["draw"]:
                if item["type"] == "line":  #线段
                    ax.plot(item["x"], item["y"], color="blue")
                elif item["type"] == "point": #点
                    ax.plot(item["x"], item["y"], "ro")
                elif item["type"] == "axes": #坐标轴
                    xlim = item.get("xlim", xlim)
                    ylim = item.get("ylim", ylim)
                    ax.axhline(0, color='black', linewidth=0.5)
                    ax.axvline(0, color='black', linewidth=0.5)
                elif item["type"] == "circle":  #圆
                    circle = Circle((item["center"][0], item["center"][1]), item["radius"], fill=False, color="green")
                    ax.add_patch(circle)
                elif item["type"] == "square":  #方
                    x0, y0 = item["x"][0], item["y"][0]
                    size = item["size"]
                    square_x = [x0, x0+size, x0+size, x0, x0]
                    square_y = [y0, y0, y0+size, y0+size, y0]
                    ax.plot(square_x, square_y, color="orange")
                elif item["type"] == "parallel":    #平行
                    x1, y1 = item["x1"], item["y1"]
                    x2, y2 = item["x2"], item["y2"]
                    offset = item.get("offset", 1)
                    ax.plot([x1, x2], [y1, y2], color="purple")
                    ax.plot([x1, x2], [y1+offset, y2+offset], color="purple", linestyle="--")
                elif item["type"] == "perpendicular":   #垂线
                    x, y = item["x"], item["y"]
                    dx, dy = item["dx"], item["dy"]
                    length = item.get("length", 2)
                    x2 = x + (-dy) * length
                    y2 = y + dx * length
                    ax.plot([x, x2], [y, y2], color="red", linestyle=":")
                elif item["type"] == "angle_bisector":  #角二等分线
                    x, y = item["x"], item["y"]
                    dx1, dy1 = item["dx1"], item["dy1"]
                    dx2, dy2 = item["dx2"], item["dy2"]
                    length = item.get("length", 2)
                    
                    v1 = np.array([dx1, dy1])
                    v2 = np.array([dx2, dy2])
                    bisector = v1/np.linalg.norm(v1) + v2/np.linalg.norm(v2)
                    bisector = bisector / np.linalg.norm(bisector)
                    x2 = x + bisector[0] * length
                    y2 = y + bisector[1] * length
                    ax.plot([x, x2], [y, y2], color="brown", linestyle="-.")
            # 缩放处理
            cx = (xlim[0] + xlim[1]) / 2
            cy = (ylim[0] + ylim[1]) / 2
            w = (xlim[1] - xlim[0]) / self._zoom
            h = (ylim[1] - ylim[0]) / self._zoom
            ax.set_xlim([cx - w/2, cx + w/2])
            ax.set_ylim([cy - h/2, cy + h/2])
            ax.set_aspect('equal')
            ax.axis('off')
            self.text.setText(f"因为：{step.get('because', '')}\n所以：{step.get('so', '')}")
            self.step_label.setText(f"当前步骤：{self.current_step + 1}/{len(self.data)}")
        self.canvas.draw()

    def next_step(self):
        if self.data and self.current_step < len(self.data) - 1:
            self.current_step += 1
            self.show_step()

    def prev_step(self):
        if self.data and self.current_step > 0:
            self.current_step -= 1
            self.show_step()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    viewer = MathViewer()
    viewer.show()
    sys.exit(app.exec_())