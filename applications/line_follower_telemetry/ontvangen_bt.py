# Importeer de pyserial bibliotheek
import serial

# Configureer de seriële poort
# Deze instelling hangt af van je computer
serial_port = 'COM4'  
# Stel de baud rate in. Deze moet dezelfde zijn als in je Dwenguino programma.
baud_rate = 19200      

# Initialize serial connection
ser = serial.Serial(serial_port, baud_rate)

# Blijf lezen van de seriële poort en print de ontvangen gegevens
print('Wachten op gegevens...')
while True:
    line = ser.readline().decode('utf-8').strip()
    print(line)