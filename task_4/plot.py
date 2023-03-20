import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import matplotlib.ticker as ticker
import json as js

with open('data.json') as f:
    data = js.loads(f.read())

x = []
y1 = []

for i in range(len(data)):
    x.append(data[i][0])
    y1.append(data[i][1])

def func1(x, a, b, c, d):
    return a * x + b

fig, ax = plt.subplots()
plt.grid(True)

z = np.arange(0, 19, 0.01)
popt, pcov = curve_fit(func1, x, y1)

ax.plot(z,func1(z, *popt) , color = 'b', linewidth = 1)


for i in range(len(x)):
    ax.scatter(x[i], y1[i], color = 'b')


#  Устанавливаем интервал основных делений:
ax.xaxis.set_major_locator(ticker.MultipleLocator(10))
#  Устанавливаем интервал вспомогательных делений:
ax.xaxis.set_minor_locator(ticker.MultipleLocator(1))
#  Тоже самое проделываем с делениями на оси "y":
ax.yaxis.set_major_locator(ticker.MultipleLocator(100000))
ax.yaxis.set_minor_locator(ticker.MultipleLocator(10000))
#  Добавляем линии основной сетки:
ax.grid(which='major',
        color = 'k')
#  Включаем видимость вспомогательных делений:
ax.minorticks_on()
#  Теперь можем отдельно задавать внешний вид
#  вспомогательной сетки:
ax.grid(which='minor',
        color = 'gray',
        linestyle = ':')

plt.xlabel('n')
plt.grid(True)
plt.ylabel('t, mcs')
plt.show()