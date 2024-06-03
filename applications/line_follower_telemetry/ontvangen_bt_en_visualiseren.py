# Importeer de pyserial bibliotheek.
import serial
# Importeer de matplotlib bibliotheek.
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Configureer de seriële poort.
# Deze instelling hangt af van je computer.
serial_port = 'COM4'  
# Stel de baud rate in. Deze moet dezelfde zijn als in je Dwenguino programma.
baud_rate = 9600     

# Het huidige tijdstip.
tijdstip = 0
# Sla het tijdstip op van een meting.
tijdstippen = [] 
# Sla de gemeten waarden op.
sensorWaarden = []

# Initialize serial connection.
ser = serial.Serial(serial_port, baud_rate)

# Deze functie zet een string in csv formaat om naar een lijst met getallen.
def parse_data_pakket(pakket):
    data = pakket.strip().split(';')
    return [float(getal) for getal in data]

def update_plot(frame):
    fig.clear()
    # Plot de gemeten waarden.
    for i in range(0, len(sensorWaarden[0])):
        # Selecteer telkens de i-de waarde van elke meting.
        plt.plot(tijdstippen, [meting[i] for meting in sensorWaarden], label='Waarde sensor ' + str(i))
    plt.xlabel('Tijd')
    plt.ylabel('Gemeten waarde')
    plt.legend()
    plt.title('Verloop van de sensorwaarden doorheen de tijd.')
    
    return plt

# Maak een figuur aan.
fig = plt.figure()
animatie = animation.FuncAnimation(fig, update_plot, interval=1000)

# Blijf lezen van de seriële poort en print de ontvangen gegevens
while True:
    print('Wachten op gegevens...')
    line = ser.readline().decode('utf-8').strip() # Lees een lijn van de seriële poort.
    
    if line.startswith('TOP') or line.startswith('BOTTOM'):
        print(line)
    else:
        print(line)
        tijdstippen.append(tijdstip) # Sla het tijdstip op.
        sensorWaarden.append(parse_data_pakket(line)) # Sla de gemeten waarden op.
        tijdstip += 1 # Verhoog het tijdstip met 1.
        
        plt.pause(0.001) # Pauzeer even om de plot te updaten.
    