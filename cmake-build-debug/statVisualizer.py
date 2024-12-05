import numpy
import re
import PyQt6
import sys
from PyQt6.QtWidgets import QApplication, QWidget, QMainWindow, QPushButton, QLabel, QLineEdit, QVBoxLayout, QHBoxLayout, QTableWidget, QTableWidgetItem

file = open("C:/Users/User/CLionProjects/SMO/cmake-build-debug/users.txt")
file1 = open("C:/Users/User/CLionProjects/SMO/cmake-build-debug/devices.txt")
cnt = 0
cnt1 = 0

res = {}
res1 = {}



class UserTable(QMainWindow):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle(f"User table")
        self.setGeometry(400, 200, 800, 600)

        layout = QVBoxLayout()
        
        table = QTableWidget(self)  # Create a table
        table.setColumnCount(9)     #Set three columns
        table.setRowCount(cnt)        # and one row
 
        # Set the table headers
        table.setHorizontalHeaderLabels(["№ источника", "Количество заявок", "p", "Отказы", "Тпреб", "Тбп", "Тобсл", "Дбп", "Добсл"])

        table.horizontalHeaderItem(0).setToolTip("Column 1 ")
        table.horizontalHeaderItem(1).setToolTip("Column 2 ")
        table.horizontalHeaderItem(2).setToolTip("Column 3 ")
        table.horizontalHeaderItem(3).setToolTip("Column 4 ")
        table.horizontalHeaderItem(4).setToolTip("Column 5 ")
        table.horizontalHeaderItem(5).setToolTip("Column 6 ")
        
        for i, a in res.items():
            table.setItem(i, 0, QTableWidgetItem(str(i)))
            table.setItem(i, 1, QTableWidgetItem(str(a[0])))
            table.setItem(i, 2, QTableWidgetItem(str(a[1])))
            table.setItem(i, 3, QTableWidgetItem(str(a[2])))
            table.setItem(i, 4, QTableWidgetItem(str(a[3])))
            table.setItem(i, 5, QTableWidgetItem(str(a[4])))
            table.setItem(i, 6, QTableWidgetItem(str(a[5])))
            table.setItem(i, 7, QTableWidgetItem(str(a[6])))
            table.setItem(i, 8, QTableWidgetItem(str(a[7])))
        
 
        # Do the resize of the columns by content
        table.resizeColumnsToContents()
 
        layout.addWidget(table)   # Adding the table to the grid
        
        widget = QWidget()
        widget.setLayout(layout)

        self.setCentralWidget(widget)

class DeviceTable(QMainWindow):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle(f"Device table")
        self.setGeometry(400, 200, 800, 600)

        layout = QVBoxLayout()
        
        table = QTableWidget(self)  # Create a table
        table.setColumnCount(2)     #Set three columns
        table.setRowCount(cnt1)        # and one row
 
        # Set the table headers
        table.setHorizontalHeaderLabels(["№ прибора", "Коэффициент использования"])

        table.horizontalHeaderItem(0).setToolTip("Column 1 ")
        table.horizontalHeaderItem(1).setToolTip("Column 2 ")
        
        for i, a in res1.items():
            table.setItem(i, 0, QTableWidgetItem(str(i)))
            table.setItem(i, 1, QTableWidgetItem(str(a)))
        
 
        # Do the resize of the columns by content
        table.resizeColumnsToContents()
 
        layout.addWidget(table)   # Adding the table to the grid
        
        widget = QWidget()
        widget.setLayout(layout)

        self.setCentralWidget(widget)

if __name__ == "__main__":
    
    for line in file:
        cnt += 1
        elements = line.split()
        time1 = file.readline().split()[1:]
        waittime = []
        alltime = []
        for i in time1:
            waittime.append(int(i))
        time2 = file.readline().split()[2:]
        servicetime = []
        for i in time2:
            servicetime.append(int(i))
        for i in range(0, len(waittime)):
            #print(waittime[i], i)
            #print(servicetime[i], i)
            alltime.append(waittime[i] + servicetime[i])
        t1 = numpy.mean(alltime)
        t2 = numpy.mean(servicetime)
        t3 = numpy.mean(waittime)
        d1 = numpy.std(waittime)
        d2 = numpy.std(servicetime)
        
        res[int(elements[1])] = [elements[4], int(elements[8])/int(elements[4]), elements[8], t1, t2, t3, d1, d2]

    for line in file1:
        print(cnt1)
        cnt1 += 1
        data = line.split()
        res1[int(data[1][0])] = float(data[2])
        
    app = QApplication(sys.argv)
    table = UserTable()
    table.show()

    table1 = DeviceTable()
    table1.show()
    app.exec()
     
