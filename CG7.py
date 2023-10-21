import tkinter as tk
from tkinter import Canvas

class BezierCurve(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Построение плоских полиномиальных кривых")
        self.geometry("800x600")
        self.canvas = Canvas(self, bg='white')
        self.canvas.pack(fill=tk.BOTH, expand=True)
        self.points = []
        self.canvas.bind('<Button-1>', self.add_point)
        self.canvas.bind('<B1-Motion>', self.move_point)
        self.active_point = None

    def add_point(self, event):
        if len(self.points) < 4:
            point = (event.x, event.y)
            self.points.append(point)
            self.canvas.create_oval(event.x-5, event.y-5, event.x+5, event.y+5, fill="red", tags="point")
            if len(self.points) == 4:
                self.draw_curve()

    def move_point(self, event):
        close_point = None
        min_distance = float('inf')
        for index, point in enumerate(self.points):
            distance = (point[0]-event.x)**2 + (point[1]-event.y)**2
            if distance < min_distance:
                min_distance = distance
                close_point = index

        if min_distance < 50:
            self.active_point = close_point
            self.points[close_point] = (event.x, event.y)
            self.canvas.delete("curve")
            self.canvas.delete("point")
            for point in self.points:
                self.canvas.create_oval(point[0]-5, point[1]-5, point[0]+5, point[1]+5, fill="red", tags="point")
            self.draw_curve()

    def bezier_curve(self, t):
        P0, P1, P2, P3 = self.points
        x = (1-t)**3*P0[0] + 3*(1-t)**2*t*P1[0] + 3*(1-t)*t**2*P2[0] + t**3*P3[0]
        y = (1-t)**3*P0[1] + 3*(1-t)**2*t*P1[1] + 3*(1-t)*t**2*P2[1] + t**3*P3[1]
        return x, y

    def draw_curve(self):
        for t in range(0, 101):
            x, y = self.bezier_curve(t/100)
            self.canvas.create_oval(x-1, y-1, x+1, y+1, fill="blue", tags="curve")

app = BezierCurve()
app.mainloop()
