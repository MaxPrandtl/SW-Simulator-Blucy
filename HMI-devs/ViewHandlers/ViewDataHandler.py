from PySide6.QtCore import QPoint, QRect, QSize, Qt
from PySide6.QtGui import QBrush, QPainter, QPainterPath
from controller.Controller import Controller
from model.AHRSDataModel import AHRSDataModel
from model.MessageModel import MessageModel


# Dedicato alla creazione dell'interfaccia grafica relativa all'orizzonte artificiale
# e all'aggiornamento delle label di stato
class ViewDataHandler:
    def __init__(self, view, controller: Controller):
        self.view = view

        self.ahrs_data: AHRSDataModel = controller.data_model.ahrs
        self.psi_reference = controller.data_model.reference.psi

        self.boat_position_spinbox = self.view.boat_position_spinbox
        self.boat_position_spinbox.valueChanged.connect(self.__update_board_position)

        self.view.compass_canvas.paintEvent = self.paint_compass
        self.view.pitch_canvas.paintEvent = self.paint_pitch
        self.view.roll_canvas.paintEvent = self.paint_roll

    def __update_board_position(self, value: MessageModel):
        self.ahrs_data.board_position_angle = value
        self.view.compass_canvas.update()

    def update_compass(self):
        self.view.heading_label.setText("%.2f°" % self.ahrs_data.heading.value)
        self.view.state_var_heading.setText("%.2f" % self.ahrs_data.heading.value)
        self.view.compass_canvas.update()

    def update_compass_reference(self):
        # TODO: aggiornare/aggiungere la label di reference
        self.view.compass_canvas.update()

    def update_pitch(self):
        self.view.pitch_label.setText("%.2f°" % self.ahrs_data.pitch.value)
        self.view.state_var_pitch.setText("%.2f" % self.ahrs_data.pitch.value)
        self.view.pitch_canvas.update()

    def update_roll(self):
        # self.ahrs_data.roll = r
        self.view.roll_label.setText("%.2f°" % self.ahrs_data.roll.value)
        self.view.state_var_roll.setText("%.2f" % self.ahrs_data.roll.value)
        self.view.roll_canvas.update()

    # Print event orizonte artificiale

    def paint_compass(self, e):
        SIZE = self.view.compass_canvas.width()
        IMAGE_SIZE = SIZE / 2
        IMAGE_COORD = IMAGE_SIZE / 2
        HALF_SIZE = SIZE / 2

        painter = QPainter(self.view.compass_canvas)
        painter.setRenderHints(QPainter.SmoothPixmapTransform)
        painter.translate(HALF_SIZE, HALF_SIZE)
        painter.drawEllipse(QPoint(0, 0), HALF_SIZE - 0.2 * HALF_SIZE, HALF_SIZE - 0.2 * HALF_SIZE)
        painter.drawText(
            QRect(
                QPoint(-HALF_SIZE, HALF_SIZE - 0.2 * HALF_SIZE),
                QSize(SIZE, 0.2 * HALF_SIZE),
            ),
            Qt.AlignCenter,
            "S",
        )
        painter.drawText(
            QRect(QPoint(-HALF_SIZE, -HALF_SIZE), QSize(SIZE, 0.2 * HALF_SIZE)),
            Qt.AlignCenter,
            "N",
        )
        painter.drawText(
            QRect(
                QPoint(HALF_SIZE - 0.2 * HALF_SIZE, -HALF_SIZE),
                QSize(0.2 * HALF_SIZE, SIZE),
            ),
            Qt.AlignCenter,
            "E",
        )
        painter.drawText(
            QRect(QPoint(-HALF_SIZE, -HALF_SIZE), QSize(0.2 * HALF_SIZE, SIZE)),
            Qt.AlignCenter,
            "W",
        )

        # print small lines rapresenting angle in degrees 30 steps
        for i in range(0, 360, 30):
            painter.rotate(i)
            painter.drawLine(0, -HALF_SIZE + 0.2 * HALF_SIZE - 3, 0, -HALF_SIZE + 0.2 * HALF_SIZE + 3)
            # Draw extrnal label
            if i % 90 == 0:
                pass
            else:
                painter.drawText(
                    QRect(QPoint(-HALF_SIZE, -HALF_SIZE), QSize(SIZE, 0.2 * HALF_SIZE)),
                    Qt.AlignCenter,
                    "%d°" % i,
                )
            painter.rotate(-i)

        painter.rotate(self.ahrs_data.heading.value + 90)
        painter.drawLine(0, 0, -HALF_SIZE + 0.2 * HALF_SIZE, 0)
        painter.drawPixmap(-IMAGE_COORD, -IMAGE_COORD, IMAGE_SIZE, IMAGE_SIZE, self.view.BLUCY_UP)

        # Draw boat position
        boat_circle = QPainterPath()
        painter.rotate(-self.ahrs_data.heading.value - 90 + self.ahrs_data.board_position_angle)
        boat_circle.addEllipse(-8, -HALF_SIZE + 0.2 * HALF_SIZE, 16, 16)
        painter.fillPath(boat_circle, QBrush(Qt.red))
        painter.drawPath(boat_circle)

        # Draw reference
        reference_circle = QPainterPath()
        painter.rotate(-self.ahrs_data.board_position_angle + self.psi_reference.value)
        reference_circle.addEllipse(-8, -HALF_SIZE + 0.2 * HALF_SIZE, 16, 16)
        painter.fillPath(reference_circle, QBrush(Qt.yellow))
        painter.drawPath(reference_circle)

    def paint_pitch(self, e):
        SIZE = self.view.pitch_canvas.width()
        IMAGE_SIZE = SIZE / 2
        IMAGE_COORD = IMAGE_SIZE / 2
        HALF_SIZE = SIZE / 2

        painter = QPainter(self.view.pitch_canvas)
        painter.setRenderHints(QPainter.SmoothPixmapTransform)
        painter.translate(HALF_SIZE, HALF_SIZE)
        painter.drawEllipse(QPoint(0, 0), HALF_SIZE - 0.2 * HALF_SIZE, HALF_SIZE - 0.2 * HALF_SIZE)

        for i in range(0, 360, 30):
            painter.rotate(i - 90)
            text = i if i <= 180 else i - 360
            painter.drawLine(0, -HALF_SIZE + 0.2 * HALF_SIZE - 3, 0, -HALF_SIZE + 0.2 * HALF_SIZE + 3)
            # Mirror text if it is in the negative quadrant

            painter.drawText(
                QRect(QPoint(-HALF_SIZE, -HALF_SIZE), QSize(SIZE, 0.2 * HALF_SIZE)),
                Qt.AlignCenter,
                "%d°" % text,
            )

            painter.rotate(-i + 90)

        # Draw line at 45°
        painter.rotate(45)
        painter.drawLine(0, 0, -HALF_SIZE + 0.2 * HALF_SIZE, 0)
        painter.rotate(-90)
        painter.drawLine(0, 0, -HALF_SIZE + 0.2 * HALF_SIZE, 0)
        painter.rotate(45)

        painter.rotate(self.ahrs_data.pitch.value)
        painter.drawLine(0, 0, -HALF_SIZE + 0.2 * HALF_SIZE, 0)
        painter.drawPixmap(-IMAGE_COORD, -IMAGE_COORD, IMAGE_SIZE, IMAGE_SIZE, self.view.BLUCY_SIDE)

    def paint_roll(self, e):
        SIZE = self.view.roll_canvas.width()
        IMAGE_SIZE = SIZE / 2
        IMAGE_COORD = IMAGE_SIZE / 2
        HALF_SIZE = SIZE / 2

        painter = QPainter(self.view.roll_canvas)
        painter.setRenderHints(QPainter.SmoothPixmapTransform)
        painter.translate(HALF_SIZE, HALF_SIZE)
        painter.drawEllipse(QPoint(0, 0), HALF_SIZE - 0.2 * HALF_SIZE, HALF_SIZE - 0.2 * HALF_SIZE)

        for i in range(0, 360, 30):
            painter.rotate(i)
            text = i if i <= 180 else i - 360
            painter.drawLine(0, -HALF_SIZE + 0.2 * HALF_SIZE - 3, 0, -HALF_SIZE + 0.2 * HALF_SIZE + 3)
            painter.drawText(
                QRect(QPoint(-HALF_SIZE, -HALF_SIZE), QSize(SIZE, 0.2 * HALF_SIZE)),
                Qt.AlignCenter,
                "%d°" % text,
            )
            painter.rotate(-i)

        painter.rotate(self.ahrs_data.roll.value)
        painter.drawLine(0, 0, 0, -HALF_SIZE + 0.2 * HALF_SIZE)
        painter.drawPixmap(-IMAGE_COORD, -IMAGE_COORD, IMAGE_SIZE, IMAGE_SIZE, self.view.BLUCY_BACK)
