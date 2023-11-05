import serial
import re

# Inicia conexión serial
ser = serial.Serial('/dev/ttyACM0', 115200)

# Expresión regular para capturar las etiquetas y valores
pattern = re.compile(r"(\w+pr): (\d+\.\d+)")

# Espera por datos
print("Esperando datos...")

# Procesa las líneas que vienen del serial
while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        
        # Busca coincidencias en la línea actual
        matches = pattern.findall(line)
        
        for match in matches:
            label, value = match
            probability = float(value)
            
            # Verifica si la probabilidad supera el umbral
            if probability > 0.85:
                print(f"Palabra detectada: {label[:-2]}")  # Elimina los últimos dos caracteres 'pr' de la etiqueta
                break  # Rompe el bucle después de encontrar una coincidencia alta
