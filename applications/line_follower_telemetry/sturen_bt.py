# Importeer de serial bibliotheek.
import serial
import time

# Kies de juiste seriële poort en baud rate
serial_port = 'COM4'
baud_rate = 9600

# Open a serial connection
ser = serial.Serial(serial_port, baud_rate, timeout=0)

try:
    while True:
        # Vraag aan de gebruiker om data in te geven
        data_to_send = input("Voer data in (typ 'stop' om te stoppen): ")

        # Controleer of de gebruiker wil stoppen
        if data_to_send.lower() == 'stop':
            break
        
        print("Verstuur data: " + data_to_send)

        # Schrijf de data naar de seriële poort
        ser.write(bytes("5", 'utf-8')) 
        
        

        # Wacht eventjes (optioneel)
        time.sleep(1)

except KeyboardInterrupt:
    print("\nDe gebruiker heeft het programma gestopt.")

finally:
    # Close the serial connection
    ser.close()
    print("De Seriële verbinding is gesloten.")