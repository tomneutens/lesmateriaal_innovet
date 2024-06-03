import csv
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Serial port configuration
serial_port = 'COM4'  # Change this to your serial port
baud_rate = 9600

# Initialize serial connection
ser = serial.Serial(serial_port, baud_rate)

# Variables for storing parsed data
timestamp = 0
timestamps = []
motor_speed = []
proportional_values = []
integral_values = []
derivative_values = []
extra_values1 = []
extra_values2 = []

# Function to parse data packet
def parse_data_packet(packet):
    data = packet.strip().split(';')
    return [float(x) for x in data]

# Function to update plot
def update_plot(frame):

    # Plot Measured Error
    axs[0, 0].clear()
    axs[0, 0].plot(timestamps, motor_speed, label='Verschil motorsnelheid', color='blue')
    axs[0, 0].set_xlabel('Tijd')
    axs[0, 0].set_ylabel('Verschil motorsnelheid')
    axs[0, 0].legend()

    # Plot Proportional
    axs[0, 1].clear()
    axs[0, 1].plot(timestamps, proportional_values, label='Proportie (P)', color='green')
    axs[0, 1].set_xlabel('Tijd')
    axs[0, 1].set_ylabel('Proportie (P)')
    axs[0, 1].legend()
    
    # Plot Proportional
    axs[2, 0].clear()
    axs[2, 0].plot(timestamps, extra_values1, label='Gemeten fout', color='green')
    axs[2, 0].set_xlabel('Tijd')
    axs[2, 0].set_ylabel('Gemeten fout')
    axs[2, 0].legend()

    # Plot Integral
    axs[1, 0].clear()
    axs[1, 0].plot(timestamps, integral_values, label='Integraal (I)', color='red')
    axs[1, 0].set_xlabel('Tijd')
    axs[1, 0].set_ylabel('Integraal (I)')
    axs[1, 0].legend()

    # Plot Derivative
    axs[1, 1].clear()
    axs[1, 1].plot(timestamps, derivative_values, label='Afgeleide/derivative (D)', color='purple')
    axs[1, 1].set_xlabel('Time')
    axs[1, 1].set_ylabel('Afgeleide/derivative (D)')
    axs[1, 1].legend()
    
    # Plot Derivative
    axs[2, 1].clear()
    axs[2, 1].plot(timestamps, motor_speed, label='Verschil motorsnelheid', color='blue')
    axs[2, 1].plot(timestamps, proportional_values, label='Proportie', color='green')
    axs[2, 1].plot(timestamps, derivative_values, label='Afgeleide', color='purple')
    axs[2, 1].plot(timestamps, integral_values, label='Integraal', color='purple')
    axs[2, 1].set_xlabel('Tijd')
    axs[2, 1].set_ylabel('Gecombineerd')
    axs[2, 1].legend()

    plt.suptitle('PID Controller Data')
    plt.tight_layout(rect=[0, 0, 1, 0.96])  # Adjust the layout to include the suptitle

    return plt
    
# Create subplots with 2 rows and 2 columns
fig, axs = plt.subplots(3, 2, sharex=True)
ani = animation.FuncAnimation(fig, update_plot, interval=1000, save_count=5000)


# Main loop to read and parse data
while True:
    print('Waiting for data...')
    line = ser.readline().decode('utf-8').strip()
    print(line)
    
    if line == 'START':
        # Reset data when a new session starts
        timestamp = 0
        timestamps = []
        motor_speed = []
        proportional_values = []
        integral_values = []
        derivative_values = []
    elif line == "WAITING" or line.startswith("CALIBR") or line.startswith("TOP") or line.startswith("BOTTOM"):
        # Wait for data to be sent
        continue
    else:
        # Parse and append data to the lists
        try:
            measured_error, proportional, integral, derivative, extra1, extra2 = parse_data_packet(line)
            timestamps.append(timestamp)
            timestamp += 1
            motor_speed.append(measured_error)
            proportional_values.append(proportional)
            integral_values.append(integral)
            derivative_values.append(derivative)
            extra_values1.append(extra1)
            extra_values2.append(extra2)
            
            # Print the parsed data
            print(f'Time: {timestamp}, Measured Error: {measured_error}, Proportional: {proportional}, Integral: {integral}, Derivative: {derivative}')
            
        except Exception as e:
            print(e)
            print('Invalid data packet received.')

    plt.pause(0.01)
