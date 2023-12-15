import matplotlib.pyplot as plt
import numpy as np

def create_plot():
    x = np.array([-100, 100])
    plt.figure(facecolor='white', edgecolor='black', figsize=(8, 6))
    plt.plot(x, x*220, color=(15/255, 95/255, 170/255), linestyle='dotted', label='R = 220 Ω', linewidth=3)
    plt.plot(x, x*550, color=(60/255, 130/255, 39/255), linestyle='dashed', label='R = 550 Ω', linewidth=3)
    plt.plot(x, x*2000, color=(244/255, 167/255, 44/255), linestyle='solid', label='R = 2 KΩ', linewidth=3)
    plt.xlim(0, 100)
    plt.ylim(0, 50000)
    plt.ylabel('Voltage (V)', fontsize=16)
    plt.xlabel('Stroom (A)', fontsize=16)
    plt.title('Verband tussen stroom en spanning voor gegeven weerstand (U = R . I).', fontsize=18, fontweight='bold')
    plt.grid(True)
    plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0)) # set y axis ticks to scientific notation
    plt.axhline(y=0, color='black', linewidth=0.5)
    plt.axvline(x=0, color='black', linewidth=0.5)
    legend = plt.legend(frameon=True, shadow=True, borderpad=1, borderaxespad=1, fontsize=14)
    legend.get_frame().set_edgecolor('black')
    plt.tight_layout()
    plt.show()
    
    


def create_plot_inverse():
    x = np.array(range(0, 40000), dtype=np.float64)
    x = x/1000 # convert to mA
    y = (5/(x/1000))/1000 # convert to KΩ
    print(y)
    plt.figure(facecolor='white', edgecolor='black', figsize=(8, 6))
    plt.plot(x, y, color=(60/255, 130/255, 39/255), linestyle='solid', label='I = 20 mA', linewidth=3)
    plt.xlim(0, 40)
    plt.ylim(0, 10)
    plt.ylabel('Weerstand (KΩ)', fontsize=16)
    plt.xlabel('stroom (mA)', fontsize=16)
    plt.title('Weerstand in functie van de stroom bij 5 V (R = U / I).', fontsize=18, fontweight='bold')
    plt.grid(True)
    #plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0)) # set y axis ticks to scientific notation
    plt.axhline(y=0, color='black', linewidth=0.5)
    plt.axvline(x=0, color='black', linewidth=0.5)
    legend = plt.legend(frameon=True, shadow=True, borderpad=1, borderaxespad=1, fontsize=14)
    legend.get_frame().set_edgecolor('black')
    plt.tight_layout()
    plt.show()

def create_plot_led():
    x = np.array(range(0, 5000), dtype=np.float64)
    x = x/1000 # convert to mV
    #y = (5-x)/0.02 # convert to Ω
    plt.figure(facecolor='white', edgecolor='black', figsize=(8, 6))
    plt.plot(x, (5-x)/0.02, color=(15/255, 95/255, 170/255), linestyle='solid', label='I = 20mA', linewidth=3)
    plt.plot(x, (5-x)/0.015, color=(60/255, 130/255, 39/255), linestyle='dotted', label='I = 15mA', linewidth=3)
    plt.plot(x, (5-x)/0.01, color=(244/255, 167/255, 44/255), linestyle='dashed', label='I = 10mA', linewidth=3)
    plt.plot(x, (5-x)/0.005, color=(115/255, 181/255, 30/255), linestyle='dashdot', label='I = 5mA', linewidth=3)
    plt.xlim(1.5, 3.5)
    plt.ylim(50, 700)
    plt.ylabel('Weerstand (Ω)', fontsize=20)
    plt.xlabel('Spanningsval led (V)', fontsize=20)
    plt.title('Weerstand in functie van de spanningsval over een led.', fontsize=24, fontweight='bold')
    plt.grid(True)
    #plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0)) # set y axis ticks to scientific notation
    plt.axhline(y=0, color='black', linewidth=0.5)
    plt.axvline(x=0, color='black', linewidth=0.5)
    legend = plt.legend(frameon=True, shadow=True, borderpad=1, borderaxespad=1, fontsize=20)
    legend.get_frame().set_edgecolor('black')
    plt.tight_layout()
    plt.show()

create_plot_led()